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

    ErsatzMoco ist Freie Software: Sie k�nnen es unter den Bedingungen
    der GNU General Public License, wie von der Free Software Foundation,
    Version 3 der Lizenz oder (nach Ihrer Wahl) jeder neueren
    ver�ffentlichten Version, weiter verteilen und/oder modifizieren.

    ErsatzMoco wird in der Hoffnung, dass es n�tzlich sein wird, aber
    OHNE JEDE GEW�HRLEISTUNG, bereitgestellt; sogar ohne die implizite
    Gew�hrleistung der MARKTF�HIGKEIT oder EIGNUNG F�R EINEN BESTIMMTEN ZWECK.
    Siehe die GNU General Public License f�r weitere Details.

    Sie sollten eine Kopie der GNU General Public License zusammen mit diesem
    Programm erhalten haben. Wenn nicht, siehe <https://www.gnu.org/licenses/>
*/

#ifndef REMOTE_H
#define REMOTE_H

#include <vector>
#include "MocoUnit.hpp"
#include "Helper.hpp"
#include "MonoOLED.hpp"
#include "UserInterface.hpp"
#include "RFM69_Module.hpp"
#include "Sercom_Module.hpp"
#include "StepperUnit.hpp"

#define NETWORK_ID 23
#define REMOTE_ID 23
#define RECEIVER_ID 24

#define DISPLAY_REFRESH 1000
#define POLL_CYCLE 40

class TestCase : public MocoUnit
{
    public:
        TestCase();
        virtual ~TestCase();

        void initTestCase();
        void parseReceiverStatus(char* data);

        MonoOLED getDisplay();
        UserInterface getInterface();

        void dispatchMessage(MocoMessage mymsg);
        void dispatchSerMessage(MocoMessage mymsg);
        void update();
        void executeMicroClock();

        static void handleButton(InputElement* ie);
        static void handleJoystick(InputElement* ie);
        static void updateSteppers();

        static float checkBattery();

    protected:
        static MonoOLED display_;
        static UserInterface interface_;
        static RFM69_Module rfmModule_;
        static Sercom_Module serModule_;

        static Label* myLabel_1;
        static Label* myLabel_2;
        static Button* myButton_1;
        //static Button* myButton_2;
        static ImageButton* myButton_2;
        static Image* myImage_1;
        static Image* myImage_2;

        static unsigned long dispTimeOut_;
        static unsigned long pollTimeOut_;
        static bool screenupdate_;

        static float receiverVoltage_;
        static int receiverRSSI_;

        static bool stepperMoving_;

    private:
};

#endif // REMOTE_H
