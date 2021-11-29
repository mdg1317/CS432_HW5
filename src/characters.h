//
//  characters.h
//
//  Created by Vegdahl, Steve on 1/11/17.
//  Copyright (c) 2017 Steven R. Vegdahl. All rights reserved.
//

#ifndef __characters__
#define __characters__
#include "cs432.h"
#include "vec.h"

ObjRef genCharacter(char theLetter, vec4 theColor, double width, double z, GLfloat theShininess,
                    int *idxVar, vec4 *pointsArray, vec3 *normalsArray, vec4 *color1Array,
                    vec4 *color2Array, vec4 *color3Array, GLfloat *shinyArray);

inline ObjRef genCharacter(char theLetter, vec4 theColor, double width, double z,
                           int *idxVar, vec4 *pointsArray, vec4 *colorArray) {
    return genCharacter(theLetter, theColor, width, z, 0,
                        idxVar, pointsArray, NULL, colorArray,
                        NULL, NULL, NULL);
}

#endif /* defined(__characters__) */
