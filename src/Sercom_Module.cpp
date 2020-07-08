/*
    Copyright (c) 2018, 2019 Hartmut Grawe

    This file is part of ErsatzMoco.

    ErsatzMoco is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ErsatzMoco is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ErsatzMoco.  If not, see <http://www.gnu.org/licenses/>.

    Diese Datei ist Teil von ErsatzMoco.

    ErsatzMoco ist Freie Software: Sie können es unter den Bedingungen
    der GNU General Public License, wie von der Free Software Foundation,
    Version 3 der Lizenz oder (nach Ihrer Wahl) jeder neueren
    veröffentlichten Version, weiter verteilen und/oder modifizieren.

    ErsatzMoco wird in der Hoffnung, dass es nützlich sein wird, aber
    OHNE JEDE GEWÄHRLEISTUNG, bereitgestellt; sogar ohne die implizite
    Gewährleistung der MARKTFÄHIGKEIT oder EIGNUNG FÜR EINEN BESTIMMTEN ZWECK.
    Siehe die GNU General Public License für weitere Details.

    Sie sollten eine Kopie der GNU General Public License zusammen mit diesem
    Programm erhalten haben. Wenn nicht, siehe <https://www.gnu.org/licenses/>
*/

#include "../include/Sercom_Module.hpp"

/** @brief Communication class handling stable serial messaging protocol.
  *
  * Keep in mind: This may also be used with ZigBee radio modules in transparent mode. Uses Serial library.
  * You must provide the number of the serial port, a baud rate and a callback function for alerts thrown in case requested acknowledge answers are missing.
  * The class can deal with different serial port configurations of different boards, see "init" method.
  * Serial configuration is always the common "8N1": 8 bits, no parity, 1 stop bit.
  *
  * @see ComModule, RFM69_Module, MocoMessage
  */
Sercom_Module::Sercom_Module(int sernum, int baud, void(*cf)(MocoMessage)) : ComModule(),
    baud_(baud),
    readingMessage_(false),
    messageComplete_(false),
    charCounter_(0),
    introCounter_(0),
    outroCounter_(0)
{
    //ctor
    type_ = ComModuleType::SERCOM;
    alertCallback_ = cf;
    ser_num_ = std::max(MAX_SER, sernum);
    strcpy(lastSent_, "empty");
    strcpy(lastReceived_, "empty");
    lastAckCheck_ = millis();
    lastExpectCheck_ = millis();
    strcpy(intro_, "@EM:");
    strcpy(outro_, ":ME@");
}

Sercom_Module::~Sercom_Module()
{
    //dtor
}

/** @brief Init serial port.
  *
  * The class can deal with different serial port configurations of different boards.
  *
  */
void Sercom_Module::init()
{
    switch(ser_num_)
    {
        case 0:
            Serial.begin(baud_);
            break;
        #if defined(__AVR_ATSAM3X8E__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATSAMD21G18A__)// Due, Zero
            case 1:
                Serial1.begin(baud_);
                break;
            case 2:
                Serial2.begin(baud_);
                break;
            case 3:
                Serial3.begin(baud_);
                break;
        #endif
        #if defined(__AVR_ATSAMD21G18A__) // zero
            case 4:
                Serial4.begin(baud_);
                break;
            case 5:
                Serial5.begin(baud_);
                break;
        #endif
    }

    lastAckCheck_ = millis();
    lastExpectCheck_ = millis();
}

/** @brief Send string data (converted to MocoMessage before raw send)
  *
  * @see ComModule, MocoMessage
  */
void Sercom_Module::send(int cmd, bool ack, char* data, int rc, int to)
{
    ComModule::send(cmd, ack, data, rc, to);
}

/** @brief Send numerical data (converted to MocoMessage before raw send)
  *
  * @see ComModule, MocoMessage
  */
void Sercom_Module::send(int cmd, bool ack, long data, int rc, int to)
{
    ComModule::send(cmd, ack, data, rc, to);
}

/** @brief Wrapper to deal with different Serial ports.
  *
  * Board compatibility checked at compile time. If you change the port number during runtime, make sure it meets the board configuration, otherwise it will be ignored.
  *
  */
char Sercom_Module::doRead()
{
    int myc;

    switch(ser_num_)
    {
        case 0:
            myc = Serial.read();
            break;
        #if defined(__AVR_ATSAM3X8E__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATSAMD21G18A__)// Due, Zero
            case 1:
                myc = Serial1.read();
                break;
            case 2:
                myc = Serial2.read();
                break;
            case 3:
                myc = Serial3.read();
                break;
        #endif
        #if defined(__AVR_ATSAMD21G18A__) // zero
            case 4:
                myc = Serial4.read();
                break;
            case 5:
                myc = Serial5.read();
                break;
        #endif
    }

    return (char)myc;
}

/** @brief Check for incoming data / number of bytes available in buffer.
  *
  * Board compatibility checked at compile time. If you change the port number during runtime, make sure it meets the board configuration, otherwise it will be ignored.
  *
  */
int Sercom_Module::getAvailable()
{
    int numbytes;

    switch(ser_num_)
    {
        case 0:
            numbytes = Serial.available();
            break;
        #if defined(__AVR_ATSAM3X8E__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATSAMD21G18A__)// Due, Zero
            case 1:
                numbytes = Serial1.available();
                break;
            case 2:
                numbytes = Serial2.available();
                break;
            case 3:
               numbytes = Serial3.available();
                break;
        #endif
        #if defined(__AVR_ATSAMD21G18A__) // zero
            case 4:
                numbytes = Serial4.available();
                break;
            case 5:
                numbytes = Serial5.available();
                break;
        #endif
    }

    return numbytes;
}


/** @brief Raw send using Serial library.
  *
  * Board compatibility checked at compile time. If you change the port number during runtime, make sure it meets the board configuration, otherwise it will be ignored.
  *
  * @see ComModule, MocoMessage
  */
void Sercom_Module::send(char* rpacket)
{
    switch(ser_num_)
    {
        case 0:
            if(Serial)
                Serial.write(rpacket);
            break;
        #if defined(__AVR_ATSAM3X8E__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATSAMD21G18A__)// Due, Zero
            case 1:
                if(Serial1)
                    Serial1.write(rpacket);
                break;
            case 2:
                if(Serial2)
                    Serial2.write(rpacket);
                break;
            case 3:
                if(Serial3)
                    Serial3.write(rpacket);
                break;
        #endif
        #if defined(__AVR_ATSAMD21G18A__) // zero
            case 4:
                if(Serial4)
                    Serial4.write(rpacket);
                break;
            case 5:
                if(Serial5)
                    Serial5.write(rpacket);
                break;
        #endif
    }
}

/** @brief Receive data and parse raw data as MocoMessage.
  *
  * Receiving asynchronous data is messy, but this method handles it very stable. The only strings that must never be sent as payload are "@EM:" and ":ME@".
  *
  * @see ComModule, MocoMessage
  */
bool Sercom_Module::receive()
{
    int numbytes = getAvailable();

    if(numbytes < 1)
        return false;

    char nextByte;

    for (int i=0; i<numbytes; i++)
    {
        nextByte = doRead();
        if(!readingMessage_)
        {
            if(nextByte == intro_[introCounter_])
            {
                introCounter_++;
            }
            else
            {
                introCounter_ = 0;
            }

            if(introCounter_ == 4)
            {
                readingMessage_ = true;
                introCounter_ = 0;
                charCounter_ = 4;
                strcpy(lastReceived_, "@EM:");
            }
        }
        else
        {
            if(charCounter_ < 39)
            {
                lastReceived_[charCounter_] = nextByte;
                if(nextByte == outro_[outroCounter_])
                {
                    outroCounter_++;
                }
                else
                {
                    outroCounter_ = 0;
                }

                if(outroCounter_ == 4)
                {
                    readingMessage_ = false;
                    messageComplete_ = true;
                    lastReceived_[charCounter_+1] = 0;
                    outroCounter_ = 0;
                    charCounter_ = 0;
                    break;
                }
                charCounter_++;
            }
            else
            {
                readingMessage_ = false;
                messageComplete_ = false;
                lastReceived_[39] = 0;
                outroCounter_ = 0;
                charCounter_ = 0;
                break;
            }
        }
    }


    if(!messageComplete_)
    {
        return false;
    }

    messageComplete_ = false;
    bool found = false;

    //try to read the message
    MocoMessage mm = MocoMessage(lastReceived_);
    if(mm.errorCode > 0) //decoding not successful
    {
        return false;
    }

    if(mm.getMsgCommand() == (int)MocoCommand::EM_ACK)
    {
        if(!expectAckBuffer_.empty())
        {
            int ctr = 0;
            for(int i = 0; i < expectAckBuffer_.size(); i++)
            {
                MocoMessage& am = expectAckBuffer_.at(i);
                if(am.getMsgID() == mm.getMsgID())
                {
                    expectAckBuffer_.erase(expectAckBuffer_.begin()+ctr);
                    break;
                }
                ctr++;
            }
        }
        //ack received and automatically handled, so no further action outside this
        //module required
        return false;
    }

    //check if message has been resent only while it is already registered
    if(!receiveBuffer_.empty())
    {
        for(int i = 0; i < receiveBuffer_.size(); i++)
        {
            MocoMessage& known = receiveBuffer_.at(i);
            if(known.getMsgID() == mm.getMsgID())
            {
                found = true;
                break;
            }
        }
    }

    if(!found)
    {
        if(receiveBuffer_.size() >= RECBUFFER_SIZE)
        {
            receiveBuffer_.erase(receiveBuffer_.begin());
        }

        if(mm.ackRequired())
        {
            mm.setResendCtr(3); //send ack three times
            mm.setTimeout(ACK_TIMEOUT);
        }
        receiveBuffer_.push_back(mm);
        return true;
    }
    else
        return false;
}

/** @brief Set new serial port number. Make sure your board supports it.
  *
  */
void Sercom_Module::setSerial(int sernum)
{
    ser_num_ = std::max(MAX_SER, sernum);
    init();
}

/** @brief Get number of current serial port.
  *
  */
int Sercom_Module::getSerial()
{
    return ser_num_;
}

/** @brief Get highest possible serial port number. Detected at compile time in the header.
  *
  */
int Sercom_Module::getMaxSer()
{
    return MAX_SER;
}

/** @brief Set new baud rate.
  *
  */
void Sercom_Module::setBaud(int baud)
{
    baud_ = baud;
    init();
}

/** @brief Get current baud rate.
  *
  */
int Sercom_Module::getBaud()
{
    return baud_;
}



