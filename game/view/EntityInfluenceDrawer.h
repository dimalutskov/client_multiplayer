#pragma once

#include "engine/layout/view/ViewDrawer.h"
#include "engine/render/RgbData.h"
#include "../data/EntityInfluence.h"

class EntityInfluenceDrawer : public ViewDrawer {
private:
    EntityInfluence influence;

    float animationProgress;

public:
    EntityInfluenceDrawer(EntityInfluence &influence) : influence(influence) {
        animationProgress = 0;
    }

    void updateProgress(float progress) {
        this->animationProgress = progress;
        invalidate();
    }

    virtual void onDraw(ViewCanvas *canvas) override {
        RgbData color = RgbData(200, 242, 150);

        int size = 30;
        int x = canvas->getSize().getX() - size;
        int y = 0 - 100 * animationProgress;

        canvas->draw(canvas->newObject()
                             ->setSize(Point<int>(30, 30))
                             ->setLocation(Point<int>(x, y))
//                         ->setShape(RenderObject::SHAPE_TYPE_CIRCLE)
                             ->setRenderData(RenderData(color, 1 - animationProgress)));
    }

};