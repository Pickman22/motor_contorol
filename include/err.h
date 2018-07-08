#ifndef ERR_H
#define ERR_H

typedef enum ErrCode {
	E_OK = 0,
	E_NOK,
	E_NULL_PTR,
	E_INV_ARG,
	E_INV_SIZE,
	E_TOO_LARGE,
	E_TOO_SMALL,
	E_OUT_OF_RANGE,
	E_BUFFER_FULL,
	E_NOT_IMPLEMENTED,
	E_NOT_FOUND,
	E_INTERNAL,
	E_GENERIC,
}  ErrCode_e;

#endif