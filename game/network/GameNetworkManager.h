#pragma once

#include "wnd_engine/platform/Platform.h"
#include "GameNetworkListener.h"
#include <string>

using namespace wnd;

class GameNetworkManager : public WebSocketListener {
private:
    PlatformWebSocket *const mWebSocket;
    GameNetworkListener *const mListener;

    std::string playerServerObjectId;
    long serverIteration;

    // Movement
    int moveAngle;
    int moveProgress;

public:
    GameNetworkManager(Platform *platform, GameNetworkListener *listener);

    void connect();

    void disconnect();

    void updatePlayerMovement(int angle, int progress);

    void skillON(int skillId);
    void skillOFF(int skillId);

    const std::string &getPlayerServerObjectId() const;

private:

    virtual void onConnected() override ;
    virtual void onDisconnected() override ;
    virtual void onMessage(std::string message) override ;

};