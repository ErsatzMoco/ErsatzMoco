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

#ifndef POTIINPUT_H
#define POTIINPUT_H

#include <Arduino.h>
#include "InputElement.hpp"
#include "Position.hpp"

#define DEBOUNCE_DELAY 10


class PotiInput : public InputElement
{
    public:
        PotiInput();
        PotiInput(char* aname, int pp, void(*cf)(InputElement *));
        virtual ~PotiInput();

        int getIntData(InputDataType mytype);
        Position getPosData(InputDataType mytype);

    protected:
        int xPin_;

        int xVal_;

    private:
};

#endif // POTIINPUT_H