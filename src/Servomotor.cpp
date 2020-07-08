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

#include "../include/Servomotor.hpp"

/** @brief Continuous rotation servo hardware wrapper to use a modified servo as a motor.
  *
  * Note: In many cases, even a single servo needs a separate power source and should be invoked by a relay. Attempting to power a servo from the microcontroller board will fail in many cases, and definitely if you have more than one servo/servo motor.
  *
  * @see ActionElement, ActionElementType
  */
Servomotor::Servomotor(char* aname) : servo_()
{
    //ctor
    speed_ = 0;
    type_ = ActionElementType::SERVOMOTOR;
    strncpy (name_, aname, 31);
    name_[31] = '\0';
}

Servomotor::~Servomotor()
{
    //dtor
}

/** @brief Invoke Servo library with standard pin and pin 1 for the relay.
  *
  */
void Servomotor::initServomotor()
{
    servo_ = Servo();
    servo_.attach(9, 600, 2400);
    relay_ = 1;
    pinMode(relay_, OUTPUT);
    digitalWrite(relay_, LOW);
}

/** @brief Invoke Servo library providing the Servo library connection pin and a pin for the relay.
  *
  */
void Servomotor::initServomotor(int mypin, int relaypin)
{
    servo_ = Servo();
    servo_.attach(mypin, 600, 2400);
    relay_ = relaypin;
    pinMode(relay_, OUTPUT);
    digitalWrite(relay_, LOW);
}

/** @brief Invoke Servo library with custom values (servo pin, relay pin, pwm min/max).
  *
  */
void Servomotor::initServomotor(int mypin, int relaypin, int pwmmin, int pwmmax)
{
    servo_ = Servo();
    servo_.attach(mypin, pwmmin, pwmmax);
    relay_ = relaypin;
    pinMode(relay_, OUTPUT);
    digitalWrite(relay_, LOW);
}

/** @brief Report current status of this unit.
  *
  * @todo Implement this functionality.
  */
char* Servomotor::getStatus()
{
    return "&RD:";
}

/** @brief Set motor speed by providing an "angle" value to the Servo library.
  *
  */
void Servomotor::setSpeed(int mys)
{
    speed_ = constrain(mys, 0, 180);
}

/** @brief Get motor speed (servo "angle" value).
  *
  */
int Servomotor::getSpeed()
{
    return speed_;
}

/** @brief Handle commands directed to this ActionElement.
  *
  * Note: Via MocoCommand this unit expects speed values as a percentage (as opposed to the low level functions providing an "angle" value).
  *
  * @see MocoCommand
  */
void Servomotor::execute(int cmd, char* data)
{
    switch(cmd)
    {
        case MocoCommand::EM_ACT_SERVOMOTOR_SPEED:
            int newspeed = atoi(data);
            if(newspeed == 50)
            {
                speed_ = 90;
                digitalWrite(relay_, LOW);
                servo_.write(speed_);
            }
            else
            {
                speed_ = map(newspeed, 1, 100, 0, 180);
                servo_.write(speed_);
                digitalWrite(relay_, HIGH);
                delay(500);
                servo_.write(speed_);
                //Serial.println("Servomotor: " + String(speed_));
            }
    }
}
