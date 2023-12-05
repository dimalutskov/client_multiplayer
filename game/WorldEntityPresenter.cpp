#include "WorldEntityPresenter.h"
#include "wnd_engine/game_v2/GameWorld.h"
#include "wnd_engine/game_v2/object/GameObject.h"
#include "wnd_engine/game_v2/presenter/GameWorldPresenter.h"
#include "wnd_engine/game_v2/collisions/GameObjectCollisionHandler.h"
#include "network/ObjectState.h"
#include "wnd_engine/Logger.h"
#include "GameConstants.h"

WorldEntityPresenter::WorldEntityPresenter(const ObjectState &state) : GameObjectPresenter(createObject(state), createView(state)) {
    lastStates.push_back(state);
}

void WorldEntityPresenter::update(const ObjectState &state) {
    lastStates.push_back(state); // TODO Sync THREADS???
    if (lastStates.size() > 4) {
        lastStates.erase(lastStates.begin());
    }
}

GameObject *WorldEntityPresenter::createObject(const ObjectState &state) {
    // Game object
    GameObject *object = new GameObject(state.getObjectType()); // TODO
    float y = state.getY();
    object->setLocation(state.getX(), state.getY());
    object->setAngle(state.getAngle());
    int size = 80;
    if (state.getObjectType() == GameConstants::GAME_OBJECT_TYPE_SHOT) {
        size = GameConstants::SHOT_SIZE;
    }
    object->setSize(size, size);
    return object;
}

View *WorldEntityPresenter::createView(const ObjectState &state) {
    View *objectView = new View();
    objectView->addDrawer(new ViewDrawer([](ViewCanvas *canvas, void *customData) {
        WorldEntityPresenter *thisClass = static_cast<WorldEntityPresenter*>(customData);
        canvas->draw(canvas->newObject()
                             ->setShape(RenderObject::SHAPE_TYPE_CIRCLE)
                             ->setRenderData(RenderData(RgbData(255, 0, 0))));
    }, this));
    return objectView;
}

void WorldEntityPresenter::step(std::uint64_t time) {
    GameObjectPresenter::step(time);

    ObjectState *stateBefore = 0;
    ObjectState *stateAfter = 0;

    for (auto it = lastStates.begin(); it != lastStates.end(); ++it) {
        ObjectState &state = *it;
        if (state.getClientTime() < time) {
            stateBefore = &state;
        }
        if (!stateAfter && state.getClientTime() >= time) {
            stateAfter = &state;
        }
    }

    if (stateBefore != 0 && stateAfter != 0) {
        long timeFrame = stateAfter->getClientTime() - stateBefore->getClientTime();
        long passedTime = time - stateBefore->getClientTime();
        float progress = passedTime / (float) timeFrame;

        int x = stateBefore->getX() + (stateAfter->getX() - stateBefore->getX()) * progress;
        int y = stateBefore->getY() + (stateAfter->getY() - stateBefore->getY()) * progress;
        gameObject->setLocation(x, y);
//        gameObject->setAngle(state.getAngle());
    }

}
