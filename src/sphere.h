/*
 *  sphere.h
 *  ioTest
 *
 *  Created by vegdahl on 8/19/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "cs432.h"
#include "vec.h"

extern ObjRef genSphere(vec4 color, int n, int* idxSpot,
				 vec4* pointArray, vec4* colorArray,
						vec3* normalArray);
extern ObjRef genFlatSphere(vec4 color, int n, int* idxSpot,
							vec4* pointArray, vec4* colorArray,
							vec3* normalArray);


#endif
