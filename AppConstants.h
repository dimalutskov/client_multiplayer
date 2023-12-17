#pragma once

class AppConstants {
public:
    static const int SHADER_TYPE_SPACE = 301;

    ///////////////////// GAME OBJECT TYPES /////////////////////////
    static const int ENTITY_TYPE_SPACESHIP = 1;
    // object_id of shot types will be: "{owner_id}||{unique_id}"
    static const int ENTITY_TYPE_SHOT = 2;

    //////////////// SKILL IDS ///////////////////
    static const int SKILL_TYPE_SHOT = 1;
    static const int SKILL_TYPE_ACCELERATION = 2;

    // TEMP CLIENTS
    static const int MAX_SPEED = 200;
    static const int SHOT_SPEED = 300;
    static const int SPACESHIP_SIZE = 100;
    static const int SHOT_SIZE = 20;
};