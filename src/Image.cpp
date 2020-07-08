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

#include "../include/Image.hpp"

/** @brief An UI element providing a bitmap image from SD card.
  *
  * @see DisplayElement
  */
Image::Image() : DisplayElement()
{
    //ctor
    strcpy(name_, "empty");
    strcpy(filename_, "\0");
    type_ = DisplayElementType::IMAGE;
}

/** @brief Construct element providing a file name as c string.
  *
  */
Image::Image(char* aname) : DisplayElement()
{
    //ctor
    strncpy(name_, aname, 31);
    name_[31] = '\0';
    strcpy(filename_, "\0");
    type_ = DisplayElementType::IMAGE;
}

/** @brief Construct element providing an element name and file name as c strings and a horizontal and vertical grid position.
  *
  */
Image::Image(char* aname, char* afile, int myx, int myy) : DisplayElement()
{
    //ctor
    strncpy(name_, aname, 31);
    name_[31] = '\0';
    strncpy(filename_, afile, 63);
    filename_[63] = '\0';
    type_ = DisplayElementType::IMAGE;


    gridPos_ = Position(myx, myy);

    grid_ = true;
    currentDisplay_ = nullptr;
}

Image::~Image()
{
    //dtor
}

/** @brief Assign new bitmap file from SD card to this element.
  *
  */
void Image::setFile(char* afile)
{
    strncpy(filename_, afile, 63);
    filename_[63] = '\0';
    redraw_ = true;
}

/** @brief Draw this bitmap element.
  *
  */
void Image::draw()
{
    if(!currentDisplay_ || (strlen(filename_) < 1))
        return;

    if(grid_)
        reframeGridPos(); //re-align to grid bounds of display in case they were changed

    pixelSize_ = currentDisplay_->bmpGridDraw(filename_, gridPos_);
    redraw_ = false;
}

