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

#include "../include/Display.hpp"

/** @brief Abstract base class for any display hardware. This is an interface defining the methods every display driver class should implement to provide user interface graphics handling that is portable between different displays.
  *
  * This interface provides a basic grid layout engine for quick and easy UI construction while maintaining pixel accurate access to the display as well. The grid may be defined as a window on the display, i.e. not using the maximum pixel resolution but only a part of the display pane. That way a whole set of elements can be repositioned by altering just the grid boundaries.
  *
  * @see AdafruitILI, AdafruitOLED, AdafruitTFT, Position, Shapesize
  */
Display::Display()
{
    //ctor
}

Display::~Display()
{
    //dtor
}

// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

/** @brief Helper method to read 16-bit types from an SD card file.
  *
  */
uint16_t Display::read16(File &f)
{
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

/** @brief Helper method to read 32-bit types from an SD card file.
  *
  */
uint32_t Display::read32(File &f)
{
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}

/** @brief Retrieve defined grid spacer values.
  *
  * @see Shapesize
  */
Shapesize Display::getGridSpace()
{
    return gridSpace_;
}

/** @brief Retrieve current horizontal start pixel position of display grid.
  *
  */
int Display::getGridMinX()
{
    return gridMinX_;
}

/** @brief Retrieve current horizontal end pixel position of display grid.
  *
  */
int Display::getGridMaxX()
{
    return gridMaxX_;
}

/** @brief Retrieve current vertical start pixel position of display grid.
  *
  */
int Display::getGridMinY()
{
    return gridMinY_;
}

/** @brief Retrieve current vertical end pixel position of display grid.
  *
  */
int Display::getGridMaxY()
{
    return gridMaxY_;
}

/** @brief Retrieve max width of display in pixels (defined by hardware).
  *
  */
int Display::getPixMaxX()
{
    return pixMaxX_;
}

/** @brief Retrieve max height of display in pixels (defined by hardware).
  *
  */
int Display::getPixMaxY()
{
    return pixMaxY_;
}

/** @brief Retrieve current start pixel position of display grid.
  *
  * @see Position
  */
Position Display::getGridTopLeft()
{
    return Position(gridMinX_, gridMinY_);
}

/** @brief Retrieve current end pixel position of display grid.
  *
  * @see Position
  */
Position Display::getGridBottomRight()
{
    return Position(gridMaxX_, gridMaxY_);
}

/** @brief Retrieve current maximum pixel values of display (defined by hardware).
  *
  * @see Position
  */
Position Display::getPixBottomRight()
{
    return Position(pixMaxX_, pixMaxY_);
}

/** @brief Set new grid spacer values.
  *
  * @see Shapesize
  */
void Display::setGridSpace(Shapesize gs)
{
    gridSpace_.width = std::min(gs.width, pixMaxX_);
    gridSpace_.height = std::min(gs.height, pixMaxY_);

    int maxgrid_x = pixMaxX_ % gridSpace_.width;
    int maxgrid_y = pixMaxY_ % gridSpace_.height;

    gridMaxX_ = std::min(maxgrid_x, gridMaxX_);
    gridMaxY_ = std::min(maxgrid_y, gridMaxY_);
}

/** @brief Set new horizontal start position of grid in pixels.
  *
  */
void Display::setGridMinX(int gmix)
{
    gridMinX_ = std::max(0, gmix);
}

/** @brief Set new vertical start position of grid in pixels.
  *
  */
void Display::setGridMinY(int gmiy)
{
    gridMinY_ = std::max(0, gmiy);
}

/** @brief Set new horizontal end position of grid in pixels.
  *
  */
void Display::setGridMaxX(int gmax)
{
    gridMaxX_ = std::min(gmax, (int)(pixMaxX_ / gridSpace_.width));
}

/** @brief Set new vertical end position of grid in pixels.
  *
  */
void Display::setGridMaxY(int gmay)
{
    gridMaxY_ = std::min(gmay, (int)(pixMaxY_ / gridSpace_.height));
}

/** @brief Set start pixel position of display grid.
  *
  * @see Position
  */
void Display::setGridMin(Position gmin)
{
    setGridMinX(gmin.x);
    setGridMinY(gmin.y);
}

/** @brief Set end pixel position of display grid.
  *
  * @see Position
  */
void Display::setGridMax(Position gmax)
{
    setGridMaxX(gmax.x);
    setGridMaxY(gmax.y);
}

/** @brief Set pixel boundaries of display grid.
  *
  * @see Position
  */
void Display::setGridBorders(Position topLeft, Position bottomRight)
{
    setGridMinX(topLeft.x);
    setGridMinY(topLeft.y);

    setGridMaxX(bottomRight.x);
    setGridMaxY(bottomRight.y);
}

/** @brief Convert grid position to pixel position.
  *
  * @see Position
  */
Position Display::gridToPix(Position gpos)
{
    gpos.x = constrain(gpos.x, gridMinX_, gridMaxX_);
    gpos.y = constrain(gpos.y, gridMinY_, gridMaxY_);

    Position pixpos;
    pixpos.x = (gridMinX_ * gridSpace_.width) + (gpos.x * gridSpace_.width);
    pixpos.y = (gridMinY_ * gridSpace_.height) + (gpos.y * gridSpace_.height);

    return pixpos;
}
