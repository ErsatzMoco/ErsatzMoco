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

#include "../include/Helper.hpp"

/** @brief Class providing several helper methods.
  *
  */
Helper::Helper()
{
    //ctor
}

Helper::~Helper()
{
    //dtor
}

/** @brief Wrapper to switch off RFM 69 module on an Adafruit Feather M0 board.
  *
  */
void Helper::pauseRFM()
{
    digitalWrite(8, HIGH);
}

/** @brief Wrapper to switch on RFM 69 module on an Adafruit Feather M0 board.
  *
  */
void Helper::runRFM()
{
    digitalWrite(8, LOW);
}

/** @brief Implementation of common c ftoa method: Convert float value to c string.
  *
  */
char* Helper::ftoa(double f, char *a, int precision, char* delim)
{
    long p[] = {0,10,100,1000,10000,100000,1000000,10000000,100000000};
    char *ret = a;

    if(f < 0)
    {//negative
        f = -f;
        *a++ = '-';
    }//negative

    long i = floor(f);
    f -= i;
    itoa(i, a, 10);
    while (*a != '\0') a++;
    *a++ = *delim;
    itoa(f*p[precision], a, 10);

    return ret;
}

/** @brief Calculate absolute difference between two integer values.
  *
  */
int Helper::dif(int val1, int val2)
{
    return abs(val1-val2);
}
