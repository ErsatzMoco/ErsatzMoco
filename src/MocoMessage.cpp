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

#include "../include/MocoMessage.hpp"

/** @brief Central communication protocol element.
  *
  * MocoMessage contains start and stop indicators, a command, a unique id, an acknowledge indicator and data payload. All parts are delimited with a colon ":"
  * Example:  "@EM:20:0025:0:Hello World:ME@"
  * This message string tells the receiver to display the text "Hello World". It uses MocoCommand EM_DISP_MSG (at the time of writing decimal 20), has the unique ID number 0025 (hex) and does not require an acknowledge message, indicated by "0" preceding the data payload.
  * The rather long start and stop indicators ensure a stable communication by allowing the transmit of any numeric data and any string except "@EM", "ME@" and the colon ":".
  * To ensure your messaging system works with all subsequent versions of ErsatzMoco, use the enum names of commands only in your program. Never use integer literals for this purpose, since the MocoCommand enum list will be extended for sure.
  *
  * @see MocoCommand, ComModule, RFM69_Module, Sercom_Module
  * @todo Implement receiver address to allow for broadcast messaging.
  */
MocoMessage::MocoMessage()
{
    //ctor
    strcpy(msgHeader_, "@EM");
    msgID_ = millis();
    msgRequireAck_ = false;

    timeout_ = 10000;
    resendCtr_ = 0;

    errorCode = 0;
}

/** @brief Construct moco message from a raw data buffer (char array).
  *
  */
MocoMessage::MocoMessage(char* data)
{
    char* token;
    char* dummyptr;
    errorCode = 0;

    int endseq = strlen(data) - 3;
    if(strcmp(data+endseq, "ME@") != 0)
    {
        errorCode = 99; //no valid MocoCommand
        Serial.println(errorCode);
        Serial.println(data);
        return;
    }

    token = strtok(data, ":");
    if(token != nullptr)
    {
        if(strcmp(data, "@EM") != 0)
        {
            errorCode = 98; //no valid MocoCommand
            Serial.println(errorCode);
            Serial.println(data);
        }

        token = strtok(nullptr, ":");
        if(token == nullptr)
        {
            errorCode = 97; //no valid MocoCommand
            Serial.println(errorCode);
            Serial.println(data);
        }
        msgCommand_ = strtol(token, &dummyptr, 10);

        token = strtok(nullptr, ":");
        if(token == nullptr)
        {
            errorCode = 96; //no valid MocoCommand
            Serial.println(errorCode);
            Serial.println(data);
        }
        msgID_ = strtol(token, &dummyptr, 16);

        token = strtok(nullptr, ":");
        if(token == nullptr)
        {
            errorCode = 95; //no valid MocoCommand
            Serial.println(errorCode);
            Serial.println(data);
        }
        msgRequireAck_ = (strtol(token, &dummyptr, 10) != 0);

        token = strtok(nullptr, ":");
        if(token == nullptr)
        {
            Serial.println("cmd w_o payload");
            return; //MocoCommand w/o payload
        }

        if(strlen(data) < 19)
        {
            strcpy(msgData_, token); //copy payload
        }
        else
        {
            errorCode = 94; //no valid MocoCommand - payload too long
            Serial.println(errorCode);
            Serial.println(data);
        }
    }
    else
    {
        errorCode = 93; //no valid MocoCommand
        Serial.println(errorCode);
        Serial.println(data);
    }
}

/** @brief Construct moco message by providing command number (enum name!), acknowledge indicator, data c-string, resend counter and timeout.
  *
  */
MocoMessage::MocoMessage(int cmd, bool ack, char* data, int rc, int to)
{
    //ctor
    strcpy(msgHeader_, "@EM");
    msgID_ = millis();
    timeout_ = to;
    resendCtr_ = rc;
    msgCommand_ = cmd;
    msgRequireAck_ = ack;
    strncpy (msgData_, data, 18);
    msgData_[18] = '\0';
}

/** @brief Construct moco message by providing command number (enum name!), acknowledge indicator, numerical integer data, resend counter and timeout.
  *
  */
MocoMessage::MocoMessage(int cmd, bool ack, long data, int rc, int to)
{
    //ctor
    strcpy(msgHeader_, "@EM");
    msgID_ = millis();
    timeout_ = to;
    resendCtr_ = rc;
    msgCommand_ = cmd;
    msgRequireAck_ = ack;
    itoa(data, msgData_, 10);
}

/** @brief Construct acknowledge message.
  *
  */
MocoMessage::MocoMessage(long id)
{
    //ctor for ack message
    strcpy(msgHeader_, "@EM");
    timeout_ = 10000;
    resendCtr_ = 0;

    msgCommand_ = MocoCommand::EM_ACK;
    msgRequireAck_ = false;
    msgID_ = id;
    itoa(0, msgData_, 10);
}

MocoMessage::~MocoMessage()
{
    //dtor
}

/** @brief Serialize object into character data string.
  *
  */
char* MocoMessage::toString()
{
    char buf[6];

    strcpy(strBuffer_, "");
    strcat(strBuffer_, "@EM:");
    strncat(strBuffer_, itoa(msgCommand_, buf, 10), 2);
    strcat(strBuffer_, ":");
    strncat(strBuffer_, ltoa(msgID_, buf, 16), 4);
    strcat(strBuffer_, ":");
    if(msgRequireAck_)
        strcat(strBuffer_, "1");
    else
        strcat(strBuffer_, "0");
    strcat(strBuffer_, ":");
    strcat(strBuffer_, msgData_);
    strcat(strBuffer_, ":");
    strcat(strBuffer_, "ME@");

    return strBuffer_;
}

/** @brief Change command this message object holds.
  *
  */
void MocoMessage::setMsgCommand(int cmd)
{
    msgCommand_ = cmd;
}

/** @brief Set acknowledge indicator.
  *
  */
void MocoMessage::setRequireAck(bool ack)
{
    msgRequireAck_ = ack;
}

/** @brief Set data payload character string.
  *
  */
void MocoMessage::setMsgData(char* data)
{
    strncpy (msgData_, data, 18);
    msgData_[18] = '\0';
}

/** @brief Set number of resend tries.
  *
  */
void MocoMessage::setResendCtr(int ctr)
{
    resendCtr_ = ctr;
}

/** @brief Set timeout for acknowledge signal.
  *
  */
void MocoMessage::setTimeout(int to)
{
    timeout_ = to;
}

/** @brief Get timeout for acknowledge signal.
  *
  */
int MocoMessage::getTimeout()
{
    return timeout_;
}

/** @brief Retrieve message header.
  *
  */
char* MocoMessage::getMsgHeader()
{
    return msgHeader_;
}

/** @brief Retrieve message command.
  *
  */
int MocoMessage::getMsgCommand()
{
    return msgCommand_;
}

/** @brief Retrieve unique message ID.
  *
  */
unsigned long MocoMessage::getMsgID()
{
    return msgID_;
}

/** @brief Check if this message needs to be confirmed.
  *
  */
bool MocoMessage::ackRequired()
{
    return msgRequireAck_;
}

/** @brief Retrieve data payload of message (c-string)
  *
  */
char* MocoMessage::getMsgData()
{
    return msgData_;
}

/** @brief Retrieve number of send repeats
  *
  */
int MocoMessage::getResendCtr()
{
    return resendCtr_;
}

/** @brief Force decrease of resend counter
  *
  */
void MocoMessage::decreaseResendCtr()
{
    --resendCtr_;
}
