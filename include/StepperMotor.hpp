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

#ifndef STEPPERMOTOR_H
#define STEPPERMOTOR_H

#include <Arduino.h>
#include <vector>
#include <tuple>
#include "Actuator.hpp"

enum class StepperMotorType
{
    UNDEFINED,
    DRV8834
};


class StepperMotor : public Actuator
{
    public:
        StepperMotor();
        StepperMotor(char* aname);
        StepperMotor(char* aname, char* drivername, int pdir, int pstep, int m0 = -1, int m1 = -1, int m2 = -1, int ena = -1);
        virtual ~StepperMotor();

        void initStepper(char* drivername, int pdir, int pstep, int m0 = -1, int m1 = -1, int m2 = -1, int ena = -1);

        void update() override;

        void setDirection(bool iscw);
        bool setMicrostep(int div = 4);
        int getMaxDivider();
        int getDivider();

        void setMaxStepsPerSec(int ms);
        int getMaxStepsPerSec();

        void setNumStepsCircle(int num);
        int getNumStepsCircle();

        int getCurrentStepsPerCircle();
        int getStepsPerCircle(int div);

        std::vector<std::array<int,4>> getMicroStepConfigList();

        void enable();
        void disable();

    protected:
        int pin_DIR_;
        int pin_STEP_;

        int pin_M_List_[3];
        int microDivider_;
        std::vector<std::array<int,4>> microStepConfigList_;

        int pin_ENABLE_;

        int maxStepsPerSec_;
        int numStepsCircle_;

        StepperMotorType driverType_;

    private:
};

#endif // STEPPERMOTOR_H
