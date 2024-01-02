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
     * {MSG_TYPE};{SERVER_INFO};
     * SERVER_INFO: "{SERVER_TIME},{UPDATE_INTERVAL}"
     */
    static const string SERVER_MSG_RESPONSE_CONNECTED;

    /**
     * Provides objectId for this joined client to retrieve current player state from server state message
     * {MSG_TYPE};{SERVER_TIME};{OBJECT_STATE}
     */
    static const string SERVER_MSG_RESPONSE_JOIN;

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

    /**
     * Notify about any new active influence(skill, shot, recover, etc) applied to any world entity
     * {MSG_TYPE};{SERVER_TIME};{ENTITY_RECEIVER_ID};{INFLUENCE_DESC};
     * INFLUENCE_DESC - influenceType,skillType,ownerId,values(described for each type)
     */
    static const string SERVER_MSG_INFLUENCE_ON;

    /**
     * Notify about stop applying influence. Notice - for single influences (like single shot or one time skills)
     * this message wont be sent
     * {MSG_TYPE};{SERVER_TIME};{ENTITY_RECEIVER_ID};{INFLUENCE_DESC};
     * INFLUENCE_DESC - influenceType,skillType,ownerId,values(described for each type)
     */
    static const string SERVER_MSG_INFLUENCE_OFF;


    /////////////////// CLIENT_MESSAGES ///////////////////////////

    /**
     * Join client as a world entity. Entity's id will be provided in response message
     * {MSG_TYPE};
     */
    static const string CLIENT_MSG_JOIN;

    /**
    * Provides player movement attributes
    * {MSG_TYPE};{SERVER_TIME};{x};{y};{angle};{speed}
    */
    static const string CLIENT_MSG_MOVEMENT;

    /**
    * Provides player skill attributes
    * {MSG_TYPE};{SERVER_ESTIMATED_TIME};{skillID};{skillParams};
    * skillParams - {x};{y};{angle} - for single shots
    */
    static const string CLIENT_MSG_SKILL_ON;

    /**
     * Provides player skill attributes
     * {MSG_TYPE};{SERVER_ESTIMATED_TIME};{skillID};{skillDuration};
     */
    static const string CLIENT_MSG_SKILL_OFF;

    /// DEBUG MESSAGES
    static const string CLIENT_MSG_SET_SERVER_DELAY;
};

const string NetworkProtocol::SERVER_URL = "wss://dl-websockets-25f48806cc22.herokuapp.com/websocket";

const string NetworkProtocol::SERVER_MSG_RESPONSE_CONNECTED = "connected";
const string NetworkProtocol::SERVER_MSG_RESPONSE_JOIN = "join";
const string NetworkProtocol::SERVER_MSG_STATE = "state";
const string NetworkProtocol::SERVER_MSG_OBJECT_ADDED = "objectAdded";
const string NetworkProtocol::SERVER_MSG_OBJECT_DESTROYED = "objectDestroyed";
const string NetworkProtocol::SERVER_MSG_RESPONSE_SKILL_OBJECTS = "skillObjects";
const string NetworkProtocol::SERVER_MSG_INFLUENCE_ON = "influenceOn";
const string NetworkProtocol::SERVER_MSG_INFLUENCE_OFF = "influenceOff";

const string NetworkProtocol::CLIENT_MSG_JOIN = "join";
const string NetworkProtocol::CLIENT_MSG_MOVEMENT = "move";
const string NetworkProtocol::CLIENT_MSG_SKILL_ON = "skillON";
const string NetworkProtocol::CLIENT_MSG_SKILL_OFF = "skillOFF";

const string NetworkProtocol::CLIENT_MSG_SET_SERVER_DELAY = "setServerDelay";
