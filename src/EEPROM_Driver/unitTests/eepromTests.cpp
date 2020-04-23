#include <stdint.h>

extern "C"{
    #include "../eeprom.h"
    #include "../hardwareDriver.h"
}

#include "fff.h"
#include <gtest/gtest.h>

DEFINE_FFF_GLOBALS;

//Mocked functions
extern "C"{
    FAKE_VOID_FUNC(spiInit);
    FAKE_VOID_FUNC(csInit);
    FAKE_VALUE_FUNC(uint8_t, readSPIBuffer);
    FAKE_VOID_FUNC(repeatedEEPROMRead, uint8_t*, uint8_t*, int);
    FAKE_VOID_FUNC(eepromWriteEnable);
    FAKE_VOID_FUNC(eepromWriteDisable);
    FAKE_VALUE_FUNC(uint8_t, eepromReadStatus);
    FAKE_VOID_FUNC(eepromWriteStatus, uint8_t);
    FAKE_VOID_FUNC(eepromSendMessage, uint8_t*, int);
    FAKE_VOID_FUNC(eepromWaitForWriteCompletion);
}

//Macro to resat all mocks
#define FFF_FAKES_LIST(FAKE)\
        FAKE(spiInit)\
        FAKE(csInit)\
        FAKE(readSPIBuffer)\
        FAKE(repeatedEEPROMRead)\
        FAKE(eepromWriteEnable)\
        FAKE(eepromWriteDisable)\
        FAKE(eepromReadStatus)\
        FAKE(eepromWriteStatus)\
        FAKE(eepromSendMessage)\
        FAKE(eepromWaitForWriteCompletion)\

class EepromTests : public testing::Test{
    protected:
        void SetUp()
        {
            FFF_FAKES_LIST(RESET_FAKE);
            FFF_RESET_HISTORY();
        }
        void TearDown(){}
};

TEST_F(EepromTests, init_test)
{
    eepromInit();
    ASSERT_EQ(spiInit_fake.call_count, 1);
    ASSERT_EQ(csInit_fake.call_count, 1);
    ASSERT_EQ(eepromWriteStatus_fake.call_count, 1);
    ASSERT_EQ(eepromWriteStatus_fake.arg0_val, 0x00);
}

TEST_F(EepromTests, erase_test)
{
    eepromErase();
    ASSERT_EQ(eepromWriteEnable_fake.call_count, 0x2000);
    ASSERT_EQ(eepromSendMessage_fake.call_count, 0x2000);
    ASSERT_EQ(eepromWaitForWriteCompletion_fake.call_count, 0x2000);
}

TEST_F(EepromTests, invalid_read_test)
{
    uint8_t value = 0;
    bool retStatus = eepromRead(0x2000, &value);
    ASSERT_EQ(retStatus, false);
    ASSERT_EQ(value, 0);
}

TEST_F(EepromTests, simple_read_valid)
{
    readSPIBuffer_fake.return_val = 0x0f;
    uint8_t expected_message[4] = {READ, 0x1f, 0xff, 0x00};
    uint8_t value = 0;
    bool retStatus = eepromRead(0x1fff, &value);

    ASSERT_EQ(retStatus, true);
    ASSERT_EQ(value, 0x0f);
    ASSERT_EQ(eepromSendMessage_fake.call_count, 1);
    uint8_t *message = eepromSendMessage_fake.arg0_val;
    for(int i = 0; i < eepromSendMessage_fake.arg1_val; i++)
    {
        ASSERT_EQ(message[i], expected_message[i]);
    }
}

TEST_F(EepromTests, invalid_blockRead_test)
{
    uint8_t value[2] = {0x00, 0x10};
    bool retStatus = eepromBlockRead(0x2000, value, 2);
    ASSERT_EQ(retStatus, false);
    ASSERT_EQ(value[0], 0);
    ASSERT_EQ(value[1], 16);
}

TEST_F(EepromTests, blockRead_test)
{
    uint8_t value[0x1fff] = {0x22};
    bool retStatus = eepromBlockRead(0x0000, value, 0x1fff);
    
    ASSERT_EQ(retStatus, true);
    ASSERT_EQ(repeatedEEPROMRead_fake.call_count, 1);
    uint8_t expected_message[3] = {READ, 0x00, 0x00};
    uint8_t* message = repeatedEEPROMRead_fake.arg0_val;
    for(int i = 0; i < 3; i++)
    {
        ASSERT_EQ(message[i], expected_message[i]);
    }
    uint8_t* data = repeatedEEPROMRead_fake.arg1_val;
    ASSERT_EQ(repeatedEEPROMRead_fake.arg2_val, 0x1fff);
    for(int i=0; i < repeatedEEPROMRead_fake.arg2_val; i++)
    {
        ASSERT_EQ(data[i], value[i]);
    }
}

TEST_F(EepromTests, invalid_write_test)
{
    bool status = eepromWrite(0x2000, 0x00);
    ASSERT_EQ(status, false);
}

TEST_F(EepromTests, write_test)
{
    bool status = eepromWrite(0x00f0, 0x90);

    ASSERT_EQ(eepromWriteEnable_fake.call_count, 1);
    ASSERT_EQ(eepromSendMessage_fake.call_count, 1);
    ASSERT_EQ(eepromWaitForWriteCompletion_fake.call_count, 1);
    uint8_t expected_message[4] = {WRITE, 0x00, 0xf0, 0x90};
    uint8_t *message = eepromSendMessage_fake.arg0_val;
    for(int i = 0; i < eepromSendMessage_fake.arg1_val; i++)
    {
        ASSERT_EQ(message[i], expected_message[i]);
    }
}

TEST_F(EepromTests, invald_addr_pageWrite_test)
{
    uint8_t data = 0;
    bool status = eepromPageWrite(0x2000, &data, 1);
    ASSERT_EQ(status, false);
}

TEST_F(EepromTests, invalid_length_pageWrite_test)
{
    uint8_t data = 0;
    bool status = eepromPageWrite(0x0000, &data, 33);
    ASSERT_EQ(status, false);
}

TEST_F(EepromTests, pageWrite_test)
{
    uint8_t data[32] = {0};
    for(uint8_t i = 0; i < 0x20; i++)
    {
        data[i] = i;
    }
    
    eepromPageWrite(0x1000, data, 32);

    ASSERT_EQ(eepromWriteEnable_fake.call_count, 1);
    ASSERT_EQ(eepromSendMessage_fake.arg1_val, 35);
    ASSERT_EQ(eepromWaitForWriteCompletion_fake.call_count, 1);
}