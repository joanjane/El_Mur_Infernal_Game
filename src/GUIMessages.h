#ifndef _GUI_MESSAGES
#define _GUI_MESSAGES

#include "ofMain.h"

class GUIMessages
{
	public:
		GUIMessages(void);
		void drawMessages(ofColor w=ofColor(255,255,0));
		void drawEndOfGameMessage();
		void updateMessagesContent(int score, int previewScore, int wallsLeft, int level, int timeLeft, int phase=0);
		void swapExtraMessages(){_extraMessages = !_extraMessages;}
		void drawAMessageWithString(string s, int x=ofGetWidth()/2-100, int y=ofGetHeight()/2-100, ofColor c=ofColor(255,255,0), int fontSize=20, bool boxBackground=true, ofColor boxColor=ofColor(30,30,30));
		void drawPerformanceMessages(int performanceIndex);
		void increaseMessagePerformanceLoop(){ messagePerformanceLoop++;}
		void decreaseMessagePerformanceLoop(){ messagePerformanceLoop--;}
		void swapSound(){ _soundsEnabled = !_soundsEnabled;}
	private:
		ofTrueTypeFont messages;
		string _score, _previewScore, _wallsLeft, _level, _timeLeft, _phase;
		bool _extraMessages;
		int messagePerformanceLoop;
		string _loadedFont;
		ofSoundPlayer  _ouch;
		ofSoundPlayer  _bad;
		ofSoundPlayer  _good;
		ofSoundPlayer  _fantastic;
		ofSoundPlayer  _perfect;
		ofSoundPlayer  _endOfGame;
		ofSoundPlayer  _gameTrack;
		bool			_isEndClipPlayed;
		bool			_soundsEnabled;
		bool			_trackDisabled;

};
#endif
