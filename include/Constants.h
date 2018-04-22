#pragma once

//misc
static const char* TITLE = "starter";
static const unsigned int SCREEN_WIDTH = 1920u;
static const unsigned int SCREEN_HEIGHT = 1080u;

//enviroment/gameplay
static const float GRAVITY = 900.f;
static const int TILE_SIZE = 60;
static const int TILES_X = SCREEN_WIDTH / TILE_SIZE; 
static const int TILES_Y = SCREEN_HEIGHT / TILE_SIZE;
static const float SCROLL_SPEED = 0;// -30.f;
static const float MAX_FISHING_SPOTS = 5;
static const int WATER_TILES = 1;

//player
static const float PLAYER_MAXSPEED = 300.f;
static const float PLAYER_SPEED = 200.f;
static const float PLAYER_SPEEDTHRESHOLD = 10.f;
static const float PLAYER_JUMPFORCE = -700.f;
static const float PLAYER_DRAGTHRESHOLD = 1.f;
static const float PLAYER_DRAG = 0.99f;
static const float PLAYER_SIZE_X = 1.f;
static const float PLAYER_SIZE_Y = 2.f;
static const float PLAYER_SPACEMULT = 2.f;
static const float PLAYER_MAXSPACEMULT = 2.f; //seconds max you can hold space
static const const char* PLAYER_RUN_ANIM = "Run";
static const const char* PLAYER_IDLE_ANIM = "Idle";
static const const char* PLAYER_IDLE_CROUCH_ANIM = "Idle Crouch";

//platform
static const float PLATFORM_SPAWN_TIME = 1.f;
static const float PLATFORM_SIZE_X = 5.f;
static const float PLATFORM_SIZE_Y = 0.5f;
static const int PLATFORM_PLAYER_START_X = 14;
static const int PLATFORM_PLAYER_START_Y = 16;
static const int PLATFORM_START_Y_SIZE = 9;
static const int PLATFORM_START_Y[PLATFORM_START_Y_SIZE]{ 0, 2, 5, 7, 8, 9 ,10, 11, 14};

//fishing spots
static const float FISHINGSPOT_SIZE_X = 1.f;
static const float FISHINGSPOT_SIZE_Y = 1.f;
static const int FISHINGSPOT_MAXACTIVE = 5;
static const float FISHINGSPOT_SPAWN_TIME = 0.65f;
static const float FISHINGSPOT_ACTIVE_TIME = 10.f;
static const int FISHINGSPOT_SPAWN_CHANCE = 5;