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

#ifndef DCMOTOR_H
#define DCMOTOR_H

#include <Arduino.h>
#include "Actuator.hpp"

enum class DCMotorType
{
    TB6612,
    MAX14870,
    DRV8838
};


class DCMotor : public Actuator
{
    public:
        DCMotor();
        DCMotor(char* aname, char* drivername);
        virtual ~DCMotor();

        void update() override;
        void update(bool iscw, int myspeed);
        void motorOff();
        void motorBrake();

    protected:
        int pin_PWM_;
        int pin_Line1_;
        int pin_Line2_;
        DCMotorType driverType_;

    private:
};

#endif // DCMOTOR_H
