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

#include "../include/StepperUnit.hpp"

extern MocoUnit thisUnit;

/** @brief Stepper motor unit using a driver board and a stepper motor (combined in one Actuator) as well as end detection switches.
  *
  * This is a high level object that automatically handles acceleration and deceleration and controls the motor by distance per time unit. It also automatically sets the microstep configuration to the highest possible value according to the requested speed (distance per time).
  * Note: Due to variable types, the maximum distance you may enter currently is about 32 meters (32768 mm). The same applies to the maximum wheel extent calculated from the wheel diameter.
  *
  * @see ActionElement, ActionElementType, StepperMotor, Actuator
  */
StepperUnit::StepperUnit(char* aname, char* drivername) : isMoving_(false),
    speedReduction_(false),
    motionStarted_(false),
    dir_isCW_(true),
    targetSpeed_(0),
    motor_ (StepperMotor(aname)),
    motionSegment_ (-1)
{
    //ctor
    type_ = ActionElementType::STEPPERUNIT;
    strncpy (name_, aname, 31);
    name_[31] = '\0';
    strncpy (drivername_, drivername, 31);
    drivername_[31] = '\0';

    //motor_ = StepperMotor(aname);
}

StepperUnit::~StepperUnit()
{
    //dtor
}

/** @brief setPins
  *
  * Pin for direction of rotation, step pin, three microstep mode pins, enable pin. If applicable, you may also provide end detection switch pins for both directions.
  */
void StepperUnit::connectPins(int pdir, int pstep, int m1, int m2, int m3, int ena, int stop_cw, int stop_ccw)
{
    motor_.initStepper(drivername_, pdir, pstep, m1, m2, m3, ena);
    pin_END_CW_ = stop_cw;
    pin_END_CCW_ = stop_ccw;

    if(pin_END_CW_ > 0)
        digitalWrite(pin_END_CW_, INPUT);
    if(pin_END_CCW_ > 0)
        digitalWrite(pin_END_CCW_, INPUT);
}

/** @brief Provide stepper motor parameters.
  *
  * Stepper resolution (steps per circle), maximum speed (max steps per second), diameter of the drive pulley.
  *
  */
void StepperUnit::setParams(int circleSteps, int mstepsPerSec, int wheelDiam)
{
    motor_.setNumStepsCircle(circleSteps);
    motor_.setMaxStepsPerSec(mstepsPerSec);
    wheelDiam_MM_ = wheelDiam;
    wheelExtent_MM_ = wheelDiam*MATH_PI;
    stepsPerCircle_ = motor_.getCurrentStepsPerCircle();
    timerMaxStepsPerSec_ = thisUnit.getMicroTicks();
}

/** @brief Calibrate the unit by exploring distances to end switches of both directions.
  *
  * @todo: Implement this functionality.
  */
void StepperUnit::init()
{
    //calibrate with start and end switch
}

/** @brief Invoke stepper by providing a distance (mm), a time frame (seconds) and a direction (boolean).
  *
  * Returns "false" and does nothing if the stepper is currently working.
  * This method set automatically calculates acceleration and deceleration and tries to move the stepper as silent as possible (highest possible microstepping divider).
  *
  */
bool StepperUnit::startStepperMotion(int way_mm, float time_s, bool dir_cw)
{
    if(isMoving_)
        return false;

    wayLength_MM_ = way_mm;
    timeFrame_S_ = time_s;
    dir_isCW_ = dir_cw;

    return startStepperMotion();
}

/** @brief Invoke stepper using parameters of last stepper motion.
  *
  * Returns "false" and does nothing if the stepper is currently working.
  * This method set automatically calculates acceleration and deceleration and tries to move the stepper as silent as possible (highest possible microstepping divider).
  *
  */
bool StepperUnit::startStepperMotion()
{
    if(isMoving_)
        return false;

    speedReduction_ = false;

    int div = motor_.getMaxDivider();
    int csps;
    motor_.setMicrostep(div);
    int mps = motor_.getMaxStepsPerSec();
    csps = motor_.getCurrentStepsPerCircle();

    speedUpTime_ = timeFrame_S_ * ACCEL_SEGMENT;
    normalRunTime_ = timeFrame_S_ * NORMAL_SEGMENT;

    float distPerStep = (float)wheelExtent_MM_ / csps;
    stepsNecessary_ = wayLength_MM_ / distPerStep;

    targetSpeed_ = wayLength_MM_ / (normalRunTime_ + speedUpTime_);
    targetSpeedBySteps_ = targetSpeed_ / distPerStep;

    while((targetSpeedBySteps_ > mps) && (div > 1))
    {
        div = div/2;
        if(motor_.setMicrostep(div))
        {
            csps = motor_.getCurrentStepsPerCircle();
            distPerStep = (float)wheelExtent_MM_ / csps;
            stepsNecessary_ = wayLength_MM_ / distPerStep;
            targetSpeedBySteps_ = targetSpeed_ / distPerStep;
        }
    }

    stepsRemaining_ = stepsNecessary_;

    if(targetSpeedBySteps_ > mps)
    {
        targetSpeedBySteps_ = mps;
        speedReduction_ = true;
    }

    if(targetSpeedBySteps_ > timerMaxStepsPerSec_)
    {
        targetSpeedBySteps_ = timerMaxStepsPerSec_;
        speedReduction_ = true;
    }

    currentSpeedBySteps_ = START_SPEED;
    v_timer_ticks_ = thisUnit.getMicroTicks() / currentSpeedBySteps_;
    v_counter_ = v_timer_ticks_;
    a_timer_ticks_ = thisUnit.getMicroTicks() / (targetSpeedBySteps_ - currentSpeedBySteps_);
    a_counter_ = a_timer_ticks_;

    motionSegment_ = 0;

    motor_.setDirection(dir_isCW_);
    motor_.update();
    --stepsRemaining_;

    isMoving_ = true;
    motionStarted_ = true;
    //timestampDecel_ = ((ACCEL_SEGMENT+NORMAL_SEGMENT)*timeFrame_S_)*1000000 + micros();

    return true;
}

/** @brief Check if stepper is available.
  *
  */
bool StepperUnit::isMoving()
{
    return isMoving_;
}

/** @brief Retrieve motion section the working stepper is currently in.
  *
  * -1: stepper is stopped, 0: acceleration, 1: moving at target speed, 2: deceleration
  *
  */
int StepperUnit::getMotionSegment()
{
    return motionSegment_;
}

/** @brief Stop stepper by forcing it into deceleration.
  *
  */
void StepperUnit::fastStop()
{
    if(!isMoving_)
        return;

    int csps = motor_.getCurrentStepsPerCircle();
    float distPerStep = (float)wheelExtent_MM_ / csps;
    float currentSpeed = currentSpeedBySteps_ * distPerStep;

    stepsRemaining_ = (0.5 * (currentSpeed*speedUpTime_)) / distPerStep;
    motionSegment_ = 2;
    a_counter_ = a_timer_ticks_;
}

/** @brief Stop stepper by ceasing motion at once.
  *
  * Caution! Using this may damage your motor and/or your machine depending on the forces/weights involved.
  * If possible use "fastStop" for emergency stops.
  *
  */
void StepperUnit::emergencyStop()
{
    if(!isMoving_)
        return;

    stepsRemaining_ = 0;
    isMoving_ = false;
    motionSegment_ = -1;
    return;
}

/** @brief Function that must be called by timer prepared appropriately to actually move the stepper.
  *
  * Refer to example unit "TestCase" on how to set up this. Basically there must be a callback function for the timer which calls all connected StepperUnits.
  * The timer itself must be updated from within the "loop" wrapper method called "update" in MocoUnits.
  * This is a software based system, deliberately set up without hardware interrupts to prevent interfering with other interrupt dependant systems like SPI. Thus, stepper timing will not be 100% accurate, but probably good enough in most cases.
  *
  * @see TestCase, MocoUnit
  */
void StepperUnit::update()
{
    if(!isMoving_)
        return;

    if(motionStarted_)
    {
        timestampDecel_ = (timeFrame_S_ - speedUpTime_)*1000000 + micros();
        motionStarted_ = false;
    }

    if(stepsRemaining_ < 1)
    {
        isMoving_ = false;
        motionSegment_ = -1;

        Serial.println("Stepper stops");
        return;
    }

    switch(motionSegment_)
    {
        case 0:
            --a_counter_;
            if(a_counter_ <= 0)
            {
                if(currentSpeedBySteps_ < targetSpeedBySteps_)
                {
                    ++currentSpeedBySteps_;
                    a_counter_ = a_timer_ticks_;
                    v_timer_ticks_ = thisUnit.getMicroTicks() / currentSpeedBySteps_;
                }
                else
                {
                    motionSegment_ = 1;
                }
            }
            break;
        case 2:
            --a_counter_;
            if(a_counter_ <= 0)
            {
                if(currentSpeedBySteps_ > STOP_SPEED)
                {
                    --currentSpeedBySteps_;
                    a_counter_ = a_timer_ticks_;
                    v_timer_ticks_ = thisUnit.getMicroTicks() / currentSpeedBySteps_;
                }
            }
            break;
    }

    --v_counter_;
    if(v_counter_ > 0)
        return;

    v_counter_ = v_timer_ticks_;

    motor_.update();
    --stepsRemaining_;

    if(motionSegment_ == 1)
    {
        if (micros() > timestampDecel_)
        {
            motionSegment_ = 2;
            a_timer_ticks_ = thisUnit.getMicroTicks() / (targetSpeedBySteps_ - STOP_SPEED);
            a_counter_ = a_timer_ticks_;
        }
    }
}

/** @brief Check whether unit had to reduce speed because time frame for distance was too short according to motor specs.
  *
  */
bool StepperUnit::runsReducedSpeed()
{
    return speedReduction_;
}

/** @brief Report current status of this unit.
  *
  * @todo Implement this functionality.
  */
char* StepperUnit::getStatus()
{
    return "&SU:";
}

/** @brief Set new direction of rotation.
  *
  * Returns "false" if you call this while the stepper is moving.
  */
bool StepperUnit::setDirectionCW(bool cw)
{
    if(isMoving_)
        return false;

    dir_isCW_ = cw;
    return true;
}

/** @brief Set distance to cover in next motion.
  *
  * Returns "false" if you call this while the stepper is moving.
  */
bool StepperUnit::setWayLength(int mm)
{
    if(isMoving_)
        return false;

    wayLength_MM_ = mm;
    return true;
}

/** @brief Set time frame for next motion.
  *
  * Returns "false" if you call this while the stepper is moving.
  */
bool StepperUnit::setTimeFrame(float secs)
{
    if(isMoving_)
        return false;

    timeFrame_S_ = secs;
    return true;
}

/** @brief Get current direction of rotation.
  *
  */
bool StepperUnit::getDirectionCW()
{
    return dir_isCW_;
}

/** @brief Handle commands directed to this ActionElement.
  *
  * @see MocoCommand
  */
void StepperUnit::execute(int cmd, char* data)
{
    switch(cmd)
    {
        case MocoCommand::EM_ACT_STM_CCW:
            if(dir_isCW_)
            {
                dir_isCW_ = false;
                motor_.setDirection(dir_isCW_);
            }
            break;

        case MocoCommand::EM_ACT_STM_CW:
            if (!dir_isCW_)
            {
                dir_isCW_ = true;
                motor_.setDirection(dir_isCW_);
            }
            break;

        case MocoCommand::EM_ACT_STM_STEP:
            motor_.update();
            break;

        case MocoCommand::EM_ACT_STM_SET_DIST:
            setWayLength(atoi(data));
            break;

        case MocoCommand::EM_ACT_STM_SET_TIME:
            setTimeFrame(atof(data));
            break;

        case MocoCommand::EM_ACT_STM_START:
            startStepperMotion();
            break;

        case MocoCommand::EM_ACT_STM_STOP:
            fastStop();
            break;

        case MocoCommand::EM_ACT_STM_ENABLE:
            motor_.enable();
            break;

        case MocoCommand::EM_ACT_STM_DISABLE:
            motor_.disable();
            break;

    }
}
