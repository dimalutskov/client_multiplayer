#pragma once

#include "engine/layout/view/text/FontTextureProvider.h"



class SoupOfJusticeFontProvider : public BaseCharTextureProvider {
public:
    SoupOfJusticeFontProvider(std::string texturePath,
                             int textureCellSize,
                             int xCellsCount,
                             int yCellsCount)
            : BaseCharTextureProvider(texturePath, textureCellSize, xCellsCount, yCellsCount) {

    }

protected:

    virtual int getCharCell(char c) override {
        switch (c) {
            case 'a': return 33;
            case 'b': return 34;
            case 'c': return 35;
            case 'd': return 36;
            case 'e': return 37;
            case 'f': return 38;
            case 'g': return 39;
            case 'h': return 40;
            case 'i': return 41;
            case 'j': return 42;
            case 'k': return 43;
            case 'l': return 44;
            case 'm': return 45;
            case 'n': return 46;
            case 'o': return 47;
            case 'p': return 48;
            case 'q': return 49;
            case 'r': return 50;
            case 's': return 51;
            case 't': return 52;
            case 'u': return 53;
            case 'v': return 54;
            case 'w': return 55;
            case 'x': return 56;
            case 'y': return 57;
            case 'z': return 58;
        }
        return BaseCharTextureProvider::getCharCell(c);
    }

    // Values for 128px cell
    int getCharWidth(char c) override {
        switch (c) {
            case ' ': return 31;

            case ',': return 42;
            case '.': return 45;
            case '/': return 45;
            case '-': return 45;
            case ':': return 43;

            case '0': return 78;
            case '1': return 42;
            case '2': return 74;
            case '3': return 67;
            case '4': return 74;
            case '5': return 74;
            case '6': return 72;
            case '7': return 66;
            case '8': return 75;
            case '9': return 74;

            case 'A': return 89;
            case 'B': return 91;
            case 'C': return 87;
            case 'D': return 81;
            case 'E': return 78;
            case 'F': return 84;
            case 'G': return 87;
            case 'H': return 87;
            case 'I': return 42;
            case 'J': return 71;
            case 'K': return 78;
            case 'L': return 71;
            case 'M': return 105;
            case 'N': return 85;
            case 'O': return 100;
            case 'P': return 73;
            case 'Q': return 102;
            case 'R': return 76;
            case 'S': return 71;
            case 'T': return 70;
            case 'U': return 81;
            case 'V': return 87;
            case 'W': return 104;
            case 'X': return 88;
            case 'Y': return 81;
            case 'Z': return 85;

            case 'a': return 89;
            case 'b': return 91;
            case 'c': return 87;
            case 'd': return 81;
            case 'e': return 78;
            case 'f': return 84;
            case 'g': return 87;
            case 'h': return 87;
            case 'i': return 42;
            case 'j': return 71;
            case 'k': return 78;
            case 'l': return 71;
            case 'm': return 105;
            case 'n': return 85;
            case 'o': return 100;
            case 'p': return 73;
            case 'q': return 102;
            case 'r': return 76;
            case 's': return 71;
            case 't': return 70;
            case 'u': return 81;
            case 'v': return 87;
            case 'w': return 104;
            case 'x': return 88;
            case 'y': return 81;
            case 'z': return 85;
        }

        return 106;
    }

};