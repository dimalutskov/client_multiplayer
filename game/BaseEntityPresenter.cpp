#include "BaseEntityPresenter.h"
#include "wnd_engine/game_v2/GameWorld.h"
#include "wnd_engine/game_v2/object/GameObject.h"
#include "wnd_engine/game_v2/presenter/GameWorldPresenter.h"
#include "wnd_engine/game_v2/collisions/GameObjectCollisionHandler.h"
#include "network/EntityState.h"
#include "EntityInfluenceDrawer.h"
#include "wnd_engine/Logger.h"
#include "../AppConstants.h"


BaseEntityPresenter::BaseEntityPresenter(const EntityState &state) : GameObjectPresenter(createObject(state), createView(state)) {
    entityType = state.getObjectType();
    destroyTime = 0;

    auto *destroyAction = new BaseAppAction([this](std::uint64_t time, std::uint64_t passedTime, float progress) {
        view->setAlpha(1 - progress);
    });
    destroyAction->setDuration(500);
    setDestroyAction(destroyAction);
}

void BaseEntityPresenter::attachInfluence(EntityInfluence &influence) {
    if (true) { // single influences
        auto *influenceDrawer = new EntityInfluenceDrawer(influence);
        view->addDrawer(influenceDrawer);

        auto *influenceAction = new BaseAppAction([this, influenceDrawer](std::uint64_t time, std::uint64_t passedTime, float progress) {
            influenceDrawer->updateProgress(progress);
            if (progress == 1.0) {
                view->removeDrawer(influenceDrawer);
            }
        });
        influenceAction->setDuration(2000);
        view->addAction(influenceAction);
    } else {
        influences.push_back(influence);
    }

}

void BaseEntityPresenter::detachInfluence(EntityInfluence &influence) {
    influences.erase(std::remove(influences.begin(), influences.end(), influence), influences.end());
}

void BaseEntityPresenter::update(const EntityState &state) {
}

GameObject *BaseEntityPresenter::createObject(const EntityState &state) {
    // Game object
    auto *object = new GameObject(state.getObjectType()); // TODO
    float y = state.getY();
    object->setAngle(state.getAngle());
    object->setSize(state.getSize(), state.getSize());
    object->setCenterLocation(state.getX(), state.getY());
    return object;
}

View *BaseEntityPresenter::createView(const EntityState &state) {
    auto *objectView = new View();
    objectView->addDrawer(new ViewDrawer([](ViewCanvas *canvas, void *customData) {
        auto *thisClass = static_cast<BaseEntityPresenter*>(customData);
        auto color = thisClass->entityType == AppConstants::ENTITY_TYPE_SHOT
                ? RgbData(255, 0, 0) : RgbData(255, 255, 0);
        canvas->draw(canvas->newObject()
                             ->setShape(RenderObject::SHAPE_TYPE_CIRCLE)
                             ->setRenderData(RenderData(color)));
    }, this));
    return objectView;
}

void BaseEntityPresenter::step(std::uint64_t time) {
    GameObjectPresenter::step(time);

    if (destroyTime > 0 && destroyTime < time) {
        gameObject->destroy();
        return;
    }
}
