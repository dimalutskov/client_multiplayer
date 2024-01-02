#pragma once

#include "wnd_engine/layout/view/ViewDrawer.h"
#include "network/EntityInfluence.h"

using namespace wnd;

class EntityInfluenceDrawer : public ViewDrawer {
private:
    EntityInfluence influence;

    float animationProgress;

public:
    EntityInfluenceDrawer(EntityInfluence &influence);

    void updateProgress(float progress) {
        this->animationProgress = progress;
        invalidate();
    }

    virtual void onDraw(ViewCanvas *canvas) override;

};