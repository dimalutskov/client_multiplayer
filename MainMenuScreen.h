#pragma once

#include <engine/App.h>
#include <engine/AppLifecycleListener.h>
#include <engine/layout/view/ViewGroup.h>

class MainMenuScreenCallback {
public:
    virtual void onMainScreenButtonClick() = 0;
};

class MainMenuScreen {
private:
    App *const mApp;
    MainMenuScreenCallback *const mCallback;

    ViewGroup *mRootView;
    View *mBtnConnect;

    View *mProgress;

public:
    MainMenuScreen(App *app, MainMenuScreenCallback *callback) : mApp(app), mCallback(callback) {
        mRootView = new ViewGroup();
        mRootView->setSize(app->getAttachedWindow()->getWidth(), app->getAttachedWindow()->getHeight());

        mBtnConnect = new View();
        mBtnConnect->setSize(300, 100);

        mBtnConnect->addDrawer(new ViewDrawer([](ViewCanvas *canvas, void *customData) {
            auto *thisClass = static_cast<MainMenuScreen*>(customData);
            auto color = RgbData(200, 100, 100);
            canvas->draw(canvas->newObject()
                                 ->setRenderData(RenderData(color)));
        }, this));

        mBtnConnect->setClickListener([this](View *view) {
            mCallback->onMainScreenButtonClick();
        });

        mProgress = new View();
        mProgress->setSize(100, 100);
        mProgress->addDrawer(new ViewDrawer([](ViewCanvas *canvas, void *customData) {
            auto *thisClass = static_cast<MainMenuScreen*>(customData);
            auto color = RgbData(200, 100, 200);
            canvas->draw(canvas->newObject()
                                 ->setRenderData(RenderData(color)));
        }, this));


        mRootView->subscribeSizeChanges([this](Point<int> oldSize, Point<int> newSize) {
            mBtnConnect->setLocation(mRootView->getWidth() / 2 - mBtnConnect->getWidth() / 2, 100);
            mProgress->setLocation(mRootView->getWidth() / 2 - mProgress->getWidth() / 2, mRootView->getHeight() - 200);
        }, true);

        mRootView->addChild(mBtnConnect);
    }

    void attach() {
        mApp->attachView(mRootView);
    }

    void detach() {
        mApp->detachView(mRootView);
    }

    void showProgress() {
        if (mProgress->getParent() == 0) {
            mRootView->addChild(mProgress);
        }
        mBtnConnect->setEnabled(false);
    }

    void hideProgress() {
        mRootView->removeChild(mProgress);
        mBtnConnect->setEnabled(true);
    }

};