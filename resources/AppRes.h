#pragma once

#include "AppResGraphics.h"
#include "AppResDimensions.h"
#include "AppResStrings.h"
#include "AppResFactory.h"

class AppRes {
private:
    AppResGraphics *mGraphics;
    AppResDimensions *mDimens;
    AppResStrings *mStrings;

//    FontTextureProvider *mFutureEarthFont;
//    FontTextureProvider *mEthnocentricFont;
    FontTextureProvider *mSoupOfJusticeFont;
public:
    AppRes() {
        invalidate();
    }

    AppResGraphics *graphics() {
        if (mGraphics) {
            mGraphics = AppResFactory::createGraphics();
        }
        return mGraphics;
    }

    AppResDimensions *dimens() {
        if (!mDimens) {
            mDimens = AppResFactory::createDimensions();
        }
        return mDimens;
    }

    AppResStrings *strings() {
        if (!mStrings) {
            mStrings = AppResFactory::createStrings();
        }
        return mStrings;
    }

    FontTextureProvider *fontDefault() {
        return mSoupOfJusticeFont;
    }

    void invalidate() {
        mGraphics = 0;
        mDimens = 0;
        mStrings = 0;
//        mFutureEarthFont = AppResFactory::createFutureEarthFont();
//        mEthnocentricFont = AppResFactory::createEthnocentricFont();
        mSoupOfJusticeFont = AppResFactory::createSoupOfJusticeFont();
    }

};

static AppRes APP_RES;