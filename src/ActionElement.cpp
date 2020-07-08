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

#include "../include/ActionElement.hpp"

/** @brief Abstract base class for any active entity.
  *
  * Elements that inherit from ActionElement typically consist of several Actuators and receive commands for execution.
  * @see Actuator
  */
ActionElement::ActionElement() : type_(ActionElementType::UNDEFINED)
{
    //ctor
    strcpy(name_, "empty");
}

ActionElement::~ActionElement()
{
    //dtor
}

/** @brief Each ActionElement has a defined type listed in ActionElementType.
  *
  * @see ActionElementType
  */
ActionElementType ActionElement::getType()
{
    return type_;
}

/** @brief Each ActionElement must get a unique name (c string).
  *
  */
char* ActionElement::getName()
{
    return name_;
}

/** @brief Central virtual function for updating the entitie's status.
  *
  */
void ActionElement::update()
{
    //overwrite if necessary - example: StepperUnit
}


