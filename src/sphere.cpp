// Sphere approximation by recursive subdivision of a tetrahedron, adapted from
// code in Angel, 6th edition.

#include "cs432.h"
#include "vec.h"
#include "sphere.h"

typedef vec4 color4;
typedef vec4 point4;

// four equally spaced points on the unit sphere
const point4 v[4]= {
	vec4(0.0, 0.0, 1.0, 1.0),
	vec4(0.0, 0.942809, -0.333333, 1.0),
	vec4(-0.816497, -0.471405, -0.333333, 1.0),
	vec4(0.816497, -0.471405, -0.333333, 1.0)
};

// move a point to unit circle
static point4 unit(const point4 &p) {
	point4 c;
	double d=0.0;
	for(int i=0; i<3; i++) d+=p[i]*p[i];
	d=sqrt(d);
	if(d > 0.0) for(int i=0; i<3; i++) c[i] = p[i]/d;
	c[3] = 1.0;
	return c;
}

static void triangle(point4  a, point4 b, point4 c, color4 color,
			  int* idxSpot, point4* pointArray, color4* colorArray,
				vec3* normalArray, bool flatShading) {
	if (idxSpot[0]+3 > idxSpot[1]) {
		*idxSpot += 3;
	}
	else {
		if (pointArray != NULL) pointArray[*idxSpot]= a;
		if (colorArray != NULL) colorArray[*idxSpot] = color;
		vec3 flatNormal;
		if (normalArray != NULL) {
			if (flatShading) {
				flatNormal = normalize(cross(b-a,b-c));
				normalArray[*idxSpot] = flatNormal;
			}
			else {
				vec4 nma = normalize(a);
				normalArray[*idxSpot] = vec3(nma.x,nma.y,nma.z);
			}
		}
		++*idxSpot;
		if (pointArray != NULL) pointArray[*idxSpot] = b;
		if (colorArray != NULL) colorArray[*idxSpot] = color;
		if (normalArray != NULL) {
			if (flatShading) {
				normalArray[*idxSpot] = flatNormal;	
			}
			else {
				vec4 nmb = normalize(b);
				normalArray[*idxSpot] = vec3(nmb.x,nmb.y,nmb.z);
			}
		}
		
		++*idxSpot;
		if (pointArray != NULL) pointArray[*idxSpot] = c;
		if (colorArray != NULL) colorArray[*idxSpot] = color;
		if (normalArray != NULL) {
			if (flatShading) {
				normalArray[*idxSpot] = flatNormal;
			}
			else {
				vec4 nmc = normalize(c);
				normalArray[*idxSpot] = vec3(nmc.x,nmc.y,nmc.z);
			}
		}
		++*idxSpot;
	}
}


static void divide_triangle(point4 a, point4 b, point4 c, color4 color, int n,
							int* idxSpot, point4* pointArray, color4* colorArray,
							vec3* normalArray, bool flatShading = false) {
	point4 v1, v2, v3;
	if(n>0) {
		v1 = unit(a + b);
		v2 = unit(a + c);
		v3 = unit(b + c);   
		divide_triangle(a ,v2, v1, color, n-1, idxSpot, pointArray, colorArray, normalArray, flatShading);
		divide_triangle(c ,v3, v2, color, n-1, idxSpot, pointArray, colorArray, normalArray, flatShading);
		divide_triangle(b ,v1, v3, color, n-1, idxSpot, pointArray, colorArray, normalArray, flatShading);
		divide_triangle(v1 ,v2, v3, color, n-1, idxSpot, pointArray, colorArray, normalArray, flatShading);
	}
	else {
		triangle(a, c, b, color, idxSpot, pointArray, colorArray, normalArray, flatShading);
	}
}

ObjRef genSphere(color4 color, int n, int* idxSpot,
					   point4* pointArray, color4* colorArray, vec3* normalArray) {
	int startIdx = *idxSpot;
	divide_triangle(v[0], v[1], v[2], color, n, idxSpot, pointArray, colorArray, normalArray);
	divide_triangle(v[3], v[2], v[1], color, n, idxSpot, pointArray, colorArray, normalArray);
	divide_triangle(v[0], v[3], v[1], color, n, idxSpot, pointArray, colorArray, normalArray);
	divide_triangle(v[0], v[2], v[3], color, n, idxSpot, pointArray, colorArray, normalArray);
	return ObjRef(startIdx, *idxSpot);
}

ObjRef genFlatSphere(color4 color, int n, int* idxSpot,
				 point4* pointArray, color4* colorArray, vec3* normalArray) {
	int startIdx = *idxSpot;
	divide_triangle(v[0], v[1], v[2], color, n, idxSpot, pointArray, colorArray, normalArray, true);
	divide_triangle(v[3], v[2], v[1], color, n, idxSpot, pointArray, colorArray, normalArray, true);
	divide_triangle(v[0], v[3], v[1], color, n, idxSpot, pointArray, colorArray, normalArray, true);
	divide_triangle(v[0], v[2], v[3], color, n, idxSpot, pointArray, colorArray, normalArray, true);
	return ObjRef(startIdx, *idxSpot);
}

