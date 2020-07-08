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

#include "../include/RFM69_Module.hpp"

/** @brief Communication class interfacing with RFM69 radio module.
  *
  * Uses RFM69 library from lowpowerlab. Set communication parameters in header of this module and RFM69 library.
  *
  * @see ComModule, Sercom_Module, MocoMessage
  */
RFM69_Module::RFM69_Module(int node, int rec, int netid, void(*cf)(MocoMessage)) : ComModule(),
    radio_(),
    nodeid_(node),
    receiver_(rec),
    netid_(netid),
    RSSI_(0)
{
    //ctor
    alertCallback_ = cf;
    type_ = ComModuleType::RFM69;
    strcpy(lastSent_, "empty");
    strcpy(lastReceived_, "empty");
    lastAckCheck_ = millis();
    lastExpectCheck_ = millis();
}

RFM69_Module::~RFM69_Module()
{
    //dtor
}

/** @brief Initialize module. et communication parameters in header of this module and RFM69 library.
  *
  */
void RFM69_Module::init()
{
    radio_ = RFM69(RFM69_CS, RFM69_IRQ, IS_RFM69HCW, RFM69_IRQN);
    // Hard Reset the RFM module
    pinMode(RFM69_RST, OUTPUT);
    digitalWrite(RFM69_RST, LOW);

    // manual reset
    digitalWrite(RFM69_RST, HIGH);
    delay(100);
    digitalWrite(RFM69_RST, LOW);
    delay(100);

    while(!radio_.initialize(FREQUENCY, nodeid_, netid_))
    {
        Serial.println("try to init...");
        delay(100);
    }
    Serial.println("RADIO initialized!");

    if (IS_RFM69HCW)
    {
        Serial.println("is HCW - "+String(FREQUENCY)+" "+String(nodeid_)+" "+String(netid_));
        Serial.println("frequency: "+String(getRadio().getFrequency()));
        radio_.setHighPower(); // Only for RFM69HCW & HW!
    }
    radio_.setPowerLevel(31); // power output ranges from 0 (5dBm) to 31 (20dBm)
    radio_.encrypt(ENCRYPTKEY);

    lastAckCheck_ = millis();
    lastExpectCheck_ = millis();
}

/** @brief Access RFM69 library directly: Returns library object.
  *
  */
RFM69 RFM69_Module::getRadio()
{
    return radio_;
}

/** @brief Get receiver ID.
  *
  */
int RFM69_Module::getReceiver()
{
    return receiver_;
}

/** @brief Set receiver ID.
  *
  */
void RFM69_Module::setReceiver(int myrec)
{
    receiver_ = myrec;
}

/** @brief Get own (sender) ID.
  *
  */
int RFM69_Module::getID()
{
    return nodeid_;
}

/** @brief Set own (sender) ID.
  *
  */
void RFM69_Module::setID(int myid)
{
    nodeid_ = myid;
    radio_.setAddress(nodeid_);
}

/** @brief Get network ID.
  *
  */
int RFM69_Module::getNetworkID()
{
    return netid_;
}

/** @brief Set network ID.
  *
  */
int RFM69_Module::setNetworkID(int myid)
{
    netid_ = myid;
    radio_.setNetwork(netid_);
}

/** @brief Send string data (converted to MocoMessage before raw send)
  *
  * @see ComModule, MocoMessage
  */
void RFM69_Module::send(int cmd, bool ack, char* data, int rc, int to)
{
    ComModule::send(cmd, ack, data, rc, to);
}

/** @brief Send numerical data (converted to MocoMessage before raw send)
  *
  * @see ComModule, MocoMessage
  */
void RFM69_Module::send(int cmd, bool ack, long data, int rc, int to)
{
    ComModule::send(cmd, ack, data, rc, to);
}

/** @brief Raw send using RFM69 library.
  *
  * @see ComModule, MocoMessage
  */
void RFM69_Module::send(char* rpacket)
{
    radio_.send(receiver_, rpacket, strlen(rpacket)+1, false);
}

/** @brief Receive data and parse raw data as MocoMessage.
  *
  * @see ComModule, MocoMessage
  */
bool RFM69_Module::receive()
{
    bool found = false;

    if (radio_.receiveDone())
    {
        RSSI_ = radio_.RSSI; //save current signal strength

        if (!strstr((char *)radio_.DATA, "@EM"))
        {
            //Serial.println("Message does not start with @EM");
            return false; //no valid moco message
        }

        if(strlen((char *)radio_.DATA) > 40)
        {
            //Serial.println("Message too long");
            return false; //no valid moco message - too long
        }

        strcpy(lastReceived_, (char*)radio_.DATA);

        //try to read the message
        MocoMessage mm = MocoMessage(lastReceived_);

        if(mm.errorCode > 0)
        {
            //Serial.println("RECEIVE ERROR");
            strcpy(lastReceived_, '\0');
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
                    //Serial.println("old message found");
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
    else
    {
        return false;
    }
}

/** @brief Retrieve signal strength (associated with last data receive).
  *
  */
int RFM69_Module::getRSSI()
{
    return RSSI_;
}

