#pragma once

#include "wnd_engine/platform/Platform.h"
#include "NetworkListener.h"
#include <string>
#include <unordered_set>

using namespace wnd;

class WorldState;

class GameNetworkManager : public WebSocketListener {
private:
    static const int MOVEMENT_MS_INTERVAL = 100;

    Platform *const mPlatform;
    PlatformWebSocket *const mWebSocket;
    NetworkListener *const mListener;

    string playerServerObjectId;
    long serverUpdateInterval;

    WorldState lastWorldState;
    std::uint64_t lastMovementUpdate;

    /**
     * Player skill object ids (this server objects won't be displayed cause it's managed on the client side)
     */
    unordered_set<string> skillObjectIds;

public:
    GameNetworkManager(Platform *platform, NetworkListener *listener);

    void connect();
    void disconnect();

    void join();
    void updatePlayerMovement(int x, int y, int angle, int speed);

    void skillON(int skillId, int x, int y, int angle);
    void skillOFF(int skillId);

    const std::string &getPlayerServerObjectId() const;
    bool isPlayerSkillObject(string objId) const;

private:

    virtual void onConnected() override ;
    virtual void onDisconnected() override ;
    virtual void onMessage(std::string message) override ;

    long getWorldTimeDiff();

};