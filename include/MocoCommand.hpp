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

#ifndef MocoCommand_H
#define MocoCommand_H

enum MocoCommand
{
    EM_ACK,                         //!< Acknowledge received command

    EM_REC_PING,                    //!< Wait for ping signal
    EM_SEND_PING,                   //!< Send ping signal

    EM_ACT_DCM_SPEED,               //!< DC motor: Set speed
    EM_ACT_DCM_STOP,                //!< DC motor: Stop motor
    EM_ACT_DCM_BRAKE,               //!< DC motor: force stop
    EM_ACT_DCM_CW,                  //!< DC motor: set direction to clockwise
    EM_ACT_DCM_CCW,                 //!< DC motor: set direction to counterclockwise

    EM_ACT_DOUT_HI,                 //!< Set the indicated digital output pin to high state
    EM_ACT_DOUT_LO,                 //!< Set the indicated digital output pin to low state
    EM_ACT_DOUT_INPUT,              //!< Set the indicated digital pin to input state
    EM_ACT_DOUT_OUTPUT,             //!< Set the indicated digital pin to output state

    EM_ACT_AOUT_VAL,                //!< Set the indicated analog pin to a value - see descendants of base type Actuator how to use this command

    EM_ACT_PWM_VAL,                 //!< Set the indicated analog pin to a pwm value - see descendants of base type Actuator how to use this command

    EM_ACT_AUDIO_TRACK,             //!< Audio module: Select track to play
    EM_ACT_AUDIO_PLAY,              //!< Audio module: Play selected track
    EM_ACT_AUDIO_STOP,              //!< Audio module: Set volume
    EM_ACT_AUDIO_VOL,               //!< Audio module: Stop playback

    EM_ACT_SERVO_ANGLE,             //!< Servo: Set angle

    EM_ACT_SERVOMOTOR_SPEED,        //!< Continuous servo: Set speed

    EM_DISP_MSG,                    //!< Display a message string
    EM_DISP_CHAR,                   //!< Display an ascii character indicated by an int number

    EM_ACT_STM_STEP,                //!< Stepper motor: Execute one step
    EM_ACT_STM_CW,                  //!< Stepper motor: Set direction to clockwise
    EM_ACT_STM_CCW,                 //!< Stepper motor: Set direction to counterclockwise
    EM_ACT_STM_SET_DIST,            //!< Stepper motor: Set distance to move the object driven by the stepper
    EM_ACT_STM_SET_TIME,            //!< Stepper motor: Set time frame for covering the currently set distance
    EM_ACT_STM_START,               //!< Stepper motor: Start motion from current position to distance in selected time frame
    EM_ACT_STM_STOP,                //!< Stepper motor: Stop the motor within the deceleration period set in the StepperUnit header
    EM_ACT_STM_EMERGENCY_STOP,      //!< Stepper motor: Stop motor immediately. CAUTION: This may damage the motor!
    EM_ACT_STM_ENABLE,              //!< Stepper motor: Set driver to "enabled", i.e. connect power and force motor to maintain current position
    EM_ACT_STM_DISABLE,             //!< Stepper motor: Set driver to "disbled", i.e. disconnect power. Motor may be moved by external forces.

	EM_ACT_RELAY_OFF,				//!< Relay: Open contact
	EM_ACT_RELAY_ON					//!< Relay: Close contact

};

#endif // MocoCommand_H
