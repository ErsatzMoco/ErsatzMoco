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

#include "../include/Boat.hpp"

RFM69_Module Boat::rfmModule_(BOAT_ID, REMOTE_ID, NETWORK_ID, &MocoUnit::handleAlert);
float Boat::batteryVoltage_ = 0.0;
unsigned long Boat::time_ = 0.0;

/** @brief A RFM69 remote controlled model boat based on Adafruit Feather M0 RFM69 using one motor and a rudder.
  *
  * @see MocoUnit, MocoUnitType
  */
Boat::Boat()
{
    //ctor
    id_ = BOAT_ID;
    type_ = MocoUnitType::BOAT;
}

Boat::~Boat()
{
    //dtor
}

/** @brief Init the device by instancing its hardware controllers.
  *
  * @see ActionElement, PowerUnit, Rudder
  */
void Boat::initBoat()
{
    time_ = millis();

    rfmModule_.init();
    Serial.println("radio initialized");

    PowerUnit* myae = new PowerUnit("boat","MAX14870");
    MocoUnit::ae_List_.push_back(myae);
    Rudder* myr = new Rudder("rudder");
    MocoUnit::ae_List_.push_back(myr);
    myr->initRudder(10);

    batteryVoltage_ = checkBattery();

}

/** @brief Process incoming messages containing command sequences.
  *
  * @see MocoMessage
  */
void Boat::update()
{
    MocoMessage recmessage;

    if (millis() > time_+2000)
    {
        time_ = millis();
        batteryVoltage_ = checkBattery();

        char techdata[16];
        Helper::ftoa(batteryVoltage_, techdata, 1, ",");
        char signal[10];
        itoa(rfmModule_.getRSSI(), signal, 10);
        strcat(techdata, "|");
        strcat(techdata, signal);

        rfmModule_.send((int)MocoCommand::EM_DISP_MSG, false, techdata);
    }

    if(rfmModule_.receive())
    {
        recmessage = rfmModule_.receiveBuffer_.back();
        dispatchMessage(recmessage);
        if(!recmessage.ackRequired())
        {
            rfmModule_.receiveBuffer_.erase(rfmModule_.receiveBuffer_.end());
        }
    }

    rfmModule_.update();
}

/** @brief Retrieve battery voltage using the Adafruit Feather M0 RFM69 battery pin
  *
  */
float Boat::checkBattery()
{

    float measuredvbat = analogRead(VBATPIN);
    measuredvbat *= 2; // we divided by 2, so multiply back
    measuredvbat *= 3.3; // Multiply by 3.3V, our reference voltage
    measuredvbat /= 1024; // convert to voltage

    return measuredvbat;
}
