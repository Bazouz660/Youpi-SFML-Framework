/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-julian.lambert
** File description:
** Packet
*/

#include "Packet.hpp"

namespace ypi::net {

    Packet::Packet()
    {
    }

    Packet::~Packet()
    {
    }

    Packet::Packet(const void* data, std::size_t sizeInBytes)
    {
        m_packet.append(data, sizeInBytes);
        calculateChecksum();
    }

    Packet::Packet(uint8_t messageType)
    {
        m_packet << messageType;
        calculateChecksum();
    }

    void Packet::append(const void* data, std::size_t sizeInBytes)
    {
        m_packet.append(data, sizeInBytes);
        calculateChecksum();
    }

    void Packet::clear()
    {
        m_packet.clear();
        calculateChecksum();
    }

    const void* Packet::getData() const
    {
        return m_packet.getData();
    }

    std::size_t Packet::getDataSize() const
    {
        return m_packet.getDataSize();
    }

    bool Packet::endOfPacket() const
    {
        return m_packet.endOfPacket();
    }

    Packet& Packet::operator >>(bool& data)
    {
        m_packet >> data;
        calculateChecksum();
        return *this;
    }

    Packet& Packet::operator >>(int8_t& data)
    {
        m_packet >> data;
        calculateChecksum();
        return *this;
    }

    Packet& Packet::operator >>(uint8_t& data)
    {
        m_packet >> data;
        calculateChecksum();
        return *this;
    }

    Packet& Packet::operator >>(int16_t& data)
    {
        m_packet >> data;
        calculateChecksum();
        return *this;
    }

    Packet& Packet::operator >>(uint16_t& data)
    {
        m_packet >> data;
        calculateChecksum();
        return *this;
    }

    Packet& Packet::operator >>(int32_t& data)
    {
        m_packet >> data;
        calculateChecksum();
        return *this;
    }

    Packet& Packet::operator >>(uint32_t& data)
    {
        m_packet >> data;
        calculateChecksum();
        return *this;
    }

    Packet& Packet::operator >>(int64_t& data)
    {
        sf::Int64 tmp;
        tmp = data;
        m_packet >> tmp;
        data = tmp;
        calculateChecksum();
        return *this;
    }

    Packet& Packet::operator >>(uint64_t& data)
    {
        sf::Uint64 tmp;
        tmp = data;
        m_packet >> tmp;
        data = tmp;
        calculateChecksum();
        return *this;
    }

    Packet& Packet::operator >>(float& data)
    {
        m_packet >> data;
        calculateChecksum();
        return *this;
    }

    Packet& Packet::operator >>(double& data)
    {
        m_packet >> data;
        calculateChecksum();
        return *this;
    }

    Packet& Packet::operator >>(char* data)
    {
        m_packet >> data;
        calculateChecksum();
        return *this;
    }

    Packet& Packet::operator >>(std::string& data)
    {
        m_packet >> data;
        calculateChecksum();
        return *this;
    }

    Packet& Packet::operator >>(wchar_t* data)
    {
        m_packet >> data;
        calculateChecksum();
        return *this;
    }

    Packet& Packet::operator >>(std::wstring& data)
    {
        m_packet >> data;
        calculateChecksum();
        return *this;
    }

    Packet& Packet::operator <<(bool data)
    {
        m_packet << data;
        calculateChecksum();
        return *this;
    }

    Packet& Packet::operator <<(int8_t data)
    {
        m_packet << data;
        calculateChecksum();
        return *this;
    }

    Packet& Packet::operator <<(uint8_t data)
    {
        m_packet << data;
        calculateChecksum();
        return *this;
    }

    Packet& Packet::operator <<(int16_t data)
    {
        m_packet << data;
        calculateChecksum();
        return *this;
    }

    Packet& Packet::operator <<(uint16_t data)
    {
        m_packet << data;
        calculateChecksum();
        return *this;
    }

    Packet& Packet::operator <<(int32_t data)
    {
        m_packet << data;
        calculateChecksum();
        return *this;
    }

    Packet& Packet::operator <<(uint32_t data)
    {
        m_packet << data;
        calculateChecksum();
        return *this;
    }

    Packet& Packet::operator <<(int64_t data)
    {
        sf::Int64 tmp;
        tmp = data;
        m_packet << tmp;
        calculateChecksum();
        return *this;
    }

    Packet& Packet::operator <<(uint64_t data)
    {
        sf::Uint64 tmp;
        tmp = data;
        m_packet << tmp;
        calculateChecksum();
        return *this;
    }

    Packet& Packet::operator <<(float data)
    {
        m_packet << data;
        calculateChecksum();
        return *this;
    }

    Packet& Packet::operator <<(double data)
    {
        m_packet << data;
        calculateChecksum();
        return *this;
    }

    Packet& Packet::operator <<(const char* data)
    {
        m_packet << data;
        calculateChecksum();
        return *this;
    }

    Packet& Packet::operator <<(const std::string& data)
    {
        m_packet << data;
        calculateChecksum();
        return *this;
    }

    Packet& Packet::operator <<(const wchar_t* data)
    {
        m_packet << data;
        calculateChecksum();
        return *this;
    }

    Packet& Packet::operator <<(const std::wstring& data)
    {
        m_packet << data;
        calculateChecksum();
        return *this;
    }

    void Packet::calculateChecksum()
    {
        m_checksum = calculateChecksum(m_packet.getData(), m_packet.getDataSize());
    }

    bool Packet::verifyChecksum()
    {
        return m_checksum == calculateChecksum(m_packet.getData(), m_packet.getDataSize());
    }

    uint32_t Packet::calculateChecksum(const void* data, std::size_t sizeInBytes)
    {
        uint32_t checksum = 0;
        const uint8_t* ptr = static_cast<const uint8_t*>(data);
        for (std::size_t i = 0; i < sizeInBytes; ++i)
            checksum += *ptr++;
        return checksum;
    }
}