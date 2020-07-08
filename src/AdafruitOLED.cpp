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

#include "../include/AdafruitOLED.hpp"

/** @brief Wrapper for displays using the SSD 1331 driver chip. Connects to the corresponding library by Adafruit.
  *
  * Wrappers of base type Display implement the complete set or a subset of standard methods for constructing graphical elements.
  * Pins for talking to driver chip (SPI) must be set up in header file AdafruitOLED.hpp!
  * @see Display
  */
AdafruitOLED::AdafruitOLED() : hardware_(Adafruit_SSD1331(ADA_CS, ADA_DC, ADA_RST))
{
    //ctor
}

AdafruitOLED::~AdafruitOLED()
{
    //dtor
}

/** @brief Switch display driver on.
  *
  */
void AdafruitOLED::enable()
{
    digitalWrite(ADA_CS, LOW);
}

/** @brief Switch display driver off.
  *
  */
void AdafruitOLED::disable()
{
    digitalWrite(ADA_CS, HIGH);
    digitalWrite(ADA_SDCS, HIGH);
}

/** @brief Access Adafruit library directly: Returns library object.
  *
  */
Adafruit_SSD1331 AdafruitOLED::accessLowlevel()
{
    return hardware_;
}

bool AdafruitOLED::isEnabled()
{
    return enabled_;
}

/** @brief Check if display is monochrome.
  *
  */
bool AdafruitOLED::isMono()
{
    return mono_;
}

/** @brief Set basic pixel dimensions, standard grid space and rotation and clear screen.
  *
  */
void AdafruitOLED::initDisplay()
{
    elemList_ = {};

    pixMaxX_ = hardware_.width() - 1;
    pixMaxY_ = hardware_.height() - 1;

    type_ = DisplayType::ADAFRUITOLED;
    gridSpace_ = Shapesize(8,8);
    gridMinX_ = 0;
    gridMinY_ = 0;
    gridMaxX_ = 11;
    gridMaxY_ = 7;

    hardware_.begin();
    hardware_.fillScreen(BLACK);
}

/** @brief Set basic pixel dimensions and standard rotation, define dedicated grid space and clear screen.
  *
  */
void AdafruitOLED::initDisplay(Shapesize gs, int gminx, int gminy, int gmaxx, int gmaxy)
{
    elemList_ = {};

    pixMaxX_ = hardware_.width() - 1;
    pixMaxY_ = hardware_.height() - 1;

    type_ = DisplayType::ADAFRUITOLED;
    gridSpace_ = gs;
    gridMinX_ = gminx;
    gridMinY_ = gminy;
    gridMaxX_ = gmaxx;
    gridMaxY_ = gmaxy;

    hardware_.begin();
    hardware_.fillScreen(BLACK);
}

/** @brief Clear screen with black.
  *
  */
void AdafruitOLED::cls()
{
    hardware_.fillScreen(BLACK);
}

/** @brief Clear screen with dedicated color.
  *
  */
void AdafruitOLED::cls(Color col)
{
    hardware_.fillScreen(col.toAdafruitColor());
}

/** @brief Add a high level display component to refresh list.
  *
  * IMPORTANT: Display list stores pointers only. The component object itself must be OWNED by another class, typically by the main application class. The display wrapper does not take care of the component's life cycle!
  * @see DisplayElement
  */
void AdafruitOLED::addElement(DisplayElement* de)
{
    de->setDisplay(this);
    elemList_.push_back(de);
}

/** @brief Remove a high level display component from refresh list.
  *
  * IMPORTANT: Display list stores pointers only. The component object itself must be OWNED by another class, typically by the main application class. The display wrapper does not take care of the component's life cycle!
  * @see DisplayElement
  */
void AdafruitOLED::removeElement(DisplayElement* de)
{
    int ctr = 0;
    for (DisplayElement* myde : elemList_)
    {
        if(strcmp(myde->getName(), de->getName()) == 0)
        {
            elemList_.erase(elemList_.begin()+ctr);
            Serial.println("removed from elemList");
            break;
        }
        ctr++;
    }
}

/** @brief Retrieve component from refresh list by providing its number in the list.
  *
  */
DisplayElement* AdafruitOLED::getElementByNum(int num)
{
    return elemList_.at(num);
}

/** @brief Redraw all components stored (by their pointers only!) in refresh list.
  *
  * @see DisplayElement, DisplayElementType
  */
void AdafruitOLED::paint()
{
    if(!elemList_.empty())
    {
        for (DisplayElement* de : elemList_)
        {
            if(de->redraw())
            {
                switch(de->getType())
                {
                    case DisplayElementType::BUTTON:
                        ((Button*)de)->draw();
                        break;
                    case DisplayElementType::LABEL:
                        ((Label*)de)->draw();
                        break;
                    case DisplayElementType::IMAGE:
                        ((Image*)de)->draw();
                        break;
                    case DisplayElementType::IMAGEBUTTON:
                        ((ImageButton*)de)->draw();
                        break;
                }
            }
        }
    }
}

/** @brief Draw empty rectangle providing pixel position, size and color.
  *
  * @see Position, Shapesize, Color
  */
void AdafruitOLED::drawRect(Position pixpos, Shapesize asize, Color col)
{
    hardware_.drawRect(pixpos.x, pixpos.y, asize.width, asize.height, col.toAdafruitColor());
}

/** @brief Draw empty rectangle providing start and end pixel positions and color.
  *
  * @see Position, Color
  */
void AdafruitOLED::drawRect(Position startpos, Position endpos, Color col)
{
    Shapesize asize((endpos.x - startpos.x + 1), (endpos.y - startpos.y + 1));
    drawRect(startpos, asize, col);
}

/** @brief Draw filled rectangle providing providing pixel position, size and color.
  *
  * @see Position, Shapesize, Color
  */
void AdafruitOLED::fillRect(Position pixpos, Shapesize asize, Color col)
{
    hardware_.fillRect(pixpos.x, pixpos.y, asize.width, asize.height, col.toAdafruitColor());
}

/** @brief Draw filled rectangle providing start and end pixel positions and color.
  *
  * @see Position, Color
  */
void AdafruitOLED::fillRect(Position startpos, Position endpos, Color col)
{
    Shapesize asize((endpos.x - startpos.x + 1), (endpos.y - startpos.y + 1));
    fillRect(startpos, asize, col);
}

/** @brief Draw line providing start and end pixel positions and color.
  *
  * @see Position, Color
  */
void AdafruitOLED::drawLine(Position startpos, Position endpos, Color col)
{
//    if(startpos.x == endpos.x)
//    {
//        hardware_.drawFastVLine(startpos.x, startpos.y, (endpos.y - startpos.y) + 1, col.toAdafruitColor());
//    }
//    else if (startpos.y == endpos.y)
//    {
//        hardware_.drawFastHLine(startpos.x, startpos.y, (endpos.x - startpos.x) + 1, col.toAdafruitColor());
//    }
//    else
//    {
        hardware_.drawLine(startpos.x, startpos.y, endpos.x, endpos.y, col.toAdafruitColor());
//    }
}

/** @brief Draw text providing pixel position, c string, foreground and background color.
  *
  * @see Position, Color
  */
void AdafruitOLED::drawText(Position pixpos, char* atext, Color fg, Color bg)
{
    hardware_.setCursor(pixpos.x, pixpos.y);
    hardware_.setTextColor(fg.toAdafruitColor(), bg.toAdafruitColor());
    hardware_.print(atext);
}

/** @brief Draw text providing pixel position, c string and foreground color.
  *
  * @see Position, Color
  */
void AdafruitOLED::drawText(Position pixpos, char* atext, Color fg)
{
    hardware_.setCursor(pixpos.x, pixpos.y);
    hardware_.setTextColor(fg.toAdafruitColor());
    hardware_.print(atext);
}

/** @brief Draw text providing pixel position, c string, size, foreground and background color.
  *
  * @see Position, Color
  */
void AdafruitOLED::drawText(Position pixpos, char* atext, int asize, Color fg, Color bg)
{
    hardware_.setTextSize(asize);
    drawText(pixpos, atext, fg, bg);
}

/** @brief Draw text providing pixel position, c string, size and foreground color.
  *
  * @see Position, Color
  */
void AdafruitOLED::drawText(Position pixpos, char* atext, int asize, Color fg)
{
    hardware_.setTextSize(asize);
    drawText(pixpos, atext, fg);
}

/** @brief Draw text providing grid position, c string, foreground and background color.
  *
  * @see Position, Color
  */
void AdafruitOLED::drawGridText(Position gridpos, char* atext, Color fg, Color bg)
{
    Position pixpos = gridToPix(gridpos);

    hardware_.setCursor(pixpos.x + 1, pixpos.y + 1);
    hardware_.setTextColor(fg.toAdafruitColor(), bg.toAdafruitColor());
    hardware_.print(atext);
}

/** @brief Draw text providing grid position, c string, size, foreground and background color.
  *
  * @see Position, Color
  */
void AdafruitOLED::drawGridText(Position gridpos, char* atext, int asize, Color fg, Color bg)
{
    hardware_.setTextSize(asize);
    drawGridText(gridpos, atext, fg, bg);
}

/** @brief Get text box boundaries for a given string using the curretly set text size. This is just a convenience wrapper for the Adafruit GFX function with same name.
  *
    @param    str     The ascii string to measure
    @param    x       The current cursor X
    @param    y       The current cursor Y
    @param    x1      The boundary X coordinate, set by function
    @param    y1      The boundary Y coordinate, set by function
    @param    w      The boundary width, set by function
    @param    h      The boundary height, set by function
  */
void AdafruitOLED::getTextBounds(char* str, int16_t x, int16_t y,
        int16_t* x1, int16_t* y1, uint16_t* w, uint16_t* h)
{
    hardware_.getTextBounds(str, x, y, x1, y1, w, h);
}

/** @brief Set font size for next text drawing. This is just a convenience wrapper for the Adafruit GFX function with same name.
  *
  */
void AdafruitOLED::setFontSize(int fsize)
{
    hardware_.setTextSize(fsize);
}

/** @brief Draw empty rectangle providing grid position, size and color.
  *
  * @see Position, Shapesize, Color
  */
void AdafruitOLED::drawGridRect(Position gridpos, Shapesize asize, Color col)
{
    Position pixpos = gridToPix(gridpos);

    hardware_.drawRect(pixpos.x, pixpos.y, asize.width, asize.height, col.toAdafruitColor());
}

/** @brief Draw filled rectangle providing grid position, size and color.
  *
  * @see Position, Shapesize, Color
  */
void AdafruitOLED::fillGridRect(Position gridpos, Shapesize asize, Color col)
{
    Position pixpos = gridToPix(gridpos);

    hardware_.fillRect(pixpos.x, pixpos.y, asize.width, asize.height, col.toAdafruitColor());
}

/** @brief Draw emphasized text providing grid position, c string, foreground and background color.
  *
  * @see Position, Shapesize, Color
  */
void AdafruitOLED::drawGridHeadline(Position gridpos, char* atext, Color fg, Color bg)
{
    Position pixpos = gridToPix(gridpos);
    int twidth = strlen(atext) * 10;
    int theight = strlen(atext) * 14;

    hardware_.drawRect(pixpos.x, pixpos.y, twidth + 2, theight + 2, bg.toAdafruitColor());
    drawText(pixpos, atext, 2, fg, bg);
}

/** @brief Draw bitmap loaded from SD card providing a filename and grid position. Returns bitmap size.
  *
  * @see Position, Shapesize
  */
Shapesize AdafruitOLED::bmpGridDraw(char* filename, Position gridpos)
{
    Position pixpos = gridToPix(gridpos);
    Shapesize mysize;
    mysize = bmpDraw(filename, int16_t(pixpos.x), int16_t(pixpos.y));
    return mysize;
}

/** @brief Retrieve size of bitmap on SD card providing a filename.
  *
  * @see Shapesize
  */
Shapesize AdafruitOLED::readBmpSize(char* filename)
{
    File     bmpFile;
    uint32_t bmpImageoffset;        // Start of image data in file

    Shapesize bmpsize = Shapesize(0, 0);

    if ((bmpFile = SD.open(filename)) == NULL)
    {
        Serial.print(F("File not found"));
        return bmpsize;
    }

    // Parse BMP header
    if(read16(bmpFile) == 0x4D42) // BMP signature
    {
//        Serial.print(F("File size: "));
//        Serial.println(read32(bmpFile));
        (void)read32(bmpFile); // Read & ignore file size

        (void)read32(bmpFile); // Read & ignore creator bytes
        bmpImageoffset = read32(bmpFile); // Start of image data

//        Serial.print(F("Image Offset: "));
//        Serial.println(bmpImageoffset, DEC);

        // Read DIB header
//        Serial.print(F("Header size: "));
//        Serial.println(read32(bmpFile));
        (void)read32(bmpFile); //Read & ignore header size

        bmpsize.width = read32(bmpFile);
        bmpsize.height = read32(bmpFile);

    }
    else
    {
        Serial.print(F("Invalid file"));
    }

    bmpFile.close();
    return bmpsize;
}

/** @brief Draw bitmap loaded from SD card providing a filename and pixel position. Returns bitmap size.
  *
  * @see Position, Shapesize
  */
Shapesize AdafruitOLED::bmpDraw(char* filename, int16_t x, int16_t y)
{
    Shapesize bmpsize = Shapesize(0, 0);
    File     bmpFile;
    int      bmpWidth = 0;
    int      bmpHeight = 0;   // W+H in pixels
    uint8_t  bmpDepth;              // Bit depth (currently must be 24)
    uint32_t bmpImageoffset;        // Start of image data in file
    uint32_t rowSize;               // Not always = bmpWidth; may have padding
    uint8_t  sdbuffer[3*BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
    uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
    boolean  goodBmp = false;       // Set to true on valid header parse
    boolean  flip    = true;        // BMP is stored bottom-to-top
    int      w, h, row, col, x2, y2, bx1, by1;
    uint8_t  r, g, b;
    uint32_t pos = 0, startTime = millis();

    if((x >= hardware_.width()) || (y >= hardware_.height())) return bmpsize;

//    Serial.println();
//    Serial.print(F("Loading image '"));
//    Serial.print(filename);
//    Serial.println('\'');

    // Open requested file on SD card
    if ((bmpFile = SD.open(filename)) == NULL)
    {
        Serial.print(F("File not found"));
        return bmpsize;
    }

    // Parse BMP header
    if(read16(bmpFile) == 0x4D42) // BMP signature
    {
//        Serial.print(F("File size: "));
//        Serial.println(read32(bmpFile));
        (void)read32(bmpFile); // Read & ignore file size

        (void)read32(bmpFile); // Read & ignore creator bytes
        bmpImageoffset = read32(bmpFile); // Start of image data

//        Serial.print(F("Image Offset: "));
//        Serial.println(bmpImageoffset, DEC);

        // Read DIB header
//        Serial.print(F("Header size: "));
//        Serial.println(read32(bmpFile));
        (void)read32(bmpFile); //Read & ignore header size

        bmpWidth  = read32(bmpFile);
        bmpHeight = read32(bmpFile);

        if(read16(bmpFile) == 1) // # planes -- must be '1'
        {
            bmpDepth = read16(bmpFile); // bits per pixel
//            Serial.print(F("Bit Depth: "));
//            Serial.println(bmpDepth);

            if((bmpDepth == 24) && (read32(bmpFile) == 0)) // 0 = uncompressed
            {
                goodBmp = true; // Supported BMP format -- proceed!
//                Serial.print(F("Image size: "));
//                Serial.print(bmpWidth);
//                Serial.print('x');
//                Serial.println(bmpHeight);

                // BMP rows are padded (if needed) to 4-byte boundary
                rowSize = (bmpWidth * 3 + 3) & ~3;

                // If bmpHeight is negative, image is in top-down order.
                // This is not canon but has been observed in the wild.
                if(bmpHeight < 0)
                {
                    bmpHeight = -bmpHeight;
                    flip      = false;
                }

                // Crop area to be loaded
                x2 = x + bmpWidth  - 1; // Lower-right corner
                y2 = y + bmpHeight - 1;

                if((x2 >= 0) && (y2 >= 0)) // On screen?
                {
                    w = bmpWidth; // Width/height of section to load/display
                    h = bmpHeight;
                    bx1 = by1 = 0; // UL coordinate in BMP file

                    if(x < 0) // Clip left
                    {
                        bx1 = -x;
                        x   = 0;
                        w   = x2 + 1;
                    }

                    if(y < 0) // Clip top
                    {
                        by1 = -y;
                        y   = 0;
                        h   = y2 + 1;
                    }

                    if(x2 >= hardware_.width())  w = hardware_.width()  - x; // Clip right
                    if(y2 >= hardware_.height()) h = hardware_.height() - y; // Clip bottom

                    // Set TFT address window to clipped image bounds
                    hardware_.startWrite(); // Requires start/end transaction now
                    hardware_.setAddrWindow(x, y, w, h);

                    for (row=0; row<h; row++) // For each scanline...
                    {

                        // Seek to start of scan line.  It might seem labor-
                        // intensive to be doing this on every line, but this
                        // method covers a lot of gritty details like cropping
                        // and scanline padding.  Also, the seek only takes
                        // place if the file position actually needs to change
                        // (avoids a lot of cluster math in SD library).
                        if(flip) // Bitmap is stored bottom-to-top order (normal BMP)
                        {
                            pos = bmpImageoffset + (bmpHeight - 1 - (row + by1)) * rowSize;
                        }
                        else     // Bitmap is stored top-to-bottom
                        {
                            pos = bmpImageoffset + (row + by1) * rowSize;
                        }

                        pos += bx1 * 3; // Factor in starting column (bx1)
                        if(bmpFile.position() != pos) // Need seek?
                        {
                            hardware_.endWrite(); // End TFT transaction
                            bmpFile.seek(pos);
                            buffidx = sizeof(sdbuffer); // Force buffer reload
                            hardware_.startWrite(); // Start new TFT transaction
                        }

                        for (col=0; col<w; col++) // For each pixel...
                        {
                            // Time to read more pixel data?
                            if (buffidx >= sizeof(sdbuffer)) // Indeed
                            {
                                hardware_.endWrite(); // End TFT transaction
                                bmpFile.read(sdbuffer, sizeof(sdbuffer));
                                buffidx = 0; // Set index to beginning
                                hardware_.startWrite(); // Start new TFT transaction
                            }

                            // Convert pixel from BMP to TFT format, push to display
                            b = sdbuffer[buffidx++];
                            g = sdbuffer[buffidx++];
                            r = sdbuffer[buffidx++];
                            hardware_.writePixel(hardware_.color565(r,g,b));

                        } // end pixel
                    } // end scanline
                    hardware_.endWrite(); // End last TFT transaction

                } // end onscreen
//                Serial.print(F("Loaded in "));
//                Serial.print(millis() - startTime);
//                Serial.println(" ms");
            } // end goodBmp
        }
    }

    bmpFile.close();
    if(!goodBmp) Serial.println(F("BMP format not recognized."));
    bmpsize.width = bmpWidth;
    bmpsize.height = bmpHeight;
    return bmpsize;
}
