#ifndef YPI_UDP_CLIENT_HPP
#define YPI_UDP_CLIENT_HPP

#include <SFML/Network.hpp>
#include <iostream>
#include <vector>
#include <functional>
#include <map>
#include <queue>
#include <chrono>

#include "../system/logger.hpp"
#include "../system/Exception.hpp"
#include "Packet.hpp"
#include "../system/Chrono.hpp"
#include "../data/SafeDequeue.hpp"

namespace ypi::net {

    class UDPClient
    {
        public:
            using PacketHandler = std::function<void(Packet &, UDPClient& client)>; // Define a type for packet handling functions

            UDPClient();
            ~UDPClient();

            // add a packet handler for a specific message type
            void registerPacketHandler(uint8_t messageType, PacketHandler handler);

            void unregisterPacketHandler(uint8_t messageType);

            // connect to a server, this will send a connection request to the server and wait for a response (or timeout)
            void connect(const std::string &serverAddress, unsigned short serverPort, uint8_t messageType);
            void disconnect(uint8_t messageType);

            void update();

            void send(Packet packet);
            void reliableSend(Packet packet);

            // set the timeout for reliable packets acknowledgements (default 1 second)
            void setAcknowledgementTimeout(std::chrono::milliseconds timeout);

            // get the timeout for reliable packets acknowledgements
            std::chrono::milliseconds getAcknowledgementTimeout() const;

            // get unanswered packets
            std::map<uint16_t, Packet> getUnacknowledgedPackets() const;

            bool isConnected() const;
            void setConnected(bool connected);

            void setTickRate(float tickRate);
            float getTickRate() const;

        private:
            sf::UdpSocket m_socket;
            bool m_binded = false;
            sf::IpAddress m_serverAddress;
            unsigned short m_serverPort;
            bool m_connected = false;

            Chrono m_chrono;
            float m_tickRate = 1.0f / 60.0f; // 60 hz

            std::mutex m_packetHandlersMutex;
            std::map<uint8_t, PacketHandler> m_packetHandlers; // Map to associate message type with handler function

            struct unacknowledgedPacket
            {
                Packet packet;
                std::chrono::steady_clock::time_point firstTimeSent;
                std::chrono::steady_clock::time_point lastTimeSent;
            };

            uint16_t m_nextPacketId = 0;
            std::map<uint16_t, unacknowledgedPacket> m_unacknowledgedPackets; // Map to store unacknowledged packets

            std::chrono::milliseconds m_acknowledgmentTimeout = std::chrono::milliseconds(3000);
            unsigned int m_maxDelay = 500;

            std::chrono::milliseconds m_connectionLivelynessTimeout = std::chrono::milliseconds(5000);
            std::chrono::steady_clock::time_point m_lastConnectionLivelynessTime = std::chrono::steady_clock::now();

            std::map<uint16_t, std::chrono::steady_clock::time_point> m_recentlyReceivedPackets;

            SafeDequeue<Packet> m_outgoingPackets;

            void enqueuePacket(Packet packet, bool reliable = false);
            void internalSendPacket(Packet &packet);

            void setPacketHeader(Packet &packet, bool reliable = false);

            void validateConnection();

            void resendUnacknowledgedPackets();
            void sendAcknowledgement(uint16_t packetId);
            void cleanupRecentlyReceivedPackets();


            // handle incoming data, this will call the appropriate packet handler for the received message type
            void handleIncomingData();

            // handle outgoing data, this will send all the packets that need to be sent
            void handleOutgoingData();
        };
}

#endif // YPI_UDP_CLIENT_HPP