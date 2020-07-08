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

#include "../include/TestCase.hpp"

/** @brief A test module using several elements of the ErsatzMoco framework.
  *
  * @see MocoUnit, MocoUnitType, Remote, KSRemote, UserInterface, RFM69_Module
  */
extern TestCase thisUnit;

MonoOLED TestCase::display_ = MonoOLED();
UserInterface TestCase::interface_ = UserInterface();
RFM69_Module TestCase::rfmModule_ = RFM69_Module(REMOTE_ID, RECEIVER_ID, NETWORK_ID, &MocoUnit::handleAlert);
Sercom_Module TestCase::serModule_ = Sercom_Module(0, 9600, &MocoUnit::handleAlert);

unsigned long TestCase::dispTimeOut_ = 0;
unsigned long TestCase::pollTimeOut_ = 0;

float TestCase::receiverVoltage_ = 0.0;
int TestCase::receiverRSSI_ = 0;
bool TestCase::screenupdate_ = false;
bool TestCase::stepperMoving_ = false;

Label* TestCase::myLabel_1 = nullptr;
Label* TestCase::myLabel_2 = nullptr;
Button* TestCase::myButton_1 = nullptr;
//Button* TestCase::myButton_2 = nullptr;
ImageButton* TestCase::myButton_2 = nullptr;
Image* TestCase::myImage_1 = nullptr;
Image* TestCase::myImage_2 = nullptr;

TestCase::TestCase() : MocoUnit()
{
    //ctor
    id_ = REMOTE_ID;
    type_ = MocoUnitType::TESTCASE;
}

TestCase::~TestCase()
{
    //dtor
}

void TestCase::initTestCase()
{
    dispTimeOut_ = millis() + DISPLAY_REFRESH;
    pollTimeOut_ = millis() + POLL_CYCLE;
    screenupdate_ = false;
    setMicroClock(50, &TestCase::updateSteppers);

    rfmModule_.init();
    serModule_.init();


    StepperUnit* myae = new StepperUnit("stepper","DRV8834");
    myae->connectPins(9, 10, 11, 12);
    myae->setParams(200, 2000, 32);
    //MocoUnit::ae_List_.push_back(myae);
    addActionElement(myae);

    //build interface
    interface_.addButton("b_lu", 13, &TestCase::handleButton);
    //interface_.addButton("b_ru", 12, &TestCase::handleButton);
    //interface_.addJoystick("joyl", A0, A1, A2, &TestCase::handleJoystick);
    //interface_.addJoystick("joyr", A3, A4, A5, &TestCase::handleJoystick);

    //build visuals
    Helper::pauseRFM();
    display_.initDisplay();

    myLabel_1 = new Label("test_l", "Label", 2, 0, 8, 2);
    //myLabel_1->setColors(Color(255,255,255), Color(0,0,255), Color(255,0,0));
    myLabel_1->setColors(Color(255,255,255), Color(0,0,0), Color(255,255,255));
    display_.addElement(myLabel_1);

    myLabel_2 = new Label("test_2", "Label2", 5, 3, 8, 2);
    //myLabel_1->setColors(Color(255,255,255), Color(0,0,255), Color(255,0,0));
    myLabel_2->setColors(Color(255,255,255), Color(0,0,0), Color(255,255,255));
    display_.addElement(myLabel_2);

    myButton_1 = new Button("test_b", "A", 0, 3, 2, 2);
    //myButton_1->setColors(Color(255,255,255), Color(0,0,255), Color(255,0,0));
    myButton_1->setColors(Color(255,255,255), Color(0,0,0), Color(255,255,255));
    myButton_1->setDownText("B");
    display_.addElement(myButton_1);

//    myButton_2 = new Button("test_b", "2", 4, 3, 2, 2);
//    //myButton_2->setColors(Color(255,255,255), Color(0,0,255), Color(255,0,0));
//    myButton_2->setColors(Color(255,255,255), Color(0,0,0), Color(255,255,255));
//    myButton_2->setDownText("3");
//    display_.addElement(myButton_2);

    myButton_2 = new ImageButton("test_b2", "\0", 2, 3, 4, 4);
    myButton_2->setColors(Color(255,255,255), Color(0,0,255), Color(255,0,0));
    myButton_2->setImageFiles("mono_a.bmp", "mono_b.bmp");
    myButton_2->setBorder(false);
    display_.addElement(myButton_2);

//    myImage_1 = new Image("test_i", "toolicon/tooli21.bmp", 0, 0);
//    display_.addElement(myImage_1);
//
//    myImage_2 = new Image("test_i2", "toolicon/tooli18.bmp", 2, 5);
//    display_.addElement(myImage_2);

    display_.cls();


    Serial.print("Initializing SD card... " + ADA_SDCS);

    if (!SD.begin(ADA_SDCS))
    {
        Serial.println(F("failed!"));
        return;
    }
    Serial.println(F("SD OK!"));

    Serial.println(F("TestCase initialized"));
    Helper::runRFM();
}

MonoOLED TestCase::getDisplay()
{
    return display_;
}

UserInterface TestCase::getInterface()
{
    return interface_;
}

void TestCase::update()
{
    microClock();

    if(speedReport_)
    {
        testSpeed();
    }

    MocoMessage recmessage;

    if(millis() > pollTimeOut_)
    {
        pollTimeOut_ = millis() + POLL_CYCLE;

        //if(millis() > dispTimeOut_)
        if((millis() > dispTimeOut_) && !TestCase::stepperMoving_)
        {
            dispTimeOut_ = millis()+DISPLAY_REFRESH;
            screenupdate_ = true;
            Helper::pauseRFM();
            display_.paint();
            Serial.println("Display paint");
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

    if(serModule_.receive())
    {
        recmessage = serModule_.receiveBuffer_.back();

        dispatchSerMessage(recmessage);
        if(!recmessage.ackRequired())
        {
            serModule_.receiveBuffer_.erase(serModule_.receiveBuffer_.end());
        }
    }

    rfmModule_.update();
    serModule_.update();
}

void TestCase::updateSteppers()
{
    if(MocoUnit::ae_List_.empty())
        return;

    StepperUnit* su = nullptr;

    TestCase::stepperMoving_ = false;

    for(int i = 0; i < MocoUnit::ae_List_.size(); i++)
    {
        ActionElement* ae = MocoUnit::ae_List_.at(i);
        if(ae->getType() == ActionElementType::STEPPERUNIT)
        {
            su = (StepperUnit*)ae;
            if(su->isMoving())
            {
                TestCase::stepperMoving_ = true;
                su->update();
            }
        }
    }
}

void TestCase::dispatchMessage(MocoMessage mymsg)
{
    int cmd = mymsg.getMsgCommand();

    switch(cmd)
    {
        case MocoCommand::EM_DISP_MSG:
            char voltText[4];
            char signalText[8];
            parseReceiverStatus(mymsg.getMsgData());
            char buffer[20];
            Helper::ftoa(receiverVoltage_, buffer, 1, ",");
            strcat(buffer, " * ");
            char rssi_buf[6];
            itoa(receiverRSSI_, rssi_buf, 10);
            strcat(buffer, rssi_buf);
            myLabel_1->setText(buffer);
            Serial.println(buffer);
    }
}

void TestCase::dispatchSerMessage(MocoMessage mymsg)
{
    int cmd = mymsg.getMsgCommand();

    switch(cmd)
    {
        case MocoCommand::EM_DISP_MSG:
            Serial.print("disp msg ");
            Serial.println(mymsg.getMsgData());
            myLabel_2->setText(mymsg.getMsgData());
            break;
        case MocoCommand::EM_ACT_SERVO_ANGLE:
            rfmModule_.sendRef(mymsg);
            break;
        default:
            MocoUnit::dispatchMessage(mymsg);
            break;
    }
}

void TestCase::parseReceiverStatus(char* data)
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

void TestCase::handleButton(InputElement* ie)
{
    if(TestCase::stepperMoving_)
        return;

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

                StepperUnit* myae = (StepperUnit*)thisUnit.getActionElement("stepper");
                myae->startStepperMotion();
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
        if(!currentState)
        {
            thisUnit.testSpeed(100000);
//            if(currentState)
//            {
//                myImage_1->setFile("toolicon/tooli21.bmp");
//                myImage_2->setFile("toolicon/tooli18.bmp");
//            }
//            else
//            {
//                myImage_1->setFile("toolicon/tooli20.bmp");
//                myImage_2->setFile("toolicon/tooli17.bmp");
//            }
        }
    }
}

void TestCase::handleJoystick(InputElement* ie)
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

float TestCase::checkBattery()
{
    float measuredvbat = analogRead(VBATPIN);
    measuredvbat *= 2; // we divided by 2, so multiply back
    measuredvbat *= 3.3; // Multiply by 3.3V, our reference voltage
    measuredvbat /= 1024; // convert to voltage

    return measuredvbat;
}
