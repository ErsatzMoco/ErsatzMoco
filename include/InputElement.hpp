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

#ifndef INPUTELEMENT_H
#define INPUTELEMENT_H

#include <Arduino.h>
#include "Position.hpp"

enum class InputElementType
{
    UNDEFINED,
    PUSHBUTTON,
    SWITCH,
    ENCODER,
    JOYSTICK,
    POTI
};

enum class InputDataType
{
    PERCENT,
    RAW,
    XPERCENT,
    XRAW,
    YPERCENT,
    YRAW,
    POSPERCENT,
    POSRAW,
    KEYSTATE,
    LEDSTATE
};

class InputElement
{
    public:
        InputElement();
        virtual ~InputElement();

        char* getName();
        InputElementType getType();

        void setName(char* aname);

        void (*callbackFunction)(InputElement *);

        int getIntData(InputDataType mytype);
        Position getPosData(InputDataType mytype);

        bool getBoolData(InputDataType mytype);

    protected:
        char name_[32];
        InputElementType type_;

    private:
};

#endif // INPUTELEMENT_H
