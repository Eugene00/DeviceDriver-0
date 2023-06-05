#pragma once
#include <stdexcept>
class FlashMemoryDevice;
class MyException : public std::exception
{
public:
    explicit MyException(char const* message) : exception (message) {}
    static constexpr char kReadFailException[] = "Read failed";
    static constexpr char kWriteFailException[] = "Write failed";
};

class DeviceDriver
{
public:
    DeviceDriver(FlashMemoryDevice* hardware) : m_hardware(hardware) {}
    unsigned char read(long address);
    void write(long address, unsigned char data);

    static constexpr int kMaxNumRead = 5;
    static constexpr unsigned char kInvalidData = 0xFF;

protected:
    FlashMemoryDevice* m_hardware;
};