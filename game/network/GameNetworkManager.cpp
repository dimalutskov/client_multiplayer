#include "GameNetworkManager.h"
#include "NetworkProtocol.h"
#include "WorldState.h"
#include "wnd_engine/Logger.h"
#include "wnd_engine/utils/CollectionUtils.h"
#include <vector>
#include <string>

using namespace std;

GameNetworkManager::GameNetworkManager(wnd::Platform *platform, NetworkListener *listener)
    : mPlatform(platform), mWebSocket(platform->addWebSocket(0, NetworkProtocol::SERVER_URL)), mListener(listener) {
    mWebSocket->setListener(this);
    lastMovementUpdate = 0;
    serverUpdateInterval = 0;
}

void GameNetworkManager::join() {
    std::ostringstream ss;
    ss << NetworkProtocol::CLIENT_MSG_JOIN << ";";
    mWebSocket->send(ss.str());
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
    ss << NetworkProtocol::CLIENT_MSG_SKILL_OFF << ";" << calculatedServerTime << ";" << skillId << ";";
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

    // CONNECTED
    if (splits[0] == NetworkProtocol::SERVER_MSG_RESPONSE_CONNECTED) {
        // Server info
        vector<string> serverInfoSplits;
        CollectionUtils::split(serverInfoSplits, splits[1], ',');
        uint64_t serverTime = stol(serverInfoSplits[0]);
        serverUpdateInterval = stol(serverInfoSplits[1]);
        ss << "onMsgConnected " << playerServerObjectId << " updateInterval: " << serverUpdateInterval;
    }
    // JOIN
    else if (splits[0] == NetworkProtocol::SERVER_MSG_RESPONSE_JOIN) {
        uint64_t serverTime = stol(splits[1]);
        EntityState entity(clientWorldTime, serverTime, splits[2]);
        playerServerObjectId = entity.getObjectId();
        mListener->onJoinGame(entity);
        ss << "onJoin " << message;
    }
    // ADD|DESTROY
    else if (splits[0] == NetworkProtocol::SERVER_MSG_OBJECT_ADDED ||
        splits[0] == NetworkProtocol::SERVER_MSG_OBJECT_DESTROYED) {
        uint64_t serverTime = stol(splits[1]);
        uint64_t objectTime = stol(splits[2]);
        std::uint64_t entityClientTime = clientWorldTime - (serverTime - objectTime);
        EntityState entity(entityClientTime, serverTime, splits[3]);
        if (splits[0] == NetworkProtocol::SERVER_MSG_OBJECT_ADDED) {
            mListener->onGameEntityAdded(entity);
            ss << "onObjectAdded " << message;
        } else if (splits[0] == NetworkProtocol::SERVER_MSG_OBJECT_DESTROYED) {
            mListener->onGameEntityDestroyed(entity);
            skillObjectIds.erase(entity.getObjectId()); // TODO????
            ss << "onObjectDestroyed " << message;
        }
    }
    // INFLUENCE ON
    else if (splits[0] == NetworkProtocol::SERVER_MSG_INFLUENCE_ON) {
        std::uint64_t serverTime = stol(splits[1]);
        std::string entityId = splits[2];
        std::uint64_t influenceAttachTime = stol(splits[3]);
        std::uint64_t attachTime = clientWorldTime - (serverTime - influenceAttachTime);
        EntityInfluence influence(attachTime, splits[4]);
        mListener->onAttachEntityInfluence(entityId, influence);
        ss << "onAttachInfluence " << message;
    } else if (splits[0] == NetworkProtocol::SERVER_MSG_INFLUENCE_OFF) {
//        std::uint64_t time = stol(splits[1]);
//        std::string entityId = splits[2];
//        EntityInfluence influence(splits[3]);
//        mListener->onDetachEntityInfluence(entityId, influence);
        ss << "onDetachInfluence " << message;
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