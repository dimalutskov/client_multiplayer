#include "WorldEntitiesController.h"
#include "wnd_engine/game_v2/GameWorld.h"
#include "wnd_engine/game_v2/object/GameObject.h"
#include "wnd_engine/game_v2/presenter/GameWorldPresenter.h"
#include "wnd_engine/game_v2/presenter/GameObjectPresenter.h"
#include "wnd_engine/game_v2/collisions/GameObjectCollisionHandler.h"
#include "network/GameNetworkObjectState.h"
#include "GameConstants.h"

WorldEntitiesController::WorldEntitiesController(GameWorldPresenter *worldPresenter) : mWorldPresenter(worldPresenter) {

}

void WorldEntitiesController::addObject(const GameNetworkObjectState &state) {
    GameObjectPresenter *objectPresenter = createObject();
    mEntities[state.getObjectId()] = objectPresenter;
    mWorldPresenter->addObjectPresenter(objectPresenter);
}

void WorldEntitiesController::removeObject(const GameNetworkObjectState &state) {
    auto entityRef = mEntities.find(state.getObjectId());
    if (entityRef != mEntities.end()) {
        entityRef->second->gameObject->destroy();
        mEntities.erase(entityRef);
    }
}

void WorldEntitiesController::update(const GameNetworkObjectState &state) {
    GameObjectPresenter *objectPresenter = 0;
    auto entityRef = mEntities.find(state.getObjectId());
    if (entityRef == mEntities.end()) {
        objectPresenter = createObject();
        mEntities[state.getObjectId()] = objectPresenter;
    } else {
        objectPresenter = entityRef->second;
    }
    float x = state.getX();
    float y = state.getY();
    objectPresenter->gameObject->setLocation(x, y);
}

GameObjectPresenter *WorldEntitiesController::createObject() {
    // Game object
    GameObject *object = new GameObject(GameConstants::GAME_OBJECT_ENTITY);
    object->setSize(20, 20);

    // Collisions
//    GameObjectCollisionHandler *playerCollisions = new GameObjectCollisionHandler(object);
//    playerCollisions->addCollideType(GameConstants::GAME_OBJECT_OBSTACLE);
//    mWorldPresenter->getWorld()->registerCollisionHandler(playerCollisions);

    // View
    View *objectView = new View();
    objectView->addDrawer(new ViewDrawer([](ViewCanvas *canvas, void *customData) {
        canvas->draw(canvas->newObject()
                             ->setShape(RenderObject::SHAPE_TYPE_CIRCLE)
                             ->setRenderData(RenderData(RgbData(255, 0, 0))));
    }));

    return new GameObjectPresenter(object, objectView);
}