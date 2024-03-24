#pragma once

// ================================================================================
// Main H-file
// ================================================================================

#include <Arduino.h>
#include <FastLED.h>
//#include <TimerMS.h>

#include "pattern_resizement.h"
#include "main_matrix_functions.h"



#define LED_PIN A6
#define LED_SIDE_COUNT 14
#define LED_BUFFER_SIZE 1
#define LED_COUNT (LED_SIDE_COUNT*LED_SIDE_COUNT)
#define PATTERN_COUNT 6
#define TIME_REGIME_COUNT 3
#define EXERCISE_DATAPACKAGE_SIZE 131
/*
#define BUTTON_SELECT_PIN = 3
#define BUTTON_ITERATE_PIN = 2
#define BUTTON_RESET_PIN = 1
*/

inline int VECTOR_SNAKE_UPDOWN[][2] { {5,1}, {4,0}, {3,1}, {2,0}, {1,1}, {0,0}, {0,1}, {1,0}, {2,1}, {3,0}, {4,1}, {5,0} };
inline int VECTOR_SNAKE_LEFTRIGHT[][2] { {1,5}, {0,4}, {1,3}, {0,2}, {1,1}, {0,0}, {1,0}, {0,1}, {1,2}, {0,3}, {1,4}, {0,5} };
inline int VECTOR_SQUARE[][2] { {0,0}, {0,1}, {0,2}, {0,3}, {1,3}, {2,3}, {3,3}, {3,2}, {3,1}, {3,0}, {2,0}, {1,0} };
inline int VECTOR_EIGHT[][2]{ {3,1}, {4,0}, {5,0}, {6,1}, {6,2}, {5,3}, {4,3}, {3,2}, {3,1}, {2,0}, {1,0}, {0,1}, {0,2}, {1,3}, {2,3}, {3,2} };
inline int VECTOR_ARROW_UPDOWN[][2]{ {0,0}, {1,0}, {2,0}, {3,0}, {4,0}, {5,0}, {6,0}, {7,0}, {6,0}, {5,0}, {4,0}, {3,0}, {2,0}, {1,0} };
inline int VECTOR_ARROW_LEFTRIGHT[][2]{ {0,0}, {0,1}, {0,2}, {0,3}, {0,4}, {0,5}, {0,6}, {0,7}, {0,6}, {0,5}, {0,4}, {0,3}, {0,2}, {0,1} };

inline bool PATTERNS_NOARROW[PATTERN_COUNT + 1];
inline bool PATTERNS_MIRROR[PATTERN_COUNT + 1];
inline bool PATTERNS_MIRROR_AXIS[PATTERN_COUNT + 1];
inline int PATTERNS_VECTORS_RESIZED[PATTERN_COUNT + 1][128][2];
inline int PATTERNS_VECTOR_SIZES[PATTERN_COUNT + 1];

inline long pattern_tick = 0, last_serial_time = 0;
inline int matrix_regime = 3, matrix_time_regime = 1;
inline bool matrix_reset_button_pressed, matrix_select_button_pressed, matrix_time_button_pressed, matrix_enabled = true;

inline uint8_t exercise_datapackage[EXERCISE_DATAPACKAGE_SIZE];
inline CRGB leds[LED_COUNT];


void presetupPatterns();
void setupPatterns();
void cleanNewPattern();
void setupNewPattern();