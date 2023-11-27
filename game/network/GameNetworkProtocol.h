#pragma once

#include <string>

using namespace std;

/**
* First int before first ; in string message(both client and server) defines message type
*      "{MSG_TYPE};{MSG_CONTENT};"
*
* {SERVER_ITERATION} - server iteration value to handle client/server event sequencing.
*/
class GameNetworkProtocol {
public:
    static const string SERVER_URL;

    /////////////////// SERVER_MESSAGES //////////////////////

    /**
    * Provides server iteration and objectId for this connected client to retrieve current player state from server state message
    * {MSG_TYPE};{OBJECT_ID};{SERVER_ITERATION}
    */
    static const string SERVER_MSG_CONNECT_ID;

    /**
    * Provides current game state with all required game objects, etc.
    * {MSG_TYPE};{SERVER_ITERATION};{OBJECT_STATE};{OBJECT_STATE};...{OBJECT_STATE};
    * {OBJECT_STATE} = id,type,health,energy,x,y,angle,speed...(rest type related props)
    */
    static const string SERVER_MSG_STATE;

    /**
    * Provides objectId for player which was connected to server
    * {MSG_TYPE};{OBJECT_STATE};
    */
    static const string SERVER_MSG_OBJECT_ADDED;

    /**
    * Provides objectId for player which was disconnected to server
    * {MSG_TYPE};{OBJECT_ID};
    */
    static const string SERVER_MSG_OBJECT_DESTROYED;


    /////////////////// CLIENT_MESSAGES ///////////////////////////

    /**
    * Provides player movement attributes
    * {MSG_TYPE};{SERVER_ITERATION};{x};{y};{angle};{speed}
    */
    static const string CLIENT_MSG_MOVEMENT;

    /**
     * Provides player skill attributes
     * {MSG_TYPE};{SERVER_ITERATION};{x};{y};{angle};{skillID};{skillParams};
     */
    static const string CLIENT_MSG_SKILL_ON;
    static const string CLIENT_MSG_SKILL_OFF;

    /// DEBUG MESSAGES
    static const string CLIENT_MSG_SET_SERVER_DELAY;
    static const string CLIENT_MSG_SET_SPEED;
    static const string CLIENT_MSG_SET_SHOT_SPEED;
    static const string CLIENT_MSG_SET_FIRE_RATE;

    ///////////////////// GAME OBJECT TYPES /////////////////////////
    static const int GAME_OBJECT_TYPE_PLAYER = 1;
    static const int GAME_OBJECT_TYPE_SHOT = 2;
};

const string GameNetworkProtocol::SERVER_URL = "wss://dl-websockets-25f48806cc22.herokuapp.com/websocket";

const string GameNetworkProtocol::SERVER_MSG_CONNECT_ID = "connected";
const string GameNetworkProtocol::SERVER_MSG_STATE = "state";
const string GameNetworkProtocol::SERVER_MSG_OBJECT_ADDED = "objectAdded";
const string GameNetworkProtocol::SERVER_MSG_OBJECT_DESTROYED = "objectDestroyed";

const string GameNetworkProtocol::CLIENT_MSG_MOVEMENT = "move";
const string GameNetworkProtocol::CLIENT_MSG_SKILL_ON = "skillON";
const string GameNetworkProtocol::CLIENT_MSG_SKILL_OFF = "skillOFF";

const string GameNetworkProtocol::CLIENT_MSG_SET_SERVER_DELAY = "setServerDelay";
const string GameNetworkProtocol::CLIENT_MSG_SET_SPEED = "setSpeed";
const string GameNetworkProtocol::CLIENT_MSG_SET_SHOT_SPEED = "setShotSpeed";
const string GameNetworkProtocol::CLIENT_MSG_SET_FIRE_RATE = "setFireRate";
