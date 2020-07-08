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

#include "../include/Actuator.hpp"

/** @brief Abstract base class for any active hardware element.
  *
  * Elements that inherit from Actuator control one single hardware component, e.g. a motor or light. They are often part of a higher control entity of base type ActionElement to combine several actuators and/or sensors etc.
  * @see ActionElement
  */
Actuator::Actuator() : type_(ActuatorType::UNDEFINED)
{
    //ctor
    strcpy(name_, "empty");
}

Actuator::~Actuator()
{
    //dtor
}

/** @brief Each Actuator has a defined type listed in ActuatorType.
  *
  * @see ActuatorType
  */
ActuatorType Actuator::getType()
{
    return type_;
}

/** @brief Each Actuator must get a unique name (c string).
  *
  */
char* Actuator::getName()
{
    return name_;
}

