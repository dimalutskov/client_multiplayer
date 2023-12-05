#include "GameplayMenu.h"
#include "wnd_engine/Logger.h"
#include "network/GameNetworkManager.h"
#include "GameConstants.h"

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
};

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
    mSkillIds[skillView1->getId()] = GameConstants::SKILL_ID_SHOT;

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
    mSkillIds[skillView2->getId()] = GameConstants::SKILL_ID_SPEED;

    view->subscribeSizeChanges([this, skillView1, skillView2](Point<int> oldSize, Point<int> newSize) {
        // 1
        skillView1->setLocation(view->getWidth() - skillView1->getWidth() - 100,
                                view->getHeight() - skillView1->getHeight() - 100);
        // 2
        skillView2->setLocation(view->getWidth() - skillView2->getWidth() - 100,
                                skillView1->getY() - skillView2->getHeight() - 100);
    }, true);
}
