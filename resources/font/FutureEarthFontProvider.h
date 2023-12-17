#pragma once

#include "../../../engine/view/text/FontTextureProvider.h"

class FutureEarthFontProvider : public BaseCharTextureProvider {
public:
	FutureEarthFontProvider(std::string texturePath,
							int textureCellSize,
							int xCellsCount,
							int yCellsCount)
		: BaseCharTextureProvider(texturePath, textureCellSize, xCellsCount, yCellsCount) {

	}

protected:

	// Values for 128px cell
	int getCharWidth(char c) {
		switch (c) {
			case ',': return 33;
			case '.': return 33;
			case '/': return 70;
			case ':': return 33;

			case '0': return 97;
			case '1': return 42;
			case '2': return 97;
			case '3': return 97;
			case '4': return 97;
			case '5': return 97;
			case '6': return 97;
			case '7': return 97;
			case '8': return 97;
			case '9': return 97;

			case 'A': return 106;
			case 'B': return 106;
			case 'C': return 97;
			case 'D': return 106;
			case 'E': return 97;
			case 'F': return 97;
			case 'G': return 106;
			case 'H': return 106;
			case 'I': return 32;
			case 'J': return 79;
			case 'K': return 106;
			case 'L': return 97;
			case 'M': return 128;
			case 'N': return 115;
			case 'O': return 106;
			case 'P': return 106;
			case 'Q': return 106;
			case 'R': return 106;
			case 'S': return 106;
			case 'T': return 97;
			case 'U': return 106;
			case 'V': return 106;
			case 'W': return 140;
			case 'X': return 115;
			case 'Y': return 106;
			case 'Z': return 106;

			case 'a': return 106;
			case 'b': return 106;
			case 'c': return 97;
			case 'd': return 106;
			case 'e': return 97;
			case 'f': return 97;
			case 'g': return 106;
			case 'h': return 106;
			case 'i': return 32;
			case 'j': return 79;
			case 'k': return 106;
			case 'l': return 97;
			case 'm': return 128;
			case 'n': return 115;
			case 'o': return 106;
			case 'p': return 106;
			case 'q': return 106;
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