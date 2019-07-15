#pragma once

#include <tchar.h>
#include <stdio.h>
#include <assert.h>

#define _USE_MATH_DEFINES	// M_PI���g������
#include <math.h>

#include <cstddef>

#define DEG2RAD(deg) (((deg)/360)*2*M_PI)
#define RAD2DEG(rad) (((rad)/2/M_PI)*360)

// ���U���g
enum RESULT {
	RESULT_OK = 0,						// �G���[��
	RESULT_ERROR = -1,					// �G���[����

	RESULT_NOTIMPLEMENTED	= -999999,	// ������API
};
