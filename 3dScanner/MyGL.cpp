#include "MyGL.h"
#include <GL/freeglut.h>
//#include <freeglut.h>
#include <string>
#include <ctime>
//#include "PlatformManager.h"



Settings* Settings::self = NULL;
KScanner   scanner;

//PlatformManager menageSerialPort;
ModelMaker pointsArray[_NUMBER_OF_FRAME];

int _FRAME_COUNT = 0;  // kac tane frame varsa o kadar birlestir



/*	This function will be used to draw the 3D scene*/
void Draw3D()
{
	//glutSolidCone(1,2,3,4);
	//glutSolidTeapot(1);
	//glutSolidCylinder(1,2,3,4);
}

/* render the scene */
void Settings::display() {
 

	glClear( GL_COLOR_BUFFER_BIT |
			 GL_DEPTH_BUFFER_BIT );

	/*
	 *	Enable lighting and the z-buffer
	 */
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_LIGHTING);

	/*
	 *	Set perspective viewing transformation
	 */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();


	/*
		 waiting for next frame 
		self->streaming && 
	*/
	sleep(1);  // uyusunda buyusun ...
	if(scanner.getInited() && _FRAME_COUNT < _NUMBER_OF_FRAME){

		//menageSerialPort.step(BACKWARD);

		ModelMaker kinectData;
		cout << "kinectData count " << mPoints.count << endl;
		scanner.writeFrameIntoArray(&kinectData);
		cout << "kinectData count " << mPoints.count << endl;

		pointsArray[_FRAME_COUNT] = kinectData;

		_FRAME_COUNT++;
		cout << "Frame  "<<_FRAME_COUNT << endl;
		self->streaming = !self->streaming;
		initAll();
	}
	
	
	
	gluPerspective(45,(winh==0)?(1):((float)winw/winh),1,100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	



	float ax= cx/180.0f * 3.14;
	float ay= cy/180.0f * 3.14;
	int pSize = 8;
	gluLookAt(	
		camPos.x*pSize + camR*cos(ay)*cos(ax), 
		camPos.y*pSize + camR*sin(ay), 
		camPos.z*pSize + camR*cos(ay)*sin(ax),

		camPos.x*pSize, 
		camPos.y*pSize,  
		camPos.z*pSize,
				
		0.0f, 1.0f,  0.0f);

	
	glBegin(GL_POINTS);
	for(int i = 0; i < mPoints.count; i+=5){
		Point3D p = mPoints[i];

        glColor3f(0.0,0.0,1.0 );
		//glColor3f( p.getColor()->r/255.0, p.getColor()->g/255.0, p.getColor()->b/255.0 );
		glVertex3f( p.getPos()->x*pSize, p.getPos()->y*pSize, p.getPos()->z*pSize);

		//cout << p->x << endl;
	}
	glEnd();


	glPointSize(3);
	
	glBegin(GL_POINTS);
		glColor3f(1,0,0);
		glVertex3f( origin.x*pSize, origin.y*pSize, origin.z*pSize);
	glEnd();
	
	
	/*
	 *	Disable depth test and lighting for 2D elements
	 */
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	/*
	 *	Set the orthographic viewing transformation
	 */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,winw,winh,0,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/*
	 *	Draw Button
	 */
	ButtonDraw();
	
	/*
	 *	Bring the back buffer to the front and vice-versa.
	 */
	glutSwapBuffers();
}

/* reshaped window */
void reshape(int width, int height) {
 
  GLfloat fieldOfView = 90.0f;
  glViewport (0, 0, (GLsizei) width, (GLsizei) height);
 
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(fieldOfView, (GLfloat) width/(GLfloat) height, 0.1, 500.0);
 
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}


void Settings::run(){
	glutMainLoop();
}

Settings::Settings()
{
	self=this;

	origin= Vector3D( _ORI_X, _ORI_Y,_ORI_Z);

	camRotating=false;
	camMoving=false;
	camR = 4;
	camPos= origin;
	streaming=false;

	if (scanner.getInited())
		initAll();
}

void Settings::initAll(){
	
	// not init oldu mu?
	if (scanner.getInited()/* && _FRAME_COUNT < _NUMBER_OF_FRAME*/){
		mPoints.count=0;
		for (int i = 0; i < _NUMBER_OF_FRAME;++i){
			
			readFrameFromFile(pointsArray[i], i * _RADIAN15);
		}

		final3DPointsToFile(mPoints);
	}
}

ModelMaker& Settings::getMpoint(){

	return mPoints;
}

void Settings::setMPoint(ModelMaker& mp)
{
	mPoints = mp;
}

void Settings::readFrameFromFile(ModelMaker& point3d,float angle){

	ModelMaker desingnedMp;

	desingnedMp = point3d;

	rotatingWithAngle(desingnedMp, angle);

	mPoints += desingnedMp;

	//printf("reading count:%d\n",count);
}

void Settings:: final3DPointsToFile(ModelMaker& point){

	FILE *inp;

	inp=fopen("finalPoints.asc","w");

	if(inp==NULL){
		printf("cannot open \"%s\" file\n", "finalPoints.asc");
	}
	Vector3D *pos;
	for(int i=0;i<point.count;++i){
		pos=point.data[i].getPos();
		fprintf(inp,"%7.4lf  %7.4lf  %7.4lf\n",pos->x,pos->y,pos->z);
	}

	fclose(inp);
}

void Settings::rotatingWithAngle(ModelMaker &temp,float angle){

	Vector3D axis(0,1,0);

	for(int i=0;i < temp.count;++i){
		Vector3D *pos=temp.data[i].getPos();
		Color24 *col = temp.data[i].getColor();
		
		Vector3D nokta(pos->x, pos->y, pos->z);

		nokta-= origin;

		pos->x = nokta.x;
		pos->y = nokta.y;
		pos->z = nokta.z;

		Vector3D newPos=pos->RotateAboutAxis(-angle, axis) + origin;
		//newPos += origin;
		Point3D newPoint(newPos);

		newPoint.getColor()->r= col->r;
		newPoint.getColor()->g= col->g;
		newPoint.getColor()->b= col->b;
		temp.data[i]=newPoint;
	}
}

void Settings::initOpenGLHooks(){

	glutKeyboardFunc(keyboardDown);
	glutKeyboardUpFunc(keyboardUp);
	glutSpecialFunc(keyboardSpecialDown);
	glutSpecialUpFunc(keyboardSpecialUp);
	glutDisplayFunc(glutDisplay);
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMotion);
	glutPassiveMotionFunc(mousePassiveMotion);
	glutIdleFunc(idle);
	//glutMouseWheelFunc(mouseWheelFunc);
	initGL(800, 600);

	/* Background color */
	glClearColor (0.1, 0.5, 0.2, 0.1);	
}

void Settings::initOpenGL(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(winw,winh);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("GROUP5 SCANNER");
	glutReshapeFunc (reshape);
	initOpenGLHooks();
	glEnable(GL_DEPTH_TEST);
	
}

/* executed when a regular key is pressed */
void Settings:: keyboardDown(unsigned char key, int x, int y) {
 
  switch(key) {
  case  27:   // ESC
    exit(0);
  case ' ':
	  self->streaming= !self->streaming;
	  break;
  }

}

/* executed when a regular key is released */
void Settings:: keyboardUp(unsigned char key, int x, int y) {
 
}
 
/* executed when a special key is pressed */
void Settings:: keyboardSpecialDown(int k, int x, int y) {
	
	if(k== GLUT_KEY_UP){
		self->origin.x += _SENSITIVE;
	} 
	if(k== GLUT_KEY_DOWN){
		self->origin.x -= _SENSITIVE;
	} 

	if(k== GLUT_KEY_LEFT){
		self->origin.z += _SENSITIVE;
	} 
	if(k== GLUT_KEY_RIGHT){
		self->origin.z -= _SENSITIVE;
	} 

	cout << self->origin.x << " " << self->origin.z << endl;
	self->initAll();

}
 
/* executed when a special key is released */
void Settings::keyboardSpecialUp(int k, int x, int y) {
 
}

/* executed when button 'button' is put into state 'state' at screen position ('x', 'y') */
void Settings:: mouseClick(int button, int state, int x, int y) {
	/*
	 *	update the mouse position
	 */
	
	/*
	 *	has the button been pressed or released?
	 */
	if (state == GLUT_DOWN) 
	{

		/*
		 *	Which button was pressed?
		 */
		switch(button) 
		{
		case GLUT_LEFT_BUTTON:
			
			
			for(int i=0;i<countOfButton;++i){
		
				if(ButtonClickTest( &buttonList[i], x, y )){
				
					OnClickButton oc= buttonList[i].getFunction();
					oc();

					return;
				}

			}

			self->camRotating=true;
			self->dragLastX=x;
			self->dragLastY=y;
		case GLUT_MIDDLE_BUTTON:
			break;
		case GLUT_RIGHT_BUTTON:
			self->camMoving= true;
			self->dragLastX = x;
			self->dragLastY = y;
			
			break;
		}
	}
	else 
	{
		
		/*
		 *	Which button was released?
		 */
		switch(button) 
		{
		case GLUT_LEFT_BUTTON:
			
			self->camRotating=false;
			break;
		case GLUT_MIDDLE_BUTTON:
			break;
		case GLUT_RIGHT_BUTTON:
			self->camMoving=false;
			break;
		}
	}

	/*
	 *	Force a redraw of the screen. If we later want interactions with the mouse
	 *	and the 3D scene, we will need to redraw the changes.
	 */
	glutPostRedisplay();
}
 
/* executed when the mouse moves to position ('x', 'y') */
void Settings:: mouseMotion(int x, int y) {
	
	int dx= x - self->dragLastX;
	int dy= y - self->dragLastY;

	self->dragLastX = x;
	self->dragLastY = y;

	if(self->camRotating){
		float speed= 0.1f;
		self->cx+= dx*speed;
		self->cy+= dy*speed;
	}
	ButtonPassive(x,y);
	glutPostRedisplay();
}


void Settings::mousePassiveMotion(int x,int y)
{
	ButtonPassive(x,y);
}

/* executed when program is idle */
void Settings::idle() { 

	glutPostRedisplay();

}

/* initialize OpenGL settings */
void Settings::initGL(int width, int height) {

	glEnable(GL_LIGHT0);
	createButton("READY", Ready,10,10,100,30);
	createButton("START", Start,10,45,100,30);
	createButton("STOP",  Stop,10,80,100,30);
	createButton("EXIT",  ExitFunc,10,115,100,30);
}

void Settings::glutDisplay(){

	Settings::self->display();
}

void Settings::init(int argc, char **argv){

	cx=0;
	cy=0;
	
	initOpenGL(argc, argv);
}
/*
void Settings::mouseWheelFunc(int wheel, int direction, int x, int y){
	self->camR -= direction;
	
	if( self->camR > 10 ){
		self->camR = 10;
	}

	if( self->camR < 2 ){
		self->camR = 2;
	}
}
*/

/*Button events */

Button::Button(int newX,int newY,int newW,int newH,char* newText,OnClickButton func)
	:x(newX),y(newY),widht(newW),height(newH),onClick(func)
{
	buttonText=new char[strlen(newText)+1];
	strcpy(buttonText,newText);
}

int createButton(char *label,OnClickButton func,int x,int y,int w,int h)
{
	Button newButton=Button(x,y,w,h,label,func);

	buttonList.push_back(newButton);
	countOfButton++;
	newButton.setID(countOfButton);
	
	return countOfButton;
}

void ButtonDraw()
{
	int fontx;
	int fonty;

	for(int i=0;i<countOfButton;++i){
		
		if(buttonList[i].getHighlighted())
			glColor3f(0.4f,0.7f,0.8f);
		else 
			glColor3f(0.6f,0.6f,0.6f);

		glBegin(GL_QUADS);
			glVertex2i(buttonList[i].getX(),buttonList[i].getY());
			glVertex2i(buttonList[i].getX(),buttonList[i].getY()+buttonList[i].getH());
			glVertex2i(buttonList[i].getX()+buttonList[i].getW(),buttonList[i].getY()+buttonList[i].getH());
			glVertex2i(buttonList[i].getX()+buttonList[i].getW(),buttonList[i].getY());
		glEnd();

		/*
		 *	Draw an outline around the button with width 3
		 */
		glLineWidth(3);

		if(buttonList[i].getState()==PRESS)
			glColor3f(0.4f,0.4f,0.4f);
		else 
			glColor3f(0.8f,0.8f,0.8f);

		glBegin(GL_LINE_STRIP);
			glVertex2i(buttonList[i].getX() + buttonList[i].getW(),buttonList[i].getY());
			glVertex2i(buttonList[i].getX(),buttonList[i].getY());
			glVertex2i(buttonList[i].getX(),buttonList[i].getY()+buttonList[i].getH());
		glEnd();

		if(buttonList[i].getState()==PRESS)
			glColor3f(0.8f,0.8f,0.8f);
		else 
			glColor3f(0.4f,0.4f,0.4f);
	
		glBegin(GL_LINE_STRIP);
			glVertex2i(buttonList[i].getX(),buttonList[i].getY()+buttonList[i].getH());
			glVertex2i(buttonList[i].getX()+buttonList[i].getW(),buttonList[i].getY()+buttonList[i].getH());
			glVertex2i(buttonList[i].getX()+buttonList[i].getW(),buttonList[i].getY());
		glEnd();

		glLineWidth(1);

		fontx=buttonList[i].getX() + (buttonList[i].getW() - glutBitmapLength(GLUT_BITMAP_HELVETICA_10,(const unsigned char*)buttonList[i].getLabel())) / 2;
		fonty = buttonList[i].getY() + (buttonList[i].getH()+10)/2;

		if(buttonList[i].getState()==PRESS){
			fontx+=2;
			fonty+=2;
		}

		if(buttonList[i].getHighlighted()){
			glColor3f(0,0,0);
			Font(GLUT_BITMAP_HELVETICA_10,buttonList[i].getLabel(),fontx,fonty);
			fontx--;
			fonty--;
		}

		glColor3f(1,1,1);
		Font(GLUT_BITMAP_HELVETICA_10,buttonList[i].getLabel(),fontx,fonty);

	}
}

/*----------------------------------------------------------------------------------------
 *	\brief	This function draws a text string to the screen using glut bitmap fonts.
 *	\param	font	-	the font to use. it can be one of the following : 
 *
 *					GLUT_BITMAP_9_BY_15		
 *					GLUT_BITMAP_8_BY_13			
 *					GLUT_BITMAP_TIMES_ROMAN_10	
 *					GLUT_BITMAP_TIMES_ROMAN_24	
 *					GLUT_BITMAP_HELVETICA_10	
 *					GLUT_BITMAP_HELVETICA_12	
 *					GLUT_BITMAP_HELVETICA_18	
 *
 *	\param	text	-	the text string to output
 *	\param	x		-	the x co-ordinate
 *	\param	y		-	the y co-ordinate
 */
void Font(void *font,char *text,int x,int y)
{
	glRasterPos2i(x, y);

	while( *text != '\0' )
	{
		glutBitmapCharacter( font, *text );
		++text;
	}
}


 /*----------------------------------------------------------------------------------------
 *	This is an example callback function. Notice that it's type is the same
 *	an the ButtonCallback type. We can assign a pointer to this function which
 *	we can store and later call.
 */
void  Ready()
{
	cout<<"Ready for scanning..."<<endl;
	_FRAME_COUNT = 0;
	scanner.init();
	scanner.setInited(false);		
	//system("b1.exe");


	//readFile(pointsArray);
}

void Start()
{
	if (scanner.getInited()== false){

		scanner.setInited(true);
		//system("b1.exe");
	}
	else 
		cout << "Press READY ..\n";
}
void Stop(){
	//cout<<"Stop Button"<<endl;
	
	cout<<"Disconnect to Kinect..."<<endl;
	scanner.close();
	scanner.setInited(false);

}
void ExitFunc(){
	
	scanner.close();
	cout<<"Application will close"<<endl;
	//Sleep(3000);
	exit(1);

}
/*----------------------------------------------------------------------------------------
 *	\brief	This function draws the specified button.
 *	\param	b	-	a pointer to the button to check.
 *	\param	x	-	the x location of the mouse cursor.
 *	\param	y	-	the y location of the mouse cursor.
 */
void ButtonPress(int x,int y)
{
	
	for(int i=0;i<countOfButton;++i){
	
		if(ButtonClickTest(&buttonList[i],x,y))
			buttonList[i].setState(PRESS);

	}
}

void readFile(ModelMaker array[]){
	

	for (int i = 0; i < _NUMBER_OF_FRAME; ++i)
	{

	
		FILE *inp;
		char file[40];
		sprintf(file,"%s%d",_POINT_FILE_NAME,i);


		inp=fopen(file,"r");
		
		if(inp==NULL){
			printf("cannot open file\n");
		}


		float x,y,z;
		int r,g,b;
		int count=0;
		ModelMaker pTemp;

		while(true){

			if(fscanf(inp,"%f%f%f%d%d%d",&x,&y,&z, &r,&g,&b)<1)
				break;

			Point3D p(x,y,z,r,g,b);

			pTemp.data[pTemp.count++] = p;			
			count++;
		}

		array[i] = pTemp;

		//char n[40];
		//sprintf(n, "%d", i);
	//	printFile(array[i],n);
		printf("Toplam point:%d\n",count);
	}

}
void printFile(ModelMaker& mp, string fName)
{

		FILE *inp;

		inp=fopen(fName.c_str(),"w");

		if(inp==NULL){
			printf("cannot open \"%s\" file\n", "finalPoints.asc");
		}
		Vector3D *pos;
		Color24  *col;
		for(int i=0;i<mp.count;++i){
			pos = mp.data[i].getPos();
			col=  mp.data[i].getColor();
			fprintf(inp,"%7.4lf  %7.4lf %7.4lf %7d %7d %7d\n",pos->x,pos->y,pos->z,col->r,col->g,col->b);
		}

		fclose(inp);
}


/*----------------------------------------------------------------------------------------
 *	\brief	This function is used to see if a mouse click or event is within a button 
 *			client area.
 *	\param	b	-	a pointer to the button to test
 *	\param	x	-	the x coord to test
 *	\param	y	-	the y-coord to test
 */
bool ButtonClickTest(Button* b,int x,int y) 
{
	if(b){
		/*
			*	If clicked within button area, then return true
			*/
		if( x > b->getX() && x < b->getX()+b->getW() && y > b->getY() &&  y < b->getY()+b->getH() ){

			return true;
	
		}
	}
		

	return false;
}

/*----------------------------------------------------------------------------------------
 *	\brief	This function draws the specified button.
 *	\param	b	-	a pointer to the button to check.
 *	\param	x	-	the x location of the mouse cursor.
 *	\param	y	-	the y location of the mouse cursor.
 */
void ButtonRelease(int x,int y)
{

	for(int i=0;i<countOfButton;++i){

		if(ButtonClickTest(&buttonList[i],x,y)){

			if(buttonList[i].getFunction()){
				buttonList[i].getFunction()();
			}
		}

		buttonList[i].setState(NOTPRESS);

	}

}

/*----------------------------------------------------------------------------------------
 *	\brief	This function draws the specified button.
 *	\param	b	-	a pointer to the button to check.
 *	\param	x	-	the x location of the mouse cursor.
 *	\param	y	-	the y location of the mouse cursor.
 */
void ButtonPassive(int x,int y)
{
	int needRedraw = 0;
	for (int i = 0; i < countOfButton; i++)
	{
		
		if(ButtonClickTest(&buttonList[i],x,y)){
			if(buttonList[i].getHighlighted()==0){
				buttonList[i].setHighlighted(1);
				needRedraw=1;
			}
		}else{

			if(buttonList[i].getHighlighted()==1){
				buttonList[i].setHighlighted(0);
				needRedraw=1;
			}
			
		}
	}

	if (needRedraw) {
		glutPostRedisplay();
	}
}

