#pragma once

#include <engine/layout/view/ViewGroup.h>
#include <engine/layout/view/text/TextDrawer.h>
#include <app/game/view/GamePadController.h>
#include <app/AppConstants.h>
#include <app/resources/AppRes.h>
#include "GameplayMenuCallback.h"
#include "network/GameNetworkManager.h"

#include "GameplayController.h"

class GameplayMenu : GamePadListener {
private:

    ViewGroup *const view;
    GameNetworkManager *const mNetworkManager;
    GameplayMenuCallback *const mCallback;
    GamePadController *gamePadController;

    std::list<View*> mSkillViews;
    // key - viewId, value - skill type(shot type)
    std::unordered_map<int, int> mSkillIds;
    // key - pointerId, value - touched view
    std::unordered_map<int, View*> mSkillTouchPointers;

    TextDrawer *mHealthTextDrawer;
    TextDrawer *mEnergyTextDrawer;

public:

    GameplayMenu(ViewGroup *rootView, GameNetworkManager *networkManager, GameplayMenuCallback *callback): view(rootView), mNetworkManager(networkManager), mCallback(callback) {
        gamePadController = new GamePadController(this);
        view->setTouchListener([this](View *view, TouchEvent event, TouchEvent touchDown) -> bool {
            handleTouchEvent(event, touchDown);
            return false;
        });
        view->subscribeSizeChanges([this](Point<int> oldSize, Point<int> newSize) {
            gamePadController->setGamePadTouchRect(Rect<int>(0, 0, view->getWidth() / 2, view->getHeight()));
        }, true);

        initSkills();
        initPropertyDrawers();
    }

    ViewGroup *getView() { return view; }

    virtual void onShowGamePad(GamePadView *view, float x, float y) {
        view->setSize(200, 200);
        view->setLocation(
                static_cast<int>(x - view->getWidth() / 2),
                static_cast<int>(y - view->getHeight() / 2));
        this->getView()->addChild(view);
    }

    virtual void onHideGamePad(GamePadView *view) {
        this->getView()->removeChild(view);
    }

    virtual void onUpdateGamePad(GamePadView *view, int angle, int progress) {
        mCallback->onGamePadMove(angle, progress);
    }

    void update(const EntityState &playerState) {
        mHealthTextDrawer->setText(playerState.get(EntityState::POS_HEALTH));
        mEnergyTextDrawer->setText(playerState.get(EntityState::POS_ENERGY));
    }

private:

    void initSkills() {
        // 1
        View *skillView1 = new View();
        int skillSize = 120;
        skillView1->setSize(skillSize, skillSize);
        skillView1->addDrawer(new ViewDrawer([](ViewCanvas *canvas, void *customData) {
            canvas->draw(canvas->newObject()
                                 ->setRenderData(RenderData(
                                         RgbData(0, 0, 200)
//                                     TextureDesc(APP_RES.graphics()->textureButtonIcon())
                                 )));
        }));
        mSkillViews.push_back(skillView1);
        view->addChild(skillView1);
        mSkillIds[skillView1->getId()] = AppConstants::SKILL_TYPE_SHOT;

        // 2
        View *skillView2 = new View();
        skillView2->setSize(skillSize, skillSize);
        skillView2->addDrawer(new ViewDrawer([](ViewCanvas *canvas, void *customData) {
            canvas->draw(canvas->newObject()
                                 ->setRenderData(RenderData(
                                         RgbData(0, 0, 100)
//                                     TextureDesc(APP_RES.graphics()->textureButtonIcon())
                                 )));
        }));
        mSkillViews.push_back(skillView2);
        view->addChild(skillView2);
        mSkillIds[skillView2->getId()] = AppConstants::SKILL_TYPE_ACCELERATION;

        view->subscribeSizeChanges([this, skillView1, skillView2](Point<int> oldSize, Point<int> newSize) {
            // 1
            skillView1->setLocation(view->getWidth() - skillView1->getWidth() - 100,
                                    view->getHeight() - skillView1->getHeight() - 100);
            // 2
            skillView2->setLocation(view->getWidth() - skillView2->getWidth() - 100,
                                    skillView1->getY() - skillView2->getHeight() - 100);
        }, true);
    }

    void initPropertyDrawers() {
        View *healthView = new View();
        healthView->setSize(300, 100);
        healthView->setLocation(100, 100);
        mHealthTextDrawer = new TextDrawer(APP_RES.fontDefault(), 60);
        mHealthTextDrawer->setText("101");
        mHealthTextDrawer->setTextColor(RgbData(200, 0, 0));
        healthView->addDrawer(mHealthTextDrawer);
        this->view->addChild(healthView);

        View *energyView = new View();
        energyView->setSize(300, 100);
        energyView->setLocation(100, 200);
        mEnergyTextDrawer = new TextDrawer(APP_RES.fontDefault(), 60);
        mEnergyTextDrawer->setText("102");
        mEnergyTextDrawer->setTextColor(RgbData(200, 0, 200));
        energyView->addDrawer(mEnergyTextDrawer);
        this->view->addChild(energyView);
    }

    void handleTouchEvent(TouchEvent event, TouchEvent downEvent) {
        gamePadController->onTouchEvent(event);

        switch (event.type) {
            case TouchEvent::ACTION_DOWN:
            case TouchEvent::ACTION_POINTER_DOWN:
                // Check if skill view is touched
                for (View *view : mSkillViews) {
                    if (!TouchEvent::belongs(event, view->getRect())) {
                        continue;
                    }
                    bool viewAlreadyPressed = false;
                    for (auto it : mSkillTouchPointers) {
                        if (it.second == view) {
                            viewAlreadyPressed = true;
                            break;
                        }
                    }
                    if (!viewAlreadyPressed) {
                        mSkillTouchPointers[event.pointerId] = view;
                        int skillId = mSkillIds[view->getId()];
                        mCallback->onSkillON(skillId);
                    }
                }
                break;

            case TouchEvent::ACTION_UP:
            case TouchEvent::ACTION_POINTER_UP:
                // Check skill views
                View *view = mSkillTouchPointers[event.pointerId];
                if (view) {
                    int skillId = mSkillIds[view->getId()];
                    mCallback->onSkillOFF(skillId);
                    mSkillTouchPointers.erase(event.pointerId);
                }
                break;
        }
    }
};