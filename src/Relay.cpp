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

#include "../include/Relay.hpp"

/** @brief Relay hardware wrapper.
  *
  * @see ActionElement, ActionElementType
  */
Relay::Relay(char* aname)
{
    //ctor
    type_ = ActionElementType::RELAY;
    strncpy (name_, aname, 31);
    name_[31] = '\0';
}

Relay::~Relay()
{
    //dtor
}

/** @brief Pin 1 for the relay.
  *
  */
void Relay::initRelay()
{
    relay_ = 1;
    pinMode(relay_, OUTPUT);
    digitalWrite(relay_, HIGH);
}

/** @brief Provide a pin for the relay.
  *
  */
void Relay::initRelay(int relaypin)
{
    relay_ = relaypin;
    pinMode(relay_, OUTPUT);
    digitalWrite(relay_, HIGH);
}

/** @brief Report current status of this unit.
  *
  * @todo Implement this functionality.
  */
char* Relay::getStatus()
{
    return "&RD:";
}

/** @brief Handle commands directed to this ActionElement.
  *
  * Note: Via MocoCommand this unit expects speed values as a percentage (as opposed to the low level functions providing an "angle" value).
  *
  * @see MocoCommand
  */
void Relay::execute(int cmd, char* data)
{
	if (strcmp(data, name_) == 0)
	{
		switch(cmd)
		{
			case MocoCommand::EM_ACT_RELAY_OFF:
				digitalWrite(relay_, HIGH);
				//Serial.println(" OFF");
				break;
			case MocoCommand::EM_ACT_RELAY_ON:
				digitalWrite(relay_, LOW);
				//Serial.println(" ON");
				break;
		}	
	}
    
}
