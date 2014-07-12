#ifndef MYGL_H
#define MYGL_H

#include <iostream>
#include <vector>
#include <cstring>
#include <time.h>

#include "ModelMaker.h"
#include "KScanner.h"
#include "Vector3D.h"


#define _RADIAN15 0.261799388
#define _RADIAN90 1.57079633
#define _SENSITIVE 0.01

#define _ORI_X  -0.1925
#define _ORI_Y   0.0
#define _ORI_Z  -0.7718

#define _NUMBER_OF_FRAME 24
#define _POINT_FILE_NAME "Points\\"
using namespace std;


void readFile(ModelMaker array[]);
void printFile(ModelMaker& mp, string fName);


class Settings
{
public:
	Settings();
	void init(int argc, char **argv);
	void run();
	unsigned int loadTexture(char *fileName);
	void readFrameFromFile(ModelMaker& point3d,float angle);
	void rotatingWithAngle(ModelMaker &temp,float angle);
	void final3DPointsToFile(ModelMaker& point);


	void setMPoint(ModelMaker& mp);
	ModelMaker& getMpoint();
	friend void Ready();
	void initAll();

protected:
	void display();
	void initOpenGL(int argc, char **argv);
	void initOpenGLHooks();

private:
	static Settings* self;
	static void idle();
	/*Keyboard events*/
	static void keyboardUp(unsigned char key, int x, int y) ;
	static void keyboardSpecialDown(int k, int x, int y);
	static void keyboardSpecialUp(int k, int x, int y);
	static void keyboardDown(unsigned char key, int x, int y);
	/*Mouse events*/
	static void mouseClick(int button, int state, int x, int y);
	static void glutMouse(int button, int state, int x, int y);
	 
	static void mouseMotion(int x, int y) ;
	static void mousePassiveMotion(int x,int y);
	static void mouseWheelFunc(int wheel, int direction, int x, int y);
	static void glutKeyboard(unsigned char key, int x, int y);
	static void initGL(int width, int height);
	static void glutDisplay();
	
	Vector3D camPos;

	Vector3D origin;

	bool streaming;

	//
	ModelMaker mPoints;
	
	//camera
	float cx;
	float cy;
	bool camRotating;
	bool camMoving;
	float camR;

	int dragLastX;
	int dragLastY;

	GLuint texture;
};

/*
 *	Variables to hold the current size of the window.
 */
static int winw = 720;	
static int winh = 456;

	
enum ButtonState{NOTPRESS,PRESS};

typedef void (*OnClickButton)();

class Button
{
public:
	Button(int newX,int newY,int newW,int newH,char* newText,OnClickButton func);
	/*set attributes*/
	void setHighlighted(int _high) {highlighted=_high;}
	void setX(int _x)	{x=_x;}
	void setY(int _y)	{y=_y;}	
	void setW(int _w)	{widht=_w;}	
	void setH(int _h)	{height=_h;}	
	void setState(ButtonState _state) {state=_state;}
	void setLabel(char *_label) {buttonText=_label;}
	void setID(int _id)	{id=_id;}
	/*get attributes*/
	int getX()	{return x;}
	int getY()	{return y;}
	int getW()	{return widht;}
	int getH()	{return height;}
	ButtonState getState() {return state;}
	int getHighlighted() {return highlighted;}
	char* getLabel()	{return buttonText;}
	int getID() {return id;}
	
	OnClickButton getFunction() {return onClick;}
	
private:
	int x;
	int y;
	int widht;
	int height;
	char* buttonText;
	int highlighted;
	ButtonState state;
	int id;

	OnClickButton onClick;
};

static vector<Button>buttonList;
static int countOfButton=0;

bool ButtonClickTest(Button* b,int x,int y);
void Font(void *font,char *text,int x,int y);
void ButtonDraw();
int createButton(char *label,OnClickButton func,int x,int y,int w,int h);


void Ready();
void Start();
void Stop();
void ExitFunc();

void ButtonPress(int x,int y);
void ButtonRelease(int x,int y);
void ButtonPassive(int x,int y);

#endif//MYGL_H
