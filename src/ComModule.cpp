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

#include "../include/ComModule.hpp"

/** @brief Abstract base class for any communication module, e.g. RFM69, serial et al.
  *
  * @see RFM69_Module, Sercom_Module, MocoMessage
  */
ComModule::ComModule() : type_(ComModuleType::UNDEFINED),
    expectAckBuffer_ {},
    receiveBuffer_ {}
{
    //ctor
}

ComModule::~ComModule()
{
    //dtor
}

/** @brief Send message based on a MocoMessage object.
  *
  * @see MocoMessage
  */
void ComModule::send(MocoMessage mm)
{
    send(mm.toString());
}

/** @brief Send message based on a MocoMessage object reference.
  *
  * @see MocoMessage
  */
void ComModule::sendRef(MocoMessage& mm)
{
    send(mm.toString());
}

/** @brief Send message based on a command number, acknowledge switch, data string, repeat counter and timeout.
  *
  * @see MocoMessage, MocoCommand
  */
void ComModule::send(int cmd, bool ack, char* data, int rc, int to)
{
    MocoMessage mm (cmd, ack, data, rc, to);
    if(ack && (expectAckBuffer_.size() < ACKBUFFER_SIZE))
    {
        expectAckBuffer_.push_back(mm);
    }
    send(mm.toString());
}

/** @brief Send message based on a command number, acknowledge switch, number data, repeat counter and timeout.
  *
  * @see MocoMessage, MocoCommand
  */
void ComModule::send(int cmd, bool ack, long data, int rc, int to)
{
    MocoMessage mm (cmd, ack, data, rc, to);
    if(ack && (expectAckBuffer_.size() < ACKBUFFER_SIZE))
    {
        expectAckBuffer_.push_back(mm);
    }
    send(mm.toString());
}

/** @brief Send message based on a MocoMessage object reference. Resend counter is decreased.
  *
  * @see MocoMessage
  */
void ComModule::resend(MocoMessage& mm)
{
    sendRef(mm);
    mm.decreaseResendCtr();
}

/** @brief Message that required an acknowledge did not receive ack message - signal this in some way.
  *
  * @see MocoMessage
  */
void ComModule::handleAlert(MocoMessage amm)
{
    if(alertCallback_ != nullptr)
    {
        alertCallback_(amm);
    }

}

/** @brief Handle asynchronous message system (control acknowledge stuff etc).
  *
  * @see MocoMessage
  */
void ComModule::update()
{
    //care for sent messages that have not been acknowleged so far
    if(!expectAckBuffer_.empty())
    {
        for(int i = 0; i < expectAckBuffer_.size(); i++)
        {
            MocoMessage& mm = expectAckBuffer_.at(i);
            if(millis() > (mm.getTimeout()+lastExpectCheck_))
            {
                if (mm.getResendCtr() > 0)
                {
                    Serial.println("Resend!");
                    Serial.println(mm.getMsgData());
                    resend(mm); //ctr decremented in sub method
                }
                else
                {
                    Serial.println("Alert");
                    handleAlert(mm);
                }
                lastExpectCheck_ = millis();
            }
        }
    }

    //erase outdated (lost) ack expectations
    if(!expectAckBuffer_.empty())
    {
        for(int i = expectAckBuffer_.size() - 1; i >= 0; i--)
        {
            if(expectAckBuffer_.at(i).getResendCtr() < 1)
            {
                Serial.println("ACK never received");
                expectAckBuffer_.erase(expectAckBuffer_.begin() + i);
            }
        }
    }

    //care for received messages that require ack
    if(!receiveBuffer_.empty())
    {
        for(int i = 0; i < receiveBuffer_.size(); i++)
        {
            MocoMessage& mm = receiveBuffer_.at(i);
            if(mm.ackRequired() && (millis() >= (mm.getTimeout()+lastAckCheck_)))
            {
                if(mm.getResendCtr() > 0)
                {
                    MocoMessage ackmm = MocoMessage(mm.getMsgID());
                    send(ackmm);
                    mm.decreaseResendCtr();
                }
                lastAckCheck_ = millis();
            }
        }
    }

    //delete expired messages that several acks have been sent for
    if(!receiveBuffer_.empty())
    {
        for(int i = receiveBuffer_.size() - 1; i >= 0; i--)
        {
            if((receiveBuffer_.at(i).ackRequired()) && (receiveBuffer_.at(i).getResendCtr() < 1))
            {
                //Serial.println("ACK sent - delete marker for:");
                //Serial.println(receiveBuffer_.at(i).getMsgID());
                receiveBuffer_.erase(receiveBuffer_.begin() + i);
            }
        }
    }
}

void ComModule::flushExpectAckBuffer()
{
    expectAckBuffer_.clear();
}

void ComModule::flushReceiveBuffer()
{
    receiveBuffer_.clear();
}

char* ComModule::getLastReceived()
{
    return lastReceived_;
}

char* ComModule::getLastSent()
{
    return lastSent_;
}
