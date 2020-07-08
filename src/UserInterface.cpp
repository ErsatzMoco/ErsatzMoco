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

#include "../include/UserInterface.hpp"

/** @brief Collection and administration of physical input elements.
  *
  * A setup that interacts with the user would typically create one instance of this class.
  * Note: This class actually *owns* the elements that are added to it! If you want to destroy this object, you must implement the destruction of all administered input elements as well.
  * This is typically not the case - normally the UserInterface lives as long as the whole controller runs and is never destroyed, so destruction of owned elements is not implemented here.
  * The list for administration (inputElemList) stores pointers of the base class type InputElement only. This is no problem since each InputElement stores its derived type, so you can always check what type of element you are dealing with and cast accordingly. For that reason, the potentially dangerous object creation on the heap with "new" was chosen deliberately to ensure nothing is ever sliced from the objects.
  * This whole construction may have much better solutions I sadly couldn't come up with. Still it seems quite logical and readable to me and works well in this confined application space. It should not cause surprising problems since we are not creating and destroying objects to our liking at runtime; we assume we always know what objects exists and for what reason, and they are created once and never destroyed.
  * @see InputElement, InputElementType, MocoUnit
  */
UserInterface::UserInterface()
{
    //ctor
}

UserInterface::~UserInterface()
{
    //dtor
}

/** @brief Create and add a switch input element.
  *
  * @see SwitchInput
  */
void UserInterface::addSwitch(char* aname, int swpin, int ledpin, void(*cf)(InputElement *))
{
    SwitchInput* news = new SwitchInput(aname, swpin, ledpin, cf);
    inputElemList.push_back(news);
}

/** @brief Create and add a button input element.
  *
  * @see ButtonInput
  */
void UserInterface::addButton(char* aname, int apin, void(*cf)(InputElement *))
{
    ButtonInput* newb = new ButtonInput(aname, apin, cf);
    inputElemList.push_back(newb);
}

/** @brief Create and add a potentiometer input element.
  *
  * @see PotiInput
  */
void UserInterface::addPoti(char* aname, int ppin, void(*cf)(InputElement *))
{
    PotiInput* newj = new PotiInput(aname, ppin, cf);
    inputElemList.push_back(newj);
}

/** @brief Create and add a joystick input element.
  *
  * @see JoystickInput
  */
void UserInterface::addJoystick(char* aname, int pinx, int piny, int pink, void(*cf)(InputElement *))
{
    JoystickInput* newj = new JoystickInput(aname, pinx, piny, pink, cf);
    inputElemList.push_back(newj);
}

/** @brief Retrieve input element by its name.
  *
  */
InputElement* UserInterface::getInputElement(char* aname)
{
    InputElement* foundEl;
    for (InputElement* ie : inputElemList)
    {
        if(strcmp(ie->getName(), aname) == 0)
        {
            foundEl = ie;
            break;
        }
    }
    return foundEl;
}
