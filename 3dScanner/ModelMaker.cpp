#include "ModelMaker.h"
#include <fstream>


ModelMaker::ModelMaker()
{
	data =  new Point3D[500000];
	this->count = 0;
}

ModelMaker::ModelMaker(ModelMaker& newPoints)
{

	delete [] data;

	data = new Point3D[newPoints.count];

	for (int i = 0; i < newPoints.count ; ++i)
	{
		data[i] = newPoints[i];
	}

	this->count = newPoints.count;

}

ModelMaker& ModelMaker::operator+=(ModelMaker&  addPoints){


	for (int i = 0; i < addPoints.count; ++i){

		this->data[this->count++] = addPoints[i];  // noktayý ekledik mi ?

	}

	return *this;   // ekledik return edelm :)
}

ModelMaker& ModelMaker:: operator =(ModelMaker& other){

	if (this != &other) // protect against invalid self-assignment
	{
		// 1: allocate new memory and copy the elements
		ModelMaker *newPoints = new ModelMaker();
		
		for (int i = 0; i < other.count ; ++i)
			newPoints->data[i] = other.data[i];

		//2: deallocate old memory
		delete [] data;

		//3: assign the new memory to the object
		data = newPoints->data;
		count = other.count;
	}
	// by convention, always return *this
	return *this;
}
ModelMaker::~ModelMaker(void)
{
	delete[] this->data;

}
bool ModelMaker::pointsToFile(string path){

	bool condition(true);
	
	FILE *inp;

	inp=fopen(path.c_str(),"w");

	if(inp==NULL){
		printf("cannot open file\n");
	}
	
	if (!inp){
		cout << "Unable to open file" << endl;
		condition = false;
		return condition;
	}

	for (int i = 0; i < count; ++i)
	{
		Vector3D *pos = data[i].getPos(); 
		Color24 *col  = data[i].getColor();
		fprintf(inp,"%7.4lf  %7.4lf %7.4lf %7d %7d %7d\n",pos->x,pos->y,pos->z,col->r,col->g,col->b);
		
	}
	fclose(inp); // close output file 

	return condition;
}



Point3D& ModelMaker:: operator[] (int index)
{
	if (index < 0 && index  >= this->count)
	{
		cout << "Invalid index " << index <<
			" Program aborted!\n" <<endl;
		//exit(1);
	}
	return this->data[index];
}


const Point3D& ModelMaker::operator [] (int index) const
{
	if (index < 0 && index  >= this->count)
	{
		cout << "Invalid index " << index <<
			" Program aborted!\n" <<endl;
		//exit(1);
	}
	return this->data[index];
}
