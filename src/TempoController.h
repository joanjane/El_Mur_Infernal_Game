#ifndef _TEMPO_CONTROLLER
#define _TEMPO_CONTROLLER

#include "ofMain.h"
#include <time.h>

class TempoController
{
	public:
		TempoController(void);
		void updateTime();
		int phase(){ return _phase;}
		bool launchNewWall();
		void launchNewWall(bool b){ _launchNewWall = b;}
		bool endGame(){ return _endGame;}
		float timeToFinish(){ return _timeToFinish;}
		double timeElapsed(){ 
			//return ofGetElapsedTimef() - _startTime;
			return (double)((double)clock() - (double)_startT0)/CLOCKS_PER_SEC;
		}
		void setTimePhases(float phase1=6, float phase2=4, float phase3=3);
		void startTimer(){
			if(!_isTimerSet){
				_startT0 = clock();
				time(&startT);
				time(&lastLaunchT);
				_isTimerSet = true;
			}
		}

	private:
		float _startTime;
		float _lastWallLaunchTime;
		float _timeToFinish;
		int _phase;
		bool _launchNewWall;
		bool _endGame;
		vector<int> _timeBetweenWallsPerPhase;
		bool _firstCheckTime;
		time_t startT;
		time_t currentT;
		time_t lastLaunchT;
		bool	_isTimerSet;
		clock_t _startT0;
};
#endif
