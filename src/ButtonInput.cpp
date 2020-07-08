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

#include "../include/ButtonInput.hpp"

/** @brief Hardware input element class for a physical button.
  *
  * @see InputElement, InputElementType
  */
ButtonInput::ButtonInput() : keyPin_(0)
{
    //ctor
}

/** @brief Constructor for providing a name, hardware pin number and pointer to global callback function.
  *
  * @see InputElementType
  */
ButtonInput::ButtonInput(char* aname, int kp, void (*cf)(InputElement *))
{
    //ctor
    type_ = InputElementType::PUSHBUTTON;
    strncpy (name_, aname, 31);
    name_[31] = '\0';
    callbackFunction = cf;
    keyPin_ = kp;

    pinMode(keyPin_, INPUT_PULLUP);
    keyState_ = digitalRead(keyPin_);
}

ButtonInput::~ButtonInput()
{
    //dtor
}

/** @brief Get existing status variable.
  *
  */
bool ButtonInput::getOldState()
{
    return keyState_;
}

/** @brief Standard query method that reads the pushbutton state after debouncing cycle.
  *
  * For standardization reasons an InputDataType is requested, but it is not evaluated in this subclass (so just pass 0 or any other integer).
  *
  * @see InputDataType
  */
bool ButtonInput::getBoolData(InputDataType mytype)
{
    bool previousState;

    previousState = digitalRead(keyPin_);
    for(int counter=0; counter < DEBOUNCE_DELAY; counter++)
    {
        delay(1);
        keyState_ = digitalRead(keyPin_);
        if(keyState_ != previousState)
        {
            counter = 0;
            previousState = keyState_;
        }
    }

    return keyState_;
}

