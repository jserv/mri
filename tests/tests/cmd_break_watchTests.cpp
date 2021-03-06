/* Copyright 2014 Adam Green (http://mbed.org/users/AdamGreen/)

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published
   by the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.   
*/

extern "C"
{
#include <try_catch.h>
#include <mri.h>

void __mriDebugException(void);
}
#include <platformMock.h>

// Include C++ headers for test harness.
#include "CppUTest/TestHarness.h"


TEST_GROUP(cmdBreakWatch)
{
    int     m_expectedException;            
    
    void setup()
    {
        m_expectedException = noException;
        platformMock_Init();
        __mriInit("MRI_UART_MBED_USB");
    }

    void teardown()
    {
        LONGS_EQUAL ( m_expectedException, getExceptionCode() );
        clearExceptionCode();
        platformMock_Uninit();
    }
    
    void validateExceptionCode(int expectedExceptionCode)
    {
        m_expectedException = expectedExceptionCode;
        LONGS_EQUAL ( expectedExceptionCode, getExceptionCode() );
    }
};

TEST(cmdBreakWatch, SetHardwareBreakpoint)
{
    platformMock_CommInitReceiveChecksummedData("+$Z1,12345678,2#", "+$c#");
        __mriDebugException();
    CHECK_TRUE ( platformMock_CommDoesTransmittedDataEqual("$T05responseT#7c+$OK#9a+") );
    CHECK_EQUAL( 1, platformMock_SetHardwareBreakpointCalls() );
    CHECK_EQUAL( 0x12345678, platformMock_SetHardwareBreakpointAddressArg() );
    CHECK_EQUAL( 2, platformMock_SetHardwareBreakpointKindArg() );
}

TEST(cmdBreakWatch, SetHardwareBreakpoint_WithInvalidArgSeparator_ShouldReturnErrorResponse)
{
    platformMock_CommInitReceiveChecksummedData("+$Z1:12345678,2#", "+$c#");
        __mriDebugException();
    CHECK_TRUE ( platformMock_CommDoesTransmittedDataEqual("$T05responseT#7c+$" MRI_ERROR_INVALID_ARGUMENT "#a6+") );
    CHECK_EQUAL( 0, platformMock_SetHardwareBreakpointCalls() );
}

TEST(cmdBreakWatch, SetHardwareBreakpoint_ThrowInvalidArgumentException_ShouldReturnErrorResponse)
{
    platformMock_CommInitReceiveChecksummedData("+$Z1,12345678,2#", "+$c#");
    platformMock_SetHardwareBreakpointException(invalidArgumentException);
        __mriDebugException();
    CHECK_TRUE ( platformMock_CommDoesTransmittedDataEqual("$T05responseT#7c+$" MRI_ERROR_INVALID_ARGUMENT "#a6+") );
    CHECK_EQUAL( 1, platformMock_SetHardwareBreakpointCalls() );
}

TEST(cmdBreakWatch, SetHardwareBreakpoint_ThrowExceededHardwareResourcesException_ShouldReturnErrorResponse)
{
    platformMock_CommInitReceiveChecksummedData("+$Z1,12345678,2#", "+$c#");
    platformMock_SetHardwareBreakpointException(exceededHardwareResourcesException);
        __mriDebugException();
    CHECK_TRUE ( platformMock_CommDoesTransmittedDataEqual("$T05responseT#7c+$" MRI_ERROR_NO_FREE_BREAKPOINT "#aa+") );
    CHECK_EQUAL( 1, platformMock_SetHardwareBreakpointCalls() );
}

TEST(cmdBreakWatch, SetHardwareBreakpoint_ThrowTimeoutException_ShouldReturnErrorResponse)
{
    platformMock_CommInitReceiveChecksummedData("+$Z1,12345678,2#", "+$c#");
    platformMock_SetHardwareBreakpointException(timeoutException);
        __mriDebugException();
    CHECK_TRUE ( platformMock_CommDoesTransmittedDataEqual("$T05responseT#7c+$" MRI_ERROR_NO_FREE_BREAKPOINT "#aa+") );
    CHECK_EQUAL( 1, platformMock_SetHardwareBreakpointCalls() );
}

TEST(cmdBreakWatch, SetHardwareWriteWatchpoint)
{
    platformMock_CommInitReceiveChecksummedData("+$Z2,87654321,4#", "+$c#");
        __mriDebugException();
    CHECK_TRUE ( platformMock_CommDoesTransmittedDataEqual("$T05responseT#7c+$OK#9a+") );
    CHECK_EQUAL( 1, platformMock_SetHardwareWatchpointCalls() );
    CHECK_EQUAL( 0x87654321, platformMock_SetHardwareWatchpointAddressArg() );
    CHECK_EQUAL( 4, platformMock_SetHardwareWatchpointSizeArg() );
    CHECK_EQUAL( MRI_PLATFORM_WRITE_WATCHPOINT, platformMock_SetHardwareWatchpointTypeArg() );
}

TEST(cmdBreakWatch, SetHardwareWriteWatchpoing_ThrowExceededHardwareResourcesException_ShouldReturnErrorResponse)
{
    platformMock_CommInitReceiveChecksummedData("+$Z2,87654321,4#", "+$c#");
    platformMock_SetHardwareWatchpointException(exceededHardwareResourcesException);
        __mriDebugException();
    CHECK_TRUE ( platformMock_CommDoesTransmittedDataEqual("$T05responseT#7c+$" MRI_ERROR_NO_FREE_BREAKPOINT "#aa+") );
    CHECK_EQUAL( 1, platformMock_SetHardwareWatchpointCalls() );
    CHECK_EQUAL( MRI_PLATFORM_WRITE_WATCHPOINT, platformMock_SetHardwareWatchpointTypeArg() );
}

TEST(cmdBreakWatch, SetHardwareReadWatchpoint)
{
    platformMock_CommInitReceiveChecksummedData("+$Z3,87654321,8#", "+$c#");
        __mriDebugException();
    CHECK_TRUE ( platformMock_CommDoesTransmittedDataEqual("$T05responseT#7c+$OK#9a+") );
    CHECK_EQUAL( 1, platformMock_SetHardwareWatchpointCalls() );
    CHECK_EQUAL( 0x87654321, platformMock_SetHardwareWatchpointAddressArg() );
    CHECK_EQUAL( 8, platformMock_SetHardwareWatchpointSizeArg() );
    CHECK_EQUAL( MRI_PLATFORM_READ_WATCHPOINT, platformMock_SetHardwareWatchpointTypeArg() );
}

TEST(cmdBreakWatch, SetHardwareReadWriteWatchpoint)
{
    platformMock_CommInitReceiveChecksummedData("+$Z4,87654321,8#", "+$c#");
        __mriDebugException();
    CHECK_TRUE ( platformMock_CommDoesTransmittedDataEqual("$T05responseT#7c+$OK#9a+") );
    CHECK_EQUAL( 1, platformMock_SetHardwareWatchpointCalls() );
    CHECK_EQUAL( 0x87654321, platformMock_SetHardwareWatchpointAddressArg() );
    CHECK_EQUAL( 8, platformMock_SetHardwareWatchpointSizeArg() );
    CHECK_EQUAL( MRI_PLATFORM_READWRITE_WATCHPOINT, platformMock_SetHardwareWatchpointTypeArg() );
}

TEST(cmdBreakWatch, InvalidSetHardwareBreakWatchpoint_ShouldReturnEmptyResponse)
{
    platformMock_CommInitReceiveChecksummedData("+$Z5,87654321,8#", "+$c#");
        __mriDebugException();
    CHECK_TRUE ( platformMock_CommDoesTransmittedDataEqual("$T05responseT#7c+$#00+") );
    CHECK_EQUAL( 0, platformMock_SetHardwareBreakpointCalls() );
    CHECK_EQUAL( 0, platformMock_SetHardwareWatchpointCalls() );
}

TEST(cmdBreakWatch, ClearHardwareBreakpoint)
{
    platformMock_CommInitReceiveChecksummedData("+$z1,12345678,2#", "+$c#");
        __mriDebugException();
    CHECK_TRUE ( platformMock_CommDoesTransmittedDataEqual("$T05responseT#7c+$OK#9a+") );
    CHECK_EQUAL( 1, platformMock_ClearHardwareBreakpointCalls() );
    CHECK_EQUAL( 0x12345678, platformMock_ClearHardwareBreakpointAddressArg() );
    CHECK_EQUAL( 2, platformMock_ClearHardwareBreakpointKindArg() );
}

TEST(cmdBreakWatch, ClearHardwareBreakpoint_WithInvalidArgSeparator_ShouldReturnErrorResponse)
{
    platformMock_CommInitReceiveChecksummedData("+$z1:12345678,2#", "+$c#");
        __mriDebugException();
    CHECK_TRUE ( platformMock_CommDoesTransmittedDataEqual("$T05responseT#7c+$" MRI_ERROR_INVALID_ARGUMENT "#a6+") );
    CHECK_EQUAL( 0, platformMock_ClearHardwareBreakpointCalls() );
}

TEST(cmdBreakWatch, ClearHardwareBreakpoint_ThrowExceededHardwareResourcesException_ShouldReturnErrorResponse)
{
    platformMock_CommInitReceiveChecksummedData("+$z1,12345678,2#", "+$c#");
    platformMock_ClearHardwareBreakpointException(exceededHardwareResourcesException);
        __mriDebugException();
    CHECK_TRUE ( platformMock_CommDoesTransmittedDataEqual("$T05responseT#7c+$" MRI_ERROR_NO_FREE_BREAKPOINT "#aa+") );
    CHECK_EQUAL( 1, platformMock_ClearHardwareBreakpointCalls() );
}

TEST(cmdBreakWatch, ClearHardwareWriteWatchpoint)
{
    platformMock_CommInitReceiveChecksummedData("+$z2,87654321,4#", "+$c#");
        __mriDebugException();
    CHECK_TRUE ( platformMock_CommDoesTransmittedDataEqual("$T05responseT#7c+$OK#9a+") );
    CHECK_EQUAL( 1, platformMock_ClearHardwareWatchpointCalls() );
    CHECK_EQUAL( 0x87654321, platformMock_ClearHardwareWatchpointAddressArg() );
    CHECK_EQUAL( 4, platformMock_ClearHardwareWatchpointSizeArg() );
    CHECK_EQUAL( MRI_PLATFORM_WRITE_WATCHPOINT, platformMock_ClearHardwareWatchpointTypeArg() );
}

TEST(cmdBreakWatch, ClearHardwareWriteWatchpoing_ThrowExceededHardwareResourcesException_ShouldReturnErrorResponse)
{
    platformMock_CommInitReceiveChecksummedData("+$z2,87654321,4#", "+$c#");
    platformMock_ClearHardwareWatchpointException(exceededHardwareResourcesException);
        __mriDebugException();
    CHECK_TRUE ( platformMock_CommDoesTransmittedDataEqual("$T05responseT#7c+$" MRI_ERROR_NO_FREE_BREAKPOINT "#aa+") );
    CHECK_EQUAL( 1, platformMock_ClearHardwareWatchpointCalls() );
    CHECK_EQUAL( MRI_PLATFORM_WRITE_WATCHPOINT, platformMock_ClearHardwareWatchpointTypeArg() );
}

TEST(cmdBreakWatch, ClearHardwareReadWatchpoint)
{
    platformMock_CommInitReceiveChecksummedData("+$z3,87654321,8#", "+$c#");
        __mriDebugException();
    CHECK_TRUE ( platformMock_CommDoesTransmittedDataEqual("$T05responseT#7c+$OK#9a+") );
    CHECK_EQUAL( 1, platformMock_ClearHardwareWatchpointCalls() );
    CHECK_EQUAL( 0x87654321, platformMock_ClearHardwareWatchpointAddressArg() );
    CHECK_EQUAL( 8, platformMock_ClearHardwareWatchpointSizeArg() );
    CHECK_EQUAL( MRI_PLATFORM_READ_WATCHPOINT, platformMock_ClearHardwareWatchpointTypeArg() );
}

TEST(cmdBreakWatch, ClearHardwareReadWriteWatchpoint)
{
    platformMock_CommInitReceiveChecksummedData("+$z4,87654321,8#", "+$c#");
        __mriDebugException();
    CHECK_TRUE ( platformMock_CommDoesTransmittedDataEqual("$T05responseT#7c+$OK#9a+") );
    CHECK_EQUAL( 1, platformMock_ClearHardwareWatchpointCalls() );
    CHECK_EQUAL( 0x87654321, platformMock_ClearHardwareWatchpointAddressArg() );
    CHECK_EQUAL( 8, platformMock_ClearHardwareWatchpointSizeArg() );
    CHECK_EQUAL( MRI_PLATFORM_READWRITE_WATCHPOINT, platformMock_ClearHardwareWatchpointTypeArg() );
}

TEST(cmdBreakWatch, InvalidClearHardwareBreakWatchpoint_ShouldReturnEmptyResponse)
{
    platformMock_CommInitReceiveChecksummedData("+$z5,87654321,8#", "+$c#");
        __mriDebugException();
    CHECK_TRUE ( platformMock_CommDoesTransmittedDataEqual("$T05responseT#7c+$#00+") );
    CHECK_EQUAL( 0, platformMock_ClearHardwareBreakpointCalls() );
    CHECK_EQUAL( 0, platformMock_ClearHardwareWatchpointCalls() );
}