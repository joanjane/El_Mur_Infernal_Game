#ifndef _LOG_GENERATOR
#define _LOG_GENERATOR

#include "ofMain.h"
#include <ctime>
#include <iostream>
#include <fstream>


class LogGenerator
{
	public:
		LogGenerator(void);
		void update( float time, float amountOfMovement, float frame =0);
		string currentDateTime();
		void modulationType(string s){ _modulationType = s; cout << "Setting modulation type: " << s << endl;}
		string generateTimeStampOfStartTime();
		void end(int score=0){
			if(!_isClosed) {
				_isClosed = true;
				if(!_isDateTimeSet){
					_fileNameLog = "log/" + _modulationType + generateTimeStampOfStartTime();
					_fileNameLog += "_score_" + ofToString(score) + ".csv";
					_isDateTimeSet = true;
					_logFile.open(_fileNameLog);
				}
				if (_logFile.is_open()){
					_logFile << _allOutput;
				} else
					cout << "The log file is not open, but it should be" << endl;
				_countLock = 0;
				_logFile.close();
				cout << "closing file" << _fileNameLog << endl;
			}
		}
		
	private:
		string _fileNameLog;
		bool _isDateTimeSet;
		ofstream _logFile;
		int _countLock;
		string _modulationType;
		bool _isClosed;
		float _firstFrame;
		bool _isFirstFrameSet;
		string _allOutput;
};
#endif
