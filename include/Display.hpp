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

#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <SD.h>

#include <vector>
#include <iterator>

#include "DisplayElement.hpp"
#include "Position.hpp"
#include "Shapesize.hpp"
#include "Color.hpp"

enum class DisplayType
{
    UNDEFINED,
    ADAFRUITOLED,   //Adafruit 0.96" 16-bit Color OLED with SSD1331 driver
    ADAFRUITTFT,    //Adafruit 1.8" SPI display http://www.adafruit.com/products/358 and http://www.adafruit.com/products/618 http://www.ladyada.net/products/18tft…
    ADAFRUITILI,    //Adafruit 2.8" Touch Shield V2 (SPI) http://www.adafruit.com/products/1651
                    //Adafruit 2.4" TFT LCD with Touchscreen Breakout w/MicroSD Socket - ILI9341 https://www.adafruit.com/product/2478
                    //2.8" TFT LCD with Touchscreen Breakout Board w/MicroSD Socket - ILI9341 https://www.adafruit.com/product/1770
                    //2.2" 18-bit color TFT LCD display with microSD card breakout - ILI9340 https://www.adafruit.com/product/1480
                    //TFT FeatherWing - 2.4" 320x240 Touchscreen For All Feathers https://www.adafruit.com/product/3315
    SPARKFUN6100,   //Nokia 6100 128x128 Color Displays with Epson S1D15G10 or Philips PCF8833 controller and SPI connection
    MONOOLED        //Monochrome OLED Display with SSD1306 controller and I2C connection, uses Adafruit SSD1306 library
};

class DisplayElement;

class Display
{
    public:
        // the file itself
        File bmpFile;

        // information we extract about the bitmap file
        int bmpWidth, bmpHeight;
        uint8_t bmpDepth, bmpImageoffset;

        Display();
        virtual ~Display();

        virtual void enable() = 0;
        virtual void disable() = 0;

        virtual void initDisplay() = 0;
        virtual void initDisplay(Shapesize gs, int gminx, int gminy, int gmaxx, int gmaxy) = 0;
        virtual void cls() = 0;
        virtual void cls(Color col) = 0;
        virtual void drawRect(Position pixpos, Shapesize asize, Color col) = 0;
        virtual void drawRect(Position startpos, Position endpos, Color col) = 0;
        virtual void fillRect(Position pixpos, Shapesize asize, Color col) = 0;
        virtual void fillRect(Position startpos, Position endpos, Color col) = 0;
        virtual void drawLine(Position startpos, Position endpos, Color col) = 0;

        virtual void drawText(Position pixpos, char* atext, Color fg, Color bg) = 0;
        virtual void drawText(Position pixpos, char* atext, Color fg) = 0;
        virtual void drawText(Position pixpos, char* atext, int asize, Color fg, Color bg) = 0;
        virtual void drawText(Position pixpos, char* atext, int asize, Color fg) = 0;
        virtual void drawGridText(Position gridpos, char* atext, Color fg, Color bg) = 0;
        virtual void drawGridText(Position gridpos, char* atext, int asize, Color fg, Color bg) = 0;

        virtual void drawGridRect(Position gridpos, Shapesize asize, Color col) = 0;
        virtual void fillGridRect(Position gridpos, Shapesize asize, Color col) = 0;
        virtual void drawGridHeadline(Position gridpos, char* atext, Color fg, Color bg) = 0;

        virtual Shapesize bmpGridDraw(char* filename, Position gridpos) = 0;
        virtual Shapesize bmpDraw(char* filename, int16_t x, int16_t y) = 0;
        virtual Shapesize readBmpSize(char* filename) = 0;

        virtual void getTextBounds(char* str, int16_t x, int16_t y,
        int16_t* x1, int16_t* y1, uint16_t* w, uint16_t* h) = 0;

        virtual void setFontSize(int fsize) = 0;

        virtual void addElement(DisplayElement* de) = 0;
        virtual void removeElement(DisplayElement* de) = 0;
        virtual DisplayElement* getElementByNum(int num) = 0;
        virtual void paint() = 0;

        virtual bool isEnabled() = 0;
        virtual bool isMono() = 0;

        uint16_t read16(File &f);
        uint32_t read32(File &f);

        Shapesize getGridSpace();
        Position gridToPix(Position gridpos);

        int getGridMinX();
        int getGridMaxX();

        int getGridMinY();
        int getGridMaxY();

        int getPixMaxX();
        int getPixMaxY();

        Position getGridTopLeft();
        Position getGridBottomRight();
        Position getPixBottomRight();

        void setGridSpace(Shapesize gs);
        void setGridMinX(int gmix);
        void setGridMaxX(int gmax);
        void setGridMinY(int gmiy);
        void setGridMaxY(int gmay);

        void setGridMin(Position gmin);
        void setGridMax(Position gmax);

        void setGridBorders(Position topLeft, Position bottomRight);

    protected:
        DisplayType type_;

        std::vector<DisplayElement*> elemList_;

        bool enabled_;
        bool mono_;

        Shapesize gridSpace_;
        int gridMinX_;
        int gridMaxX_;

        int gridMinY_;
        int gridMaxY_;

        int pixMaxX_;
        int pixMaxY_;

    private:
};

#endif // DISPLAY_H
