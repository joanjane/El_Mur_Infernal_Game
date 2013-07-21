#include "kinectController.h"

void kinectController::setup() {
	isLive			= true;
	isFiltering		= false;
	isRecording		= false;
	isCloud			= false;
	isCPBkgnd		= true;

	trackingMode    = 2; //1 means tracking with image extracted with thresholds, and 2 from skeleton mask
	
	if(trackingMode == 1){
		isMasking = false;
		isTracking = false;
	} else{
		isMasking = true;
		isTracking = true;
	}

	nearThreshold = 500;
	farThreshold  = 3050;

	filterFactor = 0.1f;
	_isKinectPlugged = false;
	setupRecording();
	_configUIMsg = "";
	ofBackground(0, 0, 0);

	_grabFrame = true;	//this trick will let us to grab 1 every 2 frames in kinect
	_isSkipFrameMode = true;
	_numberOfSkipFrames = 0;
	_maxSkipFrame = 1;
}

void kinectController::setupRecording(string _filename) {
	if(recordContext.setup() && recordDepth.setup(&recordContext) && recordImage.setup(&recordContext)){
		///recordContext.setup();	// all nodes created by code -> NOT using the xml config file at all
		///recordDepth.setup(&recordContext);
		///recordImage.setup(&recordContext);

		recordUser.setup(&recordContext);
		recordUser.setSmoothing(filterFactor);				// built in openni skeleton smoothing...
		recordUser.setUseMaskPixels(isMasking);
		//recordUser.setUseCloudPoints(isCloud);
		recordUser.setMaxNumberOfUsers(1);					// use this to set dynamic max number of users (NB: that a hard upper limit is defined by MAX_NUMBER_USERS in ofxUserGenerator)

		recordContext.toggleRegisterViewport();
		recordContext.toggleMirror();

		_isKinectPlugged = true;
	}
}

void kinectController::update(){
	//depthRangeMask.crop(0,50,depthRangeMask.getWidth(),depthRangeMask.getHeight());
	if(_isSkipFrameMode){
		
		if (_numberOfSkipFrames <= _maxSkipFrame){
			_grabFrame = false;
			_numberOfSkipFrames++;
		} else{
			_grabFrame = true;
			_numberOfSkipFrames = 0;
		}
	}
	if (_grabFrame || !_isSkipFrameMode){
		if(_isKinectPlugged){
			// update all nodes
			recordContext.update();
			recordDepth.update();
			recordImage.update();

			if(trackingMode == 1){
					// demo getting depth pixels directly from depth gen
					depthRangeMask.setFromPixels(recordDepth.getDepthPixels(nearThreshold, farThreshold),
													recordDepth.getWidth(), recordDepth.getHeight(), OF_IMAGE_GRAYSCALE);
			} else{
				// update tracking/recording nodes
				if (isTracking){
					recordUser.update(); 
					// demo getting pixels from user gen
					if (isMasking) { //TODO: switch the two lines above
						allUserMasks.setFromPixels(recordUser.getUserPixels(), recordUser.getWidth(), recordUser.getHeight(), OF_IMAGE_GRAYSCALE);
						//user1Mask.setFromPixels(recordUser.getUserPixels(1), recordUser.getWidth(), recordUser.getHeight(), OF_IMAGE_GRAYSCALE);
					}
				}
			}
		}
	}
	////UI
	string statusSkeleton	= (string)(isTracking ? "TRACKING USERS: " + (string)(ofToString(recordUser.getNumberOfTrackedUsers())) + "" : "NOT TRACKING USERS");
	string statusSmoothSkel = (string)(ofToString(recordUser.getSmoothing()));
	string statusFilter		= (string)(isFiltering ? "FILTERING" : "NOT FILTERING");
	string statusFilterLvl	= ofToString(filterFactor);
	string statusMask		= (string)(!isMasking ? "HIDE" : (isTracking ? "SHOW" : "YOU NEED TO TURN ON TRACKING!!"));
	string statusCloud		= (string)(isCloud ? "ON" : "OFF");
	string statusCloudData	= (string)(isCPBkgnd ? "SHOW BACKGROUND" : (isTracking ? "SHOW USER" : "YOU NEED TO TURN ON TRACKING!!"));
	string trackingModeStr = "F6 - Tracking mode: ";
	if(trackingMode == 1) trackingModeStr += "Depth Threshold Tracking";
	if(trackingMode == 2) trackingModeStr += "Skeleton Mask Tracking.";
	string skipFrameHack		= (string)(_isSkipFrameMode ? "ENABLED" : "DISABLED");
			
	string statusHardware;

	stringstream msg;
	string HackFramesInstructions = "";
	if(_isSkipFrameMode){
		HackFramesInstructions = "Frames Skiped: "+ofToString(_maxSkipFrame)+"\n+ / - to change skip frames";
	}
	if(trackingMode == 2){
		msg	<< "    t : skeleton tracking     : " << statusSkeleton << endl
		<< "( / ) : smooth skely (openni) : " << statusSmoothSkel << endl
		<< "    m : drawing masks         : " << statusMask << endl;
	}
	if(trackingMode == 1) {
		msg << "F7 / F8 - nearThreshold         : " << ofToString(nearThreshold) << endl
			<< "F9 / F10 - farThreshold          : " << ofToString(farThreshold) << endl;
	}
	msg << trackingModeStr << endl
	<< "F12 - Swap the Skipping Frame Hack (for lower PCs) : " << skipFrameHack << endl
	<< HackFramesInstructions << endl
	<< "F2 - to return to the game." << endl
	<< "FPS   : " << ofToString(ofGetFrameRate()) << "  " << statusHardware << endl;
	
	_configUIMsg = msg.str();
}

void kinectController::draw(){
	if(_isKinectPlugged){
		//if (_grabFrame || !_isSkipFrameMode){
			glPushMatrix();
			glScalef(0.75, 0.75, 0.75);
	
			//imatges kinect directa i depth
			//recordDepth.draw(0,0,640,480);
			recordImage.draw(640, 0, 640, 480);

			if (trackingMode == 1){
				/**Opcio 1 per implementacio**/
				
				depthRangeMask.draw(0, 0, 640, 480);	// can use this with openCV to make masks, find contours etc when not dealing with openNI 'User' like objects
			}
			else{
				if (isTracking) {
					//recordUser.draw();
					if (isMasking) drawMasks();
					//if (isCloud) drawPointCloud(&recordUser, 1);	// 0 gives you all point clouds; use userID to see point clouds for specific users
				}
			}
			glPopMatrix();
		//}
		ofSetColor(255, 255, 0);
		ofDrawBitmapString(_configUIMsg, 20, 560);
	}
}

/**
* Draws the mask when tracking users skeletons
* Opció 2
**/
void kinectController:: drawMasks() {
	//glPushMatrix();
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ZERO);
	allUserMasks.draw(0, 0, 640, 480);
	//glDisable(GL_BLEND);
    //glPopMatrix();
	//user1Mask.draw(320, 480, 320, 240);
	//user2Mask.draw(640, 480, 320, 240);
}

//not used
void kinectController::drawPointCloud(ofxUserGenerator * user_generator, int userID) {

	glPushMatrix();

	int w = user_generator->getWidth();
	int h = user_generator->getHeight();

	glTranslatef(w, h/2, -500);
	ofRotateY(pointCloudRotationY);

	glBegin(GL_POINTS);

	int step = 1;

	for(int y = 0; y < h; y += step) {
		for(int x = 0; x < w; x += step) {
			ofPoint pos = user_generator->getWorldCoordinateAt(x, y, userID);
			if (pos.z == 0 && isCPBkgnd) continue;	// gets rid of background -> still a bit weird if userID > 0...
			ofColor color = user_generator->getWorldColorAt(x,y, userID);
			glColor4ub((unsigned char)color.r, (unsigned char)color.g, (unsigned char)color.b, (unsigned char)color.a);
			glVertex3f(pos.x, pos.y, pos.z);
		}
	}

	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);

	glPopMatrix();
}

void kinectController::keyPressed(int key){
	float smooth;
	switch(key){
		case '(':
			smooth = recordUser.getSmoothing();
			if (smooth - 0.1f > 0.0f) {
				recordUser.setSmoothing(smooth - 0.1f);
			}
			break;
		case ')':
			smooth = recordUser.getSmoothing();
			if (smooth + 0.1f <= 1.0f) {
				recordUser.setSmoothing(smooth + 0.1f);
			}
			break;
		case '>':
		case OF_KEY_F10:
			farThreshold += 50;
			if (farThreshold > recordDepth.getMaxDepth()) farThreshold = recordDepth.getMaxDepth();
			break;
		case '<':
		case OF_KEY_F9:
			farThreshold -= 50;
			if (farThreshold < 0) farThreshold = 0;
			break;
		case OF_KEY_F8:
			nearThreshold += 50;
			if (nearThreshold > recordDepth.getMaxDepth()) nearThreshold = recordDepth.getMaxDepth();
			break;
		case OF_KEY_F7:
			nearThreshold -= 50;
			if (nearThreshold < 0) nearThreshold = 0;
			break;
		case 'r':
			recordContext.toggleRegisterViewport();
			break;
		case '+':
			_maxSkipFrame++;
			break;
		case '-':
			_maxSkipFrame--;
			break;
			
	}
}

void kinectController::keyReleased(int key){
	switch (key) {
		case 't':
		case 'T':
			isTracking = !isTracking;
			break;
		case 'm':
		case 'M':
			isMasking = !isMasking;
			recordUser.setUseMaskPixels(isMasking);
			break;
		case 'c':
		case 'C':
			isCloud = !isCloud;
			recordUser.setUseCloudPoints(isCloud);
			break;
		case 'b':
		case 'B':
			isCPBkgnd = !isCPBkgnd;
			break;
		default:
			break;
	}
	setTrackingMode(key);
}

void kinectController::setTrackingMode(int n) {
	if (n == 1 || n == 2){
		trackingMode = n;	//1 o 2
		switch(n){
			case '1':
				trackingMode = 1;
				isTracking = false;
				isMasking = false;
				recordUser.setUseMaskPixels(isMasking);
				break;
			case '2':
				trackingMode = 2;
				isTracking = true;
				recordUser.setMaxNumberOfUsers(1);
				recordUser.setSmoothing(filterFactor);
				isMasking = true;
				recordUser.setUseMaskPixels(isMasking);
				break;
		}
	}
}

int kinectController::getTrackingMode() {
	return trackingMode;
}

ofImage* kinectController::getMaskImage() {
	if(trackingMode == 1) {
		return &depthRangeMask;
	} else {
		return &allUserMasks;
	}
}

string kinectController::generateFileName() {

	string _root = "kinectRecord";

	string _timestamp = ofToString(ofGetDay()) +
	ofToString(ofGetMonth()) +
	ofToString(ofGetYear()) +
	ofToString(ofGetHours()) +
	ofToString(ofGetMinutes()) +
	ofToString(ofGetSeconds());

	string _filename = (_root + _timestamp + ".oni");

	return _filename;

}

string& kinectController::getKinectConfigUI(){
	return _configUIMsg;
}
void kinectController::swapTracingMode(){
	if (trackingMode == 1)  trackingMode = 2;	//skeleton
	else					trackingMode = 1;	//depth threshold

	if(trackingMode == 1){
		isMasking = false;
		isTracking = false;
		recordUser.setUseMaskPixels(isMasking);

	} else{
		isMasking = true;
		isTracking = true;
		recordUser.setUseMaskPixels(isMasking);
		recordUser.setMaxNumberOfUsers(1);
		recordUser.setSmoothing(filterFactor);
	}
}