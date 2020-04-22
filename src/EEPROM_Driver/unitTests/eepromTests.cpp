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
}