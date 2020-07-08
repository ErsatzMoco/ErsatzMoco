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

#ifndef DISPLAYELEMENT_H
#define DISPLAYELEMENT_H

#include <Arduino.h>
#include "Display.hpp"
#include "Position.hpp"
#include "Shapesize.hpp"
#include "Color.hpp"

enum class DisplayElementType
{
    LABEL,
    BUTTON,
    IMAGE,
    IMAGEBUTTON
};

class Display;

class DisplayElement
{
    public:
        DisplayElement();
        virtual ~DisplayElement();

        void setPixelPos(int myx, int myy);
        void setPixelPos(Position mypos);
        Position getPixelPos();

        void setGridPos(int myx, int myy);
        void setGridPos(Position mypos);
        Position getGridPos();

        void setSize(int width, int height);
        void setSize(Shapesize mysize);
        Shapesize getSize();

        void setGridSize(int width, int height);
        void setGridSize(Shapesize mysize);

        void reframeGridPos();
        void calculatePixelPos();

        DisplayElementType getType();
        void setType(DisplayElementType t);
        char* getName();
        void setName(char* aname);
        void setDisplay(Display* myd);

        Color getFGColor();
        Color getBGColor();
        Color getBorderColor();

        void setFGColor(Color mycol);
        void setBGColor(Color mycol);
        void setBorderColor(Color mycol);
        void setColors(Color fg, Color bg, Color bd);

        void useGrid(bool ongrid);
        bool usesGrid();

        bool redraw();

        void draw();

    protected:
        Display* currentDisplay_;

        DisplayElementType type_;
        char name_[32];

        Position gridPos_;
        Position pixelPos_;

        Shapesize pixelSize_;
        Shapesize gridSize_;

        bool grid_;
        bool redraw_;

        Color fgColor_;
        Color bgColor_;
        Color borderColor_;

    private:


};

#endif // DISPLAYELEMENT_H
