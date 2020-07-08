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

#ifndef STEPPERUNIT_H
#define STEPPERUNIT_H

#include <Arduino.h>
#include <vector>
#include <tuple>
#include "ActionElement.hpp"
#include "StepperMotor.hpp"
#include "MocoCommand.hpp"
#include "MocoUnit.hpp"
#include "Helper.hpp"

#define START_SPEED 400
#define STOP_SPEED 450
#define ACCEL_SEGMENT 0.1
#define NORMAL_SEGMENT 0.8
#define FAST_DECEL_TIME 1

enum class StepperMotionSegment
{
    ACCEL,
    MOVING,
    DECEL
};

class StepperUnit : public ActionElement
{
    public:
        StepperUnit(char* aname, char* drivername);
        virtual ~StepperUnit();

        void connectPins(int pdir, int pstep, int m1 = -1, int m2 = -1, int m3 = -1, int ena = -1, int stop_cw = -1, int stop_ccw = -1);
        void setParams(int circleSteps, int stepsPerSec, int wheelDiam);
        void init();
        bool startStepperMotion(int way_mm, float time_s, bool dir_cw);
        bool startStepperMotion();
        void update();

        bool setDirectionCW(bool cw);
        bool getDirectionCW();

        bool setWayLength(int mm);
        bool setTimeFrame(float secs);

        bool runsReducedSpeed();
        bool isMoving();
        int getMotionSegment();
        void fastStop();
        void emergencyStop();

        virtual char* getStatus() override;
        virtual void execute(int cmd, char* data) override;

    protected:
        bool dir_isCW_;
        StepperMotor motor_;
        char drivername_[32];

        int wayLength_MM_;
        float timeFrame_S_;
        int wheelDiam_MM_;
        int wheelExtent_MM_;
        long timerMaxStepsPerSec_;
        long stepsPerCircle_;
        float speedUpTime_;
        float normalRunTime_;

        bool motionStarted_;
        bool isMoving_;
        bool speedReduction_;
        int motionSegment_;

        float targetSpeed_;
        long targetSpeedBySteps_;
        long currentSpeedBySteps_;

        long stepsNecessary_;
        long stepsRemaining_;

        long v_timer_ticks_;
        long v_counter_;
        long a_timer_ticks_;
        long a_counter_;

        unsigned long timestampDecel_;

        int pin_END_CW_;
        int pin_END_CCW_;

    private:

};

#endif // STEPPERUNIT_H
