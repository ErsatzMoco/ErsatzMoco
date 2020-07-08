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

#ifndef ADAFRUITILI_H
#define ADAFRUITILI_H

#include <Arduino.h>
#include <SD.h>
#include <vector>

// You can use any (4 or) 5 pins
//#define ADA_SCLK 24 //hardware SPI - not used in constructor!
//#define ADA_MOSI 23 //hardware SPI - not used in constructor!
#define ADA_SCLK 24
#define ADA_MOSI 23
#define ADA_CS   10
#define ADA_RST  9
#define ADA_DC   6

#define ADA_MISO 22
#define ADA_SDCS 5

#define BUFFPIXEL 20

// Color definitions
#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0
#define WHITE           0xFFFF

#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "SPI.h"
#include "Display.hpp"
#include "Label.hpp"
#include "Button.hpp"
#include "Image.hpp"
#include "ImageButton.hpp"

class AdafruitILI : public Display
{
    public:
        AdafruitILI();
        virtual ~AdafruitILI();

        virtual void enable() override;
        virtual void disable() override;

        virtual void initDisplay() override;
        virtual void initDisplay(Shapesize gs, int gminx, int gminy, int gmaxx, int gmaxy) override;
        virtual void cls() override;
        virtual void cls(Color col) override;
        virtual void drawRect(Position pixpos, Shapesize asize, Color col) override;
        virtual void drawRect(Position startpos, Position endpos, Color col) override;
        virtual void fillRect(Position pixpos, Shapesize asize, Color col) override;
        virtual void fillRect(Position startpos, Position endpos, Color col) override;
        virtual void drawLine(Position startpos, Position endpos, Color col) override;

        virtual void drawText(Position pixpos, char* atext, Color fg, Color bg) override;
        virtual void drawText(Position pixpos, char* atext, Color fg) override;
        virtual void drawText(Position pixpos, char* atext, int asize, Color fg, Color bg) override;
        virtual void drawText(Position pixpos, char* atext, int asize, Color fg) override;
        virtual void drawGridText(Position gridpos, char* atext, Color fg, Color bg) override;
        virtual void drawGridText(Position gridpos, char* atext, int asize, Color fg, Color bg) override;

        virtual void drawGridRect(Position gridpos, Shapesize asize, Color col) override;
        virtual void fillGridRect(Position gridpos, Shapesize asize, Color col) override;
        virtual void drawGridHeadline(Position gridpos, char* atext, Color fg, Color bg) override;

        virtual Shapesize bmpGridDraw(char* filename, Position gridpos) override;
        virtual Shapesize bmpDraw(char* filename, int16_t x, int16_t y) override;
        virtual Shapesize readBmpSize(char* filename) override;

        virtual void getTextBounds(char* str, int16_t x, int16_t y,
        int16_t* x1, int16_t* y1, uint16_t* w, uint16_t* h) override;

        virtual void setFontSize(int fsize) override;

        virtual void addElement(DisplayElement* de) override;
        virtual void removeElement(DisplayElement* de) override;
        virtual DisplayElement* getElementByNum(int num) override;
        virtual void paint() override;

        virtual bool isEnabled() override;
        virtual bool isMono() override;

        Adafruit_ILI9341 accessLowlevel();

    protected:
        Adafruit_ILI9341 hardware_;

    private:
};

#endif // ADAFRUITILI_H
