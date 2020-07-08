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

#ifndef REMOTE_H
#define REMOTE_H

#include <vector>
#include "MocoUnit.hpp"
#include "Helper.hpp"
#include "AdafruitOLED.hpp"
#include "UserInterface.hpp"
#include "RFM69_Module.hpp"

#define NETWORK_ID 23
#define REMOTE_ID 23
#define RECEIVER_ID 24

#define DISPLAY_REFRESH 1000
#define POLL_CYCLE 40

class Remote : public MocoUnit
{
    public:
        Remote();
        virtual ~Remote();

        void initRemote();
        void parseReceiverStatus(char* data);

        AdafruitOLED getDisplay();
        UserInterface getInterface();

        void update();
        void dispatchMessage(MocoMessage mymsg);

        static void handleButton(InputElement* ie);
        static void handleJoystick(InputElement* ie);
        static float checkBattery();

    protected:
        static AdafruitOLED display_;
        static UserInterface interface_;
        static RFM69_Module rfmModule_;

        static Label* myLabel_1;
        static Button* myButton_1;
        static ImageButton* myButton_2;
        static Image* myImage_1;
        static Image* myImage_2;

        static unsigned long time_;
        static unsigned long disptime_;
        static unsigned long polltime_;
        static bool screenupdate_;

        static float receiverVoltage_;
        static int receiverRSSI_;

    private:
};

#endif // REMOTE_H
