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

#include "../include/InputElement.hpp"

/** @brief Abstract base class for any hardware input element.
  *
  * Elements that inherit from InputElement are the physical input elements of a project, e.g. pushbuttons. Dummy functions for data access are defined here to build a standardized interface that allows to store pointers to descendants in a unified container of base class type without pointer degradation.
  * @see ButtonInput, JoystickInput, SwitchInput, PotiInput, InputElementType, InputDataType, UserInterface
  */
InputElement::InputElement() : type_(InputElementType::UNDEFINED)
{
    //ctor
}

InputElement::~InputElement()
{
    //dtor
}

char* InputElement::getName()
{
    return name_;
}

/** @brief Provide name as c-string.
  *
  */
void InputElement::setName(char* aname)
{
    strncpy (name_, aname, 31);
    name_[31] = '\0';
}

InputElementType InputElement::getType()
{
    return type_;
}

/** @brief Standardized access to the input data the element provides: Pass a request indicator to the function and get the answer you ask for.
  *
  */
int InputElement::getIntData(InputDataType mytype)
{
    return 0;
}

/** @brief Standardized access to position data the element provides: Pass a request indicator to the function and get the answer you ask for.
  *
  */
Position InputElement::getPosData(InputDataType mytype)
{
    return Position(0,0);
}

/** @brief Standardized access to boolean data the element provides: Pass a request indicator to the function and get the answer you ask for.
  *
  */
bool InputElement::getBoolData(InputDataType mytype)
{
    return false;
}

