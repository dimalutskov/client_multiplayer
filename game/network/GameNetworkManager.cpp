#include "GameNetworkManager.h"
#include "NetworkProtocol.h"
#include "WorldState.h"
#include "wnd_engine/Logger.h"
#include "wnd_engine/utils/CollectionUtils.h"
#include "PlayerInfo.h"
#include <vector>
#include <string>

using namespace std;

GameNetworkManager::GameNetworkManager(wnd::Platform *platform, NetworkListener *listener)
    : mPlatform(platform), mWebSocket(platform->addWebSocket(0, NetworkProtocol::SERVER_URL)), mListener(listener) {
    mWebSocket->setListener(this);
    lastMovementUpdate = 0;
    serverUpdateInterval = 0;
}

void GameNetworkManager::connect() {
    mWebSocket->connect();
}

void GameNetworkManager::disconnect() {
    mWebSocket->disconnect();
}

void GameNetworkManager::updatePlayerMovement(int x, int y, int angle, int speed) {
    uint64_t time = mPlatform->milliseconds();
    if (time - lastMovementUpdate > MOVEMENT_MS_INTERVAL || speed == 0) {
        uint64_t serverTime = time; // TODO time no need!
        std::ostringstream ss;
        ss << NetworkProtocol::CLIENT_MSG_MOVEMENT << ";" << time << ";" << x << ";" << y << ";" << angle << ";" << speed;
        mWebSocket->send(ss.str());
        lastMovementUpdate = time;
    }
}

void GameNetworkManager::skillON(int skillId, int x, int y, int angle) {
    long timeAfterLastFrame = mPlatform->milliseconds() - lastWorldState.getClientTime();
    uint64_t calculatedServerTime = lastWorldState.getServerTime() + timeAfterLastFrame - getWorldTimeDiff();
    std::ostringstream ss;
    ss << NetworkProtocol::CLIENT_MSG_SKILL_ON << ";" << calculatedServerTime << ";" << skillId << ";"
        << x << ";" << y << ";" << angle;
    mWebSocket->send(ss.str());
}

void GameNetworkManager::skillOFF(int skillId) {
    long timeAfterLastFrame = mPlatform->milliseconds() - lastWorldState.getClientTime();
    uint64_t calculatedServerTime = lastWorldState.getServerTime() + timeAfterLastFrame - getWorldTimeDiff();
    std::ostringstream ss;
    ss << NetworkProtocol::CLIENT_MSG_SKILL_OFF << ";" << calculatedServerTime << ";" << skillId;
    mWebSocket->send(ss.str());
}

void GameNetworkManager::onConnected() {
    wnd::Logger::log("@@@ onSocketConnected");
    mListener->onConnection(true);
}

void GameNetworkManager::onDisconnected() {
    wnd::Logger::log("@@@ onSocketDisconnected");
    mListener->onConnection(false);
}

void GameNetworkManager::onMessage(std::string message) {
    std::ostringstream ss;
    ss << "@@@ ";

    std::uint64_t clientRealTime = mPlatform->milliseconds();
    // world state for client will be shown in the past for proper entity interpolation
    std::uint64_t clientWorldTime = clientRealTime + getWorldTimeDiff();

    vector<string> splits;
    CollectionUtils::split(splits, message, ';');
    if (splits.empty()) {
        return;
    }
    if (splits[0] == NetworkProtocol::SERVER_MSG_RESPONSE_CONNECTED) {
        // Server info
        vector<string> serverInfoSplits;
        CollectionUtils::split(serverInfoSplits, splits[1], ',');
        uint64_t serverTime = stol(serverInfoSplits[0]);
        serverUpdateInterval = stol(serverInfoSplits[1]);

//        ss << "!!! " << serverInfoSplits[0] << " " << serverInfoSplits[1];
//        wnd::Logger::log(ss.str());

        // Player obj id
        playerServerObjectId = splits[2];

        // Player info
//        PlayerInfo playerInfo(splits[3]);

        ss << "onMsgConnected " << playerServerObjectId << " updateInterval: " << serverUpdateInterval;
    } else if (splits[0] == NetworkProtocol::SERVER_MSG_OBJECT_ADDED) {
//        ObjectState obj(clientTime, splits[2]);
//        mListener->onGameObjectAdded(obj);
        ss << "onObjectAdded " << message;
    } else if (splits[0] == NetworkProtocol::SERVER_MSG_OBJECT_DESTROYED) {
        uint64_t serverTime = stol(splits[1]);
        ObjectState obj(clientWorldTime, serverTime, splits[2]);
        mListener->onGameObjectRemoved(obj);
        skillObjectIds.erase(obj.getObjectId());

        ss << "onObjectDestroyed " << message;
    } else if (splits[0] == NetworkProtocol::SERVER_MSG_RESPONSE_SKILL_OBJECTS) {
        std::uint64_t time = stol(splits[1]);
        for (int i = 2; i < splits.size(); i++) {
            skillObjectIds.insert(splits[i]);
        }
        ss << "onSkillObjects " << message;
    } else if (splits[0] == NetworkProtocol::SERVER_MSG_STATE) {
        lastWorldState = WorldState(clientRealTime, clientWorldTime, splits);
        mListener->onGameStateUpdated(lastWorldState);
        ss << "onMessage " << message;
    }

    wnd::Logger::log(ss.str());
}

const std::string &GameNetworkManager::getPlayerServerObjectId() const {
    return playerServerObjectId;
}

bool GameNetworkManager::isPlayerSkillObject(string objId) const {
    return skillObjectIds.count(objId);
}

long GameNetworkManager::getWorldTimeDiff() {
    return serverUpdateInterval * 1.5;
}
