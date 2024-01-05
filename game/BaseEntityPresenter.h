#pragma once

#include "engine/game_v2/GameWorld.h"
#include "engine/game_v2/object/GameObject.h"
#include "engine/game_v2/presenter/GameObjectPresenter.h"
#include "engine/game_v2/presenter/GameWorldPresenter.h"
#include "engine/game_v2/action/ObjectMovementAction.h"
#include "data/EntityInfluence.h"
#include "data/EntityState.h"
#include "view/EntityInfluenceDrawer.h"

class BaseEntityPresenter : public GameObjectPresenter {
protected:
    int entityType;

    std::vector<EntityInfluence> pendingInfluences;
    std::vector<EntityInfluence> attachedInfluences;
    // key - influenceId
    std::unordered_map<std::string, ViewDrawer*> influenceDrawers;

    std::uint64_t destroyTime;

public:
    BaseEntityPresenter(const EntityState &state) : GameObjectPresenter(createObject(state), createView(state)) {
        entityType = state.getObjectType();
        destroyTime = 0;

        auto *destroyAction = new BaseAppAction([this](std::uint64_t time, std::uint64_t passedTime, float progress) {
            view->setAlpha(1 - progress);
        });
        destroyAction->setDuration(500);
        setDestroyAction(destroyAction);
    }

    void attachInfluence(EntityInfluence &influence) {
        pendingInfluences.push_back(influence);
    }

    void detachInfluence(EntityInfluence &influence) {
        pendingInfluences.erase(std::remove(pendingInfluences.begin(), pendingInfluences.end(), influence), pendingInfluences.end());
        attachedInfluences.erase(std::remove(attachedInfluences.begin(), attachedInfluences.end(), influence), attachedInfluences.end());
    }

    virtual void update(const EntityState &state) {}

    void destroy(std::uint64_t clientWorldTime) {
        destroyTime = clientWorldTime;
    }

    virtual void step(std::uint64_t time) override {
        GameObjectPresenter::step(time);

        if (destroyTime > 0 && destroyTime < time) {
            gameObject->destroy();
            return;
        }

        auto it = pendingInfluences.begin();
        while (it != pendingInfluences.end()) {
            EntityInfluence influence = *it;
            if (influence.getAttachClientWorldTime() <= time) {
                onAttachInfluence(influence);
                it = pendingInfluences.erase(it);
            } else {
                ++it;
            }
        }
    }

protected:

    virtual GameObject *createObject(const EntityState &state) {
        auto *object = new GameObject(state.getObjectType()); // TODO
        float y = state.getY();
        object->setAngle(state.getAngle());
        object->setSize(state.getSize(), state.getSize());
        object->setCenterLocation(state.getX(), state.getY());
        return object;
    }

    virtual View *createView(const EntityState &state) {
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

    void onAttachInfluence(EntityInfluence &influence) {
        if (true) { // single attachedInfluences
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
            attachedInfluences.push_back(influence);
        }
    }
};