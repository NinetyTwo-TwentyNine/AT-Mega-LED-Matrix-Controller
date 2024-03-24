
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

void addToCoordsArray(int array[][2], int& size, int y, int x)
{
	array[size][0] = y;
	array[size][1] = x;
	size++;
}

void addOffsetToVector(int array[][2], int patternN)
{
	int size = PATTERNS_VECTOR_SIZES[patternN];
	bool shouldMirror = PATTERNS_MIRROR[patternN];

	int height = 0, width = 0;
	for (int i = 0; i < size; i++)
	{
		if (array[i][0] > height)
		{
			height = array[i][0];
		}
		if (array[i][1] > width)
		{
			width = array[i][1];
		}
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

	for (int i = 0; i < size; i++)
	{
		array[i][0] += offset_y;
		/*if (array[i][0] > LED_SIDE_COUNT - LED_BUFFER_SIZE)
		{
			array[i][0] = LED_SIDE_COUNT - LED_BUFFER_SIZE;
		}*/

		array[i][1] += offset_x;
		/*if (array[i][1] > LED_SIDE_COUNT - LED_BUFFER_SIZE)
		{
			array[i][1] = LED_SIDE_COUNT - LED_BUFFER_SIZE;
		}*/
	}
}

void convertPatternSize(int array[][2], int size, int patternN)
{
	int resizedArrSize = 0;

	int height = 0, width = 0;
	for (int i = 0; i < size; i++)
	{
		if (array[i][0] > height)
		{
			height = array[i][0];
		}
		if (array[i][1] > width)
		{
			width = array[i][1];
		}
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
		int new_y = math_round((float)array[i % size][0] * conversionSize), new_x = math_round((float)array[i % size][1] * conversionSize);

		if (i == 0)
		{
			addToCoordsArray(PATTERNS_VECTORS_RESIZED[patternN], resizedArrSize, new_y, new_x);
			continue;
		}
		if (conversionSize < 1)
		{
			if (!(PATTERNS_VECTORS_RESIZED[patternN][resizedArrSize - 1][0] == new_y && PATTERNS_VECTORS_RESIZED[patternN][resizedArrSize - 1][1] == new_x))
			{
				addToCoordsArray(PATTERNS_VECTORS_RESIZED[patternN], resizedArrSize, new_y, new_x);
			}
			continue;
		}

		int lastPoint = resizedArrSize - 1;
		int distance_y = new_y - PATTERNS_VECTORS_RESIZED[patternN][lastPoint][0], distance_x = new_x - PATTERNS_VECTORS_RESIZED[patternN][lastPoint][1];
		int additionalPointsAmount = math_getRoot(distance_x * distance_x + distance_y * distance_y);

		for (int j = 1; j < additionalPointsAmount; j++)
		{
			float percentage = (float)j / (float)additionalPointsAmount;
			int additional_y = PATTERNS_VECTORS_RESIZED[patternN][lastPoint][0] + math_round(percentage * (float)distance_y);
			int additional_x = PATTERNS_VECTORS_RESIZED[patternN][lastPoint][1] + math_round(percentage * (float)distance_x);

			if (j == size && (additional_x == new_x && additional_y == new_y)) { continue; }
			if (PATTERNS_VECTORS_RESIZED[patternN][resizedArrSize - 1][0] == additional_y && PATTERNS_VECTORS_RESIZED[patternN][resizedArrSize - 1][1] == additional_x) { continue; }
			addToCoordsArray(PATTERNS_VECTORS_RESIZED[patternN], resizedArrSize, additional_y, additional_x);
		}

		if (i == size || (PATTERNS_VECTORS_RESIZED[patternN][resizedArrSize - 1][0] == new_y && PATTERNS_VECTORS_RESIZED[patternN][resizedArrSize - 1][1] == new_x)) { continue; }
		addToCoordsArray(PATTERNS_VECTORS_RESIZED[patternN], resizedArrSize, new_y, new_x);
	}
	PATTERNS_VECTOR_SIZES[patternN] = resizedArrSize;

	addOffsetToVector(PATTERNS_VECTORS_RESIZED[patternN], patternN);
}