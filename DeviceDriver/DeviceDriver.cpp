#include "FlashMemoryDevice.h"
#include "DeviceDriver.h"
#include "windows.h"

unsigned char DeviceDriver::read(long address)
{
	int num_tries = kMaxNumRead;
	bool is_first = true;
	unsigned char read_data = 0;

	while (num_tries--)
	{
		if (is_first)
		{
			read_data = m_hardware->read(address);
			is_first = false;
			continue;
		}

		if (read_data != m_hardware->read(address))
		{
			throw MyException(MyException::kReadFailException);
		}
	}
	return read_data;
}

void DeviceDriver::write(long address, unsigned char data)
{
	if (m_hardware->read(address) != kInvalidData)
	{
		throw MyException(MyException::kWriteFailException);
	}
	m_hardware->write(address, data);
}
