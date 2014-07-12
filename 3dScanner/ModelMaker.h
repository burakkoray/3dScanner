#ifndef MODEL_POINT_H
#define MODEL_POINT_H

#include <iostream>
#include <vector>
#include "Point3D.h"
using namespace std;

class ModelMaker
{
public:

	ModelMaker();

	ModelMaker(ModelMaker& newPoints);

	bool pointsToFile(string path);

	/*ModelMaker& operator=(const ModelMaker& rhs);
	*/
	Point3D& operator [] (int index);
	const Point3D& operator [] (int index) const;
	
	ModelMaker& operator +=(ModelMaker&  addPoints);
	ModelMaker& operator =(ModelMaker& other);
	Point3D* getData() const{return data;}
	
	virtual ~ModelMaker();
	
	
	// private members
	Point3D *data;
	int count;
private:

};

#endif //
