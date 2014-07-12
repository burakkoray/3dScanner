#ifndef KSCANNER_POINT_H
#define KSCANNER_POINT_H

#include "Vector3D.h"
#include "Color24.h"

class Point3D{
public:
	Point3D() { }
	Point3D( float x, float y, float z, uint8_t r, uint8_t g, uint8_t b ) : pos(x,y,z), color(r,g,b) {}
	Point3D( float x, float y, float z ) : pos(x,y,z), color() {}
	Point3D(Vector3D& vect) : pos(vect), color() {}
	Vector3D* getPos(){ return &pos; }
	Color24* getColor(){ return &color; }
private:
	Vector3D pos;
	Color24 color;
};



#endif
