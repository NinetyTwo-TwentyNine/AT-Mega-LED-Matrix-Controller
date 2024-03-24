
// ================================================================================
// Main CPP-file
// ================================================================================

#include "main.h"


void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, LED_COUNT);

  DDRC &= ~((1<<1)|(1<<2)|(1<<3));
  PORTC |= (1<<1)|(1<<2)|(1<<3);

  bool reset_button_pressed = (~PINC&(1<<1));
  matrix_reset_button_pressed = reset_button_pressed;

  matrix_select_button_pressed = (~PINC&(1<<2));
  matrix_time_button_pressed = (~PINC&(1<<3));

  presetupPatterns();
  setupPatterns();
}

void loop()
{
  if (Serial1.available() && millis() >= (last_serial_time + 1000))
  {
    last_serial_time = millis();
    
    cleanNewPattern();
    Serial1.readBytes(exercise_datapackage, EXERCISE_DATAPACKAGE_SIZE);
	  setupNewPattern();

    delay(100);
    Serial1.flush();
	  return;
  }

  // ===== MAIN LOOP FUNCTION =====

  /*
  bool reset_button_pressed = (~PINC&(1<<1)), select_button_pressed = (~PINC&(1<<2)), time_button_pressed = (~PINC&(1<<3));

  if (matrix_reset_button_pressed != reset_button_pressed)
  {
    resetMatrix();
  }
  matrix_reset_button_pressed = reset_button_pressed;

  if (select_button_pressed)
  {
	  if (!matrix_select_button_pressed)
	  {
		  matrix_regime = (matrix_regime + 1) % PATTERN_COUNT;
    	pattern_tick = 0;
	  }
  }
  matrix_select_button_pressed = select_button_pressed;

  if (time_button_pressed)
  {
	  if (!matrix_time_button_pressed)
	  {
    	matrix_time_regime = (matrix_time_regime + 1) % TIME_REGIME_COUNT;
	  }
  }
  matrix_time_button_pressed = time_button_pressed;
  */

  if (!matrix_enabled) { return; }
  playPattern(PATTERNS_VECTORS_RESIZED[matrix_regime], matrix_regime);
  int delayTime = 15 * (matrix_time_regime + 1);
  delay(delayTime);
}


void presetupPatterns()
{
	for (int i = 0; i < PATTERN_COUNT; i++)
	{
		PATTERNS_NOARROW[i] = false;
		PATTERNS_MIRROR[i] = false;
	}
	PATTERNS_NOARROW[0] = true;
	PATTERNS_NOARROW[1] = true;
}

void setupPatterns()
{
	convertPatternSize(VECTOR_SNAKE_UPDOWN, sizeof(VECTOR_SNAKE_UPDOWN) / sizeof(VECTOR_SNAKE_UPDOWN[0]), 0);
	convertPatternSize(VECTOR_SNAKE_LEFTRIGHT, sizeof(VECTOR_SNAKE_LEFTRIGHT) / sizeof(VECTOR_SNAKE_LEFTRIGHT[0]), 1);
	convertPatternSize(VECTOR_SQUARE, sizeof(VECTOR_SQUARE) / sizeof(VECTOR_SQUARE[0]), 2);
	convertPatternSize(VECTOR_EIGHT, sizeof(VECTOR_EIGHT) / sizeof(VECTOR_EIGHT[0]), 3);
	convertPatternSize(VECTOR_ARROW_UPDOWN, sizeof(VECTOR_ARROW_UPDOWN) / sizeof(VECTOR_ARROW_UPDOWN[0]), 4);
	convertPatternSize(VECTOR_ARROW_LEFTRIGHT, sizeof(VECTOR_ARROW_LEFTRIGHT) / sizeof(VECTOR_ARROW_LEFTRIGHT[0]), 5);
}

void cleanNewPattern()
{
  for (int i = 0; i < EXERCISE_DATAPACKAGE_SIZE; i++)
  {
    exercise_datapackage[i] = 0;
  }

  PATTERNS_MIRROR[PATTERN_COUNT] = false;
  PATTERNS_MIRROR_AXIS[PATTERN_COUNT] = false;
  PATTERNS_NOARROW[PATTERN_COUNT] = false;
  for (int i = 0; i < 128; i++)
  {
    PATTERNS_VECTORS_RESIZED[PATTERN_COUNT][i][0] = 0;
    PATTERNS_VECTORS_RESIZED[PATTERN_COUNT][i][1] = 0;
  }
  PATTERNS_VECTOR_SIZES[PATTERN_COUNT] = 0;
}

void setupNewPattern()
{
	int exercise_length = exercise_datapackage[0];
	int VECTOR_NEW_EXERCISE[64][2];
	for (int i = 0; i < exercise_length; i++)
	{
		VECTOR_NEW_EXERCISE[i][0] = exercise_datapackage[i * 2 + 1];
		VECTOR_NEW_EXERCISE[i][1] = exercise_datapackage[i * 2 + 2];
	}
	PATTERNS_MIRROR[PATTERN_COUNT] = (exercise_datapackage[129] != 0);
	//PATTERNS_MIRROR_AXIS[PATTERN_COUNT] = (exercise_datapackage[129] == 2);
	PATTERNS_NOARROW[PATTERN_COUNT] = (exercise_datapackage[130] != 1);

	convertPatternSize(VECTOR_NEW_EXERCISE, exercise_length, PATTERN_COUNT);
	matrix_regime = PATTERN_COUNT;
  pattern_tick = 0;
}