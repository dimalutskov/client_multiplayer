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
    moveAngle = 0;
    moveProgress = 0;
}

void GameNetworkManager::connect() {
    mWebSocket->connect();
}

void GameNetworkManager::disconnect() {
    mWebSocket->disconnect();
}

void GameNetworkManager::updatePlayerMovement(int angle, int progress) {
    if (moveAngle != angle || moveProgress != progress) {
        std::ostringstream ss;
        ss << GameNetworkProtocol::CLIENT_MSG_MOVEMENT << ";" << serverIteration << ";" << angle<< ";" << progress;
        mWebSocket->send(ss.str());
        moveAngle = angle;
        moveProgress = progress;
    }
}

void GameNetworkManager::skillON(int skillId) {
    std::ostringstream ss;
    ss << GameNetworkProtocol::CLIENT_MSG_SKILL_ON << ";" << serverIteration << ";" << skillId;
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
    std::ostringstream ss;
    ss << GameNetworkProtocol::CLIENT_MSG_SET_SERVER_DELAY << ";500";
    mWebSocket->send(ss.str());

    std::ostringstream ss2;
    ss2 << GameNetworkProtocol::CLIENT_MSG_SET_SPEED << ";" << 200;
    mWebSocket->send(ss2.str());


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
        mListener->onPlayerConnection(playerServerObjectId);
    } else if (splits[0] == GameNetworkProtocol::SERVER_MSG_PLAYER_CONNECT) {
        std::string playerId = splits[1];
    } else if (splits[0] == GameNetworkProtocol::SERVER_MSG_PLAYER_DISCONNECT) {
        std::string playerId = splits[1];
    } else if (splits[0] == GameNetworkProtocol::SERVER_MSG_STATE) {
        GameNetworkState state(splits);
        mListener->onGameStateUpdated(state);
        ss << "onMessage " << message;
    }

    wnd::Logger::log(ss.str());
}

const std::string &GameNetworkManager::getPlayerServerObjectId() const {
    return playerServerObjectId;
}
