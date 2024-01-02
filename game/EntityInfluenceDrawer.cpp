#include "EntityInfluenceDrawer.h"
#include "network/EntityInfluence.h"
#include "wnd_engine/render/RgbData.h"

EntityInfluenceDrawer::EntityInfluenceDrawer(EntityInfluence &influence) : influence(influence) {
    animationProgress = 0;
}

void EntityInfluenceDrawer::onDraw(ViewCanvas *canvas) {
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


