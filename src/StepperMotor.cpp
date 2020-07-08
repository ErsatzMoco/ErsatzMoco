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

#include "../include/StepperMotor.hpp"

/** @brief Stepper motor hardware wrapper that can handle several driver chips.
  *
  * Currently supported: DRV8834 drivers.
  *
  * @see Actuator, ActuatorType
  */
StepperMotor::StepperMotor(char* aname) :
    pin_DIR_(-1),
    pin_STEP_(-1),
    pin_M_List_({-1, -1, -1}),
    pin_ENABLE_(false),
    maxStepsPerSec_(0),
    numStepsCircle_(0)
{
    //ctor
    type_ = ActuatorType::STEPPER;
    strncpy (name_, aname, 31);
    name_[31] = '\0';
}

/** @brief Constructor for setting all parameters by hand.
  *
  * Name, name of driver, pin for direction of rotation, step pin, three microstep mode pins, enable pin.
  *
  * Currently supported: DRV8834 drivers.
  */
StepperMotor::StepperMotor(char* aname, char* drivername, int pdir, int pstep, int m0, int m1, int m2, int ena) :
    pin_DIR_(pdir),
    pin_STEP_(pstep),
    pin_M_List_({m0, m1, m2}),
    pin_ENABLE_(ena),
    maxStepsPerSec_(5000),
    numStepsCircle_(200)
{
    //ctor
    type_ = ActuatorType::STEPPER;
    strncpy (name_, aname, 31);
    name_[31] = '\0';

    initStepper(drivername, pdir, pstep, m0, m1, m2, ena);

}

StepperMotor::~StepperMotor()
{
    //dtor
}

/** @brief Init method to set parameters if standard constuctor was used.
  *
  * Name of driver, pin for direction of rotation, step pin, three microstep mode pins, enable pin.
  *
  * Currently supported: DRV8834 drivers.
  */
void StepperMotor::initStepper(char* drivername, int pdir, int pstep, int m0, int m1, int m2, int ena)
{
    pin_DIR_ = pdir;
    pin_STEP_ = pstep;
    pin_M_List_[0] = m0;
    pin_M_List_[1] = m1;
    pin_M_List_[2] = m2;
    pin_ENABLE_ = ena;
    maxStepsPerSec_ = 5000;
    numStepsCircle_ = 200;

    int mdivider_max;
    int mdivider_default;

    if(strcmp(drivername, "DRV8834") == 0)
    {
        driverType_ = StepperMotorType::DRV8834;
        mdivider_max = 32;
        mdivider_default = 4;
        microStepConfigList_.push_back({1,0,0,-1});
        microStepConfigList_.push_back({2,1,0,-1});
        microStepConfigList_.push_back({4,2,0,-1});
        microStepConfigList_.push_back({8,0,1,-1});
        microStepConfigList_.push_back({16,1,1,-1});
        microStepConfigList_.push_back({32,2,1,-1});
    }
    else
    {
        driverType_ = StepperMotorType::UNDEFINED;
    }
//    else if(strcmp(drivername, "MAX14870") == 0)
//    {
//        driverType_ = DCMotorType::MAX14870;
//    }
//    else
//    {
//        driverType_ = DCMotorType::TB6612;
//    }

    pinMode(pin_DIR_, OUTPUT);
    pinMode(pin_STEP_, OUTPUT);
    digitalWrite(pin_DIR_, HIGH);
    digitalWrite(pin_STEP_, LOW);

    bool m_pins_connected = false;

    for(int i = 0; i < 3; i++)
    {
        int p = pin_M_List_[i];
        if(p > 0)
        {
            m_pins_connected = true; //at least one pin has been set: Microdivider accessible.
        }
    }

    if(pin_ENABLE_ > 0)
        pinMode(pin_ENABLE_, OUTPUT);

    if (m_pins_connected)
    {
        microDivider_ = mdivider_max;
        setMicrostep(microDivider_);
    }
    else
    {
        microDivider_ = mdivider_default;
    }
}

/** @brief Set direction of rotation.
  *
  */
void StepperMotor::setDirection(bool iscw)
{
    if(iscw)
        digitalWrite(pin_DIR_, HIGH);
    else
        digitalWrite(pin_DIR_, LOW);
}

/** @brief Enable stepper.
  *
  * Invokes holding torque of the stepper. Motor starts to use power.
  *
  */
void StepperMotor::enable()
{
    if(pin_ENABLE_ > -1)
        digitalWrite(pin_ENABLE_, HIGH);
}

/** @brief Disable stepper.
  *
  * No holding torque anymore. Motor does not use power, drive shaft runs free.
  *
  */
void StepperMotor::disable()
{
    if(pin_ENABLE_ > -1)
        digitalWrite(pin_ENABLE_, LOW);
}

/** @brief Set microstep mode.
  *
  * Refer to possible configurations of the driver.
  *
  */
bool StepperMotor::setMicrostep(int div)
{
    if(microStepConfigList_.empty())
        return false;

    bool found = false;

    for(int i=0; i < microStepConfigList_.size(); i++)
    {
        std::array<int,4>& cfg = microStepConfigList_.at(i);
        if(cfg[0] == div)
        {
            Serial.println("Microdivider found");
            for(int k = 1; k < 4; k++)
            {
                Serial.print(pin_M_List_[k-1]);
                Serial.print(" > ");
                Serial.println(cfg[k]);
                switch(cfg[k])
                {
                    case 0:
                        pinMode(pin_M_List_[k-1], OUTPUT);
                        digitalWrite(pin_M_List_[k-1], LOW);
                        break;
                    case 1:
                        pinMode(pin_M_List_[k-1], OUTPUT);
                        digitalWrite(pin_M_List_[k-1], HIGH);
                        break;
                    case 2:
                        pinMode(pin_M_List_[k-1], INPUT);
                        break;
                }
            }
            found = true;
            break;
        }
    }

    if(found)
    {
        microDivider_ = div;
        Serial.print("Microdivider: ");
        Serial.println(div);
        return true;
    }
    else
        return false;
}

/** @brief Get largest microstep divider the driver can handle.
  *
  * Refer to possible configurations of the driver.
  *
  */
int StepperMotor::getMaxDivider()
{
    if(microStepConfigList_.empty())
        return 1;

    int maxDiv = 1;

    for(int i=0; i < microStepConfigList_.size(); i++)
    {
        std::array<int,4>& cfg = microStepConfigList_.at(i);
        maxDiv = std::max(maxDiv, cfg[0]);
    }

    return maxDiv;
}

/** @brief Get current microstep divider.
  *
  * Refer to possible configurations of the driver.
  *
  */
int StepperMotor::getDivider()
{
    return microDivider_;
}

/** @brief For convenience: Calculate steps per circle using the current microstep divider and the base resolution of the stepper.
  *
  * Refer to possible configurations of the driver and the motor.
  *
  */
int StepperMotor::getCurrentStepsPerCircle()
{
    return microDivider_ * numStepsCircle_;
}

/** @brief Get steps per circle for a given microstep divider.
  *
  * Refer to possible configurations of the driver.
  *
  */
int StepperMotor::getStepsPerCircle(int div)
{
    int sps = 0;

    if(!microStepConfigList_.empty())
    {
        for(int i=0; i < microStepConfigList_.size(); i++)
        {
            std::array<int,4>& cfg = microStepConfigList_.at(i);
            if(div == cfg[0])
            {
                sps = numStepsCircle_ * div;
                break;
            }
        }
    }

    return sps;
}

/** @brief Get all possible microstep configurations of current driver.
  *
  */
std::vector<std::array<int,4>> StepperMotor::getMicroStepConfigList()
{
    return microStepConfigList_;
}

/** @brief Set maximum speed (steps per second) your motor can handle without being damaged.
  *
  */
void StepperMotor::setMaxStepsPerSec(int ms)
{
    maxStepsPerSec_ = ms;
}

/** @brief Get maximum speed (steps per second) of the motor as currently specified.
  *
  */
int StepperMotor::getMaxStepsPerSec()
{
    return maxStepsPerSec_;
}

/** @brief Provide resolution of the motor you use.
  *
  */
void StepperMotor::setNumStepsCircle(int num)
{
    numStepsCircle_ = num;
}

/** @brief Get resolution of the motor as currently specified.
  *
  */
int StepperMotor::getNumStepsCircle()
{
    return numStepsCircle_;
}

/** @brief Execute one step using the pin sequence the current driver requires.
  *
  */
void StepperMotor::update()
{
    switch(driverType_)
    {
        case StepperMotorType::DRV8834:
            digitalWrite(pin_STEP_, HIGH);
            digitalWrite(pin_STEP_, LOW);
            break;
    }
}

