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

#ifndef KSREMOTE_H
#define KSREMOTE_H

#include "MocoUnit.hpp"
#include "UserInterface.hpp"
#include "RFM69_Module.hpp"

#define NETWORK_ID 99
#define KSREMOTE_ID 90
#define KSRECEIVER_ID 91

#define POLL_CYCLE 40


class KSRemote : public MocoUnit
{
    public:
        KSRemote();
        virtual ~KSRemote();

        void initKSRemote();

        UserInterface getInterface();

        void addMocoUnit(MocoUnit* mu);
        MocoUnit* getMocoUnit(char* muname);

        void update();

        static void handleShutter(InputElement* ie);
        static void handleRelay(InputElement* ie);
        static void handlePlanets(InputElement* ie);
        
        static void handlePoti(InputElement* ie);

    protected:
        static UserInterface interface_;
        static RFM69_Module rfmModule_;

        static int potiValue_;
        static unsigned long polltime_;

        std::vector<MocoUnit*> mocoList_;

    private:
};

#endif // KSREMOTE_H