#include "WorldEntityPresenter.h"
#include "wnd_engine/game_v2/GameWorld.h"
#include "wnd_engine/game_v2/object/GameObject.h"
#include "wnd_engine/game_v2/presenter/GameWorldPresenter.h"
#include "wnd_engine/game_v2/collisions/GameObjectCollisionHandler.h"
#include "network/GameNetworkObjectState.h"

WorldEntityPresenter::WorldEntityPresenter(const GameNetworkObjectState &state) : GameObjectPresenter(createObject(state), createView(state)) {
}

void WorldEntityPresenter::update(const GameNetworkObjectState &state) {
    int x = state.getX();
    int y = state.getY();
    gameObject->setLocation(x, y);
    gameObject->setAngle(state.getAngle());
}

GameObject *WorldEntityPresenter::createObject(const GameNetworkObjectState &state) {
    // Game object
    GameObject *object = new GameObject(4); // TODO
    float y = state.getY();
    object->setLocation(state.getX(), state.getY());
    object->setAngle(state.getAngle());
    object->setSize(20, 20);
    return object;
}

View *WorldEntityPresenter::createView(const GameNetworkObjectState &state) {
    View *objectView = new View();
    objectView->addDrawer(new ViewDrawer([](ViewCanvas *canvas, void *customData) {
        canvas->draw(canvas->newObject()
                             ->setShape(RenderObject::SHAPE_TYPE_CIRCLE)
                             ->setRenderData(RenderData(RgbData(255, 0, 0))));
    }));
    return objectView;
}