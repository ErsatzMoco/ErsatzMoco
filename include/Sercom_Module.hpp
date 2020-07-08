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

#ifndef SERCOM_MODULE_H
#define SERCOM_MODULE_H

#include "ComModule.hpp"

#if defined(__AVR_ATSAMD21G18A__) // zero
    #define MAX_SER 5
#elif defined(__AVR_ATSAM3X8E__) // Due
    #define MAX_SER 3
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) // mega, Mega ADK
    #define MAX_SER 3
#else
    #define MAX_SER 0
#endif


class Sercom_Module : public ComModule
{
    public:
        Sercom_Module(int ser_num, int baud, void(*cf)(MocoMessage));
        virtual ~Sercom_Module();

        virtual void init() override;
        virtual void send(char* rpacket) override;
        virtual bool receive() override;

        char doRead();
        int getAvailable();

        void send(int cmd, bool ack, char* data, int rc = 1, int to = 10000);
        void send(int cmd, bool ack, long data, int rc = 1, int to = 10000);

        void setSerial(int sernum);
        int getSerial();
        int getMaxSer();

        void setBaud(int baud);
        int getBaud();

    protected:
        int ser_num_;
        int baud_;
        bool readingMessage_;
        bool messageComplete_;
        int charCounter_;
        int introCounter_;
        int outroCounter_;

        char intro_[5];
        char outro_[5];

    private:
};

#endif // SERCOM_MODULE_H
