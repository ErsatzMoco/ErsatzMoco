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

#include "../include/Rudder.hpp"

/** @brief ActionElement controlling a steering servo.
  *
  * @see ActionElement, ActionElementType
  */
Rudder::Rudder(char* aname) : servo_()
{
    //ctor
    angle_ = 0;
    type_ = ActionElementType::RUDDER;
    strncpy (name_, aname, 31);
    name_[31] = '\0';
}

Rudder::~Rudder()
{
    //dtor
}

/** @brief Invoke Servo library with standard pin.
  *
  */
void Rudder::initRudder()
{
    servo_ = Servo();
    servo_.attach(9, 600, 2400);
}

/** @brief Invoke Servo library providing the connection pin.
  *
  */
void Rudder::initRudder(int mypin)
{
    servo_ = Servo();
    servo_.attach(mypin, 600, 2400);
}

/** @brief Invoke Servo library with custom values (pin, pwm min/max).
  *
  */
void Rudder::initRudder(int mypin, int pwmmin, int pwmmax)
{
    servo_ = Servo();
    servo_.attach(mypin, pwmmin, pwmmax);
}

/** @brief Report current status of this unit.
  *
  * @todo Implement this functionality.
  */
char* Rudder::getStatus()
{
    return "&RD:";
}

/** @brief Set rudder angle in degrees.
  *
  */
void Rudder::setAngle(int mya)
{
    angle_ = constrain(mya, 0 , 180);
}

/** @brief Get rudder angle.
  *
  */
int Rudder::getAngle()
{
    return angle_;
}

/** @brief Handle commands directed to this ActionElement.
  *
  * @see MocoCommand
  */
void Rudder::execute(int cmd, char* data)
{
    switch(cmd)
    {
        case MocoCommand::EM_ACT_SERVO_ANGLE:
            int newangle = atoi(data);
            angle_ = map(newangle, 1, 100, 20, 160);
            servo_.write(angle_);
            //Serial.println("Rudder: " + String(angle_));
    }
}
