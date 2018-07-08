#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stddef.h>
#include "err.h"

#define UTILS_MAX(x, y) (((x) > (y)) ? (x) : (y))
#define UTILS_MIN(x, y) (((x) < (y)) ? (x) : (y))
#define UTILS_SAT(min, x, max) UTILS_MIN(max, UTILS_MAX(x, min))

#endif