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

#include "../include/DCMotor.hpp"

/** @brief DC motor hardware wrapper that can handle several driver chips.
  *
  * Currently supported: MAX 14870 and TB 6612 drivers.
  *
  * @see Actuator, ActuatorType
  */
DCMotor::DCMotor()
{

}

DCMotor::DCMotor(char* aname, char* drivername)
{
    //ctor
    type_ = ActuatorType::DCMOTOR;
    strncpy (name_, aname, 31);
    name_[31] = '\0';

    if(strcmp(drivername, "TB6612") == 0)
    {
        driverType_ = DCMotorType::TB6612;
    }
    else if(strcmp(drivername, "MAX14870") == 0)
    {
        driverType_ = DCMotorType::MAX14870;
    }
    else
    {
        driverType_ = DCMotorType::TB6612;
    }

    pin_PWM_ = 5;
    switch(driverType_)
    {
        case DCMotorType::TB6612:
            pin_Line1_ = 20;
            pinMode(pin_Line1_, OUTPUT);
            break;

        case DCMotorType::MAX14870:
            pin_Line1_ = 0;
            break;
    }

    pin_Line2_ = 21;
    pinMode(pin_Line2_, OUTPUT);
}

DCMotor::~DCMotor()
{
    //dtor
}

/** @brief Set driver state according to boolean direction switch (true = CW) and speed (PWM value).
  *
  */
void DCMotor::update(bool iscw, int myspeed)
{
    analogWrite(pin_PWM_, myspeed);

    switch(driverType_)
    {
        case DCMotorType::TB6612:
            if(iscw)
            {
                digitalWrite(pin_Line1_, HIGH);
                digitalWrite(pin_Line2_, LOW);
            }
            else
            {
                digitalWrite(pin_Line1_, LOW);
                digitalWrite(pin_Line2_, HIGH);
            }
            break;

        case DCMotorType::MAX14870:
            if(iscw)
            {
                digitalWrite(pin_Line2_, HIGH);
            }
            else
            {
                digitalWrite(pin_Line2_, LOW);
            }
            break;
    }

}

void DCMotor::update()
{

}

/** @brief Switch motor off.
  *
  */
void DCMotor::motorOff()
{
    analogWrite(pin_PWM_, 0);

    switch(driverType_)
    {
        case DCMotorType::TB6612:
            digitalWrite(pin_Line1_, LOW);
            digitalWrite(pin_Line2_, LOW);
            break;

        case DCMotorType::MAX14870:
            digitalWrite(pin_PWM_, LOW);
            break;
    }

}

/** @brief Slow motor down.
  *
  */
void DCMotor::motorBrake()
{
    analogWrite(pin_PWM_, 0);

    switch(driverType_)
    {
        case DCMotorType::TB6612:
            digitalWrite(pin_Line1_, HIGH);
            digitalWrite(pin_Line2_, HIGH);
            delay(100);
            digitalWrite(pin_Line1_, LOW);
            digitalWrite(pin_Line2_, LOW);
            break;

        case DCMotorType::MAX14870:
            digitalWrite(pin_PWM_, LOW);
            break;
    }

}
