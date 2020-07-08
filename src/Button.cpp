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

#include "../include/Button.hpp"

/** @brief UI element class for a classic button symbol.
  *
  * May use text and colors to indicate its two states up/down.
  *
  * @see Label, DisplayElement
  */
Button::Button() : Label()
{
    //ctor
    strcpy(name_, "empty");
    strcpy(text_, "\0");
    strcpy(downText_, "\0");

    type_ = DisplayElementType::BUTTON;
    isdown_ = false;
}

/** @brief Constructor using standard values and the provided name.
  *
  */
Button::Button(char* aname) : Label()
{
    //ctor
    strncpy(name_, aname, 31);
    name_[31] = '\0';
    strcpy(text_, "\0");
    strcpy(downText_, "\0");

    type_ = DisplayElementType::BUTTON;
    isdown_ = false;
}

/** @brief Constructor building the element with provided name, text, grid position x/y and size w/h.
  *
  * @see Position, Shapesize
  */
Button::Button(char* aname, char* atext, int myx, int myy, int myw, int myh) :
    Label()
{
    //ctor
    strncpy(name_, aname, 31);
    name_[31] = '\0';
    strncpy(text_, atext, 31);
    text_[31] = '\0';
    strcpy(downText_, "\0");

    gridSize_ = Shapesize(myw, myh);
    gridPos_ = Position(myx, myy);

    fgColor_ = Color();
    bgColor_ = Color();
    borderColor_ = Color();
    grid_ = true;
    currentDisplay_ = nullptr;
    isdown_ = false;

    type_ = DisplayElementType::BUTTON;
}

Button::~Button()
{
    //dtor
}

/** @brief Button down?
  *
  */
bool Button::isDown()
{
    return isdown_;
}

/** @brief Set button state to "down".
  *
  */
void Button::down()
{
	isdown_ = true;
	redraw_ = true;
}

/** @brief Set button state to "up".
  *
  */
void Button::up()
{
	isdown_ = false;
	redraw_ = true;
}

/** @brief Toggle button state.
  *
  */
void Button::toggle()
{
	isdown_ = !isdown_;
	redraw_ = true;
}

/** @brief Set button state using a boolean value (true = down, false = up).
  *
  */
void Button::setState(bool mystate)
{
	isdown_ = mystate;
	redraw_ = true;
}

/** @brief Set text displayed at button state "down" (30 characters max).
  *
  */
void Button::setDownText(char* atext)
{
    strncpy(downText_, atext, 31);
    text_[31] = '\0';
}

/** @brief Get text displayed at button state "down" (30 characters max).
  *
  */
char* Button::getDownText()
{
    return downText_;
}

/** @brief Draw button using elements of current button state.
  *
  */
void Button::draw()
{
    if(!currentDisplay_)
        return;

    if(grid_)
        reframeGridPos(); //re-align to grid bounds of display in case they were changed

    bool dispMono = currentDisplay_->isMono();

    Color shadow = borderColor_.darkenBy(80);
	Color innerShadow = bgColor_.darkenBy(80);
	Color darkerText = fgColor_.darkenBy(80);

	Color bgc = Color();
	Color fgc = Color(255,255,255);

	Color upleft, lowright;

	Position upl = pixelPos_;
	Position iupl = Position(pixelPos_.x+1, pixelPos_.y+1);
	Position lowr = Position(pixelPos_.x+pixelSize_.width-1, pixelPos_.y+pixelSize_.height-1);
	Position ilowr = Position(pixelPos_.x+pixelSize_.width-2, pixelPos_.y+pixelSize_.height-2);

	if(isdown_)
	{
	    if(!dispMono)
        {
            upleft = shadow;
            lowright = borderColor_;
            currentDisplay_->fillRect(pixelPos_, pixelSize_, innerShadow);
        }
        else
        {
            upleft = fgc;
            lowright = fgc;
            currentDisplay_->fillRect(pixelPos_, pixelSize_, bgc);
        }

	}
	else
	{
	    if(!dispMono)
        {
            lowright = shadow;
            upleft = borderColor_;
            currentDisplay_->fillRect(pixelPos_, pixelSize_, bgColor_);
        }
        else
        {
            upleft = fgc;
            lowright = fgc;
            currentDisplay_->fillRect(pixelPos_, pixelSize_, bgc);
        }
	}

	char* mytext;
	if(isdown_ && (strlen(downText_) > 0))
    {
        mytext = downText_;
    }
    else
    {
        mytext = text_;
    }

    if(strlen(text_) > 0)
    {
        currentDisplay_->setFontSize(fontsize_);
        int16_t tx, ty;
        uint16_t tw, th;
        currentDisplay_->getTextBounds(mytext, 0, 0, &tx, &ty, &tw, &th);

        int ix, iy;
        ix = std::max(0, (int)floor((pixelSize_.width-tw)/2));
        iy = std::max(0, (int)floor((pixelSize_.height-th)/2));
        textPos_ = Position(pixelPos_.x+ix, pixelPos_.y+iy);
        if(!dispMono)
        {
            if(isdown_)
                currentDisplay_->drawText(textPos_, mytext, darkerText, innerShadow);
            else
                currentDisplay_->drawText(textPos_, mytext, fgColor_, bgColor_);
        }
        else
        {
            currentDisplay_->drawText(textPos_, mytext, fgc, bgc);
        }
    }


    if(!dispMono || !isDown())
    {
        currentDisplay_->drawLine(upl, Position(upl.x, lowr.y), upleft);
        currentDisplay_->drawLine(upl, Position(lowr.x, upl.y), upleft);
    }
    currentDisplay_->drawLine(iupl, Position(iupl.x, ilowr.y), upleft);
    currentDisplay_->drawLine(iupl, Position(ilowr.x, iupl.y), upleft);

    if(!dispMono || isDown())
    {
        currentDisplay_->drawLine(lowr, Position(lowr.x, upl.y), lowright);
        currentDisplay_->drawLine(lowr, Position(upl.x, lowr.y), lowright);
    }
    currentDisplay_->drawLine(ilowr, Position(ilowr.x, iupl.y), lowright);
    currentDisplay_->drawLine(ilowr, Position(iupl.x, ilowr.y), lowright);

    redraw_ = false;
}

