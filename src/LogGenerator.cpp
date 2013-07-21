#include "LogGenerator.h"

LogGenerator::LogGenerator(void)
{
	_isDateTimeSet = false;
	_fileNameLog= "";
	_countLock = 0;	
	_modulationType = "";
	_isClosed = false;
	_firstFrame = 0;
	_isFirstFrameSet = false;
}

void LogGenerator::update( float time, float amountOfMovement, float frame)
{
	if(!_isFirstFrameSet){
		_firstFrame = frame;
		_isFirstFrameSet = true;
	}
	frame = frame - _firstFrame;
		/*if (_logFile.is_open()){
			//_logFile << time << ", " <<  frame << ", " << amountOfMovement << "\n";
			_logFile << frame << ", " << amountOfMovement << "\n";
		} else
			cout << "The log file is not open, but it should be" << endl;
		_countLock = 0;*/
	_allOutput += ofToString(frame) + ";" + ofToString(time) + ";" + ofToString(amountOfMovement) + "\n";
}

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
string LogGenerator::currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://www.cplusplus.com/reference/clibrary/ctime/strftime/
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y_%m_%d_%H_%M_%S", &tstruct);

    return buf;
}

string LogGenerator::generateTimeStampOfStartTime() {
	string month="";
	string day="";
	string hours="";
	string minutes="";
	string seconds="";

	int mins = ofGetMinutes()-9;
	int h = ofGetHours();		
	int d = ofGetDay();
	int m = ofGetMonth();
	
	if (m < 10)	 month = "0";		//2 digit formating
	if (d < 10)	 day = "0";
	if (h < 10)	 hours = "0";

	if (mins < 10){
		if (mins < 0){
			mins = 60 + mins;
			//minutes = ofToString(mins);	//9 minutes before
			if(h-1 >= 0)
				h--;	//rest one hour
			else{
				h = 24-h;
				d--;	//rest one day if the game started before 9 minutes midnight
			}
		} else
			minutes = "0";	//9 minutes 
	}
	
	month	+= ofToString(m);	//it will fail if anyone plays 9 minutes before the month or year changes, but nevermind ;)
	day		+= ofToString(d);
	hours	+= ofToString(h);
	minutes += ofToString(mins);
	seconds += ofToString(ofGetSeconds());

	return
		ofToString(ofGetYear()) + "_" + 
		month + "_" + 
		day + "_" + 
		hours + "_" + 
		minutes + "_" + 
		seconds;
}