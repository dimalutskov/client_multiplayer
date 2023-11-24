#pragma once

#include <wnd_engine/App.h>

using namespace wnd;

class MyApp : public App {
public:

    MyApp(Platform *platform);

    virtual ~MyApp();

    void onAttachedToWindow(Renderer *renderer) override;

    void onResume() override;

    void onPause() override;

};
