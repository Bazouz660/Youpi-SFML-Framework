#ifndef EXNG_UDP_SERVER_HPP
#define EXNG_UDP_SERVER_HPP

#include <SFML/Network.hpp>
#include <iostream>
#include <vector>
#include "logger.hpp"
#include "Packet.hpp"
#include <functional>
#include <map>
#include <queue>
#include <atomic>
#include <thread>
#include <chrono>
#include "helper/SafeDequeue.hpp"

#include "Chrono.hpp"

namespace exng::net {

class UDPServer
{
public:

    struct Client
    {
        std::string address;
        unsigned short port;
    };

    using PacketHandler = std::function<void(Packet &, UDPServer& server, const Client& client)>; // Define a type for packet handling functions

    // creates a UDP server and binds it to the specified port
    UDPServer() = default;
    ~UDPServer();

    void start(unsigned short port);
    void stop(uint8_t messageType);

    void setTickRate(float tickRate);
    float getTickRate() const;

    void setAcknowledgementTimeout(std::chrono::milliseconds timeout);
    std::chrono::milliseconds getAcknowledgementTimeout() const;

    // handle incoming data, this will call the appropriate packet handler for the received message type
    void handleIncomingData();

    // handle outgoing data, this will send all the packets that need to be sent
    void handleOutgoingData();

    // send a packet to every connected client
    void notifyPacket(Packet &packet);

    // send a packet to every connected client, and mark it as needing an acknowledgement
    void reliableNotifyPacket(Packet &packet);

    // send a packet to a specific client
    void sendPacket(Packet &packet, const std::string &address, const unsigned short port, bool reliable = false);
    void sendPacket(Packet &packet, const Client &client, bool reliable = false);

    // send a packet to a specific client, and mark it as needing an acknowledgement
    void reliableSendPacket(Packet &packet, const Client &client);
    void reliableSendPacket(Packet &packet, const std::string &address, const unsigned short port);

    // add a packet handler for a specific message type
    void registerPacketHanlder(uint8_t messageType, PacketHandler handler);

    // remove a packet handler for a specific message type
    void unregisterPacketHandler(uint8_t messageType);

    // check if a client is new
    bool isNewClient(const std::string &sender, const unsigned short &port);

    // add a client to the list of connected clients
    void addClient(const std::string &sender, const unsigned short &port);

    // remove a client from the list of connected clients
    void removeClient(const Client &client);
    void removeClient(const std::string &sender, const unsigned short &port);

    unsigned int getNbClients() const;


private:
    Chrono m_chrono;
    float m_tickRate = 1.0f / 60.0f; // 60 hz
    std::atomic<bool> m_shouldClose = false;

    struct unacknowledgedPacket
    {
        Packet packet;
        std::chrono::steady_clock::time_point lastTimeSent;
        std::chrono::steady_clock::time_point firstTimeSent;
        Client client;
    };

    struct PacketKey {
        uint16_t packetId;
        std::string ip;
        unsigned short port;

        bool operator==(const PacketKey& other) const {
            return packetId == other.packetId && ip == other.ip && port == other.port;
        }

        bool operator<(const PacketKey& other) const {
            if (packetId != other.packetId)
                return packetId < other.packetId;
            if (ip != other.ip)
                return ip < other.ip;
            return port < other.port;
        }
    };

    std::thread m_thread;

    sf::UdpSocket m_socket;
    std::vector<Client> m_clients;
    std::mutex m_packetHandlersMutex;
    std::map<uint8_t, PacketHandler> m_packetHandlers;         // Map to associate message type with handler function
    std::map<PacketKey, std::chrono::steady_clock::time_point> m_receivedPackets;

    uint16_t m_nextPacketId = 0;
    std::map<uint16_t, unacknowledgedPacket> m_unacknowledgedPackets;

    std::chrono::milliseconds m_acknowledgmenTimeout = std::chrono::milliseconds(3000);
    unsigned int m_maxDelay = 500;

    SafeDequeue<std::pair<Packet, Client>> m_outgoingPackets;

    void removeClient(const std::string &sender, unsigned short &port);

    void enqueuePacket(Packet packet, const std::string &address, const unsigned short port, bool reliable = false);
    void internalSendPacket(Packet &packet, const std::string &address, const unsigned short port);

    void setPacketHeader(Packet &packet, bool reliable = false);

    void sendAcknowledgement(uint16_t packetId, const std::string &address, const unsigned short port);

    void resendUnacknowledgedPackets();

    void cleanupRecentlyReceivedPackets();

    void run();
};

}

#endif // !EXNG_UDP_SERVER_HPP