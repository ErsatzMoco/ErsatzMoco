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

#ifndef MocoUnit_H
#define MocoUnit_H

#include <Arduino.h>
#include <vector>
#include "ActionElement.hpp"
#include "ComModule.hpp"
#include "MocoCommand.hpp"

#define VBATPIN A7

enum class MocoUnitType
{
    UNDEFINED,
    CONTROLLER,
    REMOTE,
    KSREMOTE,
    ROBOT,
    LOCOMOTIVE,
    BOAT,
    CAR,
    BEAMERBOX,
    TESTCASE
};

class MocoUnit
{
    public:
        MocoUnit();
        virtual ~MocoUnit();

        char* getName();
        void setName(char* aname);

        int getId();

        MocoUnitType getType();

        static std::vector<ActionElement*> getAeList();

        void addActionElement(ActionElement* myae);
        ActionElement* getActionElement(char* aename);

        void dispatchMessage(MocoMessage mymsg);
        static void handleAlert(MocoMessage mm);

        void update();
        void testSpeed();
        void testSpeed(int sp_times);

        static void setMicroClock(long mi, void(*cf)());
        static long getMicroInterval();
        static long getMicroTicks();
        static void microClock();

        static void (*microCallback_)();

    protected:
        char name_[32];
        int id_;
        MocoUnitType type_;

        static std::vector<ActionElement*> ae_List_;

        bool speedReport_;
        int speedCounter_;
        int speedStat_;
        long startTime_;
        double loopTime_;

        static unsigned long microClockTimeout_;
        static long microClockInterval_;

    private:
};

#endif // MocoUnit_H
