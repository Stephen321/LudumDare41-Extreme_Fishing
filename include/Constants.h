#pragma once

#include <string>
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Graphics/Color.hpp"

//misc
static const char* TITLE = "Ludum Dare 41";
static const unsigned int SCREEN_WIDTH = 1920u;
static const unsigned int SCREEN_HEIGHT = 1080u;

//enviroment/gameplay
static const float GRAVITY = 900.f;
static const int TILE_SIZE = 60;
static const int TILES_X = SCREEN_WIDTH / TILE_SIZE; 
static const int TILES_Y = SCREEN_HEIGHT / TILE_SIZE;
static const float SCROLL_SPEED = 0;// -20.f;
static const float MAX_FISHING_SPOTS = 6;
static const int WATER_TILES = 2;
static const int WATER_HEIGHT = WATER_TILES * TILE_SIZE;
static const int WATER_SURFACE_HEIGHT = 5 * TILE_SIZE;
static const int WATER_Y_OFFSET = (int)(SCREEN_HEIGHT * 0.5f) - WATER_HEIGHT;

static const float WATER_SPRING_K = 0.03f;
static const float WATER_SPRING_DAMPENER = 0.00015f;
static const int WATER_SPRINGS_COUNT = 500;
static const float NUM_BACKGROUND_WAVES = 7.f;
static const float BACKGROUND_WAVE_MAX_HEIGHT = 4.25f;
static const float BACKGROUND_WAVE_COMPRESSION = 1.f / 15;
static const sf::Color WATER_TOP_COLOR(16, 127, 191, 192);
static const sf::Color WATER_BOT_COLOR(23, 72, 146, 255);
static const sf::Color WATER_SURFACE_COLOR(22, 154, 222, 224);
static const int SPLASH_POSITION_OFFSET = 75; //TODO: manually set this to half the sum of the height of all the background waves 
static const int SPLASH_PARTICLES = 35;
static const int SPLASH_PARTICLES_SPEED = 250;
static const int SPLASH_PARTICLES_BASE_SPEED = 200;
static const int SPLASH_PARTICLES_X_MAXOFFSET = 20;
static const int SPLASH_PARTICLES_Y_MAXOFFSET = 20;
static const int SPLASH_PARTICLES_WAVE_MIN_STRENGTH = 20;
static const int SPLASH_PARTICLES_WAVE_STRENGTH = 80;

static const int BASE_SCORE = 150;
static const int DECR_SCORE = 50; //taking full qte time loses this much score per key


//qte
static const char* QTE_SUCCESS_ANIM = "KeyWin";
static const char* QTE_FAIL_ANIM = "KeyLose";
static const char* QTE_ATTEMPT_ANIM = "Key!";
static const std::string QTE_ANIM_PREFIX = "Key";
static const float QTE_STATUS_DISPLAY_TIME = 2.f;
//static const int QTE_SPEECH_OFF_X = 1;
static const int QTE_MIN = 3;
static const int QTE_MAX = 6;
static const float QTE_TIME_PER_KEY = 1.f; 
static const int QTE_RAND_OFFSET = 2;
static const int POSSIBLE_KEYS_SIZE = 4;
static const sf::Keyboard::Key POSSIBLE_QTE_KEYS[POSSIBLE_KEYS_SIZE] = { //also change in Helpers
	sf::Keyboard::Key::W,
	sf::Keyboard::Key::A,
	sf::Keyboard::Key::S,
	sf::Keyboard::Key::D
};

//player
static const float PLAYER_MAXSPEED = 300.f;
static const float PLAYER_SPEED = 200.f;
static const float PLAYER_SPEEDTHRESHOLD = 10.f;
static const float PLAYER_JUMPFORCE = -550.f;
static const float PLAYER_JUMPFORCE_EXTRA = PLAYER_JUMPFORCE * 0.9f;
static const float PLAYER_JUMPFORCE_INC = 175.f;
static const float PLAYER_DRAG = 0.99f;
static const float PLAYER_SIZE_X = 1.f;
static const float PLAYER_SIZE_Y = 2.f;
static const float PLAYER_MIN_SPACEHOLD = 0.3f;
static const float PLAYER_MAX_SPACEHOLD = 1.4f; //seconds max you can hold space
static const char* PLAYER_RUN_ANIM = "Run";
static const char* PLAYER_IDLE_ANIM = "Idle";
static const char* PLAYER_IDLE_CROUCH_ANIM = "Idle Crouch";
static const char* PLAYER_JUMP_ANIM = "Jump";
static const char* PLAYER_FALL_ANIM = "Falling";
static const char* PLAYER_FISHLAUNCH_ANIM = "FishingLauch";
static const char* PLAYER_IDLE_FISHING_ANIM = "Idle Fishing";
static const char* PLAYER_LAND_ANIM = "Landing";
static const char* PLAYER_CROUCH_FALL_ANIM = "CrouchFall";
static const float PLAYER_QTE_TIME = 5.f;
static const float PLAYER_ROD_OFFSET_X = 105.f;
static const float PLAYER_ROD_OFFSET_Y = 10.f;
static const float PLAYER_LAUNCH_FISH_MINTIME = 0.5f; //have to play fish animation for at least this time

//platform
static const float PLATFORM_SPAWN_TIME = 1.f;
static const float PLATFORM_SIZE_X = 5.f;
static const float PLATFORM_SIZE_Y = 0.5f;
static const int PLATFORM_PLAYER_START_X = 14;
static const int PLATFORM_PLAYER_START_Y = TILES_Y - (WATER_TILES + 1);
static const int PLATFORM_START_Y_SIZE = 9;
static const int PLATFORM_START_Y[PLATFORM_START_Y_SIZE]{ 0, 2, 5, 6, 7, 8, 9 ,10, 12};

//fishing spots
static const float FISHINGSPOT_Y_OFFSET = 5.f; //place them below top of water
static const float FISHINGSPOT_SIZE_X = 1.f;
static const float FISHINGSPOT_SIZE_Y = 1.f;
static const int FISHINGSPOT_MAXACTIVE = 16;
static const float FISHINGSPOT_SPAWN_TIME = 0.55f;
static const int SHARKSPOT_SPAWN_CHANCE = 12; //todo find a good value for this
static const int FISHINGSPOT_ACTIVE_OFFSET = 2;
static const int FISHINGSPOT_ACTIVE_TIME_MULTIPLE = 4;
//static const float FISHINGSPOT_ACTIVE_TIME = 3.f;
static const int FISHINGSPOT_SPAWN_CHANCE = 5;
static const char* FISHINGSPOT_FADEIN_ANIM = "BubblesFadeIn";
static const char* FISHINGSPOT_BUBBLE_ANIM = "Bubbles";
static const char* FISHINGSPOT_FADEOUT_ANIM = "BubblesFadeOut";
static const char* SHARKSPOT_FADEIN_ANIM = "Surface";
static const char* SHARKSPOT_BUBBLE_ANIM = "Idle";
static const char* SHARKSPOT_FADEOUT_ANIM = "Submerge";

//fishing line
static const int FISHINGLINE_THICKNESS = 2;
static const float FISHINGLINE_TIME = 0.35f;
static const float FISHINGLINE_TIME_DIST = 300.f; //how far line should move in line time above

//menu
static const float FADE_TIME = 2.f;

//shark
static const int SHARK_SIZE_X = 4;
static const int SHARK_SIZE_Y = 12;
static const char* SHARK_IDLE_ANIM = "Idle";
static const int SHARK_SPEED = 250.f;
static const int MAX_SHARKS_ACTIVE = MAX_FISHING_SPOTS;
static const int SHARK_COL_CIRCLES = 1;
static const float SHARK_COL_RAD = SHARK_SIZE_X * 0.33f * TILE_SIZE;

//clouds
static const float LARGE_CLOUD_SPEED = 0.33f;
static const float MEDIUM_CLOUD_SPEED = 0.22f;
static const float SMALL_CLOUD_SPEED = 0.11f;
static const float CLOUD_SPAWN_DELAY = 10.f;
static const int LARGE_CLOUD_PROBILITY = 90;
static const int MEDIUM_CLOUD_PROBILITY = 60;

//ui
static const int UI_X = SCREEN_WIDTH / 96;
static const int UI_Y = SCREEN_HEIGHT / 47;
static const char* UI_SCORE_PREFIX = "Score: ";
static const char* UI_TIMER_PREFIX = "Timer: ";
static const char* UI_QTE_TIMER_SUFFIX = "!";
static const int UI_FONT_SIZE = 25;
static const int UI_SCORE_X_OFFS = -TILE_SIZE;
static const int UI_SCORE_Y_OFFS = -TILE_SIZE * PLAYER_SIZE_Y * 1.5f;

//palmtree
static const char* PALMTREE_IDLE_ANIM = "Idle";

//dev
static const int GOD_MODE = false;