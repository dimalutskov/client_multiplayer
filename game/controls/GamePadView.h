#pragma once

#include <functional>
#include <wnd_engine/utils/MathUtils.h>

class GamePadView : public View {
private:
    int mAngle;
    int mProgress;

    std::function<void(int angle, int progress)> mListener;

public:

    GamePadView() {
        mAngle = 0;
        mProgress = 0;
    }

    void setListener(std::function<void(int degrees, int progress)> listener) {
        mListener = listener;
    }

    void stop() {
        update(mAngle, 0);
    }

    void updateTouchInfo(const TouchEvent &touchEvent) {
        int progress = 0;
        int angle = mAngle;
        if (touchEvent.type != TouchEvent::ACTION_UP) {
            int cx = getCX(), cy = getCY();
            float distance = MathUtils::calculateDistance(cx, cy, touchEvent.x, touchEvent.y);
            angle = static_cast<int>(MathUtils::calculateAngleToTarget(cx, cy, touchEvent.x, touchEvent.y));
            progress = MathUtils::minFloat(1.0f, distance / (getWidth() / 2)) * 100;
            progress = MathUtils::customRound(progress, 10);
        }
        update(angle, progress);
    }

    void onDraw(ViewCanvas *canvas, std::uint64_t time) override {
        canvas->draw(canvas->newObject()
                             ->setRenderData(RenderData(RgbData(100, 200, 60))));
        // Draw touch indicator
        if (mAngle > 0 && mProgress > 0) {
//            int pointerSize = 50;
//
//            int pointerX = mLastTouchEvent.x;
//            int pointerY = mLastTouchEvent.y;
//
//            Point<int> localTouch = getLocalTouchPoint(mLastTouchEvent);
//
//            canvas->draw(canvas->newObject()
//                                ->setSize(Point<int>(pointerSize, pointerSize))
//                                ->setLocation(localTouch)
//                                 ->setRenderData(RenderData(
//                                         RgbData(200, 0, 0))));
        }
    }

private:

    void update(int angle, int progress) {
        if (progress != mProgress || mAngle != angle) {
            mProgress = progress;
            mAngle = angle;
            if (mListener) {
                mListener(mAngle, mProgress);
            }
        }
        invalidate();
    }

};