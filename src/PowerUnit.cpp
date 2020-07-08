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

#include "../include/PowerUnit.hpp"

/** @brief ActionElement controlling a DC motor.
  *
  * @see ActionElement, ActionElementType
  */
PowerUnit::PowerUnit(char* aname, char* drivername)
{
    //ctor
    dir_isCW_ = true;
    speed_ = 0;
    type_ = ActionElementType::POWERUNIT;
    strncpy (name_, aname, 31);
    name_[31] = '\0';
    motor_ = DCMotor(aname, drivername);
}

PowerUnit::~PowerUnit()
{
    //dtor
}

/** @brief Report current status of this unit.
  *
  * @todo Implement this functionality.
  */
char* PowerUnit::getStatus()
{
    return "&PU:";
}

/** @brief Set direction of rotation.
  *
  */
void PowerUnit::setDirectionCW(bool cw)
{
    dir_isCW_ = cw;
}

/** @brief Get direction of rotation.
  *
  */
bool PowerUnit::getDirectionCW()
{
    return dir_isCW_;
}

/** @brief Set motor speed.
  *
  */
void PowerUnit::setSpeed(int mysp)
{
    speed_ = mysp;
}

/** @brief Get motor speed.
  *
  */
int PowerUnit::getSpeed()
{
    return speed_;
}

/** @brief Handle commands directed to this ActionElement.
  *
  * @see MocoCommand
  */
void PowerUnit::execute(int cmd, char* data)
{
    int cur_speed = speed_;

    switch(cmd)
    {
        case MocoCommand::EM_ACT_DCM_CCW:
            if(dir_isCW_)
            {
                dir_isCW_ = false;
                speed_ = 0;
                motor_.update(dir_isCW_, speed_);
            }
            break;
        case MocoCommand::EM_ACT_DCM_CW:
            if (!dir_isCW_)
            {
                dir_isCW_ = true;
                speed_ = 0;
                motor_.update(dir_isCW_, speed_);
            }
            break;
        case MocoCommand::EM_ACT_DCM_BRAKE:
            speed_ = 0;
            motor_.motorBrake();
            break;
        case MocoCommand::EM_ACT_DCM_STOP:
            speed_ = 0;
            motor_.motorOff();
            break;
        case MocoCommand::EM_ACT_DCM_SPEED:
            int newspeed = atoi(data);
            speed_ = map(newspeed, 0, 100, 0, 255);
            if(cur_speed < speed_)
            {
                for(int i = cur_speed; i <= speed_; i++)
                {
                    motor_.update(dir_isCW_, i);
                }
            }
            else if(cur_speed > speed_)
            {
                for(int i = cur_speed; i >= speed_; i--)
                {
                    motor_.update(dir_isCW_, i);
                }
            }
    }
}
