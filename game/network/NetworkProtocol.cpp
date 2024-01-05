#include "NetworkProtocol.h"

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
