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

TEST(FlashTest, WriteFail)
{
	constexpr long kAddress = 0x01;
	constexpr unsigned char written_data = 0x01;
	constexpr unsigned char new_data = 0x02;

	MockFlashMemoryDevice mock_flash_device;

	EXPECT_CALL(mock_flash_device, read(kAddress))
		.Times(1)
		.WillOnce(Return(written_data));
	DeviceDriver flash_driver(&mock_flash_device);
	EXPECT_ANY_THROW(flash_driver.write(kAddress, new_data));
}

TEST(FlashTest, WriteSuceed)
{
	constexpr long kAddress = 0x01;
	constexpr unsigned char new_data = 0x02;

	MockFlashMemoryDevice mock_flash_device;

	EXPECT_CALL(mock_flash_device, read(kAddress))
		.Times(6)
		.WillOnce(Return(DeviceDriver::kInvalidData))
		.WillRepeatedly(Return(new_data));
	EXPECT_CALL(mock_flash_device, write(kAddress, new_data))
		.Times(1)
		.WillOnce(Return());

	DeviceDriver flash_driver(&mock_flash_device);
	flash_driver.write(kAddress, new_data);
	EXPECT_THAT(new_data, Eq(flash_driver.read(kAddress)));
}