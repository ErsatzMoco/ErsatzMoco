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

#ifndef RUDDER_H
#define RUDDER_H

#include <Arduino.h>
#include <algorithm>
#include "Servo.h"
#include "ActionElement.hpp"
#include "MocoCommand.hpp"


class Rudder : public ActionElement
{
    public:
        Rudder(char* aname);
        virtual ~Rudder();

        virtual char* getStatus() override;

        void initRudder();
        void initRudder(int mypin);
        void initRudder(int mypin, int pwmmin, int pwmmax);
        void setAngle(int mya);
        int getAngle();

        virtual void execute(int cmd, char* data) override;

    protected:
        int angle_;
        Servo servo_;

    private:
};

#endif // RUDDER_H
