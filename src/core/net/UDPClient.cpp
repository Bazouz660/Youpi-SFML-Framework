/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-julian.lambert
** File description:
** UDPClient
*/

#include "UDPClient.hpp"
#include "Chrono.hpp"

namespace ypi::net {

    UDPClient::UDPClient()
    {
        // register ping packet handler for connection liveliness check
        registerPacketHandler(0, [](Packet &packet, UDPClient &client) {
            client.m_lastConnectionLivelynessTime = std::chrono::steady_clock::now();
        });
    }

    UDPClient::~UDPClient()
    {
        m_socket.unbind();
    }

    void UDPClient::update()
    {
        m_chrono.update();

        handleIncomingData();

        if (m_chrono.getElapsedTime().asSeconds() > m_tickRate) {
            // send a ping packet to the server to check if the connection is still alive
            send(Packet(0));
            // send outgoing packets
            handleOutgoingData();
        }

        cleanupRecentlyReceivedPackets();

        if (m_lastConnectionLivelynessTime - std::chrono::steady_clock::now() > m_connectionLivelynessTimeout) {
            setConnected(false);
            logger::warn() << "Connection to server lost";
        }
    }

    void UDPClient::registerPacketHandler(uint8_t messageType, PacketHandler handler)
    {
        std::lock_guard<std::mutex> lock(m_packetHandlersMutex);
        if (m_packetHandlers.find(messageType) != m_packetHandlers.end()) {
            THROW("Packet handler already exists for message type");
        } else {
            m_packetHandlers.insert_or_assign(messageType, handler);
        }
    }

    void UDPClient::connect(const std::string &serverAddress, unsigned short serverPort, uint8_t messageType)
    {
        m_binded = true;

        m_socket.setBlocking(false);

        m_serverAddress = serverAddress;
        m_serverPort = serverPort;
        m_chrono.reset();

        Packet packet(messageType);
        send(packet);
    }


    void UDPClient::send(Packet packet)
    {
        setPacketHeader(packet);
        enqueuePacket(packet);
    }

    void UDPClient::handleIncomingData()
    {
        sf::IpAddress sender;
        unsigned short port;
        char data[1000];
        std::size_t received;

        if (!m_binded)
            return;

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

            uint16_t packetId;
            packet >> packetId;
            bool requiresAck;
            packet >> requiresAck;
            uint8_t messageType;
            packet >> messageType;

            std::lock_guard<std::mutex> lock(m_packetHandlersMutex);
            // try to find a handler for the message type
            auto handler = m_packetHandlers.find(messageType);
            if (handler != m_packetHandlers.end()) {
                if (requiresAck) {
                    if (m_recentlyReceivedPackets.find(packetId) != m_recentlyReceivedPackets.end()) {
                        auto now = std::chrono::steady_clock::now();
                        auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_recentlyReceivedPackets[packetId]);
                        if (diff.count() < m_acknowledgmentTimeout.count()) {
                            // We already received this packet, ignore it
                            return;
                        }
                    }

                    // Update the timestamp for the packet ID
                    m_recentlyReceivedPackets[packetId] = std::chrono::steady_clock::now();

                    sendAcknowledgement(packetId);
                }

                handler->second(packet, *this);
            } else {
                //logger::error() << "Unknown message type " << static_cast<int>(messageType);
            }

        } else if (status == sf::Socket::Error) {
            //logger::error() << "Error receiving data";
        }
    }

    bool UDPClient::isConnected() const
    {
        return m_connected;
    }

    void UDPClient::enqueuePacket(Packet packet, bool reliable)
    {
        if (reliable) {
            m_unacknowledgedPackets[m_nextPacketId] = {packet, std::chrono::steady_clock::now(), std::chrono::steady_clock::now()};
        }
        m_nextPacketId++;
        m_outgoingPackets.push(packet);
    }

    void UDPClient::internalSendPacket(Packet &packet)
    {
        sf::Packet sfPacket;
        sfPacket.append(packet.getData(), packet.getDataSize());
        sf::Socket::Status status = m_socket.send(sfPacket, m_serverAddress, m_serverPort);
        if (status == sf::Socket::Error) {
            logger::error("Could not send packet to server");
        }
    }

    void UDPClient::setPacketHeader(Packet &packet, bool reliable)
    {
        Packet tmp;
        tmp << m_nextPacketId;
        tmp << reliable;
        tmp.append(packet.getData(), packet.getDataSize());
        packet = tmp;
    }

    void UDPClient::resendUnacknowledgedPackets()
    {
        auto now = std::chrono::steady_clock::now();
        for (auto it = m_unacknowledgedPackets.begin(); it != m_unacknowledgedPackets.end();) {
            const Packet& packet = it->second.packet;
            const auto& firstTimeSent = it->second.firstTimeSent;
            auto& lastTimeSent = it->second.lastTimeSent;

            // If the packet has timed out, remove it from the unacknowledged packets
            if (now - firstTimeSent > m_acknowledgmentTimeout) {
                it = m_unacknowledgedPackets.erase(it);
                continue;
            }

            // If the time since the last send is less than the max delay, skip this packet
            auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTimeSent);
            if (diff.count() < m_maxDelay) {
                ++it;
                continue;
            }

            enqueuePacket(packet, false);

            lastTimeSent = now;

            ++it;
        }
    }

    void UDPClient::handleOutgoingData()
    {
        if (!m_binded)
            return;

        while (!m_outgoingPackets.empty()) {
            Packet packet = m_outgoingPackets.front();
            m_outgoingPackets.pop();
            internalSendPacket(packet);
        }

        resendUnacknowledgedPackets();
    }

    void UDPClient::reliableSend(Packet packet)
    {
        setPacketHeader(packet, true);
        enqueuePacket(packet, true);
    }

    void UDPClient::sendAcknowledgement(uint16_t packetId)
    {
        Packet ackPacket;
        ackPacket << packetId;
        enqueuePacket(ackPacket, false);
    }

    void UDPClient::setAcknowledgementTimeout(std::chrono::milliseconds timeout)
    {
        m_acknowledgmentTimeout = timeout;
    }

    std::chrono::milliseconds UDPClient::getAcknowledgementTimeout() const
    {
        return m_acknowledgmentTimeout;
    }

    void UDPClient::cleanupRecentlyReceivedPackets() {
        auto now = std::chrono::steady_clock::now();

        for (auto it = m_recentlyReceivedPackets.begin(); it != m_recentlyReceivedPackets.end();) {
            const auto& receivedTime = it->second;
            auto diff = std::chrono::duration_cast<std::chrono::seconds>(now - receivedTime);

            if (diff.count() > 10) {
                it = m_recentlyReceivedPackets.erase(it);
                continue;
            }

            ++it;
        }
    }

    // get unanswered packets
    std::map<uint16_t, Packet> UDPClient::getUnacknowledgedPackets() const
    {
        std::map<uint16_t, Packet> packets;
        for (auto it = m_unacknowledgedPackets.begin(); it != m_unacknowledgedPackets.end(); ++it) {
            packets[it->first] = it->second.packet;
        }
        return packets;
    }

    void UDPClient::unregisterPacketHandler(uint8_t messageType)
    {
        std::lock_guard<std::mutex> lock(m_packetHandlersMutex);
        m_packetHandlers.erase(messageType);
    }

    void UDPClient::setConnected(bool connected)
    {
        m_connected = connected;
        m_binded = connected;
    }

    void UDPClient::disconnect(uint8_t messageType)
    {
        ypi::net::Packet packet(messageType);
        reliableSend(packet);
    }

    void UDPClient::setTickRate(float tickRate)
    {
        m_tickRate = tickRate;
    }

    float UDPClient::getTickRate() const
    {
        return m_tickRate;
    }

}
