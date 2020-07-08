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

#include "../include/Remote.hpp"

/** @brief A RFM69 remote controller module based on Adafruit Feather M0 RFM69 using two X/Y joysticks, pushbuttons (currently two) and a display.
  *
  * Intended for use as a radio remote control for scale models that can handle any special function.
  *
  * @see MocoUnit, MocoUnitType, Boat, Locomotive, UserInterface, RFM69_Module
  */

AdafruitOLED Remote::display_ = AdafruitOLED();
UserInterface Remote::interface_ = UserInterface();
RFM69_Module Remote::rfmModule_ = RFM69_Module(REMOTE_ID, RECEIVER_ID, NETWORK_ID, &MocoUnit::handleAlert);

unsigned long Remote::time_ = 0;
unsigned long Remote::disptime_ = 0;
unsigned long Remote::polltime_ = 0;
float Remote::receiverVoltage_ = 0.0;
int Remote::receiverRSSI_ = 0;
bool Remote::screenupdate_ = false;

Label* Remote::myLabel_1 = nullptr;
Button* Remote::myButton_1 = nullptr;
ImageButton* Remote::myButton_2 = nullptr;
Image* Remote::myImage_1 = nullptr;
Image* Remote::myImage_2 = nullptr;

Remote::Remote()
{
    //ctor
    id_ = REMOTE_ID;
    type_ = MocoUnitType::REMOTE;
}

Remote::~Remote()
{
    //dtor
}

/** @brief Initialize this MocoUnit by adding two pushbuttons, two X/Y joysticks and a display.
  *
  */
void Remote::initRemote()
{
    time_ = millis();
    disptime_ = millis();
    polltime_ = millis();
    screenupdate_ = false;

    rfmModule_.init();

    //build interface
    interface_.addButton("b_lu", 11, &Remote::handleButton);
    interface_.addButton("b_ru", 12, &Remote::handleButton);
    interface_.addJoystick("joyl", A0, A1, A2, &Remote::handleJoystick);
    interface_.addJoystick("joyr", A3, A4, A5, &Remote::handleJoystick);

    //build visuals
    Helper::pauseRFM();
    display_.initDisplay();
    myLabel_1 = new Label("test_l", "Label", 2, 0, 8, 2);
    myLabel_1->setColors(Color(255,255,255), Color(0,0,255), Color(255,0,0));
    display_.addElement(myLabel_1);

    myButton_1 = new Button("test_b", "A", 1, 3, 2, 2);
    myButton_1->setColors(Color(255,255,255), Color(0,0,255), Color(255,0,0));
    myButton_1->setDownText("B");
    display_.addElement(myButton_1);

    myButton_2 = new ImageButton("test_b2", "\0", 4, 3, 4, 4);
    myButton_2->setColors(Color(255,255,255), Color(0,0,255), Color(255,0,0));
    myButton_2->setImageFiles("toolicon/tooli21.bmp", "toolicon/tooli20.bmp");
    myButton_2->setBorder(false);
    display_.addElement(myButton_2);

    myImage_1 = new Image("test_i", "toolicon/tooli21.bmp", 0, 0);
    display_.addElement(myImage_1);

    myImage_2 = new Image("test_i2", "toolicon/tooli18.bmp", 2, 6);
    display_.addElement(myImage_2);

    display_.cls(Color(100,100,100));

    Serial.print("Initializing SD card... " + ADA_SDCS);

    if (!SD.begin(ADA_SDCS))
    {
        Serial.println(F("failed!"));
        return;
    }
    Serial.println(F("SD OK!"));

    Serial.println(F("remote initialized"));
    Helper::runRFM();
}

/** @brief Direct access to Display sub-component.
  *
  * @see Display
  */
AdafruitOLED Remote::getDisplay()
{
    return display_;
}

/** @brief Direct access to UserInterface sub-component.
  *
  * @see UserInterface
  */
UserInterface Remote::getInterface()
{
    return interface_;
}

/** @brief Poll all elements that make up the user interface of this unit and refresh display periodically.
  *
  * @see MocoUnit, UserInterface, Display
  */
void Remote::update()
{
    MocoMessage recmessage;

    if(millis() > polltime_+POLL_CYCLE)
    {

        if(millis() > disptime_+DISPLAY_REFRESH)
        {
            screenupdate_ = true;
            disptime_ = millis();
            Helper::pauseRFM();
            display_.paint();
            Helper::runRFM();
        }
        else
        {
            screenupdate_ = false;
        }

        for (InputElement* ie : interface_.inputElemList)
        {
            ie->callbackFunction(ie);
        }
        polltime_ = millis();
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

/** @brief Handle incoming messages: Battery status and signal strength perceived by receiver. Display these values.
  *
  * @see MocoMessage
  */
void Remote::dispatchMessage(MocoMessage mymsg)
{
    int cmd = mymsg.getMsgCommand();

    switch(cmd)
    {
        case MocoCommand::EM_DISP_MSG:
            char voltText[4];
            char signalText[8];
            parseReceiverStatus(mymsg.getMsgData());
            Helper::pauseRFM();
            char buffer[20];
            Helper::ftoa(receiverVoltage_, buffer, 1, ",");
            strcat(buffer, " * ");
            char rssi_buf[6];
            itoa(receiverRSSI_, rssi_buf, 10);
            strcat(buffer, rssi_buf);
            myLabel_1->setText(buffer);
            Helper::runRFM();
    }
}

/** @brief Extract receiver data (voltage, signal strength) from data set just received.
  *
  */
void Remote::parseReceiverStatus(char* data)
{
    char* token;
    char* dummyptr;

    token = strtok(data, "|");
    if(token != nullptr)
    {
        receiverVoltage_ = atof(token);
        receiverRSSI_ = strtol(token+strlen(token)+1, &dummyptr, 10);
    }
}

/** @brief Callback function for poll cycle of the two pushbuttons  attached to this unit.
  *
  * @see ButtonInput, UserInterface
  */
void Remote::handleButton(InputElement* ie)
{
    ButtonInput* bi = (ButtonInput*)ie;
    bool oldState = bi->getOldState();
    bool currentState = bi->getBoolData(InputDataType::KEYSTATE);

    if(strcmp(bi->getName(), "b_lu") == 0)
    {
        if(currentState != oldState)
        {
            if(currentState)
            {
                myButton_1->up();
                myButton_2->down();
            }
            else
            {
                myButton_1->down();
                myButton_2->up();
            }
        }
    }
    else if(strcmp(bi->getName(), "b_ru") == 0)
    {
        if(currentState != oldState)
        {
            if(currentState)
            {
                myImage_1->setFile("toolicon/tooli21.bmp");
                myImage_2->setFile("toolicon/tooli18.bmp");
            }
            else
            {
                myImage_1->setFile("toolicon/tooli20.bmp");
                myImage_2->setFile("toolicon/tooli17.bmp");
            }
        }
    }
}

/** @brief Callback function for poll cycle of the two pushbuttons  attached to this unit.
  *
  * @see JoystickInput, UserInterface
  */
void Remote::handleJoystick(InputElement* ie)
{
    JoystickInput* ji = (JoystickInput*)ie;

    if(strcmp(ji->getName(), "joyr") == 0)
    {
        int oldpercent = ji->getXState();
        int xpercent = ji->getIntData(InputDataType::XPERCENT);

        if(ji->getBoolData(InputDataType::KEYSTATE) && (Helper::dif(oldpercent, xpercent) > 2))
        {
            rfmModule_.send((int)MocoCommand::EM_ACT_SERVO_ANGLE, false, xpercent);
        }
    }
    else if(strcmp(ji->getName(), "joyl") == 0)
    {
        int oldpercent = ji->getYState();
        int ypercent = ji->getIntData(InputDataType::YPERCENT);

        if(ji->getBoolData(InputDataType::KEYSTATE) && (Helper::dif(oldpercent, ypercent) > 2))
        {
            if((ypercent < 48) || (ypercent > 52))
            {
                if(ypercent < 48)
                {
                    rfmModule_.send((int)MocoCommand::EM_ACT_DCM_CW, false, 0L);
                    ypercent = map((49 - ypercent), 1, 48, 1, 100);

                }
                else if (ypercent > 52)
                {
                    rfmModule_.send((int)MocoCommand::EM_ACT_DCM_CCW, false, 0L);
                    ypercent = map((ypercent - 52), 1, 48, 1, 50);
                }

                rfmModule_.send((int)MocoCommand::EM_ACT_DCM_SPEED, false, ypercent);
            }
            else
            {
                rfmModule_.send((int)MocoCommand::EM_ACT_DCM_SPEED, false, 0L);
            }
        }
        else if (!ji->getKeyState())
        {
            rfmModule_.send((int)MocoCommand::EM_ACT_DCM_BRAKE, false, 0L);
        }
    }
}

/** @brief Retrieve battery voltage using the Adafruit Feather M0 RFM69 battery pin
  *
  */
float Remote::checkBattery()
{
    float measuredvbat = analogRead(VBATPIN);
    measuredvbat *= 2; // we divided by 2, so multiply back
    measuredvbat *= 3.3; // Multiply by 3.3V, our reference voltage
    measuredvbat /= 1024; // convert to voltage

    return measuredvbat;
}
