#pragma once

#include "../../../wnd_engine/layout/view/View.h"
#include "GamePadView.h"

class GamePadListener {
public:
    virtual void onShowGamePad(GamePadView *view, float x, float y) {}
    virtual void onHideGamePad(GamePadView *view) {}
    virtual void onUpdateGamePad(GamePadView *view, int angle, int progress) {}
};

class GamePadController {

    GamePadView *mGamePadView;
    Rect<int> mGamePadTouchRect;

    // pointerId which is used for handling gamepad touches
    int mGamepadTouchPointer;

    GamePadListener *const mListener;

public:
    GamePadController(GamePadListener *listener) : mListener(listener) {
        mGamepadTouchPointer = -1;
        mGamePadView = new GamePadView();
        mGamePadView->setListener([this](int angle, int progress) {
            mListener->onUpdateGamePad(mGamePadView, angle, progress);
        });
    }

    void setGamePadTouchRect(const Rect<int> &touchRect) {
        GamePadController::mGamePadTouchRect = touchRect;
    }

    bool onTouchEvent(TouchEvent event) {
        switch (event.type) {
            case TouchEvent::ACTION_DOWN:
            case TouchEvent::ACTION_POINTER_DOWN:
                if (mGamepadTouchPointer < 0 && (mGamePadTouchRect.isEmpty() || TouchEvent::belongs(event, mGamePadTouchRect))) {
                    mGamepadTouchPointer = event.pointerId;
                    mListener->onShowGamePad(mGamePadView, event.x, event.y);
                    return true;
                }
                break;

            case TouchEvent::ACTION_MOVE:
            case TouchEvent::ACTION_POINTER_MOVE:
                if (event.pointerId == mGamepadTouchPointer) {
                    mGamePadView->updateTouchInfo(event);
                    return true;
                }
                break;

            case TouchEvent::ACTION_UP:
            case TouchEvent::ACTION_POINTER_UP:
                // onHideGamePad
                if (event.pointerId == mGamepadTouchPointer) {
                    mGamepadTouchPointer = -1;
                    mGamePadView->stop();
                    mListener->onHideGamePad(mGamePadView);
                    return true;
                }
                break;
        }

        return false;
    }

};