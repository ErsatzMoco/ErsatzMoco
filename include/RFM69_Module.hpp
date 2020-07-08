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

#ifndef RFM69_MODULE_H
#define RFM69_MODULE_H

#include "ComModule.hpp"
#include <Arduino.h>
#include "SPI.h"
//#include <RH_RF69.h> //get it here: http://www.airspayce.com/mikem/arduino/RadioHead/
#include <RFM69.h> // //get it here: https://www.github.com/lowpowerlab/rfm69

#define FREQUENCY RF69_868MHZ
#define ENCRYPTKEY "Moco@@Encrypt@@@" //exactly the same 16 characters/bytes on all nodes!
#define IS_RFM69HCW true // set to 'true' if you are using an RFM69HCW module
//*********************************************************************************************

// for Feather M0 Radio
#define RFM69_CS 8
#define RFM69_IRQ 3
#define RFM69_IRQN 3 // Pin 3 is IRQ 3!
#define RFM69_RST 4

class RFM69_Module : public ComModule
{
    public:
        RFM69_Module(int node, int rec, int netid, void(*cf)(MocoMessage));
        virtual ~RFM69_Module();

        void init() override;
        void send(char* rpacket) override;
        bool receive() override;

        void send(int cmd, bool ack, char* data, int rc = 1, int to = 10000);
        void send(int cmd, bool ack, long data, int rc = 1, int to = 10000);

        RFM69 getRadio();
        int getNumPackets();
        int getRSSI();
        int getReceiver();
        void setReceiver(int myrec);
        int getID();
        void setID(int myid);
        int getNetworkID();
        int setNetworkID(int myid);

    protected:
        RFM69 radio_;
        int nodeid_;
        int netid_;
        int receiver_;
        int RSSI_;

    private:
};

#endif // RFM69_MODULE_H
