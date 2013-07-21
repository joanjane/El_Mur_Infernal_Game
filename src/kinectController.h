#ifndef _KINNECT_CONTROLLER
#define _KINNECT_CONTROLLER

//#define USE_IR // Uncomment this to use infra red instead of RGB cam...

#include "ofxOpenNI.h"
#include "ofMain.h" //COMENTED

class kinectController : public ofBaseApp{
//class kinectController {
public:
	void setup();
	void update();
	void draw();

	void keyPressed  (int key);
	void keyReleased(int key);

	ofImage* getMaskImage();
	void setTrackingMode(int n);
	int getTrackingMode();
	string& getKinectConfigUI();

	void	setupRecording(string _filename = "");
	string	generateFileName();
	bool isKinectPlugged(){return _isKinectPlugged;}
	void swapTracingMode();
	void skipFrameMode(bool b=false){ _isSkipFrameMode = b;}
	void swapSkipFrameMode(){ _isSkipFrameMode = !_isSkipFrameMode;}
	bool isSkipFrameMode(){ return _isSkipFrameMode;}
	void saveImageForCaptures(){
		if(trackingMode == 1){
			depthRangeMask.saveImage("/capturesFromKinect/"+ofGetTimestampString()+".png");
		} else{
			allUserMasks.saveImage("/capturesFromKinect/"+ofGetTimestampString()+".png");
		}
	}

private:

	bool				isLive, isTracking, isRecording, isCloud, isCPBkgnd, isMasking;
	bool				isTrackingHands, isFiltering;

	ofxOpenNIContext	recordContext;
	ofxDepthGenerator	recordDepth;

#ifdef USE_IR
	ofxIRGenerator		recordImage;
#else
	ofxImageGenerator	recordImage;
#endif

	ofxUserGenerator	recordUser;

	void				drawMasks();
	void				drawPointCloud(ofxUserGenerator * user_generator, int userID);

	int					nearThreshold, farThreshold;
	int					pointCloudRotationY;

	ofImage				allUserMasks, user1Mask, user2Mask, depthRangeMask;

	float				filterFactor;

	int                 trackingMode;
	string				_configUIMsg;
	bool				_isKinectPlugged;
	bool				_grabFrame;
	bool				_isSkipFrameMode;
	int					_numberOfSkipFrames;
	int					_maxSkipFrame;

};

#endif
