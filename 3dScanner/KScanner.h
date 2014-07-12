#ifndef KSCANNER_H
#define KSCANNER_H

#include <XnOS.h>
#if (XN_PLATFORM == XN_PLATFORM_MACOSX)
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif
#include <math.h>

#include <XnCppWrapper.h>
#include <iostream>
//#include <vector>
//#include "Point3D.h"
#include "ModelMaker.h"
using namespace xn;
using namespace std;


#define SAMPLE_XML_PATH "../../Config/SamplesConfig.xml"


class KScanner{
public:
	KScanner();

	bool init();
	bool getInited();
	void setInited(bool cond);
	bool writeFrameIntoArray(ModelMaker *pa);

	void close();
private:


	float* g_pDepthHist;
	XnDepthPixel g_nZRes;

	Context g_context;
	ScriptNode g_scriptNode;
	DepthGenerator g_depth;
	ImageGenerator g_image;
	DepthMetaData g_depthMD;
	ImageMetaData g_imageMD;

	/*

	openni::Device device;
	openni::VideoStream depthStream;
	openni::VideoStream colorStream;

	openni::VideoFrameRef depthFrame;
	openni::VideoFrameRef colorFrame;

	float depthHist[MAX_DEPTH];*/
	int width;
	int height;

	bool inited;
};

#endif
