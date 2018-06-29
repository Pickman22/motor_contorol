#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stddef.h>
#include "err.h"


#define UTILS_MAX(x, y) (((x) > (y)) ? (x) : (y))
#define UTILS_MIN(x, y) (((x) < (y)) ? (x) : (y))

ErrCode_e tobytes(void* data, size_t datasz, void* buf, size_t bufsz);

#endif