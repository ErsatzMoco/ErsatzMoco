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

#include "../include/ImageButton.hpp"

/** @brief UI element class for a button symbol that may display images.
  *
  * May use bitmaps from SD for displaying its two states up/down as well as text and colors. Note: Currently the image files are not passed to the constructor but using the separate method setImageFiles.
  *
  * @see Label, DisplayElement
  */
ImageButton::ImageButton() : Button()
{
    //ctor
    strcpy(name_, "empty");
    strcpy(text_, "\0");
    strcpy(downText_, "\0");
    strcpy(upfile_, "\0");
    strcpy(downfile_, "\0");
    type_ = DisplayElementType::IMAGEBUTTON;
    isdown_ = false;
    border_ = true;
}

/** @brief Constructor using standard values and the provided name.
  *
  */
ImageButton::ImageButton(char* aname) : Button()
{
    //ctor
    strncpy(name_, aname, 31);
    name_[31] = '\0';
    strcpy(text_, "\0");
    strcpy(downText_, "\0");
    strcpy(upfile_, "\0");
    strcpy(downfile_, "\0");
    type_ = DisplayElementType::IMAGEBUTTON;
    isdown_ = false;
    border_ = true;
}

/** @brief Constructor building the element with provided name, text, grid position x/y and size w/h.
  *
  * @see Position, Shapesize
  */
ImageButton::ImageButton(char* aname, char* atext, int myx, int myy, int myw, int myh) :
    Button()
{
    //ctor
    strncpy(name_, aname, 31);
    name_[31] = '\0';
    strncpy(text_, atext, 31);
    text_[31] = '\0';
    strcpy(downText_, "\0");
    strcpy(upfile_, "\0");
    strcpy(downfile_, "\0");

    gridSize_ = Shapesize(myw, myh);
    gridPos_ = Position(myx, myy);

    fgColor_ = Color();
    bgColor_ = Color();
    borderColor_ = Color();
    grid_ = true;
    currentDisplay_ = nullptr;
    isdown_ = false;
    border_ = true;

    type_ = DisplayElementType::IMAGEBUTTON;
}

ImageButton::~ImageButton()
{
    //dtor
}

/** @brief Provide the file paths (c-strings) of two images that are loaded from SD card (via SD library).
  *
  */
void ImageButton::setImageFiles(char* upfile, char* downfile)
{
    strncpy(upfile_, upfile, 63);
    upfile_[63] = '\0';
    strncpy(downfile_, downfile, 63);
    downfile_[63] = '\0';
    redraw_ = true;
}

/** @brief Get current file name for state "up".
  *
  */
char* ImageButton::getUpFile()
{
    return upfile_;
}

/** @brief Get current file name for state "down".
  *
  */
char* ImageButton::getDownFile()
{
    return downfile_;
}

/** @brief Display button with or without bounding box/border.
  *
  */
void ImageButton::setBorder(bool hasborder)
{
    border_ = hasborder;
}

/** @brief Check whether this button currently uses a bounding box/border.
  *
  */
bool ImageButton::hasBorder()
{
    return border_;
}

/** @brief Draw button using elements of current button state.
  *
  */
void ImageButton::draw()
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

	Shapesize imgsize;
	int xoff = 0, yoff = 0;

	if(isdown_)
	{
	    if(border_)
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

		if(strlen(downfile_) > 0)
        {
            imgsize = currentDisplay_->readBmpSize(downfile_);
            if(border_)
            {
                xoff = constrain(floor((pixelSize_.width-imgsize.width)/2), 0, floor(pixelSize_.width/2));
                yoff = constrain(floor((pixelSize_.height-imgsize.height)/2), 0, floor(pixelSize_.height/2));
            }
            currentDisplay_->bmpDraw(downfile_, pixelPos_.x+xoff+1, pixelPos_.y+yoff+1);
        }
	}
	else
	{
	    if(border_)
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

		if(strlen(upfile_) > 0)
        {
            imgsize = currentDisplay_->readBmpSize(upfile_);
            if(border_)
            {
                xoff = constrain(floor((pixelSize_.width-imgsize.width)/2), 0, floor(pixelSize_.width/2));
                yoff = constrain(floor((pixelSize_.height-imgsize.height)/2), 0, floor(pixelSize_.height/2));
            }
            currentDisplay_->bmpDraw(upfile_, pixelPos_.x+xoff, pixelPos_.y+yoff);
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

    if(strlen(mytext) > 0)
    {
        currentDisplay_->setFontSize(fontsize_);
        int16_t tx, ty;
        uint16_t tw, th;
        currentDisplay_->getTextBounds(mytext, 0, 0, &tx, &ty, &tw, &th);

        int ix, iy;
        ix = std::max(0, (int)floor((pixelSize_.width-tw)/2));
        iy = std::max(0, (int)floor((pixelSize_.height-th)/2));
        textPos_ = Position(pixelPos_.x+ix, pixelPos_.y+iy);
        if(isdown_)
            currentDisplay_->drawText(textPos_, mytext, darkerText);
        else
            currentDisplay_->drawText(textPos_, mytext, fgColor_);
    }

    if(border_)
    {
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
    }

    redraw_ = false;
}
