#pragma once

// ================================================================================
// Pattern resizement H-file
// ================================================================================

#include "fake_math.h"
#include "main.h"

bool checkIfArrayHasCoords(int y, int x, int array[][2], int size);

void addToCoordsArray(int array[][2], int& size, int y, int x);
void addOffsetToVector(int array[][2], int patternN);
void convertPatternSize(int array[][2], int size, int patternN);