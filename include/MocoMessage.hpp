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

#ifndef MOCOMESSAGE_H
#define MOCOMESSAGE_H

#include <Arduino.h>
#include "MocoCommand.hpp"

class MocoMessage
{
    public:
        MocoMessage();
        MocoMessage(char* msg);
        MocoMessage(int cmd, bool ack, char* data, int rc, int to);
        MocoMessage(int cmd, bool ack, long data, int rc, int to);
        MocoMessage(long id);
        virtual ~MocoMessage();

        char* toString();

        void setMsgCommand(int cmd);
        void setRequireAck(bool ack);
        void setMsgData(char* data);
        void setTimeout(int to);
        void setResendCtr(int ctr);
        void decreaseResendCtr();

        char* getMsgHeader();
        int getMsgCommand();
        unsigned long getMsgID();
        bool ackRequired();
        char* getMsgData();

        int getTimeout();
        int getResendCtr();

        int errorCode;

    protected:
        char msgHeader_[4];
        int msgCommand_;
        long msgID_;
        bool msgRequireAck_;
        char msgData_[19];

        int timeout_;
        int resendCtr_; //used for re-sending messages as well as re-sending acknowlegedments

        char strBuffer_[40];

    private:


};

#endif // MOCOMESSAGE_H
