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

#include "../include/DisplayElement.hpp"

/** @brief Abstract base class for any UI element on a display.
  *
  * Elements may be positioned using either grid or pixel position and size.
  *
  * @see Label, Button, Image, ImageButton, DisplayElementType
  */
DisplayElement::DisplayElement() :  pixelPos_(Position(0, 0)),
    gridPos_(Position (0, 0)),
    gridSize_(Shapesize(1, 1)),
    pixelSize_(Shapesize(1, 1)),
    fgColor_(Color()),
    bgColor_(Color()),
    borderColor_(Color()),
    grid_(true),
    redraw_(true),
    currentDisplay_(nullptr)
{
    //ctor
    strcpy(name_, "empty");
}

DisplayElement::~DisplayElement()
{
    //dtor
}

/** @brief Redraw element. Needs to be overwritten.
  *
  */
void DisplayElement::draw()
{
    redraw_ = false;
    Serial.println("DISPLAY ELEMENT draw");
}

/** @brief Position element on display using two pixel coordinates.
  *
  */
void DisplayElement::setPixelPos(int myx, int myy)
{
    if(!currentDisplay_)
        return;
    pixelPos_.x = constrain(myx, 0, currentDisplay_->getPixMaxX());
    pixelPos_.y = constrain(myy, 0, currentDisplay_->getPixMaxY());

    grid_ = false;
    redraw_ = true;
}

/** @brief Position element on display using a pixel position.
  *
  * @see Position
  */
void DisplayElement::setPixelPos(Position mypos)
{
    setPixelPos(mypos.x, mypos.y);
}

/** @brief Position element on display using two grid coordinates.
  *
  */
void DisplayElement::setGridPos(int myx, int myy)
{
    if(!currentDisplay_)
        return;
    gridPos_.x = constrain(myx, currentDisplay_->getGridMinX(), currentDisplay_->getGridMaxX());
    gridPos_.y = constrain(myy, currentDisplay_->getGridMinY(), currentDisplay_->getGridMaxY());

    grid_ = true;

    calculatePixelPos();
    redraw_ = true;
}

/** @brief Position element on display using a pixel position.
  *
  * @see Position
  */
void DisplayElement::setGridPos(Position mypos)
{
    setGridPos(mypos.x, mypos.y);
}

/** @brief Set size of element using width and height pixel values.
  *
  */
void DisplayElement::setSize(int width, int height)
{
    if(!currentDisplay_)
        return;

    grid_ = false;
    pixelSize_.width = constrain(width, 1, currentDisplay_->getPixMaxX());
    pixelSize_.height = constrain(height, 1, currentDisplay_->getPixMaxY());
    redraw_ = true;
}

/** @brief Set element size using a pixel size object.
  *
  * @see Shapesize
  */
void DisplayElement::setSize(Shapesize mysize)
{
    setSize(mysize.width, mysize.height);
}

/** @brief Set element's grid size providing grid width and height values (uses current grid spacing values).
  *
  */
void DisplayElement::setGridSize(int width, int height)
{
    if(!currentDisplay_)
        return;

    grid_ = true;

    int maxx = currentDisplay_->getGridMaxX()-currentDisplay_->getGridMinX();
    int maxy = currentDisplay_->getGridMaxY()-currentDisplay_->getGridMinY();

    gridSize_.width = std::min(width, maxx);
    gridSize_.height = std::min(height, maxy);
    redraw_ = true;
}

/** @brief Set element's grid size providing a grid size object (uses current grid spacing values).
  *
  * @see Shapesize
  */
void DisplayElement::setGridSize(Shapesize mysize)
{
    setGridSize(mysize.width, mysize.height);

}

/** @brief Internal function that recalculates pixel position and size values if grid parameters have changed.
  *
  */
void DisplayElement::reframeGridPos()
{
    if(!currentDisplay_)
        return;

    int cdminX = currentDisplay_->getGridMinX();
    int cdminY = currentDisplay_->getGridMinY();
    int cdmaxX = currentDisplay_->getGridMaxX();
    int cdmaxY = currentDisplay_->getGridMaxY();

    gridPos_.x = constrain(gridPos_.x, cdminX, cdmaxX);
    gridPos_.y = constrain(gridPos_.y, cdminY, cdmaxY);

    gridSize_.width = std::min(gridSize_.width, (cdmaxX-cdminX));
    gridSize_.height = std::min(gridSize_.height, (cdmaxY-cdminY));

    calculatePixelPos();
    redraw_ = true;
}

/** @brief Get current pixel position of element.
  *
  * @see Position
  */
Position DisplayElement::getPixelPos()
{
    return pixelPos_;
}

/** @brief Get current grid position of element.
  *
  * @see Position
  */
Position DisplayElement::getGridPos()
{
    return gridPos_;
}

/** @brief Get type of this element.
  *
  * @see DisplayElementType
  */
DisplayElementType DisplayElement::getType()
{
    return type_;
}

/** @brief Alter type of this element and redraw.
  *
  * @see DisplayElementType
  */
void DisplayElement::setType(DisplayElementType t)
{
    type_ = t;
    redraw_ = true;
}

/** @brief Get name of this element.
  *
  */
char* DisplayElement::getName()
{
    return name_;
}

/** @brief Set name of this element.
  *
  */
void DisplayElement::setName(char* aname)
{
    strcpy(name_, aname);
}

/** @brief Assign this element to a new display hardware.
  *
  */
void DisplayElement::setDisplay(Display* myd)
{
    currentDisplay_ = myd;
    if(grid_)
    {
        reframeGridPos();
    }
    redraw_ = true;
}

/** @brief Recalculate pixel position of this element.
  *
  */
void DisplayElement::calculatePixelPos()
{
    if(!currentDisplay_)
        return;

    if(grid_)
    {
        pixelPos_ = currentDisplay_->gridToPix(gridPos_);

        Shapesize gridspace = currentDisplay_->getGridSpace();
        pixelSize_.width = constrain(gridSize_.width*gridspace.width, 1, currentDisplay_->getPixMaxX());
        pixelSize_.height = constrain(gridSize_.height*gridspace.height, 1, currentDisplay_->getPixMaxY());
    }
}

/** @brief Get foreground color of this element.
  *
  */
Color DisplayElement::getFGColor()
{
    return fgColor_;
}

/** @brief Get background color of this element.
  *
  */
Color DisplayElement::getBGColor()
{
    return bgColor_;
}

/** @brief Get border color of this element.
  *
  */
Color DisplayElement::getBorderColor()
{
    return borderColor_;
}

/** @brief Set foreground color of this element.
  *
  */
void DisplayElement::setFGColor(Color mycol)
{
    fgColor_ = mycol;
    redraw_ = true;
}

/** @brief Set background color of this element.
  *
  */
void DisplayElement::setBGColor(Color mycol)
{
    bgColor_ = mycol;
    redraw_ = true;
}

/** @brief Set border color of this element.
  *
  */
void DisplayElement::setBorderColor(Color mycol)
{
    borderColor_ = mycol;
    redraw_ = true;
}

/** @brief Set foreground, background and border color of this element.
  *
  */
void DisplayElement::setColors(Color fg, Color bg, Color bd)
{
    fgColor_ = fg;
    bgColor_ = bg;
    borderColor_ = bd;
    redraw_ = true;
}

/** @brief Reassign this element to grid layout engine.
  *
  */
void DisplayElement::useGrid(bool ongrid)
{
    grid_ = ongrid;
    redraw_ = true;
}

/** @brief Check if this element currently uses grid layout engine.
  *
  */
bool DisplayElement::usesGrid()
{
    return grid_;
}

/** @brief Check if this element is marked for next display redraw cycle (i.e. needs to be redrawn).
  *
  */
bool DisplayElement::redraw()
{
    return redraw_;
}
