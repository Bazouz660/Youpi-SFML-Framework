#ifndef YPI_NET_PACKET_HPP
#define YPI_NET_PACKET_HPP

#include <SFML/Network.hpp>
#include <cstdint>

namespace ypi::net {

    class Packet
    {
        public:

            Packet();
            Packet(const void* data, std::size_t sizeInBytes);
            Packet(uint8_t messageType);
            ~Packet();

            void append(const void* data, std::size_t sizeInBytes);
            void clear();
            const void* getData() const;
            std::size_t getDataSize() const;
            bool endOfPacket() const;

            void calculateChecksum();
            bool verifyChecksum();

            Packet& operator >>(bool&         data);
            Packet& operator >>(int8_t&         data);
            Packet& operator >>(uint8_t&        data);
            Packet& operator >>(int16_t&        data);
            Packet& operator >>(uint16_t&       data);
            Packet& operator >>(int32_t&        data);
            Packet& operator >>(uint32_t&       data);
            Packet& operator >>(int64_t&        data);
            Packet& operator >>(uint64_t&       data);
            Packet& operator >>(float&        data);
            Packet& operator >>(double&       data);
            Packet& operator >>(char*         data);
            Packet& operator >>(std::string&  data);
            Packet& operator >>(wchar_t*      data);
            Packet& operator >>(std::wstring& data);

            Packet& operator <<(bool                data);
            Packet& operator <<(int8_t                data);
            Packet& operator <<(uint8_t               data);
            Packet& operator <<(int16_t               data);
            Packet& operator <<(uint16_t              data);
            Packet& operator <<(int32_t               data);
            Packet& operator <<(uint32_t              data);
            Packet& operator <<(int64_t               data);
            Packet& operator <<(uint64_t              data);
            Packet& operator <<(float               data);
            Packet& operator <<(double              data);
            Packet& operator <<(const char*         data);
            Packet& operator <<(const std::string&  data);
            Packet& operator <<(const wchar_t*      data);
            Packet& operator <<(const std::wstring& data);

        private:
            sf::Packet m_packet;
            uint32_t m_checksum;

            static uint32_t calculateChecksum(const void* data, std::size_t sizeInBytes);
    };

}


#endif // !YPI_NET_PACKET_HPP
