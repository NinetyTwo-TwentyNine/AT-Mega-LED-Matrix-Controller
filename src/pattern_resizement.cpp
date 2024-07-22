
// ================================================================================
// Pattern resizement CPP-file
// ================================================================================

#include "pattern_resizement.h"


bool checkIfArrayHasCoords(int y, int x, int array[][2], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (array[i][0] == y && array[i][1] == x)
		{
			return true;
		}
	}
	return false;
}

void clearCoordsArray()
{
	for (int i = 0; i < PATTERN_VECTOR_SIZE; i++) {
		PATTERN_VECTOR_RESIZED[i][0] = 0;
		PATTERN_VECTOR_RESIZED[i][1] = 0;
	}
	PATTERN_VECTOR_SIZE = 0;
}

void addToCoordsArray(int y, int x)
{
	PATTERN_VECTOR_RESIZED[PATTERN_VECTOR_SIZE][0] = y;
	PATTERN_VECTOR_RESIZED[PATTERN_VECTOR_SIZE][1] = x;
	PATTERN_VECTOR_SIZE++;
}

void addOffsetToVector(int patternN)
{
	bool shouldMirror = PATTERNS_MIRROR[patternN];

	int height = 0, width = 0;
	for (int i = 0; i < PATTERN_VECTOR_SIZE; i++)
	{
		height = max(height, PATTERN_VECTOR_RESIZED[i][0]);
		width = max(width, PATTERN_VECTOR_RESIZED[i][1]);
	}
	height++; width++;

	int offset_x = (LED_SIDE_COUNT - width) / 2 + 1, offset_y = (LED_SIDE_COUNT - height) / 2 + 1;
	shouldMirror = shouldMirror && (height != width);
	if (shouldMirror)
	{
		bool mirror_vertical = (height < width);
		if (mirror_vertical)
		{
			offset_y = LED_SIDE_COUNT / 2 - height + 1;
		}
		else
		{
			offset_x = LED_SIDE_COUNT / 2 - width + 1;
		}
		PATTERNS_MIRROR_AXIS[patternN] = mirror_vertical;
	}
	PATTERNS_MIRROR[patternN] = shouldMirror;

	for (int i = 0; i < PATTERN_VECTOR_SIZE; i++)
	{
		PATTERN_VECTOR_RESIZED[i][0] += offset_y;
		// PATTERN_VECTOR_RESIZED[i][0] = max(0, PATTERN_VECTOR_RESIZED[i][0]);
		// PATTERN_VECTOR_RESIZED[i][0] = min(PATTERN_VECTOR_RESIZED[i][0], LED_SIDE_COUNT - LED_BUFFER_SIZE);

		PATTERN_VECTOR_RESIZED[i][1] += offset_x;
		// PATTERN_VECTOR_RESIZED[i][1] = max(0, PATTERN_VECTOR_RESIZED[i][1]);
		// PATTERN_VECTOR_RESIZED[i][1] = min(PATTERN_VECTOR_RESIZED[i][1], LED_SIDE_COUNT - LED_BUFFER_SIZE);
	}
}

void convertPatternSize(uint8_t array[][2], int size)
{
	clearCoordsArray();

	int height = 0, width = 0;
	for (int i = 0; i < size; i++)
	{
		height = max(height, array[i][0]);
		width = max(width, array[i][1]);
	}
	height++; width++;

	float conversionSize;
	if (height >= width)
	{
		conversionSize = (float)(LED_SIDE_COUNT - LED_BUFFER_SIZE*2 - 1) / (float)(height-1);
	}
	else
	{
		conversionSize = (float)(LED_SIDE_COUNT - LED_BUFFER_SIZE*2 - 1) / (float)(width-1);
	}

	for (int i = 0; i <= size; i++)
	{
		uint8_t old_x = array[i % size][1], old_y = array[i % size][0];
		int new_y = math_round((float)old_y * conversionSize), new_x = math_round((float)old_x * conversionSize);

		if (i == 0)
		{
			addToCoordsArray(new_y, new_x);
			continue;
		}
		if (conversionSize < 1)
		{
			if (!(PATTERN_VECTOR_RESIZED[PATTERN_VECTOR_SIZE - 1][0] == new_y && PATTERN_VECTOR_RESIZED[PATTERN_VECTOR_SIZE - 1][1] == new_x))
			{
				addToCoordsArray(new_y, new_x);
			}
			continue;
		}

		int lastPoint = PATTERN_VECTOR_SIZE - 1;
		int distance_y = new_y - PATTERN_VECTOR_RESIZED[lastPoint][0], distance_x = new_x - PATTERN_VECTOR_RESIZED[lastPoint][1];
		int additionalPointsAmount = math_getRoot(distance_x * distance_x + distance_y * distance_y);

		for (int j = 1; j < additionalPointsAmount; j++)
		{
			float percentage = (float)j / (float)additionalPointsAmount;
			
			int additional_y = PATTERN_VECTOR_RESIZED[lastPoint][0] + math_round(percentage * (float)distance_y);
			additional_y = max(0, additional_y);
			additional_y = min(additional_y, LED_SIDE_COUNT - LED_BUFFER_SIZE * 2 - 1);

			int additional_x = PATTERN_VECTOR_RESIZED[lastPoint][1] + math_round(percentage * (float)distance_x);
			additional_x = max(0, additional_x);
			additional_x = min(additional_x, LED_SIDE_COUNT - LED_BUFFER_SIZE * 2 - 1);

			if (PATTERN_VECTOR_RESIZED[PATTERN_VECTOR_SIZE - 1][0] == additional_y && PATTERN_VECTOR_RESIZED[PATTERN_VECTOR_SIZE - 1][1] == additional_x) { continue; }
			if (j >= 2) {
				if ( (PATTERN_VECTOR_RESIZED[PATTERN_VECTOR_SIZE - 2][0] - additional_y) % 2 != 0 && (PATTERN_VECTOR_RESIZED[PATTERN_VECTOR_SIZE - 2][1] - additional_x) % 2 != 0 &&
					// ( (PATTERN_VECTOR_RESIZED[PATTERN_VECTOR_SIZE - 1][0] == additional_y) != (PATTERN_VECTOR_RESIZED[PATTERN_VECTOR_SIZE - 1][1] == additional_x)) && ((PATTERN_VECTOR_RESIZED[PATTERN_VECTOR_SIZE - 1][0] == PATTERN_VECTOR_RESIZED[PATTERN_VECTOR_SIZE - 2][0]) != (PATTERN_VECTOR_RESIZED[PATTERN_VECTOR_SIZE - 1][1] == PATTERN_VECTOR_RESIZED[PATTERN_VECTOR_SIZE - 2][1]) ) )
					( (PATTERN_VECTOR_RESIZED[PATTERN_VECTOR_SIZE - 1][0] == additional_y) && (PATTERN_VECTOR_RESIZED[PATTERN_VECTOR_SIZE - 1][1] == PATTERN_VECTOR_RESIZED[PATTERN_VECTOR_SIZE - 2][1]) || (PATTERN_VECTOR_RESIZED[PATTERN_VECTOR_SIZE - 1][0] == PATTERN_VECTOR_RESIZED[PATTERN_VECTOR_SIZE - 2][0]) && (PATTERN_VECTOR_RESIZED[PATTERN_VECTOR_SIZE - 1][1] == additional_x) ) )
				{
					PATTERN_VECTOR_RESIZED[PATTERN_VECTOR_SIZE - 1][0] = additional_y;
					PATTERN_VECTOR_RESIZED[PATTERN_VECTOR_SIZE - 1][1] = additional_x;
					continue;
				}
			}
			if (additional_x == new_x && additional_y == new_y) { continue; }
			addToCoordsArray(additional_y, additional_x);
		}

		if (i == size || (PATTERN_VECTOR_RESIZED[PATTERN_VECTOR_SIZE - 1][0] == new_y && PATTERN_VECTOR_RESIZED[PATTERN_VECTOR_SIZE - 1][1] == new_x)) { continue; }
		addToCoordsArray(new_y, new_x);
	}

	addOffsetToVector(matrix_regime);
}