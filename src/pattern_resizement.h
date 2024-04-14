#pragma once

// ================================================================================
// Pattern resizement H-file
// ================================================================================

#include "fake_math.h"
#include "main.h"

bool checkIfArrayHasCoords(int y, int x, int array[][2], int size);

void clearCoordsArray();
void addToCoordsArray(int y, int x);
void addOffsetToVector(int patternN);
void convertPatternSize(uint8_t array[][2], int size);