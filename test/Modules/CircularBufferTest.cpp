#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "CircularBuffer.h"

TEST_GROUP(CircularBufferTest){};

// TEST(CircularBufferTest, ShouldPushAndPullDataAsExpected)
// {
//     CircularBuffer<char, 255> buffer;

//     const uint8_t SIZE = 20;
//     char dataIn[SIZE] = "123456789 987654321";
//     char dataOut[SIZE];

//     const uint8_t expectedHeadBefore = 0;
//     const uint8_t expectedHeadAfter = SIZE;

//     uint8_t writeHeadBefore = buffer.writeHead();
//     uint8_t writeLen = buffer.push(dataIn, SIZE);
//     uint8_t writeHeadAfter = buffer.writeHead();

//     uint8_t readHeadBefore = buffer.readHead();
//     uint8_t readLen = buffer.pull(dataOut, SIZE);
//     uint8_t readHeadAfter = buffer.readHead();

//     bool isEmpty = buffer.isEmpty();

//     CHECK_EQUAL(expectedHeadBefore, writeHeadBefore);
//     CHECK_EQUAL(expectedHeadAfter, writeHeadAfter);
//     CHECK_EQUAL(SIZE, writeLen);

//     CHECK_EQUAL(expectedHeadBefore, readHeadBefore);
//     CHECK_EQUAL(expectedHeadAfter, readHeadAfter);
//     CHECK_EQUAL(SIZE, readLen);

//     STRNCMP_EQUAL(dataIn, dataOut, SIZE);
//     CHECK(isEmpty);
// }

// TEST(CircularBufferTest, ShouldTruncateDataIfItDoesntFitInTheBuffer)
// {
//     const uint8_t MAX_BUFFER_SIZE = 9;
//     CircularBuffer<char, MAX_BUFFER_SIZE> buffer;

//     const uint8_t SIZE = 20;
//     char dataIn[SIZE] = "123456789 987654321";
//     char dataOut[SIZE];

//     char expectedDataOut[] = "123456789";
//     uint8_t expectedHeadBeforeAndAfter = 0;

//     uint8_t headBefore = buffer.writeHead();
//     uint8_t writeLen = buffer.push(dataIn, SIZE);
//     uint8_t headAfter = buffer.writeHead();

//     uint8_t readLen = buffer.pull(dataOut, SIZE);

//     CHECK_EQUAL(expectedHeadBeforeAndAfter, headBefore);
//     CHECK_EQUAL(expectedHeadBeforeAndAfter, headAfter);
//     CHECK_EQUAL(MAX_BUFFER_SIZE, writeLen);
//     CHECK_EQUAL(MAX_BUFFER_SIZE, readLen);
//     STRNCMP_EQUAL(expectedDataOut, dataOut, MAX_BUFFER_SIZE);
// }

TEST(CircularBufferTest, ShouldWrapToTheStartWhenBufferIsFull)
{
    const uint8_t MAX_BUFFER_SIZE = 10;
    CircularBuffer<char, MAX_BUFFER_SIZE> buffer;

    // Buffer size is 10 and data is 12
    // Therfore, 2 chars should wrap around to the start
    // Based on the test data below:
    // the buffer should contain:   67CDE12345
    // and when read it should be:  CDE1234567

    // char expectedDataOut[] = "ABCDE";
    // char expectedDataOut[] = "234567";

    const uint8_t SIZE = 7;
    char dataInFirst[SIZE] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G' };
    char dataOutFirst[SIZE];

    char dataInSecond[SIZE] = { '1', '2', '3', '4', '5', '6', '7' };
    char dataOutSecond[SIZE];

    uint8_t writeLen = buffer.push(dataInFirst, SIZE);
    uint8_t availLen = buffer.availableData();
    uint8_t readLen = buffer.pull(dataOutFirst, SIZE);
    
    CHECK_EQUAL(SIZE, writeLen);
    CHECK_EQUAL(SIZE, availLen);
    CHECK_EQUAL(SIZE, readLen);
    STRNCMP_EQUAL(dataInFirst, dataOutFirst, SIZE);

    writeLen = buffer.push(dataInSecond, SIZE);
    availLen = buffer.availableData();
    readLen = buffer.pull(dataOutSecond, SIZE);
    
    CHECK_EQUAL(SIZE, writeLen);
    CHECK_EQUAL(SIZE, availLen);
    CHECK_EQUAL(SIZE, readLen);
    STRNCMP_EQUAL(dataInSecond, dataOutSecond, SIZE);
}

TEST(CircularBufferTest, ShouldReadNothingAfterFlushingTheBuffer)
{
    const uint8_t MAX_BUFFER_SIZE = 10;
    CircularBuffer<char, MAX_BUFFER_SIZE> buffer;

    char expectedDataOut[MAX_BUFFER_SIZE] = {};
    char dataOut[MAX_BUFFER_SIZE] = {};

    const uint8_t SIZE = 5;
    char dataIn[SIZE] = { 'A', 'B', 'C', 'D', 'E' };

    uint8_t writeLen = buffer.push(dataIn, SIZE);

    buffer.flush();

    uint8_t readLen = buffer.pull(dataOut, MAX_BUFFER_SIZE);

    CHECK_EQUAL(SIZE, writeLen);
    CHECK_EQUAL(0, readLen);
    STRNCMP_EQUAL(expectedDataOut, dataOut, MAX_BUFFER_SIZE);
}