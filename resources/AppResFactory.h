#pragma once

#include "AppResGraphics.h"
#include "AppResStrings.h"
#include "AppResDimensions.h"
#include "AppResDimensionsMobile.h"
#include "wnd_engine/layout/view/text/FontBaseTextureProvider.h"
//#include "app/resources/font/FutureEarthFontProvider.h"
//#include <app/resources/font/EthnocentricFontProvider.h>
//#include "font/SoupOfJusticeFontProvider.h"

using namespace wnd;

class AppResFactory {
public:

    static AppResGraphics *createGraphics() {
        return new AppResGraphics();
    }

    static AppResDimensions *createDimensions() {
//        const AppWindow *window = App::getApp()->getPlatform()->getAttachedWindow();
//        float density = window == 0 ? 1 : window->getDensity();
          float density = 3;
//        switch (AppProvider::get().getApp()->getPlatform()->getInfo().type) {
//            case PLATFORM_TYPE_ANDROID:
//                return new AppResDimensionsMobile(density);
//        }
        return new AppResDimensions(density);
    }

    static AppResStrings *createStrings() {
        return new AppResStrings();
    }

//    static FontTextureProvider* createFutureEarthFont() {
//        return new FutureEarthFontProvider("font/font_future_earth.png", 128, 16, 6);
//    }
//
//    static FontTextureProvider* createEthnocentricFont() {
//        return new EnthoCentricFontProvider("font/font_ethnocentric.png", 128, 8, 8);
//    }
//
    static FontTextureProvider* createSoupOfJusticeFont() {
        return new FontBaseTextureProvider("font/font_soup_of_justice.png", 128, 8, 8);
    }

};