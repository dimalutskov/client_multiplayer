#pragma once

#include <string>

using namespace std;

/**
* First int before first ; in string message(both client and server) defines message type
* "{MSG_TYPE};{MSG_CONTENT};"
*/
class GameNetworkProtocol {
public:
    static const string SERVER_URL;

    /////////////////// SERVER_MESSAGES //////////////////////

    /**
    * Provides server information and objectId for this connected client to retrieve current player state from server state message
    * {MSG_TYPE};{OBJECT_ID};{SERVER_INFO};
    * SERVER_INFO: "{SERVER_TIME},{UPDATE_INTERVAL}"
    */
    static const string SERVER_MSG_RESPONSE_CONNECTED;

    /**
    * Provides current game state with all required game objects, etc.
    * {MSG_TYPE};{SERVER_TIME};{OBJECT_STATE};{OBJECT_STATE};...{OBJECT_STATE};
    * {OBJECT_STATE} = id,type,x,y,angle,speed...(rest type related props)
    */
    static const string SERVER_MSG_STATE;

    /**
     * Provides object which was added to the game world
     * {MSG_TYPE};{SERVER_TIME};{OBJECT_STATE};
     */
    static const string SERVER_MSG_OBJECT_ADDED;

    /**
    * Provides object which was removed from the game world
    * {MSG_TYPE};{SERVER_TIME},{OBJECT_STATE};
    */
    static const string SERVER_MSG_OBJECT_DESTROYED;

    /**
     * Response to client when some skill activated and new game objects created on server side. Proved new object server ids
     * {MSG_TYPE};{SERVER_TIME};{SKILL_ID};{OBJECT_ID};...{OBJECT_ID};
     */
    static const string SERVER_MSG_RESPONSE_SKILL_OBJECTS;


    /////////////////// CLIENT_MESSAGES ///////////////////////////

    /**
    * Provides player movement attributes
    * {MSG_TYPE};{SERVER_TIME};{x};{y};{angle};{speed}
    */
    static const string CLIENT_MSG_MOVEMENT;

    /**
     * Provides player skill attributes
     * {MSG_TYPE};{SERVER_TIME};{x};{y};{angle};{skillID};{skillParams};
     */
    static const string CLIENT_MSG_SKILL_ON;
    static const string CLIENT_MSG_SKILL_OFF;

    /// DEBUG MESSAGES
    static const string CLIENT_MSG_SET_SERVER_DELAY;

    ///////////////////// GAME OBJECT TYPES /////////////////////////
    static const int GAME_OBJECT_TYPE_PLAYER = 1;
    static const int GAME_OBJECT_TYPE_SHOT = 2;
};

const string GameNetworkProtocol::SERVER_URL = "wss://dl-websockets-25f48806cc22.herokuapp.com/websocket";

const string GameNetworkProtocol::SERVER_MSG_RESPONSE_CONNECTED = "connected";
const string GameNetworkProtocol::SERVER_MSG_STATE = "state";
const string GameNetworkProtocol::SERVER_MSG_OBJECT_ADDED = "objectAdded";
const string GameNetworkProtocol::SERVER_MSG_OBJECT_DESTROYED = "objectDestroyed";
const string GameNetworkProtocol::SERVER_MSG_RESPONSE_SKILL_OBJECTS = "skillObjects";

const string GameNetworkProtocol::CLIENT_MSG_MOVEMENT = "move";
const string GameNetworkProtocol::CLIENT_MSG_SKILL_ON = "skillON";
const string GameNetworkProtocol::CLIENT_MSG_SKILL_OFF = "skillOFF";

const string GameNetworkProtocol::CLIENT_MSG_SET_SERVER_DELAY = "setServerDelay";
