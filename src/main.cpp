
// ================================================================================
// Main CPP-file
// ================================================================================

#include "main.h"


void setup()
{
  Serial.begin(9600);

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, LED_COUNT);

  DDRC &= ~((1<<1)|(1<<2)|(1<<3));
  PORTC |= (1<<1)|(1<<2)|(1<<3);

  bool reset_button_pressed = (~PINC&(1<<1));
  matrix_reset_button_pressed = reset_button_pressed;

  matrix_select_button_pressed = (~PINC&(1<<2));
  matrix_time_button_pressed = (~PINC&(1<<3));

  setupDefaultPattern();
}

void loop()
{
  if (Serial.available() && millis() >= (last_serial_time + 1000))
  {
    last_serial_time = millis();
    
    cleanNewPattern();
    Serial.readBytes(exercise_datapackage, EXERCISE_DATAPACKAGE_SIZE);
	  setupNewPattern();

    delay(100);
    Serial.flush();
	  return;
  }

  // ===== MAIN LOOP FUNCTION =====

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
      setupDefaultPattern();
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

  if (!matrix_enabled) { return; }
  playPattern(matrix_regime);
  int delayTime = 15 * (matrix_time_regime + 1);
  delay(delayTime);
}

void setupDefaultPattern()
{
	uint8_t VECTOR_CURR_EXERCISE[32][2];
  uint8_t size = pgm_read_byte(&(VECTOR_DEFAULT_PATTERNS_SIZES[matrix_regime]));
  Serial.println(size);
  for (int i = 0; i < size; i++)
  {
    VECTOR_CURR_EXERCISE[i][0] = pgm_read_byte(&(VECTOR_DEFAULT_PATTERNS[matrix_regime][i][0]));
    VECTOR_CURR_EXERCISE[i][1] = pgm_read_byte(&(VECTOR_DEFAULT_PATTERNS[matrix_regime][i][1]));
  }
  convertPatternSize(VECTOR_CURR_EXERCISE, size);
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
}

void setupNewPattern()
{
	int exercise_length = exercise_datapackage[0];
	uint8_t VECTOR_NEW_EXERCISE[32][2];
	for (int i = 0; i < exercise_length; i++)
	{
		VECTOR_NEW_EXERCISE[i][0] = exercise_datapackage[i * 2 + 1];
		VECTOR_NEW_EXERCISE[i][1] = exercise_datapackage[i * 2 + 2];
	}
	PATTERNS_MIRROR[PATTERN_COUNT] = (exercise_datapackage[129] != 0);
	//PATTERNS_MIRROR_AXIS[PATTERN_COUNT] = (exercise_datapackage[129] == 2);
	PATTERNS_NOARROW[PATTERN_COUNT] = (exercise_datapackage[130] != 1);

	convertPatternSize(VECTOR_NEW_EXERCISE, exercise_length);
	matrix_regime = PATTERN_COUNT;
  pattern_tick = 0;
}