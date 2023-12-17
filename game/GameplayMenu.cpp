#include "GameplayMenu.h"
#include "wnd_engine/Logger.h"
#include "wnd_engine/layout/view/text/TextDrawer.h"
#include "app/resources/AppRes.h"
#include "network/GameNetworkManager.h"
#include "../AppConstants.h"

GameplayMenu::GameplayMenu(ViewGroup *rootView, GameNetworkManager *networkManager, GameplayMenuCallback *callback)
    : view(rootView), mNetworkManager(networkManager), mCallback(callback) {
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

void GameplayMenu::onShowGamePad(GamePadView *view, float x, float y) {
    GamePadListener::onShowGamePad(view, x ,y);

    view->setSize(200, 200);
    view->setLocation(
            static_cast<int>(x - view->getWidth() / 2),
            static_cast<int>(y - view->getHeight() / 2));
    this->getView()->addChild(view);
}

void GameplayMenu::onHideGamePad(GamePadView *view) {
    GamePadListener::onHideGamePad(view);
    this->getView()->removeChild(view);
}

void GameplayMenu::onUpdateGamePad(GamePadView *view, int angle, int progress) {
    GamePadListener::onUpdateGamePad(view, angle, progress);
    mCallback->onGamePadMove(angle, progress);
}

void GameplayMenu::update(const EntityState &playerState) {
    mHealthTextDrawer->setText(playerState.get(EntityState::POS_HEALTH));
    mEnergyTextDrawer->setText(playerState.get(EntityState::POS_ENERGY));
}

void GameplayMenu::handleTouchEvent(TouchEvent event, TouchEvent downEvent) {
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

void GameplayMenu::initSkills() {
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

void GameplayMenu::initPropertyDrawers() {
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


