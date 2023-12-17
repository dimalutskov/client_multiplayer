#pragma once

class EnthoCentricFontProvider : public BaseCharTextureProvider {
public:
    EnthoCentricFontProvider(std::string texturePath,
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
            case ' ': return 80;

            case ',': return 33;
            case '.': return 36;
            case '/': return 70;
            case ':': return 36;

            case '0': return 97;
            case '1': return 46;
            case '2': return 97;
            case '3': return 97;
            case '4': return 97;
            case '5': return 97;
            case '6': return 97;
            case '7': return 94;
            case '8': return 98;
            case '9': return 98;

            case 'A': return 116;
            case 'B': return 106;
            case 'C': return 97;
            case 'D': return 106;
            case 'E': return 97;
            case 'F': return 92;
            case 'G': return 106;
            case 'H': return 106;
            case 'I': return 34;
            case 'J': return 79;
            case 'K': return 106;
            case 'L': return 94;
            case 'M': return 128;
            case 'N': return 100;
            case 'O': return 116;
            case 'P': return 106;
            case 'Q': return 116;
            case 'R': return 106;
            case 'S': return 106;
            case 'T': return 97;
            case 'U': return 106;
            case 'V': return 106;
            case 'W': return 128;
            case 'X': return 115;
            case 'Y': return 106;
            case 'Z': return 106;

            case 'a': return 116;
            case 'b': return 106;
            case 'c': return 97;
            case 'd': return 106;
            case 'e': return 97;
            case 'f': return 92;
            case 'g': return 106;
            case 'h': return 106;
            case 'i': return 34;
            case 'j': return 79;
            case 'k': return 106;
            case 'l': return 94;
            case 'm': return 128;
            case 'n': return 100;
            case 'o': return 116;
            case 'p': return 106;
            case 'q': return 116;
            case 'r': return 106;
            case 's': return 106;
            case 't': return 97;
            case 'u': return 106;
            case 'v': return 106;
            case 'w': return 128;
            case 'x': return 115;
            case 'y': return 106;
            case 'z': return 106;
        }

        return 106;
    }

};