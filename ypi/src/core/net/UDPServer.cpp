/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-julian.lambert
** File description:
** UDPServer
*/

#include "UDPServer.hpp"
#include "Exception.hpp"

namespace exng::net {

    UDPServer::~UDPServer()
    {
        m_socket.unbind();
        if (m_thread.joinable())
            m_thread.join();
    }

    void UDPServer::handleIncomingData() {
        sf::IpAddress sender;
        unsigned short port;
        char data[1000];
        std::size_t received;

        sf::Socket::Status status = m_socket.receive(data, sizeof(data), received, sender, port);
        if (status == sf::Socket::Done) {
            // Handle incoming data
            Packet packet;
            packet.append(data, received);

            if (!packet.verifyChecksum()) {
                logger::warn() << "Packet checksum failed, ignoring packet";
                return;
            }

            // if the packet only contains an id, it's an acknowledgement
            if (packet.getDataSize() == sizeof(uint16_t)) {
                uint16_t packetId;
                packet >> packetId;
                m_unacknowledgedPackets.erase(packetId);
                return;
            }

            // exctract the packet header
            uint16_t packetId;
            packet >> packetId;
            bool requiresAck;
            packet >> requiresAck;
            uint8_t messageType;
            packet >> messageType;

            // try to find a handler for the message type
            std::lock_guard<std::mutex> lock(m_packetHandlersMutex);
            auto handler = m_packetHandlers.find(messageType);
            if (handler != m_packetHandlers.end()) {
                // if the packet requires an acknowledgement, send one
                if (requiresAck) {
                    PacketKey key {packetId, sender.toString(), port};
                    auto it = m_receivedPackets.find(key);
                    if (it != m_receivedPackets.end()) {
                        auto now = std::chrono::steady_clock::now();
                        auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - it->second);
                        if (diff.count() < m_acknowledgmenTimeout.count()) {
                            // Ignore the packet
                            return;
                        }
                    }

                    // Update the timestamp for the packet ID
                    m_receivedPackets[key] = std::chrono::steady_clock::now();

                    sendAcknowledgement(packetId, sender.toString(), port);
                }

                handler->second(packet, *this, {sender.toString(), port});
            } else {
                logger::error() << "Unknown message type " << static_cast<int>(messageType);
            }

        } else if (status == sf::Socket::Error) {
            //logger::error() << "Error receiving data";
        }
    }

    void UDPServer::notifyPacket(Packet &packet)
    {
        setPacketHeader(packet);
        for (const auto &client : m_clients) {
            enqueuePacket(packet, client.address, client.port, false);
        }
    }

    bool UDPServer::isNewClient(const std::string &sender, const unsigned short &port)
    {
        if (std::find_if(m_clients.begin(), m_clients.end(),
                         [&sender, &port](const Client &client) {
                           return client.address == sender &&
                                  client.port == port;
                         }) == m_clients.end()) {
            return true;
        }
        return false;
    }

    void UDPServer::addClient(const std::string &sender, const unsigned short &port)
    {
        logger::log() << "Adding client " << sender << " on port " << port;
        m_clients.push_back({sender, port});
    }

    void UDPServer::removeClient(const std::string &sender, unsigned short &port)
    {
        logger::log() << "Removing client " << sender << " on port " << port;
        m_clients.erase(std::remove_if(m_clients.begin(), m_clients.end(), [&sender, &port](const Client &client) {
            return client.address == sender && client.port == port; })
        , m_clients.end());
    }

    void UDPServer::registerPacketHanlder(uint8_t messageType, PacketHandler handler)
    {
        std::lock_guard<std::mutex> lock(m_packetHandlersMutex);
        if (m_packetHandlers.find(messageType) == m_packetHandlers.end()) {
            m_packetHandlers[messageType] = handler;
        } else {
            THROW("Packet handler already exists for message type");
        }
    }

    void UDPServer::sendPacket(Packet &packet, const std::string &address, const unsigned short port, bool reliable)
    {
        setPacketHeader(packet, reliable);
        enqueuePacket(packet, address, port, reliable);
    }

    void UDPServer::setPacketHeader(Packet &packet, bool reliable)
    {
        Packet tmp;
        tmp << m_nextPacketId;
        tmp << reliable;
        tmp.append(packet.getData(), packet.getDataSize());
        packet = tmp;
    }

    void UDPServer::sendPacket(Packet &packet, const Client &client, bool reliable)
    {
        setPacketHeader(packet, reliable);
        enqueuePacket(packet, client.address, client.port, reliable);
    }

    void UDPServer::reliableSendPacket(Packet &packet, const Client &client)
    {
        setPacketHeader(packet, true);
        enqueuePacket(packet, client.address, client.port, true);
    }

    void UDPServer::reliableSendPacket(Packet &packet, const std::string &address, const unsigned short port)
    {
        reliableSendPacket(packet, {address, port});
    }

    void UDPServer::resendUnacknowledgedPackets()
    {
        auto now = std::chrono::steady_clock::now();
        for (auto it = m_unacknowledgedPackets.begin(); it != m_unacknowledgedPackets.end();) {
            const Packet& packet = it->second.packet;
            const Client& client = it->second.client;
            const auto& firstSentTime = it->second.firstTimeSent;
            auto& lastSentTime = it->second.lastTimeSent;

            // If the packet has timed out, remove it from the unacknowledged packets
            if (now - firstSentTime > m_acknowledgmenTimeout) {
                it = m_unacknowledgedPackets.erase(it);
                continue;
            }

            // If the time since the last send is less than the max delay, skip this packet
            auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastSentTime);
            if (diff.count() < m_maxDelay) {
                ++it;
                continue;
            }

            enqueuePacket(packet, client.address, client.port, false);

            // Update the lastSentTime for the packet
            lastSentTime = now;

            ++it;
        }
    }
    void UDPServer::reliableNotifyPacket(Packet &packet)
    {
        setPacketHeader(packet, true);
        for (const auto &client : m_clients) {
            enqueuePacket(packet, client.address, client.port, true);
        }
    }

    void UDPServer::enqueuePacket(Packet packet, const std::string &address, const unsigned short port, bool reliable)
    {
        if (reliable) {
            m_unacknowledgedPackets[m_nextPacketId] = {packet, std::chrono::steady_clock::now(), std::chrono::steady_clock::now(), {address, port}};
        }
        m_nextPacketId++;
        m_outgoingPackets.push({packet, {address, port}});
    }

    void UDPServer::internalSendPacket(Packet &packet, const std::string &address, const unsigned short port)
    {
        sf::Packet sfPacket;
        sfPacket.append(packet.getData(), packet.getDataSize());
        if (sfPacket.getDataSize() > sf::UdpSocket::MaxDatagramSize) {
            logger::error() << "Packet too big to send";
            return;
        }
        m_socket.send(sfPacket, address, port);
    }

    void UDPServer::handleOutgoingData()
    {
        while (!m_outgoingPackets.empty()) {
            Packet packet = m_outgoingPackets.front().first;
            Client client = m_outgoingPackets.front().second;
            internalSendPacket(packet, client.address, client.port);
            m_outgoingPackets.pop();
        }

        resendUnacknowledgedPackets();
    }

    void UDPServer::sendAcknowledgement(uint16_t packetId, const std::string &address, const unsigned short port)
    {
        Packet ackPacket;
        ackPacket << packetId;
        enqueuePacket(ackPacket, address, port, false);
    }

    void UDPServer::setTickRate(float tickRate)
    {
        m_tickRate = 1.f / tickRate;
    }

    float UDPServer::getTickRate() const
    {
        return m_tickRate;
    }

    void UDPServer::start(unsigned short port)
    {
        if (m_socket.bind(port) != sf::Socket::Done) {
            logger::error() << "Failed to bind to port " << port;
            THROW("Failed to bind to port");
        }

        m_socket.setBlocking(false);

        logger::log() << "Server started on port " << port;

        m_chrono.reset();
        m_shouldClose = false;

        m_thread = std::thread(&UDPServer::run, this);
    }

    void UDPServer::run()
    {
        while (!m_shouldClose) {
            m_chrono.update();
            handleIncomingData();

            if (m_chrono.getElapsedTime().asSeconds() > m_tickRate) {
                handleOutgoingData();
            }

            cleanupRecentlyReceivedPackets();
        }
    }

    void UDPServer::stop(uint8_t messageType)
    {
        Packet packet;
        packet << messageType;
        notifyPacket(packet);
        m_shouldClose = true;
    }

    void UDPServer::setAcknowledgementTimeout(std::chrono::milliseconds timeout)
    {
        m_acknowledgmenTimeout = timeout;
    }

    std::chrono::milliseconds UDPServer::getAcknowledgementTimeout() const
    {
        return m_acknowledgmenTimeout;
    }

    void UDPServer::cleanupRecentlyReceivedPackets() {
        auto now = std::chrono::steady_clock::now();

        for (auto it = m_receivedPackets.begin(); it != m_receivedPackets.end(); /* no increment here */) {
            auto diff = std::chrono::duration_cast<std::chrono::seconds>(now - it->second);
            if (diff.count() >= 10) {  // Change this value to adjust the cleanup interval
                it = m_receivedPackets.erase(it);
            } else {
                ++it;
            }
        }
    }

    void UDPServer::removeClient(const Client &client)
    {
        removeClient(client.address, client.port);
    }

    void UDPServer::removeClient(const std::string &sender, const unsigned short &port)
    {
        m_clients.erase(std::remove_if(m_clients.begin(), m_clients.end(), [&sender, &port](const Client &client) {
            return client.address == sender && client.port == port; })
            , m_clients.end());
    }

    void UDPServer::unregisterPacketHandler(uint8_t messageType)
    {
        std::lock_guard<std::mutex> lock(m_packetHandlersMutex);
        m_packetHandlers.erase(messageType);
    }

    unsigned int UDPServer::getNbClients() const
    {
        return m_clients.size();
    }

}