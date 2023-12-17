#pragma once

#include "../wnd_engine/layout/view/text/FontBaseTextureProvider.h"

namespace wnd {

	class MyFontProvider : public FontBaseTextureProvider {
	public:
		MyFontProvider(std::string texturePath,
								  int textureCellSize,
								  int xCellsCount,
								  int yCellsCount)
			: FontBaseTextureProvider(texturePath, textureCellSize, xCellsCount, yCellsCount) {

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
			return FontBaseTextureProvider::getCharCell(c);
		}

		// Values for 128px cell
		float getCharWidthRatio(char c) override {
			switch (c) {
				case ' ': return 31 / 128.0f;

				case ',': return 42 / 128.0f;
				case '.': return 45 / 128.0f;
				case '/': return 45 / 128.0f;
				case '-': return 45 / 128.0f;
				case ':': return 43 / 128.0f;

				case '0': return 78 / 128.0f;
				case '1': return 42 / 128.0f;
				case '2': return 74 / 128.0f;
				case '3': return 67 / 128.0f;
				case '4': return 74 / 128.0f;
				case '5': return 74 / 128.0f;
				case '6': return 72 / 128.0f;
				case '7': return 66 / 128.0f;
				case '8': return 75 / 128.0f;
				case '9': return 74 / 128.0f;

				case 'A': return 89 / 128.0f;
				case 'B': return 91 / 128.0f;
				case 'C': return 87 / 128.0f;
				case 'D': return 81 / 128.0f;
				case 'E': return 78 / 128.0f;
				case 'F': return 84 / 128.0f;
				case 'G': return 87 / 128.0f;
				case 'H': return 87 / 128.0f;
				case 'I': return 42 / 128.0f;
				case 'J': return 71 / 128.0f;
				case 'K': return 78 / 128.0f;
				case 'L': return 71 / 128.0f;
				case 'M': return 105 / 128.0f;
				case 'N': return 85 / 128.0f;
				case 'O': return 100 / 128.0f;
				case 'P': return 73 / 128.0f;
				case 'Q': return 102 / 128.0f;
				case 'R': return 76 / 128.0f;
				case 'S': return 71 / 128.0f;
				case 'T': return 70 / 128.0f;
				case 'U': return 81 / 128.0f;
				case 'V': return 87 / 128.0f;
				case 'W': return 104 / 128.0f;
				case 'X': return 88 / 128.0f;
				case 'Y': return 81 / 128.0f;
				case 'Z': return 85 / 128.0f;

				case 'a': return 89 / 128.0f;
				case 'b': return 91 / 128.0f;
				case 'c': return 87 / 128.0f;
				case 'd': return 81 / 128.0f;
				case 'e': return 78 / 128.0f;
				case 'f': return 84 / 128.0f;
				case 'g': return 87 / 128.0f;
				case 'h': return 87 / 128.0f;
				case 'i': return 42 / 128.0f;
				case 'j': return 71 / 128.0f;
				case 'k': return 78 / 128.0f;
				case 'l': return 71 / 128.0f;
				case 'm': return 105 / 128.0f;
				case 'n': return 85 / 128.0f;
				case 'o': return 100 / 128.0f;
				case 'p': return 73 / 128.0f;
				case 'q': return 102 / 128.0f;
				case 'r': return 76 / 128.0f;
				case 's': return 71 / 128.0f;
				case 't': return 70 / 128.0f;
				case 'u': return 81 / 128.0f;
				case 'v': return 87 / 128.0f;
				case 'w': return 104 / 128.0f;
				case 'x': return 88 / 128.0f;
				case 'y': return 81 / 128.0f;
				case 'z': return 85 / 128.0f;
			}

			return 106 / 128.0f;
		}

	};
}