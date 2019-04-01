/*
 * E-Lib
 * Copyright (C) 2019 EnAccess
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include "mbedserial.h"
#include "platform/Callback.h"

using namespace EnAccess;

MbedSerial::MbedSerial(PinName tx, PinName rx) :
    _rawSerial(tx, rx, 115200)
{
    _rawSerial.attach(mbed::callback(this, &MbedSerial::handleInterrupt),
                      RawSerial::RxIrq);
}

bool MbedSerial::open()
{
    return true;
}

bool MbedSerial::isOpen()
{
    return true;
}

bool MbedSerial::setSerialConfig(uint32_t baudRate, uint8_t dataBits)
{
    _rawSerial.baud(baudRate);
    _rawSerial.format(dataBits);

    return true;
}

void MbedSerial::close()
{ }

const char* MbedSerial::portName() const
{
    return NULL;
}

uint16_t MbedSerial::write(const char* data, uint16_t size)
{
    uint16_t written = BufferedSerial::write(data, size);
    _rawSerial.attach(mbed::callback(this, &MbedSerial::handleInterrupt),
                      RawSerial::TxIrq);
    return written;
}

void MbedSerial::write(char data)
{
    BufferedSerial::write(data);
    _rawSerial.attach(mbed::callback(this, &MbedSerial::handleInterrupt),
                      RawSerial::TxIrq);
}

bool MbedSerial::rawRead(uint8_t& data)
{
    if (_rawSerial.readable()) {
        data = _rawSerial.getc();
        return true;
    }

    return false;
}

bool MbedSerial::rawWrite(uint8_t data)
{
    if (_rawSerial.writeable()) {
        if (_rawSerial.putc(data) == data) {
            return true;
        }
    }

    return false;
}

void MbedSerial::handleInterrupt()
{
    transferToAndFromBuffer();

    if (!_writeBuffer.availableData()) {
        _rawSerial.attach(NULL, RawSerial::TxIrq);
    }
}