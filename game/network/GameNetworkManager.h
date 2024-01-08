#pragma once

#include <string>
#include <unordered_set>
#include <engine/platform/Platform.h>
#include <engine/platform/PlatformWebSocket.h>
#include <engine/Logger.h>
#include "NetworkListener.h"
#include "NetworkProtocol.h"

class GameNetworkManager : public WebSocketListener {
private:
    static const int MOVEMENT_MS_INTERVAL = 100;

    Platform *const mPlatform;
    PlatformWebSocket *const mWebSocket;

    std::list<NetworkListener*> listeners;

    string playerServerObjectId;
    long serverUpdateInterval;

    WorldState lastWorldState;
    std::uint64_t lastMovementUpdate;

    /**
     * Player skill object ids (this server objects won't be displayed cause it's managed on the client side)
     */
    unordered_set<string> skillObjectIds;

public:
    GameNetworkManager(Platform *platform, PlatformWebSocket *webSocket) : mPlatform(platform), mWebSocket(webSocket) {
        mWebSocket->addListener(this);
        lastMovementUpdate = 0;
        serverUpdateInterval = 0;
    }

    void subscribe(NetworkListener *listener) {
        listeners.push_back(listener);
    }

    void connect() {
        mWebSocket->connect();
    }

    void disconnect() {
        mWebSocket->disconnect();
    }

    void join() {
        std::ostringstream ss;
        ss << NetworkProtocol::CLIENT_MSG_JOIN << ";";
        mWebSocket->send(ss.str());
    }

    void updatePlayerMovement(int x, int y, int angle, int speed) {
        uint64_t time = mPlatform->milliseconds();
        if (time - lastMovementUpdate > MOVEMENT_MS_INTERVAL || speed == 0) {
            uint64_t serverTime = time; // TODO time no need!
            std::ostringstream ss;
            ss << NetworkProtocol::CLIENT_MSG_MOVEMENT << ";" << time << ";" << x << ";" << y << ";" << angle << ";" << speed;
            mWebSocket->send(ss.str());
            lastMovementUpdate = time;
        }
    }

    void skillON(int skillId, int x, int y, int angle) {
        long timeAfterLastFrame = mPlatform->milliseconds() - lastWorldState.getClientTime();
        uint64_t calculatedServerTime = lastWorldState.getServerTime() + timeAfterLastFrame - getWorldTimeDiff();
        std::ostringstream ss;
        ss << NetworkProtocol::CLIENT_MSG_SKILL_ON << ";" << calculatedServerTime << ";" << skillId << ";"
           << x << ";" << y << ";" << angle;
        mWebSocket->send(ss.str());
    }

    void skillOFF(int skillId) {
        long timeAfterLastFrame = mPlatform->milliseconds() - lastWorldState.getClientTime();
        uint64_t calculatedServerTime = lastWorldState.getServerTime() + timeAfterLastFrame - getWorldTimeDiff();
        std::ostringstream ss;
        ss << NetworkProtocol::CLIENT_MSG_SKILL_OFF << ";" << calculatedServerTime << ";" << skillId << ";";
        mWebSocket->send(ss.str());
    }

    const std::string &getPlayerServerObjectId() const {
        return playerServerObjectId;
    }

    bool isPlayerSkillObject(string objId) const {
        return skillObjectIds.count(objId);
    }

private:

    virtual void onWebSocketConnected(int socketId) override {
        Logger::log("@@@ onSocketConnected");
        for (NetworkListener *listener : listeners) listener->onConnection(true);
    }

    virtual void onWebSocketDisconnected(int socketId) override {
        Logger::log("@@@ onSocketDisconnected");
        for (NetworkListener *listener : listeners) listener->onConnection(false);
    }

    virtual void onWebSocketMessage(int socketId, std::string message) override {
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
            for (NetworkListener *listener : listeners) listener->onJoinGame(entity);
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
                for (NetworkListener *listener : listeners) listener->onGameEntityAdded(entity);
                ss << "onObjectAdded " << message;
            } else if (splits[0] == NetworkProtocol::SERVER_MSG_OBJECT_DESTROYED) {
                for (NetworkListener *listener : listeners) listener->onGameEntityDestroyed(entity);
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
            for (NetworkListener *listener : listeners) listener->onAttachEntityInfluence(entityId, influence);
            ss << "onAttachInfluence " << message;
        } else if (splits[0] == NetworkProtocol::SERVER_MSG_INFLUENCE_OFF) {
//        std::uint64_t time = stol(splits[1]);
//        std::string entityId = splits[2];
//        EntityInfluence influence(splits[3]);
//        listener->onDetachEntityInfluence(entityId, influence);
            ss << "onDetachInfluence " << message;
        } else if (splits[0] == NetworkProtocol::SERVER_MSG_RESPONSE_SKILL_OBJECTS) {
            std::uint64_t time = stol(splits[1]);
            for (int i = 2; i < splits.size(); i++) {
                skillObjectIds.insert(splits[i]);
            }
            ss << "onSkillObjects " << message;
        } else if (splits[0] == NetworkProtocol::SERVER_MSG_STATE) {
            lastWorldState = WorldState(clientRealTime, clientWorldTime, splits);
            for (NetworkListener *listener : listeners) listener->onGameStateUpdated(lastWorldState);
            ss << "onWebSocketMessage " << message;
        }

        Logger::log(ss.str());
    }

    long getWorldTimeDiff() {
        return serverUpdateInterval * 1.5;
    }

};