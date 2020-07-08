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

#include "../include/MocoMessage.hpp"
#include "../include/MocoUnit.hpp"

/** @brief Abstract base class for any ErsatzMoco construction.
  *
  * Classes derived from MocoUnit represent the physical box etc. that makes use of lower level components it consists of.
  * One single MocoUnit child must be instantiated in the .ino file (which defines setup and loop function).
  *
  * @see MocoUnitType, Beamerbox, Boat, KSRemote, Locomotive, Remote
  */

std::vector<ActionElement*> MocoUnit::ae_List_ = {};

unsigned long MocoUnit::microClockTimeout_ = 0;
long MocoUnit::microClockInterval_ = 0;
void (*MocoUnit::microCallback_)() = nullptr;

MocoUnit::MocoUnit() : speedCounter_(0),
    speedStat_(0),
    speedReport_(false),
    startTime_(0),
    loopTime_(0)
{
    //ctor
}

MocoUnit::~MocoUnit()
{
    //dtor
}

char* MocoUnit::getName()
{
    return name_;
}

int MocoUnit::getId()
{
    return id_;
}

/** @brief Retrieve the list of ActionElements this unit consists of.
  *
  * @see ActionElement
  */
std::vector<ActionElement*> MocoUnit::getAeList()
{
    return MocoUnit::ae_List_;
}

void MocoUnit::setName(char* aname)
{
    strncpy (name_, aname, 31);
    name_[31] = '\0';
}

MocoUnitType MocoUnit::getType()
{
    return type_;
}

/** @brief To be overwritten in descendants. This method contains tasks that need to be executed periodically.
  *
  */
void MocoUnit::update()
{
    if(speedReport_)
    {
        testSpeed();
    }
}

/** @brief Set a timeout for the clock running with microsecond resolution.
  *
  * You must provide a pointer to a callback function that is executed after
  *
  */
void MocoUnit::setMicroClock(long mi, void(*cf)())
{
    MocoUnit::microClockInterval_ = mi;
    MocoUnit::microCallback_ = cf;
    MocoUnit::microClockTimeout_ = micros() + MocoUnit::microClockInterval_;
}

/** @brief Get timeout for the clock running with microsecond resolution.
  *
  */
/** @brief getMicroClock
  *
  * @todo: document this function
  */
long MocoUnit::getMicroInterval()
{
    return MocoUnit::microClockInterval_;
}

/** @brief Get number of clock calls defined by timeout.
  *
  */
long MocoUnit::getMicroTicks()
{
    return (long)(1000000 / MocoUnit::microClockInterval_);
}

/** @brief Run the clock with microsecond resolution.
  *
  * Execute callback function when timeout was reached.
  *
  */
void MocoUnit::microClock()
{
    if(micros() >= MocoUnit::microClockTimeout_)
    {
        MocoUnit::microClockTimeout_ = micros() + MocoUnit::microClockInterval_;
        MocoUnit::microCallback_();
    }
}

/** @brief Method for testing roughly how long your program stays in Arduino main loop.
  *
  * Start a new measurement by specifying how often you want the main loop to be called before calculating execution time.
  */
void MocoUnit::testSpeed(int sp_times)
{
    speedStat_ = sp_times;
    startTime_ = millis();
    speedReport_ = true;
}

/** @brief Method for testing roughly how long your program stays in Arduino main loop.
  *
  * Run the clock by calling this method from within "loop".
  */
void MocoUnit::testSpeed()
{
    if(speedCounter_ < speedStat_)
    {
        ++speedCounter_;
        return;
    }

    loopTime_ = double(millis() - startTime_) / (double)speedStat_;

    Serial.print(speedCounter_);
    Serial.println(" times looped. Millis start/end:");
    Serial.println(startTime_);
    Serial.println(millis());
    Serial.print("Loop milliseconds: ");
    Serial.println(loopTime_);

    speedReport_ = false;
    speedCounter_ = 0;
    speedStat_ = 0;
}

/** @brief Add a new ActionElement to this unit.
  *
  * @see ActionElement
  */
void MocoUnit::addActionElement(ActionElement* myae)
{
    MocoUnit::ae_List_.push_back(myae);
}

/** @brief Retrieve an ActionElement by name.
  *
  * @see ActionElement
  */
ActionElement* MocoUnit::getActionElement(char* aename)
{
    ActionElement* aefound;
    for(ActionElement* ae : MocoUnit::ae_List_)
    {
        if (strcmp(aename, ae->getName()) == 0)
        {
            aefound = ae;
            break;
        }
    }

    return aefound;
}

/** @brief Forward a message to all registered ActionElements.
  *
  * ActionElements decide themselves if they are a valid receiver and need to react.
  *
  * @see ActionElement, MocoMessage
  */
void MocoUnit::dispatchMessage(MocoMessage mymsg)
{
    for(ActionElement* ae : MocoUnit::ae_List_)
    {
        ae->execute(mymsg.getMsgCommand(), mymsg.getMsgData());
    }
}

/** @brief Flag an alert if a sent message that required an acknowledge did not receive one even after repeated sends.
  *
  * @see MocoMessage
  */
void MocoUnit::handleAlert(MocoMessage mm)
{
    Serial.println("ALERT " + String(mm.getMsgCommand()) + " " + String(mm.getMsgID()));
}
