/* 
 * Turan Son Gece :) uykulu 
 */

#include "KScanner.h"

using namespace std;

//---------------------------------------------------------------------------
// Code
//---------------------------------------------------------------------------
KScanner::KScanner(){
	inited=false;
}
void KScanner:: close(){
    
}
bool KScanner::writeFrameIntoArray(ModelMaker *mp)
{
    
    if (!inited){
    
        cout << "Waiting for connection\n";
        return false;
    }
   
    float px, py,pz;
    
	XnStatus rc = XN_STATUS_OK;

	// Read a new frame
	rc = g_context.WaitAnyUpdateAll();
	if (rc != XN_STATUS_OK)
	{
		printf("Read failed: %s\n", xnGetStatusString(rc));
		return false;
	}
 
	g_depth.GetMetaData(g_depthMD);
	g_image.GetMetaData(g_imageMD);

	const XnDepthPixel* pDepth = g_depthMD.Data();


	// Calculate the accumulative histogram (the yellow display...)
	//xnOSMemSet(g_pDepthHist, 0, g_nZRes*sizeof(float));

	int count = 0;
	for (XnUInt y = 0; y < g_depthMD.YRes(); ++y)
	{
		for (XnUInt x = 0; x < g_depthMD.XRes(); ++x, ++pDepth)
		{
			if (*pDepth != 0 && *pDepth < 800)
			{
			    if (x > 250 && x <  450 ){
				     px= -(x/500.0f-0.5f);
				     py= -(y/500.0f-0.5f);
				     pz= -(*pDepth) /1000.0f;
				     				          
                     Point3D p(px, py, pz, 0, 0, 0);
                     mp->data[count]= p;

				     count++;
				}
			}
		}
	}
	
	mp->count = count;
	
    printf("points size :  %5d\n",count );
	//xnOSMemSet(g_pTexMap, 0, g_nTexMapX*g_nTexMapY*sizeof(XnRGB24Pixel));
  /*  for (int i =0; i < 100; ++i)
    {
        Vector3D *pos = mp->data[i].getPos();
        printf("%f   %f   %f\n",pos->x, pos->y,pos->z);
    }
    */
    return true;
}


bool KScanner:: getInited(){
    return inited;
}
void KScanner:: setInited(bool cond){
    inited = cond;
}

bool KScanner::init(){
    
    XnStatus rc;
    cout << "Debug INIT Scanner...\n";
	EnumerationErrors errors;
	rc = g_context.InitFromXmlFile(SAMPLE_XML_PATH, g_scriptNode, &errors);
	if (rc == XN_STATUS_NO_NODE_PRESENT)
	{
		XnChar strError[1024];
		errors.ToString(strError, 1024);
		printf("%s\n", strError);
		return false;
	}
	else if (rc != XN_STATUS_OK)
	{
		printf("Open failed: %s\n", xnGetStatusString(rc));
		return false;
	}

	rc = g_context.FindExistingNode(XN_NODE_TYPE_DEPTH, g_depth);
	if (rc != XN_STATUS_OK)
	{
		printf("No depth node exists! Check your XML.");
		return false;
	}

	rc = g_context.FindExistingNode(XN_NODE_TYPE_IMAGE, g_image);
	if (rc != XN_STATUS_OK)
	{
		printf("No image node exists! Check your XML.");
		return false;
	}

	g_depth.GetMetaData(g_depthMD);
	g_image.GetMetaData(g_imageMD);

	// Hybrid mode isn't supported in this sample
	if (g_imageMD.FullXRes() != g_depthMD.FullXRes() || g_imageMD.FullYRes() != g_depthMD.FullYRes())
	{
		printf ("The device depth and image resolution must be equal!\n");
		return false;
	}

	// RGB is the only image format supported.
	if (g_imageMD.PixelFormat() != XN_PIXEL_FORMAT_RGB24)
	{
		printf("The device image format must be RGB24\n");
		return false;
	}

    return true;
}
