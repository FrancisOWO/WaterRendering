#ifndef MISC_H
#define MISC_H

#include <vector>
#include <string>
#include <stdlib.h>
#include <math.h>

#include "complex.h"

char* loadFile(const char *filename);

std::string pad(int x);
std::string number_format(int x);

float uniformRandomVariable();
complex gaussianRandomVariable();

#endif