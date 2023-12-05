#pragma once

class GameConstants {
public:
    ///////////////////// GAME OBJECT TYPES /////////////////////////
    static const int GAME_OBJECT_TYPE_PLAYER = 1;
    // object_id of shot types will be: "{owner_id}||{unique_id}"
    static const int GAME_OBJECT_TYPE_SHOT = 2;
    static const int GAME_OBJECT_TYPE_NPC = 3;

    //////////////// SKILL IDS ///////////////////
    static const int SKILL_ID_SHOT = 1;
    static const int SKILL_ID_SPEED = 2;

    // TEMP CLIENTS
    static const int MAX_SPEED = 200;
    static const int SHOT_SPEED = 300;
    static const int SHOT_SIZE = 20;
};