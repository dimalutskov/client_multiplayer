#pragma once

#include "GameObjectExtension.h"
#include "wnd_engine/action/AppAction.h"

class PropertyChangeAction : public AppAction {
protected:
    float mMaxProperty;
    float mCurProperty;

    // Amount of property which will be recovered in 1 second
    float mRecovery;

    std::list<std::function<void(GameObject *object, float max, float cur)>> mListeners;

public:
    GameObjectPropertyExtension(GameObject *object, int type, int max, int cur) : GameObjectExtension(type, object) {
        mMaxProperty = max;
        mCurProperty = cur;
        mRecovery = 0;
    }

    void setRecovery(float recovery) {
        mRecovery = recovery;
    }

    void addListener(std::function<void(GameObject*, float, float)> listener) {
        mListeners.push_back(listener);
    }

    float getMaxProperty() const {return mMaxProperty;}
    float getCurrentProperty() const {return mCurProperty;}
    float getRatio() {return mCurProperty / mMaxProperty;}

    void modify(float diff) {
        mCurProperty += diff;
        if (mCurProperty < 0) mCurProperty = 0;
        if (mCurProperty > mMaxProperty) mCurProperty = mMaxProperty;

        for (auto listener : mListeners) {
            listener(mObject, mMaxProperty, mCurProperty);
        }
    }

    void onWorldStep(GameWorld *world, std::uint64_t time) override {
        if (mRecovery != 0 && mLastWorldStepTime != 0) {
            std::uint64_t passedTime = time - mLastWorldStepTime;
            float recoveredValue = (passedTime / 1000.0f) * mRecovery;
            modify(recoveredValue);
        }

        GameObjectExtension::onWorldStep(world, time);
    }

};