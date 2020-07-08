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

#include "../include/PotiInput.hpp"

/** @brief Hardware input element class for a physical potentiometer.
  *
  * @see InputElement, InputElementType
  */
PotiInput::PotiInput()
{
    //ctor
}

/** @brief Constructor for providing name (c-string), analog input pin and callback function.
  *
  */
PotiInput::PotiInput(char* aname, int pp, void(*cf)(InputElement *))
{
    //ctor
    type_ = InputElementType::POTI;
    strncpy (name_, aname, 31);
    name_[31] = '\0';
    callbackFunction = cf;
    xPin_ = pp;

    xVal_ = analogRead(xPin_);

}

PotiInput::~PotiInput()
{
    //dtor
}

/** @brief Standard query method that reads the analog potentiometer value and returns absolute or percentage value.
  *
  * You must pass the InputDataType you want to read.
  *
  * @see InputDataType
  */
int PotiInput::getIntData(InputDataType mytype)
{
    int retval;

    switch(mytype)
    {
        case InputDataType::RAW:
            xVal_ = analogRead(xPin_);
            retval = xVal_;
            break;
        case InputDataType::PERCENT:
            xVal_ = analogRead(xPin_);
            retval = map(xVal_, 0, 1023, 0, 100);
            break;
    }

    return retval;
}

/** @brief Standard query method that reads potentiometer and returns absolute or percentage value pair with second value set to "0".
  *
  * You must pass the InputDataType you want to read. This function is provided for convenience when dealing with positioning tasks.
  *
  * @see InputDataType, Position
  */
Position PotiInput::getPosData(InputDataType mytype)
{
    xVal_ = analogRead(xPin_);
    Position retpos;

    switch(mytype)
    {
        case InputDataType::POSRAW:
            retpos = Position(xVal_, 0);
            break;
        case InputDataType::POSPERCENT:
            retpos = Position(map(xVal_, 0, 1023, 0, 100), 0);
            break;
    }

    return retpos;
}


