#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include "vec.h"
#include "characters.h"

//
//  characters.cpp
//
//  Created by Vegdahl, Steve on 1/11/17, modified on 11/9/21.
//  Copyright (c) 2017-2021 Steven R. Vegdahl. All rights reserved.
//

using namespace std;

static const double LINE = 0.0;
static const double LINE_EXTEND = 1.0;
static const double ARC = 2.0;
static const double INVISIBLE = 3.0;

static const double CAP_LENGTH = 0.0001;

class Coord {
public:
    double coords[6];
    Coord(double a, double b, double c, double d, double e) {
        coords[0] = a;
        coords[1] = b;
        coords[2] = c;
        coords[3] = d;
        coords[4] = e;
        coords[5] = 0;
    }
    Coord(double a, double b, double c, double d, double e, double f) {
        coords[0] = a;
        coords[1] = b;
        coords[2] = c;
        coords[3] = d;
        coords[4] = e;
        coords[5] = f;
    }
};

class CoordList {
public:
    const Coord thisOne;
    const CoordList* next;
    CoordList(const Coord& c): thisOne(c) {
        next = NULL;
    }
    CoordList(const Coord& c1, const Coord& c2): thisOne(c1){
        next = new CoordList(c2);
    }
    CoordList(const Coord& c1, const Coord& c2, const Coord& c3): thisOne(c1) {
        next = new CoordList(c2, c3);
    }
    CoordList(const Coord& c1, const Coord& c2, const Coord& c3, const Coord& c4): thisOne(c1) {
        next = new CoordList(c2, c3, c4);
    }
    CoordList(const Coord& c1, const Coord& c2, const Coord& c3, const Coord& c4,
              const Coord& c5): thisOne(c1) {
        next = new CoordList(c2, c3, c4, c5);
    }
    CoordList(const Coord& c1, const Coord& c2, const Coord& c3, const Coord& c4,
              const Coord& c5, const Coord& c6): thisOne(c1) {
        next = new CoordList(c2, c3, c4, c5, c6);
    }
    CoordList(const Coord& c1, const Coord& c2, const Coord& c3, const Coord& c4,
              const Coord& c5, const Coord& c6, const Coord& c7): thisOne(c1) {
        next = new CoordList(c2, c3, c4, c5, c6, c7);
    }
    CoordList(const Coord& c1, const Coord& c2, const Coord& c3, const Coord& c4,
              const Coord& c5, const Coord& c6, const Coord& c7, const Coord& c8): thisOne(c1) {
        next = new CoordList(c2, c3, c4, c5, c6, c7, c8);
    }
};

static CoordList characterCoords[] = {
    // space character
    CoordList(
              Coord(INVISIBLE,0,-0.9001,0,-0.9)
              ),
    // '!' character
    CoordList(
              Coord(LINE_EXTEND,0,0.85,0,-0.45),
              Coord(LINE_EXTEND,0,-0.9,0,-0.9-CAP_LENGTH)
              ),
    // '"' character
    CoordList(
              Coord(LINE_EXTEND,-0.2,0.75,-0.2,0.60),
              Coord(LINE_EXTEND,0.2,0.75,0.2,0.60)
              ),
    // '#' character
    CoordList(
              Coord(LINE_EXTEND,-0.1,0.4,-0.4,-0.7),
              Coord(LINE_EXTEND,0.4,0.4,0.1,-0.7),
              Coord(LINE_EXTEND,-0.4,0.1,0.5,0.1),
              Coord(LINE_EXTEND,-0.5,-0.4,0.4,-0.4)
              ),
    // '$' character
    CoordList(
              Coord(LINE,-0.15,0.6,0.15,0.6),
              Coord(LINE,-0.15,-0.6,0.15,-0.6),
              Coord(LINE,-0.15,0,0.15,0),
              Coord(ARC,-0.15,0.3,0.3,0.25,0.75),
              Coord(ARC,0.15,-0.3,0.3,-0.25,0.25),
              Coord(ARC,0.15,0.3,0.3,0.1,0.25),
              Coord(ARC,-0.15,-0.3,0.3,0.6,0.75),
              Coord(LINE_EXTEND,0,0.8,0,-0.8)
              ),
    // '%' character
    CoordList(
              Coord(LINE_EXTEND,-0.55,-0.6,0.55,0.6),
              Coord(ARC,-0.2,0.4,0.2,0,1),
              Coord(ARC,0.2,-0.4,0.2,0,1)
              ),
    // '&' character
    CoordList(
              Coord(ARC,0,0.5,0.25,-0.125,0.6),
              Coord(ARC,-0.07564,-0.4231,0.35,0.375,0.875),
              Coord(LINE,-0.21,0.36,(-0.17678+0.7),(0.32322-0.95)),
              Coord(LINE,0.1791,0.3263,(0.17678-0.5),(0.32322-0.5)),
              Coord(LINE,0.174,-0.6725,(0.174+0.4),(-0.6725+0.4)),
              Coord(LINE,0.172,-0.6725,(0.174+0.4),(-0.6725+0.4))
              ),
    // "'" character
    CoordList(
              Coord(LINE_EXTEND,0.05,0.75,-0.05,0.60)
              ),
    // '(' character
    CoordList(
              Coord(ARC,0.8,0,1,0.35,0.65)
              ),
    // ')' character
    CoordList(
              Coord(ARC,-0.8,0,1,-0.15,0.15)
              ),
    // '*' character
    CoordList(
              Coord(LINE_EXTEND,0,0.1,0,-0.7),
              Coord(LINE_EXTEND,-0.3,-0.1,0.3,-0.5),
              Coord(LINE_EXTEND,0.3,-0.1,-0.3,-0.5)
              ),
    // '+' character
    CoordList(
              Coord(LINE_EXTEND,0,0.1,0,-0.7),
              Coord(LINE_EXTEND,-0.4,-0.3,0.4,-0.3)
              ),
    // ',' character
    CoordList(
              Coord(ARC,-0.2,-0.7,0.2,0.75,1)
              ),
    // '-' character
    CoordList(
              Coord(LINE_EXTEND,-0.4,-0.3,0.4,-0.3)
              ),
    // '.' character
    CoordList(
              Coord(LINE_EXTEND,0,-0.9,0,-0.9-CAP_LENGTH)
              ),
    // '/' character
    CoordList(
              Coord(LINE_EXTEND,-0.7,-1,0.7,1)
              ),
    // digit '0'
    CoordList(
              Coord(ARC,0,0.45,0.45,0,0.5),
              Coord(ARC,0,-0.55,0.45,0.5,1),
              Coord(LINE,-0.45,0.45,-0.45,-0.55),
              Coord(LINE,0.45,0.45,0.45,-0.55)
              ),
    // digit '1'
    CoordList(
              Coord(LINE_EXTEND,0,0.9,0,-1),
              Coord(LINE,0,0.9,-0.25,0.65),
              Coord(LINE_EXTEND,-0.3,-1,0.3,-1)
              ),
    // digit '2'
    CoordList(
              Coord(ARC,0,0.4,0.5,-0.125,0.5),
              Coord(LINE,-0.5,-1,0.36,0.05),
              Coord(LINE_EXTEND,-0.5,-1,0.5,-1)
              ),
    // digit '3'
    CoordList(
              Coord(ARC,0,0.4,0.45,-0.25,0.5),
              Coord(ARC,0,-0.5,0.45,-0.5,0.25),
              Coord(LINE,0,-0.05,-0.2,-0.05)
              ),
    // digit '4'
    CoordList(
              Coord(LINE_EXTEND,0.2,-1,0.2,0.9),
              Coord(LINE,0.2,0.9,-0.45,-0.4),
              Coord(LINE_EXTEND,-0.45,-0.4,0.45,-0.4)
              ),
    // digit '5'
    CoordList(
              Coord(ARC,-0.1,-0.45,0.55,-0.375,0.25),
              Coord(LINE,-0.1,0.1,-0.45,0.1),
              Coord(LINE_EXTEND,-0.45,0.1,-0.45,0.9),
              Coord(LINE_EXTEND,-0.45,0.9,0.4,0.9)
              ),
    // digit '6'
    CoordList(
              Coord(ARC,0,-0.5,0.5,0,1),
              Coord(LINE,-0.5,-0.5,-0.5,0.4),
              Coord(ARC,0,0.4,0.5,0.05,0.5)
              ),
    // digit '7'
    CoordList(
              Coord(LINE,-0.3,-1,0.45,0.9),
              Coord(LINE_EXTEND,0.45,0.9,-0.45,0.9)
              ),
    // digit '8'
    CoordList(
              Coord(ARC,0,0.4,0.45,0,1),
              Coord(ARC,0,-0.5,0.45,0,1)
              ),
    // digit '9'
    CoordList(
              Coord(ARC,0,0.4,0.5,0,1),
              Coord(LINE,0.5,0.5,0.5,-0.5),
              Coord(ARC,0,-0.5,0.5,-0.35,0)
              ),
    // ':' character
    CoordList(
              Coord(LINE_EXTEND,0,-0.9,0,-0.9-CAP_LENGTH),
              Coord(LINE_EXTEND,0,-0.5,0,-0.5-CAP_LENGTH)
              ),
    // ';' character
    CoordList(
              Coord(LINE_EXTEND,0,-0.4,0,-0.4-CAP_LENGTH),
              Coord(ARC,-0.2,-0.7,0.2,0.75,1)
              ),
    // '<' character
    CoordList(
              Coord(LINE_EXTEND,0.2,-0.6,-0.2,-0.2),
              Coord(LINE_EXTEND,0.2,0.2,-0.2,-0.2)
              ),
    // '=' character
    CoordList(
              Coord(LINE_EXTEND,-0.4,0,0.4,0),
              Coord(LINE_EXTEND,-0.4,-0.4,0.4,-0.4)
              ),
    // '>' character
    CoordList(
              Coord(LINE_EXTEND,-0.2,-0.6,0.2,-0.2),
              Coord(LINE_EXTEND,-0.2,0.2,0.2,-0.2)
              ),
    // '?' character
    CoordList(
              Coord(ARC,0,0.3,0.4,-0.25,0.5),
              Coord(LINE_EXTEND,0,-0.1,0,-0.5),
              Coord(LINE_EXTEND,0,-0.9,0,-0.9-CAP_LENGTH)
              ),
    // '@' character
    CoordList(
              Coord(ARC,0,-0.2,0.15,0,1),
              Coord(ARC,0,-0.2,0.5,0,0.8),
              Coord(ARC,0.3,-0.2,0.2,0.70,1),
              Coord(LINE,-0.05,-0.35,0.3,-0.39)
              ),
    // letter 'A'
    CoordList(
              Coord(LINE,-0.7,-1,0,1),
              Coord(LINE,0.7,-1,0,1),
              Coord(LINE,-0.42,-0.2,0.42,-0.2),
              Coord(LINE_EXTEND,0,.75,0,0.9)
              ),
    // letter 'B'
    CoordList(
              Coord(LINE_EXTEND,-0.7,-1,-0.7,1),
              Coord(LINE,-0.7,1,0.3,1),
              Coord(LINE,-0.7,0,0.3,0),
              Coord(ARC,0.3,0.5,0.5,-0.25,0.25),
              Coord(LINE,-0.7,-1,0.3,-1),
              Coord(ARC,0.3,-0.5,0.5,-0.25,0.25)
              ),
    // letter 'C'
    CoordList(
              Coord(ARC,0,0.3,0.7,0.125,0.5),
              Coord(ARC,0,-0.3,0.7,0.5,0.875),
              Coord(LINE,-0.7,0.3,-0.7,-0.3)
              ),
    // letter 'D'
    CoordList(
              Coord(LINE_EXTEND,-0.7,-1,-0.7,1),
              Coord(LINE,-0.7,1,-0.3,1),
              Coord(LINE,-0.7,-1,-0.3,-1),
              Coord(ARC,-0.3,0,1,-0.25,0.25)
              ),
    // letter 'E'
    CoordList(
              Coord(LINE_EXTEND,-0.7,1,-0.7,-1),
              Coord(LINE,-0.7,1,0.7,1),
              Coord(LINE,-0.7,-1,0.7,-1),
              Coord(LINE,-0.7,0,0.7,0)
              ),
    // letter 'F'
    CoordList(
              Coord(LINE_EXTEND,-0.7,1,-0.7,-1),
              Coord(LINE,-0.7,1,0.7,1),
              Coord(LINE,-0.7,0,0.7,0)
              ),
    // letter 'G'
    CoordList(
              Coord(ARC,0,0.3,0.7,0.125,0.5),
              Coord(ARC,0,-0.3,0.7,0.5,1.0),
              Coord(LINE,-0.7,0.3,-0.7,-0.3),
              Coord(LINE,0,0,0.7,0),
              Coord(LINE_EXTEND,0.7,-0.3,0.7,0)
              ),
    // letter 'H'
    CoordList(
              Coord(LINE_EXTEND,-0.7,1,-0.7,-1),
              Coord(LINE_EXTEND,0.7,1,0.7,-1),
              Coord(LINE,-0.7,0,0.7,0)
              ),
    // letter 'I'
    CoordList(
              Coord(LINE,0,1,0,-1),
              Coord(LINE_EXTEND,-0.3,1,0.3,1),
              Coord(LINE_EXTEND,-0.3,-1,0.3,-1)
              ),
    // letter 'J'
    CoordList(
              Coord(ARC,0,-0.4,0.6,0.58,1),
              Coord(LINE,0.6,-0.4,0.6,1)
              ),
    // letter 'K'
    CoordList(
              Coord(LINE_EXTEND,-0.7,-1,-0.7,1),
              Coord(LINE,-0.7,-0.2,0.7,1),
              Coord(LINE,-0.35,0.1,0.7,-1)
              ),
    // letter 'L'
    CoordList(
              Coord(LINE_EXTEND,-0.7,1,-0.7,-1),
              Coord(LINE_EXTEND,-0.7,-1,0.7,-1)
              ),
    // letter 'M'
    CoordList(
              Coord(LINE_EXTEND,-0.8,-1,-0.8,1),
              Coord(LINE,-0.8,1,0,-0.8),
              Coord(LINE,0,-0.8,0.8,1),
              Coord(LINE_EXTEND,0.8,1,0.8,-1),
              Coord(LINE_EXTEND,0,-0.8,0,-0.8+CAP_LENGTH)
              ),
    // letter 'N'
    CoordList(
              Coord(LINE_EXTEND,-0.7,-1,-0.7,1),
              Coord(LINE,-0.7,1,0.7,-1),
              Coord(LINE_EXTEND,0.7,-1,0.7,1)
              ),
    // letter 'O'
    CoordList(
              Coord(ARC,0,0.3,0.7,0,0.5),
              Coord(ARC,0,-0.3,0.7,0.5,1),
              Coord(LINE,-0.7,0.3,-0.7,-0.3),
              Coord(LINE,0.7,0.3,0.7,-0.3)
              ),
    // letter 'P'
    CoordList(
              Coord(LINE_EXTEND,-0.7,-1,-0.7,1),
              Coord(LINE,-0.7,1,0.2,1),
              Coord(LINE,-0.7,0,0.2,0),
              Coord(ARC,0.2,0.5,0.5,-0.25,0.25)
              ),
    // letter 'Q'
    CoordList(
              Coord(ARC,0,0.3,0.7,0,0.5),
              Coord(ARC,0,-0.3,0.7,0.5,1),
              Coord(LINE,-0.7,0.3,-0.7,-0.3),
              Coord(LINE,0.7,0.3,0.7,-0.3),
              Coord(LINE,0.7,-1,0.2,-0.5)
              ),
    // letter 'R'
    CoordList(
              Coord(LINE_EXTEND,-0.7,-1,-0.7,1),
              Coord(LINE,-0.7,1,0.2,1),
              Coord(LINE,-0.7,0,0.2,0),
              Coord(ARC,0.2,0.5,0.5,-0.25,0.25),
              Coord(LINE,0.7,-1,0,0)
              ),
    // letter 'S'
    CoordList(
              Coord(LINE,-0.2,1,0.2,1),
              Coord(LINE,-0.2,-1,0.2,-1),
              Coord(LINE,-0.2,0,0.2,0),
              Coord(ARC,-0.2,0.5,0.5,0.25,0.75),
              Coord(ARC,0.2,-0.5,0.5,-0.25,0.25),
              Coord(ARC,0.2,0.5,0.5,0.125,0.25),
              Coord(ARC,-0.2,-0.5,0.5,0.625,0.75)
              ),
    // letter 'T'
    CoordList(
              Coord(LINE_EXTEND,-0.7,1,0.7,1),
              Coord(LINE_EXTEND,0,1,0,-1)
              ),
    // letter 'U'
    CoordList(
              Coord(LINE,-0.7,1,-0.7,-0.3),
              Coord(LINE,0.7,1,0.7,-0.3),
              Coord(ARC,0,-0.3,0.7,0.5,1)
              ),
    // letter 'V'
    CoordList(
              Coord(LINE,-0.7,1,0,-1),
              Coord(LINE,0,-1,0.7,1),
              Coord(LINE_EXTEND,0,-1,0,-1+CAP_LENGTH)
              ),
    // letter 'W'
    CoordList(
              Coord(LINE,-0.8,1,-0.4,-1),
              Coord(LINE,-0.4,-1,0,0.2),
              Coord(LINE,0,0.2,0.4,-1),
              Coord(LINE,0.4,-1,0.8,1),
              Coord(LINE_EXTEND,-0.4,-1,-0.4,-1+CAP_LENGTH),
              Coord(LINE_EXTEND,0.4,-1,0.4,-1+CAP_LENGTH),
              Coord(LINE_EXTEND,0,0.2,0,0.2-CAP_LENGTH)
              ),
    // letter 'X'
    CoordList(
              Coord(LINE_EXTEND,-0.7,-1,0.7,1),
              Coord(LINE_EXTEND,0.7,-1,-0.7,1)
              ),
    // letter 'Y'
    CoordList(
              Coord(LINE,-0.7,1,0,0),
              Coord(LINE,0,0,0.7,1),
              Coord(LINE_EXTEND,0,0,0,-1)
              ),
    // letter 'Z'
    CoordList(
              Coord(LINE_EXTEND,-0.7,1,0.7,1),
              Coord(LINE,0.7,1,-0.7,-1),
              Coord(LINE_EXTEND,-0.7,-1,0.7,-1)
              ),
    // '[' character
    CoordList(
              Coord(LINE_EXTEND,-0.2,-0.9,-0.2,0.7),
              Coord(LINE,-0.2,-0.9,0.2,-0.9),
              Coord(LINE,-0.2,0.7,0.2,0.7)
              ),
    // '\' character
    CoordList(
              Coord(LINE_EXTEND,0.7,-1,-0.7,1)
              ),
    // ']' character
    CoordList(
              Coord(LINE_EXTEND,0.2,-0.9,0.2,0.7),
              Coord(LINE,-0.2,-0.9,0.2,-0.9),
              Coord(LINE,-0.2,0.7,0.2,0.7)
              ),
    // '^' character
    CoordList(
              Coord(LINE_EXTEND,-0.3,0,0,0.3),
              Coord(LINE_EXTEND,0.3,0,0,0.3)
              ),
    // '_' character
    CoordList(
              Coord(LINE_EXTEND,-0.7,-1.1,0.7,-1.1)
              ),
    // '`' character
    CoordList(
              Coord(LINE_EXTEND,-0.05,0.75,0.05,0.60)
              ),
    // letter 'a'
    CoordList(
              Coord(ARC,0,-0.5,0.5,0,1),
              Coord(LINE_EXTEND,0.5,0,0.5,-1)
              ),
    // letter 'b'
    CoordList(
              Coord(ARC,0,-0.5,0.5,0,1),
              Coord(LINE_EXTEND,-0.5,0.75,-0.5,-1)
              ),
    // letter 'c'
    CoordList(
              Coord(ARC,0,-0.5,0.5,0.125,0.875)
              ),
    // letter 'd'
    CoordList(
              Coord(ARC,0,-0.5,0.5,0,1),
              Coord(LINE_EXTEND,0.5,0.75,0.5,-1)
              ),
    // letter 'e'
    CoordList(
              Coord(ARC,0,-0.5,0.5,0,0.875),
              Coord(LINE_EXTEND,-0.5,-0.5,0.5,-0.5)
              ),
    // letter 'f'
    CoordList(
              Coord(LINE,0,-1,0,0.5),
              Coord(LINE_EXTEND,-0.3,0,0.3,0),
              Coord(ARC,0.2,0.5,0.2,0.075,0.5)
              ),
    // letter 'g'
    CoordList(
              Coord(ARC,0,-0.5,0.5,0,1),
              Coord(LINE_EXTEND,0.5,0,0.5,-1.5),
              Coord(ARC,0,-1.5,0.5,0.5,1)
              ),
    // letter 'h'
    CoordList(
              Coord(ARC,0,-0.5,0.5,0,0.5),
              Coord(LINE,-0.5,0.75,-0.5,-1),
              Coord(LINE,0.5,-0.5,0.5,-1)
              ),
    // letter 'i'
    CoordList(
              Coord(LINE_EXTEND,0,-1,0,0),
              Coord(LINE_EXTEND,0,0.6,0,0.6+CAP_LENGTH)
              //Coord(ARC,0,0.6,0.05,0,1)
              ),
    // letter 'j'
    CoordList(
              Coord(LINE,0.5,-1.5,0.5,0),
              Coord(LINE_EXTEND,0.5,0.6,0.5,0.6+CAP_LENGTH),
              Coord(ARC,0,-1.5,0.5,0.5,1)
              ),
    // letter 'k'
    CoordList(
              Coord(LINE_EXTEND,-0.5,0.75,-0.5,-1),
              Coord(LINE,-0.5,-0.5,0.5,0),
              Coord(LINE,-0.15,-0.32,0.5,-1)
              ),
    // letter 'l'
    CoordList(
              Coord(LINE_EXTEND,0,-1,0,0.75),
              Coord(LINE_EXTEND,0,0.75,-0.05,0.75),
              Coord(LINE_EXTEND,0,-1,0.05,-1)
              ),
    // letter 'm'
    CoordList(
              Coord(LINE_EXTEND,-0.8,-1,-0.8,0),
              Coord(ARC,-0.4,-0.4,0.4,0,0.5),
              Coord(LINE_EXTEND,0,-1,0,-0.4),
              Coord(ARC,0.4,-0.4,0.4,0,.5),
              Coord(LINE_EXTEND,0.8,-1,0.8,-0.4)
              ),
    // letter 'n'
    CoordList(
              Coord(LINE_EXTEND,-0.5,-1,-0.5,0),
              Coord(ARC,0,-0.5,0.5,0,0.5),
              Coord(LINE_EXTEND,0.5,-1,0.5,-0.5)
              ),
    // letter 'o'
    CoordList(
              Coord(ARC,0,-0.5,0.5,0,1)
              ),
    // letter 'p'
    CoordList(
              Coord(ARC,0,-0.5,0.5,0,1),
              Coord(LINE_EXTEND,-0.5,0,-0.5,-1.75)
              ),
    // letter 'q'
    CoordList(
              Coord(ARC,0,-0.5,0.5,0,1),
              Coord(LINE_EXTEND,0.5,0,0.5,-1.75),
              Coord(LINE,0.5,-1.75,0.8,-1.55)
              ),
    // letter 'r'
    CoordList(
              Coord(LINE_EXTEND,-0.5,-1,-0.5,0),
              Coord(ARC,0,-0.5,0.5,0.125,.5)
              ),
    // letter 's'
    CoordList(
              Coord(LINE,-0.2,0,0.2,0),
              Coord(LINE,-0.2,-0.5,0.2,-0.5),
              Coord(LINE,-0.2,-1,0.2,-1),
              Coord(ARC,-0.2,-0.25,0.25,0.25,0.75),
              Coord(ARC,0.2,-0.75,0.25,-0.25,0.25),
              Coord(ARC,0.2,-0.25,0.25,0.125,0.25),
              Coord(ARC,-0.2,-0.75,0.25,0.625,0.75)
              ),
    // letter 't'
    CoordList(
              Coord(LINE,0,-0.8,0,0.75),
              Coord(LINE_EXTEND,-0.3,0,0.3,0),
              Coord(ARC,0.2,-0.8,0.2,0.5,0.75)
              ),
    // letter 'u'
    CoordList(
              Coord(LINE_EXTEND,0.5,-1,0.5,0),
              Coord(ARC,0,-0.5,0.5,0.5,1),
              Coord(LINE_EXTEND,-0.5,0,-0.5,-0.5)
              ),
    // letter 'v'
    CoordList(
              Coord(LINE,-0.5,0,0,-1),
              Coord(LINE,0,-1,0.5,0),
              Coord(LINE_EXTEND,0,-1,0,-1+CAP_LENGTH)
              ),
    // letter 'w'
    CoordList(
              Coord(LINE,-0.8,0,-0.4,-1),
              Coord(LINE,-0.4,-1,0,0),
              Coord(LINE,0,0,0.4,-1),
              Coord(LINE,0.4,-1,0.8,0),
              Coord(LINE_EXTEND,-0.4,-1,-0.4,-1+CAP_LENGTH),
              Coord(LINE_EXTEND,0.4,-1,0.4,-1+CAP_LENGTH),
              Coord(LINE_EXTEND,0,0,0,0-CAP_LENGTH)
              ),
    // letter 'x'
    CoordList(
              Coord(LINE_EXTEND,-0.5,-1,0.5,0),
              Coord(LINE_EXTEND,0.5,-1,-0.5,0)
              ),
    // letter 'y'
    CoordList(
              Coord(LINE,-0.3,-1.75,0.5,0),
              Coord(LINE,0.05,-1,-0.5,0)
              ),
    // letter 'z'
    CoordList(
              Coord(LINE_EXTEND,-0.5,0,0.5,0),
              Coord(LINE,0.5,0,-0.5,-1),
              Coord(LINE_EXTEND,-0.5,-1,0.5,-1)
              ),
    // 'Coord(' character
    CoordList(
              Coord(ARC,-0.3,0.3,0.3,-0.25,0),
              Coord(ARC,-0.3,-0.3,0.3,0,0.25),
              Coord(ARC,0.3,0.3,0.3,0.25,0.5),
              Coord(ARC,0.3,-0.3,0.3,0.5,0.75)
              ),
    // '|' character
    CoordList(
              Coord(LINE_EXTEND,0,-1,0,1)
              ),
    // ')' character
    CoordList(
              Coord(ARC,0.3,0.3,0.3,0.5,0.75),
              Coord(ARC,0.3,-0.3,0.3,0.25,0.5),
              Coord(ARC,-0.3,0.3,0.3,0,0.25),
              Coord(ARC,-0.3,-0.3,0.3,-0.25,0)
              ),
    // '~' character
    CoordList(
              Coord(ARC,-0.248,-0.5,0.35,0.125,0.42),
              Coord(ARC,0.248,0,0.35,0.625,0.92)
              ),
};

static void genPoint(double x, double y, double z, vec4 theColor, GLfloat shininess, vec3 normal,
                     int *idxVar, vec4 *pointsArray, vec3 *normalsArray, vec4 *color1Array,
                     vec4 *color2Array , vec4 *color3Array , GLfloat *shininessArray) {
    // add the first point to the array
    if (idxVar[0] < idxVar[1]) {
        pointsArray[*idxVar] = vec4(x, y, z, 1.0);
        color1Array[*idxVar] = theColor;
        if (normalsArray != NULL) normalsArray[*idxVar] = normal;
        if (color2Array != NULL) color2Array[*idxVar] = theColor;
        if (color3Array != NULL) color3Array[*idxVar] = theColor;
        if (shininessArray != NULL) shininessArray[*idxVar] = shininess;
    }
    idxVar[0]++;
}

static void genPolyhedron6(double x1, double y1, double x2, double y2,
                           double x3, double y3, double x4, double y4, double dz, vec4 theColor,
                           GLfloat shininess,
                           int *idxVar, vec4 *pointsArray, vec3 *normalsArray, vec4 *color1Array,
                           vec4 *color2Array, vec4 *color3Array, GLfloat *shininessArray) {
    // **** here is a picture ****
    //   (x1,y1)
    //       *
    //               *(x2,y2)
    //
    //     *
    //   (x3,y3)   *(x4,y4)
    
#define COLOR_FIXED 0
    
#if COLOR_FIXED
    theColor = vec4(1.0,0.0,0.0,1.0); // RED
#endif
    // generate vertices for front face
    vec3 normal = -normalize(cross(vec3(x2-x1,y2-y1,0),vec3(x4-x2,y4-y2,0)));
    genPoint(x1,y1,dz,theColor,shininess,normal,idxVar,pointsArray,
             normalsArray,color1Array,color2Array,color3Array,shininessArray);
    genPoint(x4,y4,dz,theColor,shininess,normal,idxVar,pointsArray,
             normalsArray,color1Array,color2Array,color3Array,shininessArray);
    
    
    genPoint(x2,y2,dz,theColor,shininess,normal,idxVar,pointsArray,
             normalsArray,color1Array,color2Array,color3Array,shininessArray);
    
    
    normal = normalize(cross(vec3(x4-x2,y4-y2,0),vec3(x3-x4,y3-y4,0)));
    genPoint(x2,y2,dz,theColor,shininess,normal,idxVar,pointsArray,
             normalsArray,color1Array,color2Array,color3Array,shininessArray);
    genPoint(x4,y4,dz,theColor,shininess,normal,idxVar,pointsArray,
             normalsArray,color1Array,color2Array,color3Array,shininessArray);
    genPoint(x3,y3,dz,theColor,shininess,normal,idxVar,pointsArray,
             normalsArray,color1Array,color2Array,color3Array,shininessArray);
    
    
#if COLOR_FIXED
    theColor = vec4(1.0,1.0,1.0,1.0); // WHITE
#endif
    // generate vertices for rear face   v1 = x2-x2;
    normal = -normalize(cross(vec3(x4-x1,y4-y1,0),vec3(x2-x4,y2-y4,0)));
    genPoint(x1,y1,-dz,theColor,shininess,normal,idxVar,pointsArray,
             normalsArray,color1Array,color2Array,color3Array,shininessArray);
    genPoint(x2,y2,-dz,theColor,shininess,normal,idxVar,pointsArray,
             normalsArray,color1Array,color2Array,color3Array,shininessArray);
    
    
    genPoint(x4,y4,-dz,theColor,shininess,normal,idxVar,pointsArray,
             normalsArray,color1Array,color2Array,color3Array,shininessArray);
    
    
    normal = normalize(cross(vec3(x2-x4,y2-y4,0),vec3(x3-x2,y3-y2,0)));
    genPoint(x4,y4,-dz,theColor,shininess,normal,idxVar,pointsArray,
             normalsArray,color1Array,color2Array,color3Array,shininessArray);
    genPoint(x2,y2,-dz,theColor,shininess,normal,idxVar,pointsArray,
             normalsArray,color1Array,color2Array,color3Array,shininessArray);
    genPoint(x3,y3,-dz,theColor,shininess,normal,idxVar,pointsArray,
             normalsArray,color1Array,color2Array,color3Array,shininessArray);
    
#if COLOR_FIXED
    theColor = vec4(1.0,1.0,0.0,1.0); // YELLOW
#endif
    // generate vertices for top face
    normal = -normalize(cross(vec3(0,0,-dz-dz),vec3(x2-x1,y2-y1,dz-(-dz))));
    genPoint(x1,y1,dz,theColor,shininess,normal,idxVar,pointsArray,
             normalsArray,color1Array,color2Array,color3Array,shininessArray);
    genPoint(x2,y2,dz,theColor,shininess,normal,idxVar,pointsArray,
             normalsArray,color1Array,color2Array,color3Array,shininessArray);
    normal = normalize(cross(vec3(x2-x1,y2-y1,dz-(-dz)),vec3(0,0,-dz-dz)));
    
    
    genPoint(x1,y1,-dz,theColor,shininess,normal,idxVar,pointsArray,
             normalsArray,color1Array,color2Array,color3Array,shininessArray);
    
    genPoint(x1,y1,-dz,theColor,shininess,normal,idxVar,pointsArray,
             normalsArray,color1Array,color2Array,color3Array,shininessArray);
    genPoint(x2,y2,dz,theColor,shininess,normal,idxVar,pointsArray,
             normalsArray,color1Array,color2Array,color3Array,shininessArray);
    genPoint(x2,y2,-dz,theColor,shininess,normal,idxVar,pointsArray,
             normalsArray,color1Array,color2Array,color3Array,shininessArray);
    
#if COLOR_FIXED
    theColor = vec4(0.0,0.0,1.0,1.0); // BLUE
#endif
    // generate vertices for bottom face
    normal = normalize(cross(vec3(x4-x3,y4-y3,0),vec3(x3-x4,y3-y4,-dz-dz)));
    genPoint(x3,y3,dz,theColor,shininess,normal,idxVar,pointsArray,
             normalsArray,color1Array,color2Array,color3Array,shininessArray);
    genPoint(x3,y3,-dz,theColor,shininess,normal,idxVar,pointsArray,
             normalsArray,color1Array,color2Array,color3Array,shininessArray);
    
    
    genPoint(x4,y4,dz,theColor,shininess,normal,idxVar,pointsArray,
             normalsArray,color1Array,color2Array,color3Array,shininessArray);
    
    normal = -normalize(cross(vec3(x4-x3,y4-y3,0),vec3(0,0,dz-(-dz))));
    genPoint(x3,y3,-dz,theColor,shininess,normal,idxVar,pointsArray,
             normalsArray,color1Array,color2Array,color3Array,shininessArray);
    genPoint(x4,y4,-dz,theColor,shininess,normal,idxVar,pointsArray,
             normalsArray,color1Array,color2Array,color3Array,shininessArray);
    genPoint(x4,y4,dz,theColor,shininess,normal,idxVar,pointsArray,
             normalsArray,color1Array,color2Array,color3Array,shininessArray);
    
#if COLOR_FIXED
    theColor = vec4(0.0,1.0,0.0,1.0); // GREEN
#endif
    // generate vertices for left face
    normal = normalize(cross(vec3(0,0,-dz-dz),vec3(x4-x1,y4-y1,dz-(-dz))));
    genPoint(x1,y1,dz,theColor,shininess,normal,idxVar,pointsArray,
             normalsArray,color1Array,color2Array,color3Array,shininessArray);
    genPoint(x1,y1,-dz,theColor,shininess,normal,idxVar,pointsArray,
             normalsArray,color1Array,color2Array,color3Array,shininessArray);
    
    
    genPoint(x4,y4,dz,theColor,shininess,normal,idxVar,pointsArray,
             normalsArray,color1Array,color2Array,color3Array,shininessArray);

    normal = normalize(cross(vec3(x1-x4,y1-y4,-dz-dz),vec3(x4-x1,y4-y1,0)));
    genPoint(x4,y4,dz,theColor,shininess,normal,idxVar,pointsArray,
             normalsArray,color1Array,color2Array,color3Array,shininessArray);
    genPoint(x1,y1,-dz,theColor,shininess,normal,idxVar,pointsArray,
             normalsArray,color1Array,color2Array,color3Array,shininessArray);
    genPoint(x4,y4,-dz,theColor,shininess,normal,idxVar,pointsArray,
             normalsArray,color1Array,color2Array,color3Array,shininessArray);
    
#if COLOR_FIXED
    theColor = vec4(0.0,0.25,0.0,1.0); // dark green
#endif
    // generate vertices for right face
    normal = -normalize(cross(vec3(x2-x3,y2-y3,0),vec3(x3-x2,y3-y2,-dz-dz)));
    genPoint(x3,y3,dz,theColor,shininess,normal,idxVar,pointsArray,
             normalsArray,color1Array,color2Array,color3Array,shininessArray);
    genPoint(x3,y3,-dz,theColor,shininess,normal,idxVar,pointsArray,
             normalsArray,color1Array,color2Array,color3Array,shininessArray);
    
    
    genPoint(x2,y2,dz,theColor,shininess,normal,idxVar,pointsArray,
             normalsArray,color1Array,color2Array,color3Array,shininessArray);
    

    normal = -normalize(cross(vec3(0,0,-dz-dz),vec3(x3-x2,y3-y2,0)));
    genPoint(x2,y2,dz,theColor,shininess,normal,idxVar,pointsArray,
             normalsArray,color1Array,color2Array,color3Array,shininessArray);
    genPoint(x3,y3,-dz,theColor,shininess,normal,idxVar,pointsArray,
             normalsArray,color1Array,color2Array,color3Array,shininessArray);
    genPoint(x2,y2,-dz,theColor,shininess,normal,idxVar,pointsArray,
             normalsArray,color1Array,color2Array,color3Array,shininessArray);
}

static void genPrism(double at1X, double at1Y, double at2X, double at2Y,
                     double dyWidth, double dxWidth, double dz, vec4 theColor,
                     GLfloat shininess,
                     int *idxVar, vec4 *pointsArray, vec3 *normalsArray, vec4 *color1Array,
                     vec4 *color2Array, vec4 *color3Array, GLfloat *shininessArray) {
    
    double x1 = at1X-dyWidth;
    double x2 = at1X+dyWidth;
    double x3 = at2X+dyWidth;
    double x4 = at2X-dyWidth;
    double y1 = at1Y+dxWidth;
    double y2 = at1Y-dxWidth;
    double y3 = at2Y-dxWidth;
    double y4 = at2Y+dxWidth;

    genPolyhedron6(x1, y1, x2, y2, x3, y3, x4, y4, dz,theColor,shininess,
                   idxVar,pointsArray,normalsArray,color1Array,color2Array,color3Array,
                   shininessArray);
}

static void genArc(double centerX, double centerY, double radius,
                   double startAngle, double endAngle,
                   double width, double divCircle, double dz, vec4 theColor, GLfloat shininess,
                   int *idxVar, vec4 *pointsArray, vec3 *normalsArray, vec4 *color1Array,
                   vec4 *color2Array, vec4 *color3Array, GLfloat *shininessArray) {
    if (startAngle >= endAngle) return;
    if (divCircle <= 0) divCircle = 1;
    
    int count = (int)(divCircle*(endAngle-startAngle));
    startAngle *= 2*M_PI;
    endAngle *= 2*M_PI;
    double totalAngle = endAngle-startAngle;
    double angleDelta = totalAngle/count;
    double halfWidth = width/2;
    for (int i = 0; i < count; i++) {
        double angle1 = i*angleDelta;
        double angle2 = (i+1)*angleDelta;
        double sin1 = sin(startAngle+angle1);
        double cos1 = cos(startAngle+angle1);
        double sin2 = sin(startAngle+angle2);
        double cos2 = cos(startAngle+angle2);

        genPolyhedron6(centerX + cos2*(radius-halfWidth),
                       centerY + sin2*(radius-halfWidth),
                       centerX + cos2*(radius+halfWidth),
                       centerY + sin2*(radius+halfWidth),
                       centerX + cos1*(radius+halfWidth),
                       centerY + sin1*(radius+halfWidth),
                       centerX + cos1*(radius-halfWidth),
                       centerY + sin1*(radius-halfWidth),
                       

                       dz,theColor,shininess,idxVar,pointsArray,normalsArray,
                       color1Array,color2Array,color3Array,shininessArray);
    }
    
}

static void genLetter(const CoordList& list, double width, double dz, vec4 theColor, GLfloat shininess,
                      int *idxVar, vec4 *pointsArray, vec3 *normalsArray, vec4 *color1Array,
                      vec4 *color2Array, vec4 *color3Array, GLfloat *shininessArray) {
    for (const CoordList* cl = &list; cl != NULL; cl = cl->next) {
        double code = cl->thisOne.coords[0];
        if ((code == LINE || code == LINE_EXTEND)) {
            double x1 = cl->thisOne.coords[1];
            double y1 = cl->thisOne.coords[2];
            double x2 = cl->thisOne.coords[3];
            double y2 = cl->thisOne.coords[4];
            double dx = x1-x2;
            double dy = y1-y2;
            double distanceSquared = dx*dx + dy*dy;
            if (distanceSquared != 0) {
                double dist = sqrt(distanceSquared);
                double vec2To1x = dx/dist;
                double vec2To1y = dy/dist;
                double halfWidth = width/2;
                double dxWidth = vec2To1x*halfWidth;
                double dyWidth = vec2To1y*halfWidth;
                double at1X = x1;
                double at1Y = y1;
                double at2X = x2;
                double at2Y = y2;
                if (code == LINE_EXTEND) {
                    at1X += dxWidth;
                    at1Y += dyWidth;
                    at2X -= dxWidth;
                    at2Y -= dyWidth;
                }
                genPrism(at1X, at1Y, at2X, at2Y, dyWidth, dxWidth, dz, theColor, shininess,
                         idxVar, pointsArray, normalsArray, color1Array, color2Array, color3Array,
                         shininessArray);
            }
        }
        else if (code == ARC) {
            genArc(cl->thisOne.coords[1], cl->thisOne.coords[2],
                   cl->thisOne.coords[3], cl->thisOne.coords[4],
                   cl->thisOne.coords[5], width, 32, dz, theColor, shininess,
                   idxVar, pointsArray, normalsArray, color1Array,
                   color2Array, color3Array, shininessArray);
        }
        else if (code != INVISIBLE) {
            cerr << "Invalid spec for letter sub-object" << endl;
        }
    }
    
}

ObjRef genCharacter(char theLetter, vec4 theColor, double width, double z, GLfloat theShininess,
                    int *idxVar, vec4 *pointsArray, vec3 *normalsArray, vec4 *color1Array,
                    vec4 *color2Array, vec4 *color3Array, GLfloat *shinyArray)
{
    int first = idxVar[0];
    
    if (theLetter < ' ' || theLetter > '~') {
        theLetter = ' ';
    }
    
    genLetter(characterCoords[theLetter-' '], width, z/2, theColor, theShininess,
              idxVar, pointsArray, normalsArray, color1Array, color2Array, color3Array, shinyArray);
    
    return ObjRef(first,idxVar[0]);
}
