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

    ErsatzMoco ist Freie Software: Sie k�nnen es unter den Bedingungen
    der GNU General Public License, wie von der Free Software Foundation,
    Version 3 der Lizenz oder (nach Ihrer Wahl) jeder neueren
    ver�ffentlichten Version, weiter verteilen und/oder modifizieren.

    ErsatzMoco wird in der Hoffnung, dass es n�tzlich sein wird, aber
    OHNE JEDE GEW�HRLEISTUNG, bereitgestellt; sogar ohne die implizite
    Gew�hrleistung der MARKTF�HIGKEIT oder EIGNUNG F�R EINEN BESTIMMTEN ZWECK.
    Siehe die GNU General Public License f�r weitere Details.

    Sie sollten eine Kopie der GNU General Public License zusammen mit diesem
    Programm erhalten haben. Wenn nicht, siehe <https://www.gnu.org/licenses/>
*/

#ifndef COMMODULE_H
#define COMMODULE_H

#include <Arduino.h>
#include <vector>
#include "MocoMessage.hpp"

#define RECBUFFER_SIZE 10
#define ACKBUFFER_SIZE 10
#define ACK_TIMEOUT 200 //repeat ack message every ... milliseconds

enum class ComModuleType
{
    UNDEFINED,
    RFM69,
    XBEE,
    SERCOM
};

class ComModule
{
    public:
        ComModule();
        virtual ~ComModule();

        void flushExpectAckBuffer();
        void flushReceiveBuffer();

        char* getLastReceived();
        char* getLastSent();

        virtual void init() = 0;
        virtual void send(char* rpacket) = 0;
        virtual bool receive() = 0;

        void send(MocoMessage mm);
        void sendRef(MocoMessage& mm);
        void send(int cmd, bool ack, char* data, int rc = 1, int to = 10000);
        void send(int cmd, bool ack, long data, int rc = 1, int to = 10000);
        void resend(MocoMessage& mm);

        void handleAlert(MocoMessage mm);
        void update();

        std::vector<MocoMessage> receiveBuffer_;
        std::vector<MocoMessage> expectAckBuffer_;

    protected:
        ComModuleType type_;

        char lastSent_[40];
        char lastReceived_[40];

        void(*alertCallback_)(MocoMessage);
        long lastAckCheck_;
        long lastExpectCheck_;

    private:
};

#endif // COMMODULE_H
