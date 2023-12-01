#pragma once

#include "wnd_engine/platform/Platform.h"
#include "GameNetworkListener.h"
#include <string>

using namespace wnd;

class GameNetworkManager : public WebSocketListener {
private:
    Platform *const mPlatform;
    PlatformWebSocket *const mWebSocket;
    GameNetworkListener *const mListener;

    string playerServerObjectId;
    uint64_t firstStateTime; // client clientTime when first state message received
    uint64_t lastServerTime;
    long serverUpdateInterval;

    std::uint64_t lastMovementUpdate;

public:
    GameNetworkManager(Platform *platform, GameNetworkListener *listener);

    void connect();

    void disconnect();

    void updatePlayerMovement(std::uint64_t time, int x, int y, int angle, int speed);

    void skillON(int skillId, int x, int y, int angle);
    void skillOFF(int skillId);

    const std::string &getPlayerServerObjectId() const;

private:

    virtual void onConnected() override ;
    virtual void onDisconnected() override ;
    virtual void onMessage(std::string message) override ;

};