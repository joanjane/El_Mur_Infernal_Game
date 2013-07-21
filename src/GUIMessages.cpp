#include "GUIMessages.h"
#define SOUND_EFFECTS

GUIMessages::GUIMessages(void)
{
	//messages.loadFont("fonts/sinclair_zx_spectrum_es.ttf",20);
	//messages.loadFont("fonts/Roboto-Bold.ttf",20);
	//messages.loadFont("fonts/Roboto-Light.ttf",20);
	//messages.loadFont("fonts/vag.ttf",20);
	//messages.loadFont("fonts/Prosto.ttf",20);
	messages.loadFont("fonts/RobotoSlab-Bold.ttf",20);
	_loadedFont = "fonts/RobotoSlab-Bold.ttf";
	_score = "";
	_previewScore = "";
	_wallsLeft = "";
	_timeLeft = "";
	_phase = "";
	_extraMessages = false;
	messagePerformanceLoop = 0;

#ifdef SOUND_EFFECTS
	///////////SOUND//////////////
	_ouch.loadSound("sounds/ouch.mp3");
	_ouch.setMultiPlay(false);
	_bad.loadSound("sounds/bad.mp3");
	_bad.setMultiPlay(false);
	_good.loadSound("sounds/good.mp3");
	_good.setMultiPlay(false);
	_fantastic.loadSound("sounds/fantastic.mp3");
	_fantastic.setMultiPlay(false);
	_perfect.loadSound("sounds/perfect.mp3");
	_perfect.setMultiPlay(false);
	_endOfGame.loadSound("sounds/time_out.mp3");
	_endOfGame.setMultiPlay(false);
	_gameTrack.loadSound("sounds/RelaxingTable/RelaxingTableLQ.mp3");
	//_gameTrack.loadSound("sounds/UnderTheSea/UnderTheSea.mp3");
	_gameTrack.setVolume(0.05);
	_gameTrack.setMultiPlay(false);
	///////////SOUND//////////////
#endif
	_isEndClipPlayed = false;
	_trackDisabled = false;
	_soundsEnabled = false;
}

void GUIMessages::drawMessages(ofColor w){
	glDisable(GL_LIGHTING);
	ofSetColor(w);
	messages.drawString(_score, 20, 50);
	messages.drawString(_previewScore, ofGetWidth() - 400, 50);
	messages.drawString(_level, ofGetWidth()/2-100, 50);
	if (_extraMessages){
		messages.drawString(_timeLeft, 20, 100);
		messages.drawString(_phase, ofGetWidth() - 450, 100);
		//messages.drawString(_wallsLeft, ofGetWidth() - 600, 100);
	}
	glEnable(GL_LIGHTING);
}

void GUIMessages::updateMessagesContent(int score, int previewScore, int wallsLeft, int level, int timeLeft, int phase){
#ifdef SOUND_EFFECTS
	if (_soundsEnabled) ofSoundUpdate();
	if(!_gameTrack.getIsPlaying() && _soundsEnabled && _gameTrack.isLoaded()){
		_gameTrack.play();
	}
#endif
	_score = "Punts: " + ofToString(score);
	_previewScore = "Propers punts: " + ofToString(previewScore);
	_wallsLeft = "Murs restants: " + ofToString(wallsLeft);
	_level = "Nivell: " + ofToString(level);

	int min = timeLeft / 60;
	int sec = timeLeft % 60;
	string zeroForSeconds = "";
	if (sec < 10) zeroForSeconds = "0";
	string timeInMinutesFormat =  ofToString(min) + ":" + zeroForSeconds + ofToString(sec);
	_timeLeft = "Temps restant: " + timeInMinutesFormat + "";
	_phase = "Fase Actual: ( " + ofToString(phase) + "/3 ) ";
}

void GUIMessages::drawEndOfGameMessage(){
	if (!_isEndClipPlayed && _soundsEnabled){
		_endOfGame.play();
		_isEndClipPlayed = true;
	}
	if (_score == "") _score = "Punts 00000";
	glDisable(GL_DEPTH_TEST);
	drawAMessageWithString("Final del Joc\n"+_score, ofGetWidth()/4, 100,ofColor(255,255,0),50,true);
	glEnable(GL_DEPTH_TEST);
}

void GUIMessages::drawAMessageWithString(string s, int x, int y, ofColor c, int fontSize, bool boxBackground, ofColor boxColor){
	
	ofTrueTypeFont m;
	m.loadFont(_loadedFont,fontSize);

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
		if(boxBackground){ //box
			ofSetColor(boxColor);
			ofRectangle rect = m.getStringBoundingBox(s, x, y);
			int padding = 50;
			ofRect(rect.x-padding/2, rect.y-padding/2,-0.1, rect.width+padding, rect.height+padding);
		}
		//message
		
		ofSetColor(c);
		m.drawString(s, x, y);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

}

void GUIMessages::drawPerformanceMessages(int index){
	glDisable(GL_LIGHTING);
	ofPushStyle();
	string m;
	ofColor c;
	ofColor c2;
	//index = messagePerformanceLoop;		//to test manually
 	switch(index+1){
	case 1:
		c = ofColor(255,0,0);
		c2 = ofColor(255);
		m = "Ouch!";
#ifdef SOUND_EFFECTS
		if(!_ouch.getIsPlaying() && _soundsEnabled) _ouch.play();
#endif
		break;
	case 2:
		c = ofColor(255,63,26);
		c2 = ofColor(255);
		m = "Malament!";
#ifdef SOUND_EFFECTS
		if(!_bad.getIsPlaying() && _soundsEnabled){
			_bad.setVolume(0.01);
			_bad.play();
		}
#endif
		break;
	case 3:
		c = ofColor(242,117,0);
		c2 = ofColor(50);
		m = "Be!";
#ifdef SOUND_EFFECTS
		if(!_good.getIsPlaying() && _soundsEnabled) _good.play();
#endif
		break;
	case 4:
		c = ofColor(217,255,35);
		c2 = ofColor(50);
		m = "Fantastic!";
#ifdef SOUND_EFFECTS
		if(!_fantastic.getIsPlaying() && _soundsEnabled) _fantastic.play();
#endif
		break;
	case 5:
		c = ofColor(0,255,0);
		c2 = ofColor(50);
		m = "Perfecte!";
#ifdef SOUND_EFFECTS
		if(!_perfect.getIsPlaying() && _soundsEnabled) _perfect.play();
#endif
		break;
	default:
		m = "";
		break;
	}

	int w = ofGetWidth()-200;
	int h = ofGetHeight()/2;
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	if (m != ""){
		ofEnableBlendMode(OF_BLENDMODE_ALPHA);
		//ofSetColor(255,255,0);
		ofSetColor(c);
		ofRectangle rect = messages.getStringBoundingBox(m, w, h);
		int padding = 20;
		ofRect(rect.x-padding/2, rect.y-padding/2,-0.1, rect.width+padding, rect.height+padding);
		ofDisableBlendMode();
	}
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	ofSetColor(c2);
	//ofSetColor(255,255,255,255);
	//messages.drawStringAsShapes(m, w, h);
	glDisable(GL_LIGHTING);
	messages.drawString(m, w, h);
	glEnable(GL_LIGHTING);
	ofDisableBlendMode();	
	ofPopStyle();
}