#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../DeviceDriver/FlashMemoryDevice.h"
#include "../DeviceDriver/DeviceDriver.cpp"

using namespace std;
using namespace testing;

class MockFlashMemoryDevice : public FlashMemoryDevice
{
public:
	MOCK_METHOD(unsigned char, read, (long address), (override));
	MOCK_METHOD(void, write, (long address, unsigned char data), (override));
};

TEST(FlashTest, ReadFail1)
{
	constexpr long kReadAddress = 0x01;
	constexpr unsigned char read_data = 0x01;
	MockFlashMemoryDevice mock_flash_device;

	EXPECT_CALL(mock_flash_device, read(kReadAddress))
		.Times(2)
		.WillOnce(Return(read_data - 1))
		.WillOnce(Return(read_data))
		.WillOnce(Return(read_data))
		.WillOnce(Return(read_data))
		.WillOnce(Return(read_data));
	DeviceDriver flash_driver(&mock_flash_device);
	EXPECT_ANY_THROW(flash_driver.read(kReadAddress));
}

TEST(FlashTest, ReadFail2)
{
	constexpr long kReadAddress = 0x01;
	constexpr unsigned char read_data = 0x01;
	MockFlashMemoryDevice mock_flash_device;

	EXPECT_CALL(mock_flash_device, read(kReadAddress))
		.Times(5)
		.WillOnce(Return(read_data))
		.WillOnce(Return(read_data))
		.WillOnce(Return(read_data))
		.WillOnce(Return(read_data))
		.WillOnce(Return(read_data - 1));
	DeviceDriver flash_driver(&mock_flash_device);
	EXPECT_ANY_THROW(flash_driver.read(kReadAddress));
}

TEST(FlashTest, ReadSucceed)
{
	constexpr long kReadAddress = 0x01;
	constexpr unsigned char kReadData = 0x01;
	MockFlashMemoryDevice mock_flash_device;

	EXPECT_CALL(mock_flash_device, read(kReadAddress))
		.Times(5)
		.WillOnce(Return(kReadData))
		.WillOnce(Return(kReadData))
		.WillOnce(Return(kReadData))
		.WillOnce(Return(kReadData))
		.WillOnce(Return(kReadData));
	DeviceDriver flash_driver(&mock_flash_device);
	EXPECT_THAT(kReadData, Eq(flash_driver.read(kReadAddress)));
}
