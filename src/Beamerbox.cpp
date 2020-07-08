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

#include "../include/Beamerbox.hpp"

RFM69_Module Beamerbox::rfmModule_(BEAMERBOX_ID, REMOTE_ID, NETWORK_ID, &MocoUnit::handleAlert);

/** @brief A RFM69 remote controlled module based on Adafruit Feather M0 RFM69.
  *
  * Uses a rc servo to open and close the lens of a projector as well as a continuous servo motor for rotating something, e.g. a mobile structure (fragile carousel for decorating or theater use).
  * Two relay units for general purposes are added and are remote controlled as well.
  * The corresponding remote control class is "KSRemote".
  *
  * @see MocoUnit, MocoUnitType, KSRemote
  */
Beamerbox::Beamerbox()
{
    //ctor
    id_ = BEAMERBOX_ID;
    type_ = MocoUnitType::BEAMERBOX;
}

Beamerbox::~Beamerbox()
{
    //dtor
}

/** @brief Init the device by instancing its hardware controllers.
  *
  * @see ActionElement, Shutter, Servomotor
  */
void Beamerbox::initBeamerbox()
{
    rfmModule_.init();
    Serial.println("beamerbox radio initialized");

    Shutter* myshutter = new Shutter("shutter");
    MocoUnit::ae_List_.push_back(myshutter);
    myshutter->initShutter(9);
    
    Servomotor* myplanets = new Servomotor("planets");
    MocoUnit::ae_List_.push_back(myplanets);
    myplanets->initServomotor(10, 20);
    
    Relay* myrelay_red = new Relay("relay_red");
    MocoUnit::ae_List_.push_back(myrelay_red);
    myrelay_red->initRelay(12);
    
    Relay* myrelay_orange = new Relay("relay_orange");
    MocoUnit::ae_List_.push_back(myrelay_orange);
    myrelay_orange->initRelay(13);
}

/** @brief Process incoming messages containing command sequences.
  *
  * @see MocoMessage
  */
void Beamerbox::update()
{
    MocoMessage recmessage;

    if(rfmModule_.receive())
    {
        recmessage = rfmModule_.receiveBuffer_.back();
        //Serial.println(recmessage.toString());
        dispatchMessage(recmessage);
        if(!recmessage.ackRequired())
        {
            rfmModule_.receiveBuffer_.erase(rfmModule_.receiveBuffer_.end());
        }
    }

    rfmModule_.update();
}
