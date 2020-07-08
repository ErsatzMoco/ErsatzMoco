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

#include "../include/Label.hpp"

/** @brief UI element class for a classic label symbol.
  *
  * May use text and colors.
  *
  * @see Button, DisplayElement
  */
Label::Label() : DisplayElement(),
    fontsize_(1),
    textPos_(Position())
{
    //ctor
    strcpy(name_, "empty");
    strcpy(text_, "\0");
    type_ = DisplayElementType::LABEL;
}

/** @brief Constructor using standard values and the provided name.
  *
  */
Label::Label(char* aname) : DisplayElement(),
    fontsize_(1),
    textPos_(Position())
{
    //ctor
    strncpy(name_, aname, 31);
    name_[31] = '\0';
    strcpy(text_, "\0");
    type_ = DisplayElementType::LABEL;
}

/** @brief Constructor building the element with provided name, text, grid position x/y and size w/h.
  *
  * @see Position, Shapesize
  */
Label::Label(char* aname, char* atext, int myx, int myy, int myw, int myh) :
    DisplayElement(),
    fontsize_(1),
    textPos_(Position())
{
    //ctor
    strncpy(name_, aname, 31);
    name_[31] = '\0';
    strncpy(text_, atext, 31);
    text_[31] = '\0';
    type_ = DisplayElementType::LABEL;

    gridSize_ = Shapesize(myw, myh);
    gridPos_ = Position(myx, myy);

    fgColor_ = Color();
    bgColor_ = Color();
    borderColor_ = Color();
    grid_ = true;
    currentDisplay_ = nullptr;
}

Label::~Label()
{
    //dtor
}

int Label::getFontSize()
{
    return fontsize_;
}

char* Label::getText()
{
    return text_;
}

void Label::setFontSize(int fs)
{
    fontsize_ = fs;
    redraw_ = true;
}

/** @brief Set label text (c-string) with max length of 30 characters.
  *
  */
void Label::setText(char* atext)
{
    strncpy(text_, atext, 31);
    text_[31] = '\0';
    redraw_ = true;
}

/** @brief Draw label.
  *
  */
void Label::draw()
{
    if(!currentDisplay_)
        return;

    if(grid_)
        reframeGridPos(); //re-align to grid bounds of display in case they were changed

    currentDisplay_->fillRect(pixelPos_, pixelSize_, bgColor_);
    currentDisplay_->drawRect(pixelPos_, pixelSize_, borderColor_);

    if(strlen(text_) > 0)
    {
        currentDisplay_->setFontSize(fontsize_);
        int16_t tx, ty;
        uint16_t tw, th;
        currentDisplay_->getTextBounds(text_, 0, 0, &tx, &ty, &tw, &th);

        int ix, iy;
        ix = std::max(0, (int)floor((pixelSize_.width-tw)/2));
        iy = std::max(0, (int)floor((pixelSize_.height-th)/2));
        textPos_ = Position(pixelPos_.x+ix, pixelPos_.y+iy);

        currentDisplay_->drawText(textPos_, text_, fgColor_, bgColor_);
    }

    redraw_ = false;
}
