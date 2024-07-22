
// ================================================================================
// Main CPP-file
// ================================================================================

#include "main.h"

ISR(INT2_vect) {
  matrix_time_regime = (matrix_time_regime + 1) % TIME_REGIME_COUNT;
}

ISR(INT1_vect) {
	matrix_regime = (matrix_regime + 1) % PATTERN_COUNT;
  setupDefaultPattern();
  pattern_tick = 0;
}

void setup()
{
  Serial.begin(9600);

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, LED_COUNT);

  DDRD &= ~((1<<0)|(1<<1)|(1<<2));
  PORTD |= ((1<<0)|(1<<1)|(1<<2));

  EIMSK |= (1<<INT1) | (1<<INT2);
  EICRA |= (1<<ISC11 | 1<<ISC21);
  sei();

  bool reset_button_pressed = (~PIND&(1<<0));
  matrix_reset_button_pressed = reset_button_pressed;

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

  int delayTime = 15 * (matrix_time_regime + 1);
  if (millis() < last_matrix_time + delayTime) { return; }

  last_matrix_time = millis();

  bool reset_button_pressed = (~PIND&(1<<0));

  if (matrix_reset_button_pressed != reset_button_pressed)
  {
    resetMatrix();
  }
  matrix_reset_button_pressed = reset_button_pressed;

  if (!matrix_enabled) { return; }
  playPattern(matrix_regime);
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
}

void setupNewPattern()
{
  int security_code_duplicate = SECURITY_CODE_CONDENSED;
  for (int i = 0; i < EXERCISE_SECURITY_CODE_LENGTH; i++) {
    if (security_code_duplicate % 2 != exercise_datapackage[i]) {
      return;
    }
    security_code_duplicate /= 2;
  }

  security_code_duplicate = SECURITY_CODE_CONDENSED;
  for (int i = 0; i < EXERCISE_SECURITY_CODE_LENGTH; i++) {
    if (security_code_duplicate % 2 != exercise_datapackage[EXERCISE_DATAPACKAGE_SIZE - i - 1]) {
      return;
    }
    security_code_duplicate /= 2;
  }
  

	int exercise_length = exercise_datapackage[EXERCISE_SECURITY_CODE_LENGTH];
	uint8_t VECTOR_NEW_EXERCISE[32][2];
	for (int i = 0; i < exercise_length; i++)
	{
		VECTOR_NEW_EXERCISE[i][0] = exercise_datapackage[i * 2 + EXERCISE_SECURITY_CODE_LENGTH + 1];
		VECTOR_NEW_EXERCISE[i][1] = exercise_datapackage[i * 2 + EXERCISE_SECURITY_CODE_LENGTH + 2];
	}
	PATTERNS_MIRROR[PATTERN_COUNT] = (exercise_datapackage[EXERCISE_DATAPACKAGE_SIZE - EXERCISE_SECURITY_CODE_LENGTH - 2] != 0);
	PATTERNS_NOARROW[PATTERN_COUNT] = (exercise_datapackage[EXERCISE_DATAPACKAGE_SIZE - EXERCISE_SECURITY_CODE_LENGTH - 1] != 1);

	convertPatternSize(VECTOR_NEW_EXERCISE, exercise_length);
	matrix_regime = PATTERN_COUNT;
  pattern_tick = 0;
}