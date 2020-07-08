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

#include "../include/Color.hpp"

/** @brief Color handling class (8 bit per color component).
  *
  * Automatically converts color values for use with color, greyscale and monochrome displays.
  *
  */
Color::Color() : red(0), green(0), blue(0)
{
    //ctor
}

/** @brief Constructor using red, green and blue values.
  *
  */
Color::Color(unsigned int r, unsigned int g, unsigned int b)
{
    //ctor
    r = constrain(r, 0, 255);
    g = constrain(g, 0, 255);
    b = constrain(b, 0, 255);

    red = r;
    green = g;
    blue = b;
}

Color::~Color()
{
    //dtor
}

/** @brief Calculate darkened color by subtracting an int value from all three color components.
  *
  */
Color Color::darkenBy(int darken)
{
    int r, g, b;

    int rd = red-darken;
    int gd = green-darken;
    int bd = blue-darken;

    r = (rd > 0) ? rd : 0;
    g = (gd > 0) ? gd : 0;
    b = (bd > 0) ? bd : 0;

    return Color(r, g, b);
}

/** @brief Calculate brightened color by adding an int value to all three color components.
  *
  */
Color Color::brightenBy(int brighten)
{
    int r, g, b;

    r = constrain(red+brighten, 0, 255);
    g = constrain(green+brighten, 0, 255);
    b = constrain(blue+brighten, 0, 255);

    return Color(r, g, b);
}

/** @brief Convert 8 bit RGB color to special combined 16 bit color value.
  *
  */
uint16_t Color::toAdafruitColor()
{
    uint16_t result = 0;

    red = constrain(red, 0, 255);
    green = constrain(green, 0, 255);
    blue = constrain(blue, 0, 255);

    uint16_t redbits = (uint16_t)(red / 8);
    uint16_t greenbits = (uint16_t)(green / 4);
    uint16_t bluebits = (uint16_t)(blue / 8);

    result = result | (redbits << 11) | (greenbits << 5) | bluebits;

    return result;
}

/** @brief Calculate greyscale value of this RGB color adjusted to optimized color approximation.
  *
  */
int Color::toGrayscale()
{
    int result = floor(0.21*red + 0.72*green + 0.07*blue);
    return constrain(result, 0, 255);
}

/** @brief Calculate B/W value of this RGB color using a threshold.
  *
  */
int Color::toBW(int threshold)
{
    if(toGrayscale() > threshold)
        return 1;
    else
        return 0;
}
