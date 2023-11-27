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

    std::uint64_t lastMovementUpdate;

public:
    GameNetworkManager(Platform *platform, GameNetworkListener *listener);

    void connect();

    void disconnect();

    void updatePlayerMovement(std::uint64_t time, int x, int y, int angle, int speed);

    void skillON(int skillId);
    void skillOFF(int skillId);

    const std::string &getPlayerServerObjectId() const;

private:

    virtual void onConnected() override ;
    virtual void onDisconnected() override ;
    virtual void onMessage(std::string message) override ;

};