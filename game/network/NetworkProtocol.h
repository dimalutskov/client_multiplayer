#pragma once

#include <string>

using namespace std;

/**
 * First int before first ; in string message(both client and server) defines message type
 * "{MSG_TYPE};{MSG_CONTENT};"
 *
 * {SERVER_TIME} - server time
 * {SERVER_ESTIMATED_TIME} - server time which is calculated on client side based on last server state
 */
class NetworkProtocol {
public:
    static const string SERVER_URL;

    /////////////////// SERVER_MESSAGES //////////////////////

    /**
     * Provides server information and objectId for this connected client to retrieve current player state from server state message
     * {MSG_TYPE};{SERVER_INFO};{OBJECT_ID};{PLAYER_INFO}
     * SERVER_INFO: "{SERVER_TIME},{UPDATE_INTERVAL}"
     * PLAYER_INFO: "{HEALTH},{ENERGY},{SPEED}"
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
};

const string NetworkProtocol::SERVER_URL = "wss://dl-websockets-25f48806cc22.herokuapp.com/websocket";

const string NetworkProtocol::SERVER_MSG_RESPONSE_CONNECTED = "connected";
const string NetworkProtocol::SERVER_MSG_STATE = "state";
const string NetworkProtocol::SERVER_MSG_OBJECT_ADDED = "objectAdded";
const string NetworkProtocol::SERVER_MSG_OBJECT_DESTROYED = "objectDestroyed";
const string NetworkProtocol::SERVER_MSG_RESPONSE_SKILL_OBJECTS = "skillObjects";

const string NetworkProtocol::CLIENT_MSG_MOVEMENT = "move";
const string NetworkProtocol::CLIENT_MSG_SKILL_ON = "skillON";
const string NetworkProtocol::CLIENT_MSG_SKILL_OFF = "skillOFF";

const string NetworkProtocol::CLIENT_MSG_SET_SERVER_DELAY = "setServerDelay";
