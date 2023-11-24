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
    * {OBJECT_STATE} = id;type;x;y;angle;...(rest type related props)
    */
    static const string SERVER_MSG_STATE;

    /**
    * Provides objectId for player which was connected to server
    * {MSG_TYPE};{OBJECT_ID};
    */
    static const string SERVER_MSG_PLAYER_CONNECT;

    /**
    * Provides objectId for player which was disconnected to server
    * {MSG_TYPE};{OBJECT_ID};
    */
    static const string SERVER_MSG_PLAYER_DISCONNECT;


    /////////////////// CLIENT_MESSAGES ///////////////////////////

    /**
    * Provides player movement attributes
    * {MSG_TYPE};{SERVER_ITERATION};{angle(int)};{progress(int)};
    */
    static const string CLIENT_MSG_MOVEMENT;

    /**
     * Provides player skill attributes
     * {MSG_TYPE};{SERVER_ITERATION};{skillID};{skillParams};
     */
    static const string CLIENT_MSG_SKILL_ON;
    static const string CLIENT_MSG_SKILL_OFF;


    ///////////////////// GAME OBJECT TYPES /////////////////////////
    static const int GAME_OBJECT_TYPE_PLAYER = 1;
};

const string GameNetworkProtocol::SERVER_URL = "wss://dl-websockets-25f48806cc22.herokuapp.com/websocket";

const string GameNetworkProtocol::SERVER_MSG_CONNECT_ID = "connected";
const string GameNetworkProtocol::SERVER_MSG_STATE = "state";
const string GameNetworkProtocol::SERVER_MSG_PLAYER_CONNECT = "playerConnected";
const string GameNetworkProtocol::SERVER_MSG_PLAYER_DISCONNECT = "playerDisconnected";

const string GameNetworkProtocol::CLIENT_MSG_MOVEMENT = "move";
const string GameNetworkProtocol::CLIENT_MSG_SKILL_ON = "skillON";
const string GameNetworkProtocol::CLIENT_MSG_SKILL_OFF = "skillOFF";
