#ifndef __TEAPOT_H__
#define __TEAPOT_H__

#include "vec.h"
typedef vec4 point4;
typedef vec4 color4;

extern ObjRef genHalfTeapot(color4 color, int n, int* idxSpot,
						point4* pointArray, color4* colorArray);

#endif
