
// ================================================================================
// Main matrix functions CPP-file
// ================================================================================

#include "main_matrix_functions.h"

int getPosThroughCoords(int pos_y, int pos_x)
{
  int pos;
  if (pos_y % 2 != 0)
  {
    pos = pos_x + (pos_y - 1)*LED_SIDE_COUNT;
  }
  else
  {
    pos = (LED_SIDE_COUNT + 1 - pos_x) + (pos_y - 1)*LED_SIDE_COUNT;
  }
  pos--;
  return pos;
}

void resetMatrix()
{
    matrix_reset_button_pressed = !matrix_reset_button_pressed;
    matrix_enabled = !matrix_enabled;
    if (!matrix_enabled)
    {
      pattern_tick = 0;
      for (int i = 0; i < LED_COUNT; i++)
      {
        leds[i] = CRGB::Black;
      }
      FastLED.show();
    }
}

void playPattern(int array[][2], int patternN)
{
  int size = PATTERNS_VECTOR_SIZES[patternN];

  int num = pattern_tick % (size*2);
  for (int i = 0; i < LED_COUNT; i++)
  {
    leds[i] = CRGB::Black;
  }

  int pos = num/2;
  int pos_y = array[pos][0], pos_x = array[pos][1];
  leds[getPosThroughCoords(pos_y, pos_x)] = CRGB::Green;
 
  if (pattern_tick != 0)
  {
  	if (pos > 0) { pos--; } else { pos = size - 1; }
  	int prev_pos_y = array[pos][0], prev_pos_x = array[pos][1];

	if (!PATTERNS_NOARROW[patternN])
	{
		if (prev_pos_y == pos_y && prev_pos_x != pos_x)
		{
  			leds[getPosThroughCoords(prev_pos_y + 1, prev_pos_x)] = CRGB::Green;
  			leds[getPosThroughCoords(prev_pos_y - 1, prev_pos_x)] = CRGB::Green;
		}
		else if (prev_pos_y != pos_y && prev_pos_x == pos_x)
		{
  			leds[getPosThroughCoords(prev_pos_y, prev_pos_x + 1)] = CRGB::Green;
  			leds[getPosThroughCoords(prev_pos_y, prev_pos_x - 1)] = CRGB::Green;
		}
		else
		{
  			leds[getPosThroughCoords(pos_y, prev_pos_x)] = CRGB::Green;
  			leds[getPosThroughCoords(prev_pos_y, pos_x)] = CRGB::Green;
		}
	}
	else
	{
		leds[getPosThroughCoords(prev_pos_y, prev_pos_x)] = CRGB::Green;
	}
  }

  if (PATTERNS_MIRROR[patternN])
  {
  	if (PATTERNS_MIRROR_AXIS[patternN])
	{
		for (int i = 1; i <= LED_SIDE_COUNT/2; i++)
			for (int j = 1; j <= LED_SIDE_COUNT; j++)
			{
				if (leds[getPosThroughCoords(i, j)] != CRGB::Black)
				{
					leds[getPosThroughCoords(LED_SIDE_COUNT - i + 1, j)] = CRGB::Green;
				}
			}
	}
	else
	{
		for (int i = 1; i <= LED_SIDE_COUNT; i++)
			for (int j = 1; j <= LED_SIDE_COUNT/2; j++)
			{
				if (leds[getPosThroughCoords(i, j)] != CRGB::Black)
				{
					leds[getPosThroughCoords(i, LED_SIDE_COUNT - j + 1)] = CRGB::Green;
				}
			}
	}
  }

  FastLED.show();
  pattern_tick++;
}