#include "WorldEntityPresenter.h"
#include "wnd_engine/game_v2/GameWorld.h"
#include "wnd_engine/game_v2/object/GameObject.h"
#include "wnd_engine/game_v2/presenter/GameWorldPresenter.h"
#include "wnd_engine/game_v2/collisions/GameObjectCollisionHandler.h"
#include "network/GameNetworkObjectState.h"
#include "wnd_engine/Logger.h"

WorldEntityPresenter::WorldEntityPresenter(const GameNetworkObjectState &state) : GameObjectPresenter(createObject(state), createView(state)) {
    lastStates.push_back(state);
}

void WorldEntityPresenter::update(const GameNetworkObjectState &state) {
    lastStates.push_back(state); // TODO Sync THREADS???
    if (lastStates.size() > 3) {
        lastStates.erase(lastStates.begin());
    }

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
    object->setSize(80, 80);
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

void WorldEntityPresenter::step(std::uint64_t time) {
    GameObjectPresenter::step(time);

    std::uint64_t pastTime = time - 2000;

    GameNetworkObjectState *stateBefore;
    GameNetworkObjectState *stateAfter;

    std::ostringstream ss;
    ss << "@@@ pastTime: " << pastTime;

    for (auto it = lastStates.begin(); it != lastStates.end(); ++it) {
        GameNetworkObjectState state = *it;
        if (state.getClientTime() <= pastTime) {
            stateBefore = it;//&state;
//            ss << " stateBefore: " << state.getClientTime();
        }
        if (stateAfter != 0 && state.getClientTime() >= pastTime) {
            stateAfter = it;//&state;
//            ss << " stateAfter: " << state.getClientTime();
        }
    }

    if (stateBefore && stateAfter) {
        ss << " stateBefore: " << stateBefore->getClientTime();
        ss << " stateAfter: " << stateAfter->getClientTime();
    }

    Logger::log(ss.str());

//    if (stateBefore != 0 && stateAfter != 0) {
//        long timeFrame = stateAfter->getClientTime() - stateBefore->getClientTime();
//        long passedTime = pastTime - stateBefore->getClientTime();
//
//        std::ostringstream ss;
//        ss << "@@@ timeFrame: " << timeFrame << " passed: " << passedTime;
//        Logger::log(ss.str());
//    }


}
