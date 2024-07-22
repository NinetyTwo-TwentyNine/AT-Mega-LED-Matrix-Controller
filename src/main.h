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
#define LED_SIDE_COUNT 16
#define LED_BUFFER_SIZE 1
#define LED_COUNT (LED_SIDE_COUNT*LED_SIDE_COUNT)
#define PATTERN_COUNT 6
#define TIME_REGIME_COUNT 3
#define EXERCISE_SECURITY_CODE_LENGTH 10
#define EXERCISE_DATAPACKAGE_SIZE (EXERCISE_SECURITY_CODE_LENGTH*2 + 128 + 3)
/*
#define BUTTON_SELECT_PIN = 3
#define BUTTON_ITERATE_PIN = 2
#define BUTTON_RESET_PIN = 1
*/

const PROGMEM uint8_t VECTOR_DEFAULT_PATTERNS_SIZES[] = {12, 12, 12, 16, 14, 14};

const PROGMEM uint8_t VECTOR_DEFAULT_PATTERNS[][16][2] = {
    // { {5,1}, {4,0}, {3,1}, {2,0}, {1,1}, {0,0}, {0,1}, {1,0}, {2,1}, {3,0}, {4,1}, {5,0} },
    // { {1,5}, {0,4}, {1,3}, {0,2}, {1,1}, {0,0}, {1,0}, {0,1}, {1,2}, {0,3}, {1,4}, {0,5} },
    { {0,1}, {1,0}, {2,0}, {3,1}, {3,2}, {2,3}, {1,3}, {0,2} },
	{ {0,0}, {1,0}, {2,0}, {1,1}, {0,2}, {1,2}, {2,2}, {1,1} },
    { {0,0}, {0,1}, {0,2}, {0,3}, {1,3}, {2,3}, {3,3}, {3,2}, {3,1}, {3,0}, {2,0}, {1,0} },
    { {3,1}, {4,0}, {5,0}, {6,1}, {6,2}, {5,3}, {4,3}, {3,2}, {3,1}, {2,0}, {1,0}, {0,1}, {0,2}, {1,3}, {2,3}, {3,2} },
    { {0,0}, {1,0}, {2,0}, {3,0}, {4,0}, {5,0}, {6,0}, {7,0}, {6,0}, {5,0}, {4,0}, {3,0}, {2,0}, {1,0} },
    { {0,0}, {0,1}, {0,2}, {0,3}, {0,4}, {0,5}, {0,6}, {0,7}, {0,6}, {0,5}, {0,4}, {0,3}, {0,2}, {0,1} }
};

inline bool PATTERNS_NOARROW[PATTERN_COUNT + 1] = {true, true, false, false, false, false};
inline bool PATTERNS_MIRROR[PATTERN_COUNT + 1] = {false, false, false, false, false, false};
inline bool PATTERNS_MIRROR_AXIS[PATTERN_COUNT + 1] = {false, false, false, false, false, false};

inline int PATTERN_VECTOR_RESIZED[128][2];
inline int PATTERN_VECTOR_SIZE = 0;

inline int SECURITY_CODE_CONDENSED = 470;

inline long pattern_tick = 0, last_serial_time = 0, last_matrix_time = 0;
inline int matrix_regime = 2, matrix_time_regime = 1;
inline bool matrix_reset_button_pressed, matrix_enabled = true;

inline uint8_t exercise_datapackage[EXERCISE_DATAPACKAGE_SIZE];
inline CRGB leds[LED_COUNT];


void setupDefaultPattern();
void cleanNewPattern();
void setupNewPattern();