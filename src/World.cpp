#include "World.h"
#include <glut.h>
void World::setup(){
	ofHideCursor();
    ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetFrameRate(40);

	glEnable(GL_DEPTH_TEST);
	
    //lights
    glShadeModel(GL_SMOOTH);
    light.enable();
	light.setPosition(10,50,-50);
	light.setAttenuation(1.f,0.1f,0.f);
	light2.setPosition(10,-27,88);
	light2.setAttenuation(0.5f,0.1f,0.f);
	light2.disable();
    //ofEnableSeparateSpecularLight();

	////////////////Camera////////////////
	ofBackground(0, 0, 0, 0);   

	cam.setNearClip(0.1);
	cam.setFarClip(1000);
	//cam.setPosition(4,6,-16);//2,6,16
	int x = (0.4 * GRID_COLS)/2;
	cam.setPosition(x,6,-8);//x,6,-13
	if( GRID_COLS == 26 && GRID_ROWS==18 )	cam.setPosition(5,10,-13);//for 26x18 resolution use 5, 10, -13
	cam.lookAt(ofVec3f(x,0,20));
	////////////////Camera////////////////

	_rm = new MyResourceManager();	//serveix per controlar les posicions de les parets i obtenir els models
#ifdef DEBUG_TOOLS
	_player = new Player(60,80, &_rm->getModelByName("cube"), ofVec3f( -10, 0, 0), "yellow");
#else
	_player = new Player(GRID_ROWS, GRID_COLS, &_rm->getModelByName("cube"), ofVec3f( 0, 0, 0), "blue");
#endif
	loadWallsFromXML();

	_isKinectViewEnabled = false;
	_wallsCount = 0;
	_distanceOfWallFromPlayer = -1;
	_debugMessages = false;
	_lastRandomWallIndex = -1;

	_evenOdd = false;
	_startGame = false;

	_view = INSTRUCTIONS_VIEW;
	_lastView = _view;

	_barColor = ofColor (162,2,26);
	_timer = -1;
	_simulateEnd = false;
#ifdef DEBUG_TOOLS
	_player->getUserMapRef()->getKinectControllerRef()->setTrackingMode(1);
	setTempoModulation("H-M-L");
	_view = END_GAME_VIEW;
	_view = FIRST_PERSON_VIEW;
#endif
}

void World::update(){
	_player->update();
	updateCollisions();
	wallsController();
	tempo.updateTime();
}

void World::draw(){
	launchView();
}

void World::keyPressed(int key){
	if (View() == KINECT_VIEW)
		_player->getUserMapRef()->getKinectControllerRef()->keyPressed(key);	//pass the keys to the kinect controller
	ofVec3f p;
	int moveAmmount = 1;
    switch (key) {
		case 'f':
		case OF_KEY_F11:	//F11 key toggles fullscreen
			ofToggleFullscreen();
			break;
		case OF_KEY_F1:
			_debugMessages = !_debugMessages;
			break;
#ifdef DEBUG_TOOLS
		case ',':
			if (_view < END_GAME_VIEW)
				_view++;
			break;
		case '.':
			if (_view >= START_MENU_VIEW)
				_view--;
			break;
#endif
        default:
            break;
    }
#ifdef DEBUG_TOOLS
		if(View() == GAME_VIEW || View() == END_GAME_VIEW || View() == FIRST_PERSON_VIEW){
			switch (key) {
				//case 'q':
				//case 'Q':
				case OF_KEY_PAGE_DOWN:
					p = cam.getPosition() + ofVec3f(0,0,moveAmmount);
					cam.setPosition(p);
					break;
				//case 'e':
				//case 'E':
				case OF_KEY_PAGE_UP:
					p = cam.getPosition() + ofVec3f(0,0,-moveAmmount);
					cam.setPosition(p);
					break;
				//case 'd':
				//case 'D':
				case OF_KEY_LEFT:
					p = cam.getPosition() + ofVec3f(moveAmmount,0,0);
					cam.setPosition(p);
					break;
				//case 'a':
				//case 'A':
				case OF_KEY_RIGHT:
					p = cam.getPosition() + ofVec3f(-moveAmmount,0,0);
					cam.setPosition(p);
					break;
				//case 'w':
				//case 'W':
				case OF_KEY_UP:
					p = cam.getPosition() + ofVec3f(0,moveAmmount,0);
					cam.setPosition(p);
					break;
				//case 's':
				//case 'S':
				case OF_KEY_DOWN:
					p = cam.getPosition() + ofVec3f(0,-moveAmmount,0);
					cam.setPosition(p);
					break;
				case 'c':
				case 'C':
					cam.lookAt(_player->getPosition()+ofVec3f(5,0,0));
					break;
				case 'l':	//look at the avatar
					cam.setPosition(ofVec3f(10,5,-17));
					break;
			}
			cout	<< "Camera position: " << cam.getPosition().x << ", " << cam.getPosition().y << ", " << cam.getPosition().z << endl
					<< "view vector: " << cam.getLookAtDir().x << ", "<< cam.getLookAtDir().y << ", " << cam.getLookAtDir().z << endl;
		}/* else if(View() == END_GAME_VIEW){
			switch (key) {
				case OF_KEY_BACKSPACE:
					light2.lookAt(_player->getPosition()+ofVec3f(5,0,0));
					break;
				//case 'q':
				//case 'Q':
				case OF_KEY_PAGE_DOWN:
					p = light2.getPosition() + ofVec3f(0,0,moveAmmount);
					light2.setPosition(p);
					break;
				//case 'e':
				//case 'E':
				case OF_KEY_PAGE_UP:
					p = light2.getPosition() + ofVec3f(0,0,-moveAmmount);
					light2.setPosition(p);
					break;
				//case 'd':
				//case 'D':
				case OF_KEY_LEFT:
					p = light2.getPosition() + ofVec3f(moveAmmount,0,0);
					light2.setPosition(p);
					break;
				//case 'a':
				//case 'A':
				case OF_KEY_RIGHT:
					p = light2.getPosition() + ofVec3f(-moveAmmount,0,0);
					light2.setPosition(p);
					break;
				//case 'w':
				//case 'W':
				case OF_KEY_UP:
					p = light2.getPosition() + ofVec3f(0,moveAmmount,0);
					light2.setPosition(p);
					break;
				//case 's':
				//case 'S':
				case OF_KEY_DOWN:
					p = light2.getPosition() + ofVec3f(0,-moveAmmount,0);
					light2.setPosition(p);
					break;
			}
		}*/
#endif
}

void World::keyReleased(int key){
	if (View() == KINECT_VIEW)
		_player->getUserMapRef()->getKinectControllerRef()->keyReleased(key);
	switch(key){
		case OF_KEY_F6:
			_player->getUserMapRef()->getKinectControllerRef()->swapTracingMode();
			break;
		case OF_KEY_F12:
			_player->getUserMapRef()->getKinectControllerRef()->swapSkipFrameMode();
			break;
	}
	if(View() == GAME_VIEW){
		switch(key){
		case ' ':
			_startGame = true;
			break;
		case 'r':
			break;
		case OF_KEY_F3:
			messages.swapExtraMessages();
			break;
		case OF_KEY_F4:	
			setView(END_GAME_VIEW);
			_simulateEnd = true;
			break;
		case 's':
			messages.swapSound();
			//messages.increaseMessagePerformanceLoop();
			break;
		case 'p':
			pauseResumeGame();
			break;
		/*case 'M':
			messages.decreaseMessagePerformanceLoop();
			break;*/
		}
	} else if(View() == START_MENU_VIEW){
		switch(key){
		case 'e':
			loadWallsFromXML("postures1.0.xml");
			break;
		case 'h':
			loadWallsFromXML("postures.xml");
			break;
		case 'c':
			_view = CAPTURE_POSTURE_VIEW;
			break;
		case '1':
			setTempoModulation("H-M-L");
			_log.modulationType("HML_");
			break;
		case '2':
			setTempoModulation("M-H-L");
			_log.modulationType("MHL_");
			break;
		case '3':
			setTempoModulation("L-H-M");
			_log.modulationType("LHM_");
			break;
		/*case '4':
			setTempoModulation("L-M-H");
			_log.modulationType("LMH_");
			break;*/
		}
	} else if(View() == CAPTURE_POSTURE_VIEW){
		if(key == ' '){
			//_player->keyPressed(key);	//space saves posture
			_player->savePosture("postures-captured.xml");
			_player->getUserMapRef()->getKinectControllerRef()->saveImageForCaptures(); //MAYBE USED IN FUTURE
		}
	} else if(View() == INSTRUCTIONS_VIEW){
		switch(key){
		case ' ':
		case OF_KEY_RETURN:
		case OF_KEY_PAGE_DOWN:
			_instructions.nextSlide();
			//glutTimerFunc(2000,test,GAME_VIEW);
			break;
		case OF_KEY_BACKSPACE:
		case OF_KEY_PAGE_UP:
			_instructions.prevSlide();
			break;
		case OF_KEY_F5:
		case 's':
			_instructions.skipAll();
			break;
		}
	} else if (View() == END_GAME_VIEW){
		switch(key){
			case OF_KEY_F4:		//force end
				_walls.clear();
				_screenWalls.clear();
				ofExit();
				break;
			case OF_KEY_F1:
				setView(FIRST_PERSON_VIEW);
				break;
		}
	}
	if (_view != CAPTURE_POSTURE_VIEW || _view != INSTRUCTIONS_VIEW || _view != END_GAME_VIEW){
		switch(key){
		case '0':
		case OF_KEY_F2:
			_isKinectViewEnabled = !_isKinectViewEnabled;
			if (_isKinectViewEnabled){
				_lastView = _view;
				setView(KINECT_VIEW);
			} else
				setView(_lastView);
			break;
		}
	}
}

void World::mouseDragged(int x, int y, int button){
#ifdef DEBUG_TOOLS
	//cam.setOrientation(model.getPosition());
	if(prevMouseX < x)	cam.rotate( 1, 0,1,0);
	else				cam.rotate(-1, 0,1,0);
	
	if(prevMouseY < y)	cam.rotate( 1, 1,0,0);
	else				cam.rotate(-1, 1,0,0);
	
	prevMouseX = x;
	prevMouseY = y;
#endif
}

void World::mouseMoved(int x, int y ){

}

void World::mousePressed(int x, int y, int button){
}

void World::mouseReleased(int x, int y, int button){
	if(View() == CAPTURE_POSTURE_VIEW){
		_player->savePosture("postures-captured.xml");	//space saves posture
		_player->getUserMapRef()->getKinectControllerRef()->saveImageForCaptures();
	} else if(View() == INSTRUCTIONS_VIEW){
		_instructions.nextSlide();
	} else if(View() == FIRST_PERSON_VIEW || View() == END_GAME_VIEW){
		ofSaveScreen("captures/captures_"+ofGetTimestampString()+".png");
	}
}

void World::windowResized(int w, int h){

}

void World::gotMessage(ofMessage msg){

}

void World::dragEvent(ofDragInfo dragInfo){

}

void World::drawScene(){
	ofPushMatrix();
	ofPushStyle();
		ofTranslate(0, -0.6, 50);
		ofRotate(90,1,0,0);
		ofScale(30, 10, 10);
		ofSetColor(70);
		_rm->getModelByName("plane").drawFacesMesh();
	ofPopMatrix();
	ofPushMatrix();
	ofPushStyle();
		//ofRotate(0,0,0,0);
		ofTranslate(0, -0.6, 0);
		ofScale(30, 10, 10);
		ofSetColor(0,255,255);
		_rm->getModelByName("plane").drawFacesMesh();
	ofPopStyle();
	ofPopMatrix();

	ofPushStyle();
	ofSetColor(255,117,22);
	ofPushMatrix();
		//ofRotate(0,0,0,0);
		ofTranslate(-9.9, 0, 46.6);
		ofScale(0.65, 1, 4.2);
		if(!_evenOdd)
			ofSetColor(0,255,0);
		else
			ofSetColor(255,117,22);
		_rm->getModelByName("plane").drawFacesMesh();
	ofPopMatrix();
	ofPushMatrix();
		//ofRotate(0,0,0,0);
		ofTranslate(20.1, 0, 46.6);
		ofScale(0.65, 1, 4.2);
		if(_evenOdd)
			ofSetColor(0,255,0);
		else
			ofSetColor(255,117,22);
		_rm->getModelByName("plane").drawFacesMesh();
	ofPopStyle();
	ofPopMatrix();
	//part delantera
	ofPushMatrix();
	ofPushStyle();
		//ofRotate(0,0,0,0);
		ofTranslate(5.12, 0, 0);
		ofScale(1.206, 1, 3);
		ofSetColor(0,255,0);
		_rm->getModelByName("plane").drawFacesMesh();
	ofPopStyle();
	ofPopMatrix();
}

void World::updateCollisions(){
	if(isAnyWall()){
		_screenWalls.at(0)->updateCollisionData(_player->getMap());
	}
}

void World::wallsController(){
	if(isAnyWall() && _screenWalls.at(0)->isWallDisabled()){
		_screenWalls.erase(_screenWalls.begin());
		_scoreC.isWallScored(false);		//prepare for the next wall
	}
}

void World::addWall(float delayedTime){
	_wallsCount++;
	Wall* w = new Wall( GRID_ROWS, GRID_COLS, &_rm->getModelByName("cube"), ofVec3f( 0, 0, 100), _player->getPosition(), "yellow");
	_walls.push_back(w);
}

void World::loadWallsFromXML(string filePath){
	if(filePath == ""){
		_rm->loadMapsFromXML();
	} else{
		_rm->loadMapsFromXML(filePath);
	}
	_rm->createWallsFromMaps(_player->getPosition());
	_walls = _rm->getWallsRef();
}

float World::checkDistanceOfWallFromPlayer(){
	if(isAnyWall()){
		_distanceOfWallFromPlayer = _player->getPosition().z - _screenWalls.at(0)->getPosition().z;
	}
	return _distanceOfWallFromPlayer;
}

bool World::isAnyWall(){
	//if(_walls.size() > 0)
	if(_screenWalls.size() > 0)
		return true;
	else
		return false;
}

void World::drawMessages(){
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	drawRectangleOnCoordinates(0,0,ofGetWidth(),70,_barColor);
	//ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	ofPushMatrix();
	//ofSetColor(255,255,0);
	if (_debugMessages){		//Debug messages
		ofPushStyle();
		ofSetColor(255,255,0);
		ofDrawBitmapString("Camera position:" + ofToString(cam.getPosition().x) + " " + ofToString(cam.getPosition().y) + " " + ofToString(cam.getPosition().z), 10, 30);
		ofDrawBitmapString(_player->getUserMapRef()->getKinectControllerRef()->getKinectConfigUI(),10,60);
		if (_player->getUserMapRef()->getKinectControllerRef()->isSkipFrameMode()){
			ofDrawBitmapString("Skipping Frames Mode On KINECT Enabled",500,50);
		} else{
			ofDrawBitmapString("Skipping Frames Mode On KINECT Disabled",500,50);
		}
		//ofDrawBitmapString(_player->getUserMapRef()->getTxtCells(), 640, 0);
		//ofDrawBitmapString("fps: "+ofToString(ofGetFrameRate(), 2), 10, 15);
		if(isAnyWall()){
			ofDrawBitmapString("First Wall position:" + ofToString(_screenWalls.at(0)->getPosition().x) + " " + ofToString(_screenWalls.at(0)->getPosition().y) + " " + ofToString(_screenWalls.at(0)->getPosition().z), 10, 45);
			_scoreC.updateScore(checkDistanceOfWallFromPlayer(), _screenWalls.at(0)->collisionNumber(), _screenWalls.at(0)->correctBlocks(), _screenWalls.at(0)->notUsedBlocks());	
			ofDrawBitmapString(_scoreC.outputInfo(),10,150);
		} else{
			ofDrawBitmapString("Score: " + ofToString(_scoreC.score()),10,150);
		}
		ofPopStyle();
	} else{						//Nice GUI messages with font
		if(isAnyWall()){
			_scoreC.updateScore(checkDistanceOfWallFromPlayer(), _screenWalls.at(0)->collisionNumber(), _screenWalls.at(0)->correctBlocks(), _screenWalls.at(0)->notUsedBlocks());
		}
		messages.updateMessagesContent(_scoreC.score(), _scoreC.previewScore(), _screenWalls.size(), _scoreC.level(), tempo.timeToFinish(), tempo.phase()+1);
		messages.drawMessages();
		
		ofSetColor(0,0,0);
		ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate()), 5, ofGetHeight()-10);
		string mK = "";
		if (_player->getUserMapRef()->getKinectControllerRef()->isSkipFrameMode()) mK = "disable";
		else																		mK = "enable";
		ofDrawBitmapString("Press F1 for debug messages, F2 for kinect View, F3 to show time and F12 to "+ mK +" Kinect Hack", 120, ofGetHeight()-10);
	}
	ofPopMatrix();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	
	drawRectangleOnCoordinates(0,0,ofGetWidth(),70,_barColor,false);
}

void World::setTempoModulation(string m){
	float low = 9;
	float medium = 6;
	float high = 3;
	float phase1, phase2, phase3;
	if (m == "L-H-M"){
		phase1 = low;
		phase2 = high;
		phase3 = medium;
	} else if(m == "M-H-L"){
		phase1 = medium;
		phase2 = high;
		phase3 = low;
	} else if(m == "L-M-H"){
		phase1 = low;
		phase2 = medium;
		phase3 = high;
	} else{ // "H-M-L"
		phase1 = high;
		phase2 = medium;
		phase3 = low;
	}

	tempo.setTimePhases(phase1, phase2, phase3);
	_view = GAME_VIEW;
}

bool World::isPostureRepeated(int index){
	if(_lastRandomPostures.size() > 5){
		_lastRandomPostures.pop_front();
	}
	 
	for (std::list<int>::iterator it = _lastRandomPostures.begin(); it != _lastRandomPostures.end(); it++){
		if(*it == index)
			return true;		//as it was stored, means is repeated
	}
	_lastRandomPostures.push_back(index);
	return false;
}

//VIEWS SUMMARY: START_MENU_VIEW, GAME_VIEW, KINECT_VIEW, END_GAME_VIEW, CAPTURE_POSTURE_VIEW
void World::launchView(){
	if (View() == START_MENU_VIEW){
		messages.drawAMessageWithString("Configurar tipus de modulacio\n1. H-M-L\n2. M-H-L\n3. L-H-M",ofGetWidth()/2-200,ofGetHeight()/2-100,ofColor(255,255,0));
		messages.drawAMessageWithString("INFO:\nKeys 1, 2 or 3.\nH: High, M: Medium, L: Low",ofGetWidth()/2-200,ofGetHeight()/2+300,ofColor(255,255,0),10,false);
		string kinectHack		= (string)(_player->getUserMapRef()->getKinectControllerRef()->isSkipFrameMode() ? "DISABLE" : "ENABLE");
		string out = "+Press F12 to " + kinectHack + " Kinect Low FPS FIX\n";
		out+= "+To switch postures file press\n  'h' [DEFAULT] to load Laura Postures and\n  'e' the Pascal ones.";
		out+= "\n+Press F6 to swap kinect tracking method";
		out+= "\n+Press 'c' to capture postures";
		out+= "\n+Press 's' to swap sound";
		messages.drawAMessageWithString(out,ofGetWidth()*2/3-100,ofGetHeight()*4/5,ofColor(30),10,true,ofColor(100,200,100));
		if(_player->getUserMapRef()->getKinectControllerRef()->isSkipFrameMode()){
			messages.drawAMessageWithString("Kinect Speed Hack Enabled",ofGetWidth()*2/3-100,ofGetHeight()/5,ofColor(0),10,true,ofColor(0,255,0));
		} else
			messages.drawAMessageWithString("Kinect Speed Hack Disabled",ofGetWidth()*2/3-100,ofGetHeight()/5,ofColor(255),10,true,ofColor(255,0,0));
	} else if(View() == KINECT_VIEW){
		messages.drawAMessageWithString("KINECT");
		glDisable(GL_LIGHTING);
		_player->getUserMapRef()->getKinectControllerRef()->draw();
		ofScale(0.5, 0.5, 0.5);
		ofDrawBitmapString(_player->getUserMapRef()->drawTxtCells(), 850, 750);
		glEnable(GL_LIGHTING);
	} else if(View() == CAPTURE_POSTURE_VIEW){
		messages.drawAMessageWithString("CAPTURE POSTURES VIEW",50,50);
		cam.begin();
			_player->draw();
		cam.end();
		messages.drawAMessageWithString("Captures made: "+ofToString(_player->capturesDone())+"\n-Click or Press space to capture-",ofGetWidth()/2-200,100,ofColor(255,255,0),20,false);
	} else if(View() == END_GAME_VIEW){
		_log.end(_scoreC.score());
		moveCamAroundPlayer();
		_player->enableRainbowMode();		//lets fun!
		cam.begin();
			light.enable();
			light2.enable();
			_player->draw();
			drawScene();
		cam.end();
		messages.drawEndOfGameMessage();

#ifdef DEBUG_TOOLS
		ofSetColor(0,255,0);
		ofDrawBitmapString("Light position:" + ofToString(light2.getPosition().x) + " " + ofToString(light2.getPosition().y) + " " + ofToString(light2.getPosition().z), 10, 15);
		ofDrawBitmapString("Light position:" + ofToString(light.getPosition().x) + " " + ofToString(light.getPosition().y) + " " + ofToString(light.getPosition().z), 10, 30);
#endif
	} else if(View() == GAME_VIEW) {
		if(canGameStart())
			tempo.startTimer();
		drawCircleProgress();
		drawGame();
		messages.drawPerformanceMessages(_scoreC.playerPerformance());
	} else if (View() == INSTRUCTIONS_VIEW) {
		if(!_instructions.isEnd())	_instructions.draw();
		else						_view = START_MENU_VIEW;
		messages.drawAMessageWithString("Clic or Press SPACE to continue...\n(Backspace to return, F5 to skip)",ofGetWidth()/2-200,ofGetHeight()-40,ofColor(255),15,false);
	} else if(View() == ERROR_VIEW){
		messages.drawAMessageWithString("There are no walls loaded.\nCheck data/postures.xml file!",ofGetWidth()/3,ofGetHeight()/2,ofColor(255,255,0),15,true,ofColor(255,0,0));
	} else if(View() == FIRST_PERSON_VIEW){
		//cam.setPosition(5,6,-10);//for 26x16 resolution use 5, 10, -13
		//cam.lookAt(ofVec3f((0.4 * GRID_COLS)/2,0,20));
		//moveCamAroundPlayer();
		//_player->enableRainbowMode();		//lets fun!
		cam.begin();
			light.enable();
			light2.enable();
			_player->draw();
			//drawScene();
		cam.end();
	}
}

void World::drawGame(){
	if(canGameStart()){
		if(!tempo.endGame()){
			drawMessages();		//draw the GUI
			cam.begin();// elements en la camara
				drawScene();		//draws the scenario
				int a = 0;
				//tempo.launchNewWall(false);		//set launch to false to check if really has to 
				if(tempo.launchNewWall()){
					
					//randomize walls and control that never choses a wall that has been chosen last 5 times
					if(_walls.size() > 0){	//if is loaded at least one posture on the xml
						int index = rand() % _walls.size();
						while (_walls.size() > 1 && isPostureRepeated(index) ){
							index = rand() % _walls.size();
						}

						_evenOdd = !_evenOdd;
						ofVec3f p;
						if (_evenOdd){
							p = ofVec3f(15,-10,50);
						} else{
							p = ofVec3f(-15,-10,50);
						}
						//ofVec3f p = ofVec3f(0,0,50);
						//restore the wall to the start position
						//if (_screenWalls.size() < 1){	//if we want to see only one wall to debug
							_screenWalls.push_back(_walls.at(index));
							_screenWalls.at(_screenWalls.size()-1)->setPosition(p);
							_screenWalls.at(_screenWalls.size()-1)->restoreGrid();
							_screenWalls.at(_screenWalls.size()-1)->enableWall();
						//}
					} else{	//draw a message if there are no walls loaded
						setView(ERROR_VIEW);
					}
				}
				if(isAnyWall()){
					for (int j=0; j < _screenWalls.size(); j++){
						if(checkDistanceOfWallFromPlayer() >= WALL_COLLISION_THRESHOLD && j == 0){
							ofEnableAlphaBlending();
								_screenWalls.at(j)->draw();
							ofDisableAlphaBlending();
						} else{
							_screenWalls.at(j)->draw();
						}
					}
				}
				_player->draw();
				_log.update(tempo.timeElapsed(), _player->amountOfMovement(),ofGetFrameNum());
			cam.end();
		} else{
			//delete remaining walls
			_screenWalls.clear();
			//_walls.clear();
			setView(END_GAME_VIEW);
		}
	}
}

bool World::canGameStart(){
	int i = 0;
	#ifdef DEBUGGING_WITHOUT_PLAYER
		_player->isReady(true);
	#endif

	if(_player->isReady())
		i=1;
	else if(!_player->getUserMapRef()->getKinectControllerRef()->isKinectPlugged())
		i=2;
	if(_startGame)
		i+=10;

	switch(i){
		case 0:
			messages.drawAMessageWithString("Jugador no detectat",ofGetWidth()/2-200);
			return false;
			break;
		case 1:	//player detected by kinect, but not pressed space
			messages.drawAMessageWithString("Premer tecla ESPAI\n    per iniciar",ofGetWidth()/2-200);
			return false;
			break;
		case 2:	//demo mode without kinect, but not pressed space
			messages.drawAMessageWithString("KINECT no conectada.\nEntrant en mode demo.\nPrem ESPAI per continuar...",ofGetWidth()/2-200);
			return false;
			break;
		case 10: //kinect plugged but not user found
			messages.drawAMessageWithString("Jugador no detectat",ofGetWidth()/2-200);
			return false;
			break;
		case 11: //confirmed working and pressed space key
		case 12:
			return true;
			break;
		}
	return false;
}

void World::drawCircleProgress(){
	glDisable(GL_LIGHTING);
	ofPushMatrix();

		if(isAnyWall()){	//draw the inside circle
			ofSetColor(255, 34, 0);
			if (checkDistanceOfWallFromPlayer() < WALL_COLLISION_THRESHOLD){
				ofEnableAlphaBlending();
				if(checkDistanceOfWallFromPlayer() > WALL_COLLISION_THRESHOLD - 1)
					ofSetColor(255, 255, 0);
				ofSetCircleResolution(100);
				ofCircle( ofGetWidth()-100, ofGetHeight()-100,0, 50 * ( _screenWalls.at(0)->getDistanceFromPlayerPercentage()));
				ofDisableAlphaBlending();
			} else if(_screenWalls.size() > 1){
				ofSetCircleResolution(100);
				ofCircle( ofGetWidth()-100, ofGetHeight()-100,0, 50 * ( _screenWalls.at(1)->getDistanceFromPlayerPercentage()));
			}
		}
		//draw the max circle
		ofSetColor(10);	
		ofSetCircleResolution(100);
		ofCircle( ofGetWidth()-100, ofGetHeight()-100, -0.1, 50);
	ofPopMatrix();
	glEnable(GL_LIGHTING);
}

void World::drawRectangleOnCoordinates(int x, int y, float w, float h, ofColor c,  bool enableDepth){
	glDisable(GL_LIGHTING);
	if(enableDepth)	glDisable(GL_DEPTH_TEST);
	ofPushMatrix();
		ofSetColor(c);
		ofFill();	
		ofRect(x,y,0,w,h);	
	ofPopMatrix();
	if(enableDepth)	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}

void World::moveCamAroundPlayer(){
	if (_timer == -1)
		_timer = ofGetElapsedTimef();
	float deltaT = ofGetElapsedTimef() - _timer;
	ofVec3f p = cam.getPosition();
	
	/*light2.enable();
	light.setAttenuation(1.f,0.1f,0.f);
	//light2.setPosition(0,50,-15);
	light2.setAttenuation(1.f,0.1f,0.f);
	*/
	cam.rotateAround(1,ofVec3f(0,2,0),_player->getPosition()+ofVec3f(5,0,0));
	cam.lookAt(_player->getPosition()+ofVec3f(5,5,0));
	
	//cam.setPosition(_player->getPosition().x+15 + sin(deltaT),10,_player->getPosition().z-15 + cos(deltaT));		//y fixed
	//cam.setPosition(p.x + sin(deltaT),20,p.z + sin(deltaT));		//y fixed
	//cam.lookAt(_player->getPosition()+ofVec3f(5,5,0));
	
}