#include "GameNetworkManager.h"
#include "GameNetworkProtocol.h"
#include "GameNetworkState.h"
#include "wnd_engine/Logger.h"
#include "wnd_engine/utils/CollectionUtils.h"
#include <vector>
#include <string>

using namespace std;

GameNetworkManager::GameNetworkManager(wnd::Platform *platform, GameNetworkListener *listener)
    : mWebSocket(platform->addWebSocket(0, GameNetworkProtocol::SERVER_URL)), mListener(listener) {
    mWebSocket->setListener(this);
    serverIteration = 0;
    lastMovementUpdate = 0;
}

void GameNetworkManager::connect() {
    mWebSocket->connect();
}

void GameNetworkManager::disconnect() {
    mWebSocket->disconnect();
}

void GameNetworkManager::updatePlayerMovement(std::uint64_t time, int x, int y, int angle, int speed) {
    if (time - lastMovementUpdate > 50) { // TODO
        std::ostringstream ss;
        ss << GameNetworkProtocol::CLIENT_MSG_MOVEMENT << ";" << serverIteration << ";" << x << ";"
           << y << ";" << angle << ";" << speed;
        mWebSocket->send(ss.str());
    }
    lastMovementUpdate = time;
}

void GameNetworkManager::skillON(int skillId, int x, int y, int angle) {
    std::ostringstream ss;
    ss << GameNetworkProtocol::CLIENT_MSG_SKILL_ON << ";" << serverIteration << ";" << skillId << ";"
        << x << ";" << y << ";" << angle;
    mWebSocket->send(ss.str());
}

void GameNetworkManager::skillOFF(int skillId) {
    std::ostringstream ss;
    ss << GameNetworkProtocol::CLIENT_MSG_SKILL_OFF << ";" << serverIteration << ";" << skillId;
    mWebSocket->send(ss.str());
}

void GameNetworkManager::onConnected() {
    wnd::Logger::log("@@@ onConnected");
    mListener->onConnection(true);

    // DEBUG
//    std::ostringstream ss;
//    ss << GameNetworkProtocol::CLIENT_MSG_SET_SERVER_DELAY << ";500";
//    mWebSocket->send(ss.str());
//
//    std::ostringstream ss2;
//    ss2 << GameNetworkProtocol::CLIENT_MSG_SET_SPEED << ";" << 200;
//    mWebSocket->send(ss2.str());
}

void GameNetworkManager::onDisconnected() {
    wnd::Logger::log("@@@ onDisconnected");
    mListener->onConnection(false);
}

void GameNetworkManager::onMessage(std::string message) {
    std::ostringstream ss;
    ss << "@@@ ";

    vector<string> splits;
    CollectionUtils::split(splits, message, ';');
    if (splits.empty()) {
        return;
    }
    if (splits[0] == GameNetworkProtocol::SERVER_MSG_CONNECT_ID) {
        playerServerObjectId = splits[1];
        serverIteration = stol(splits[2]);
        ss << "onConnected " << playerServerObjectId << " " << serverIteration;
    } else if (splits[0] == GameNetworkProtocol::SERVER_MSG_OBJECT_ADDED) {
        GameNetworkObjectState obj(splits[1]);
        mListener->onGameObjectAdded(obj);
        ss << "onObjectAdded " << message;
    } else if (splits[0] == GameNetworkProtocol::SERVER_MSG_OBJECT_DESTROYED) {
        GameNetworkObjectState obj(splits[1]);
        mListener->onGameObjectRemoved(obj);
        ss << "onObjectDestroyed " << message;
    } else if (splits[0] == GameNetworkProtocol::SERVER_MSG_STATE) {
        GameNetworkState state(splits);
        serverIteration = state.getServerIteration();
        mListener->onGameStateUpdated(state);
        ss << "onMessage " << message;
    }

    wnd::Logger::log(ss.str());
}

const std::string &GameNetworkManager::getPlayerServerObjectId() const {
    return playerServerObjectId;
}
