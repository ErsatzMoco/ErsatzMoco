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

#include "../include/JoystickInput.hpp"

/** @brief Wrapper class for a combined "joystick" input element.
  *
  * Typically consists of two potentiometers and one push button for x/y input and one extra function.
  *
  * @see InputElement
  */
JoystickInput::JoystickInput()
{
    //ctor
}

/** @brief Constructor providing a name (c-string), two analog input pins, one digital input pin and a callback function.
  *
  */
JoystickInput::JoystickInput(char* aname, int xp, int yp, int kp, void(*cf)(InputElement *))
{
    //ctor
    type_ = InputElementType::JOYSTICK;
    strncpy (name_, aname, 31);
    name_[31] = '\0';
    callbackFunction = cf;
    keyPin_ = kp;
    xPin_ = xp;
    yPin_ = yp;

    xVal_ = analogRead(xPin_);
    yVal_ = analogRead(yPin_);
    pinMode(keyPin_, INPUT_PULLUP);

}

JoystickInput::~JoystickInput()
{
    //dtor
}

/** @brief Get last read position value (x potentiometer).
  *
  */
int JoystickInput::getXState()
{
    return xVal_;
}

/** @brief Get last read position value (y potentiometer).
  *
  */
int JoystickInput::getYState()
{
    return yVal_;
}

/** @brief Get last read pushbutton state.
  *
  */
bool JoystickInput::getKeyState()
{
    return keyState_;
}

/** @brief Standard query method that reads analog potentiometer and returns absolute or percentage value.
  *
  * You must pass the InputDataType you want to read.
  *
  * @see InputDataType
  */
int JoystickInput::getIntData(InputDataType mytype)
{
    int retval;

    switch(mytype)
    {
        case InputDataType::XRAW:
            xVal_ = analogRead(xPin_);
            retval = xVal_;
            break;
        case InputDataType::YRAW:
            yVal_ = analogRead(yPin_);
            retval = yVal_;
            break;
        case InputDataType::XPERCENT:
            xVal_ = analogRead(xPin_);
            retval = map(xVal_, 0, 1023, 0, 100);
            break;
        case InputDataType::YPERCENT:
            yVal_ = analogRead(yPin_);
            retval = map(yVal_, 0, 1023, 0, 100);
            break;
    }

    return retval;
}

/** @brief Standard query method that reads both analog potentiometers and returns absolute or percentage value pair.
  *
  * You must pass the InputDataType you want to read.
  *
  * @see InputDataType, Position
  */
Position JoystickInput::getPosData(InputDataType mytype)
{
    xVal_ = analogRead(xPin_);
    yVal_ = analogRead(yPin_);
    Position retpos;

    switch(mytype)
    {
        case InputDataType::POSRAW:
            retpos = Position(xVal_, yVal_);
            break;
        case InputDataType::POSPERCENT:
            retpos = Position(map(xVal_, 0, 1023, 0, 100), map(yVal_, 0, 1023, 0, 100));
            break;
    }

    return retpos;
}

/** @brief Standard query method that reads the pushbutton state after debouncing cycle.
  *
  * For standardization reasons an InputDataType is requested, but it is not evaluated in this subclass (so just pass 0 or any other integer).
  *
  * @see InputDataType
  */
bool JoystickInput::getBoolData(InputDataType mytype)
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
