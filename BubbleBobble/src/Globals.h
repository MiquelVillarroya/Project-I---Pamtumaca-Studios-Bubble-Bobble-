#pragma once

//Displaying scale factor
#define GAME_SCALE_FACTOR	3.0f

//Function return values
enum class AppStatus { OK = 0, ERROR, QUIT };
enum class ObjectType { MUSHROOM, BANANA, CHERRY, ICE_CREAM, FLAM, CAKE, YELLOW_CANDY, SHOES };


//Main return values
#define EXIT_SUCCESS	0
#define EXIT_FAILURE	1

//Log messages
#define LOG(text, ...) log(__FILE__, __LINE__, text, __VA_ARGS__);
void log(const char file[], int line, const char* text, ...);

//Window size
#define WINDOW_WIDTH    256
#define WINDOW_HEIGHT   224

//Game constants
#define MARGIN_GUI_Y	16
#define TILE_SIZE		8
#define SHADOW_OFFSET_X	3	
#define SHADOW_OFFSET_Y 4	
#define LEVEL_WIDTH		32
#define LEVEL_HEIGHT	26

//Entities animation delay
#define ANIM_DELAY		8