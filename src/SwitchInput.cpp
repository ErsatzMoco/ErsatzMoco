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

#include "../include/SwitchInput.hpp"

/** @brief Hardware input element class for a physical switch with an optional indicator LED.
  *
  * Set LED pin to -1 to disable the indicator functions.
  * Note: The indicator is not necessarily a LED - it may be anything that can be switched on or off via a digital output pin.
  * @see InputElement, InputElementType
  */
SwitchInput::SwitchInput() : keyPin_(0), ledPin_(0)
{
    //ctor
}

/** @brief Constructor for providing a name, hardware pin number and pointer to global callback function.
  *
  * Set LED pin to -1 to disable the indicator functions.
  * @see InputElementType
  */
SwitchInput::SwitchInput(char* aname, int kp, int lp, void (*cf)(InputElement *))
{
    //ctor
    type_ = InputElementType::SWITCH;
    strncpy (name_, aname, 31);
    name_[31] = '\0';
    callbackFunction = cf;
    keyPin_ = kp;
    ledPin_ = lp;
    ledState_ = false;

    pinMode(keyPin_, INPUT_PULLUP);
    pinMode(ledPin_, OUTPUT);
    if(ledPin_ > -1)
    {
        digitalWrite(ledPin_, LOW);
    }
}

SwitchInput::~SwitchInput()
{
    //dtor
}

/** @brief Standard query method that reads the switch state.
  *
  * Depending on requested InputDataType the method either returns the switch status after debouncing cycle or the status of the associated indicator pin.
  *
  * @see InputDataType
  */
bool SwitchInput::getBoolData(InputDataType mytype)
{
    bool retval;

    switch(mytype)
    {
        case InputDataType::KEYSTATE:
            retval = checkKey();
            break;

        case InputDataType::LEDSTATE:
            retval = ledState_;
            break;
    }

    return retval;
}

/** @brief Low level method that takes care of debouncing and returns the state of the switch.
  *
  */
bool SwitchInput::checkKey()
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

/** @brief Method to set the indicator pin (e.g. LED on/off).
  *
  * This is deliberately decoupled from testing the physical switch, i.e. must always be set from another object dealing with this switch.
  * The reason is that, say, an element that is switched on or off may be handled parallel to this switch. In that case one must be able to indicate the status of the handled element - and the functionality of the switch can be reversed.
  * An example for this would be a motor that may be switched on/off by a physical switch *and* by a software command coming from elsewhere. This allows for an automation that can still be overridden by a manual switch.
  *
  * @see KSRemote
  */
void SwitchInput::setLED(bool state)
{
    if(state)
    {
        if(ledPin_ != -1)
        {
            digitalWrite(ledPin_, HIGH);
        }
        ledState_ = true;
    }
    else
    {
        if(ledPin_ != -1)
        {
            digitalWrite(ledPin_, LOW);
        }
        ledState_ = false;
    }
}
