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

#include "../include/KSRemote.hpp"

/** @brief A RFM69 remote controller module based on Adafruit Feather M0 RFM69 using two switches with LED indicators and one potentiometer.
  *
  * @see MocoUnit, MocoUnitType, Beamerbox, Remote, UserInterface, RFM69_Module
  */
UserInterface KSRemote::interface_ = UserInterface();
RFM69_Module KSRemote::rfmModule_ = RFM69_Module(KSREMOTE_ID, KSRECEIVER_ID, NETWORK_ID, &MocoUnit::handleAlert);
int KSRemote::potiValue_ = 50;
unsigned long KSRemote::polltime_ = 0.0;

/** @brief A RFM69 remote controller for use in a theater. The other 
  * end is the "Beamerbox" class. The "shutter" input controls a RC 
  * servo that opens and closes the lens of a projector (Germans 
  * wrongly call a projector "Beamer" because of the light beam). The 
  * "planets" and "planetspeed" inputs control another RC servo that is hacked for 
  * continuous rotation. "relay_red" and "relay_orange" both activate a relay 
  * on the press of a push button.
  *
  * @see MocoUnit, MocoUnitType, Beamerbox
  */
KSRemote::KSRemote()
{
    //ctor
    id_ = KSREMOTE_ID;
    type_ = MocoUnitType::KSREMOTE;
}

KSRemote::~KSRemote()
{
    //dtor
}

/** @brief Initialize this MocoUnit by adding two switches and one potentiometer.
  *
  */
void KSRemote::initKSRemote()
{
    rfmModule_.init();

    polltime_ = millis();

    //build interface
    interface_.addSwitch("shutter", 20, 11, &KSRemote::handleShutter);
    interface_.addSwitch("planets", 21, 12, &KSRemote::handlePlanets);
    interface_.addSwitch("relay_red", 5, 10, &KSRemote::handleRelay);
    interface_.addSwitch("relay_orange", 6, 9, &KSRemote::handleRelay);

    interface_.addPoti("planetspeed", A0, &KSRemote::handlePoti);

    //Thread threadCallback(std::string elemName);


    Serial.println("ksremote initialized");
}

/** @brief Direct access to UserInterface sub-component.
  *
  * @see UserInterface
  */
UserInterface KSRemote::getInterface()
{
    return interface_;
}

/** @brief Add sub unit.
  *
  * @see MocoUnit
  */
void KSRemote::addMocoUnit(MocoUnit* mu)
{
    mocoList_.push_back(mu);
}

/** @brief Retrieve sub unit with the provided name (c-string).
  *
  * @see MocoUnit
  */
MocoUnit* KSRemote::getMocoUnit(char* muname)
{
    MocoUnit* mufound;
    for(MocoUnit* mu : mocoList_)
    {
        if (strcmp(muname, mu->getName()) == 0)
        {
            mufound = mu;
            break;
        }
    }

    return mufound;
}

/** @brief Poll all elements that make up the user interface of this unit.
  *
  * @see MocoUnit, UserInterface
  */
void KSRemote::update()
{
    if(millis() > polltime_+POLL_CYCLE)
    {
        for (InputElement* ie : interface_.inputElemList)
        {
            ie->callbackFunction(ie);
        }

        polltime_ = millis();
    }
}

/** @brief Callback function for poll cycle of shutter switch attached to this unit.
  *
  * @see SwitchInput, UserInterface
  */
void KSRemote::handleShutter(InputElement* ie)
{
    SwitchInput* si = (SwitchInput*)ie;

	if(si->getBoolData(InputDataType::KEYSTATE))
	{
		if(si->getBoolData(InputDataType::LEDSTATE))
		{
			si->setLED(false);
			rfmModule_.send((int)MocoCommand::EM_ACT_SERVO_ANGLE, false, 0L);
		}
	}
	else
	{
		if(!(si->getBoolData(InputDataType::LEDSTATE)))
		{
			si->setLED(true);
			rfmModule_.send((int)MocoCommand::EM_ACT_SERVO_ANGLE, false, 100L);
		}
	}
}
    
/** @brief Callback function for poll cycle of the two relay switches attached to this unit.
  *
  * @see SwitchInput, UserInterface
  */    
void KSRemote::handleRelay(InputElement* ie)
{
	SwitchInput* si = (SwitchInput*)ie;

	if(si->getBoolData(InputDataType::KEYSTATE))
	{
		if(si->getBoolData(InputDataType::LEDSTATE))
		{
			si->setLED(false);
			rfmModule_.send((int)MocoCommand::EM_ACT_RELAY_OFF, false, si->getName());
		}
	}
	else
	{
		if(!(si->getBoolData(InputDataType::LEDSTATE)))
		{
			si->setLED(true);
			rfmModule_.send((int)MocoCommand::EM_ACT_RELAY_ON, false, si->getName());
		}
	}
}

/** @brief Callback function for poll cycle of planet switch attached to this unit.
  *
  * @see SwitchInput, UserInterface
  */
void KSRemote::handlePlanets(InputElement* ie)
{
	SwitchInput* si = (SwitchInput*)ie;
	
	if(si->getBoolData(InputDataType::KEYSTATE))
	{
		if(si->getBoolData(InputDataType::LEDSTATE))
		{
			si->setLED(false);
			rfmModule_.send((int)MocoCommand::EM_ACT_SERVOMOTOR_SPEED, false, 50L);
		}
	}
	else
	{
		if(!(si->getBoolData(InputDataType::LEDSTATE)))
		{
			si->setLED(true);
			rfmModule_.send((int)MocoCommand::EM_ACT_SERVOMOTOR_SPEED, false, KSRemote::potiValue_);
		}
	}
}

/** @brief Callback function for poll cycle of potentiometer attached to this unit.
  *
  * @see PotiInput, UserInterface
  */
void KSRemote::handlePoti(InputElement* ie)
{
    PotiInput* pi = (PotiInput*)ie;

    if(strcmp(pi->getName(), "planetspeed") == 0)
    {
        KSRemote::potiValue_ = map(pi->getIntData(InputDataType::RAW), 0, 1023, 40, 60); //constrain input value for this special application
    }

}


