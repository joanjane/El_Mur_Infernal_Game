#include "TempoController.h"
#define WALL_TEMPO_FACTOR 1
#define MINUTES_PER_FASE 3

TempoController::TempoController(void)
{
	_startTime = ofGetElapsedTimef();
	_launchNewWall = true;
	_endGame = false;
	_phase = 1;
	_lastWallLaunchTime = 0;
	setTimePhases();		//set the time between walls by defauld. Can be overriden
	_firstCheckTime = false;
	time(&startT);			//overridden when startTimer() is called. Calling that function lets start the timer when needed.
	time(&lastLaunchT);
	_timeToFinish = 3 * MINUTES_PER_FASE * 60;
	_isTimerSet = false;
}

/** Method that updates the tempo modulation phase intervals
 *	with the calculation of the elapsed time. The phases are
 *  initially thought to swap every 3 minutes, 3 in total.
 **/
void TempoController::updateTime(){
	if(_isTimerSet){
		time(&currentT);
		double timer = difftime(currentT,startT);
		//float timer = ofGetElapsedTimef() - _startTime;
		//float timeOfGameFactor = 0.5;//3
		if (timer < 60 * 1 * MINUTES_PER_FASE){			//three first minutes
			_phase = 0;
		} else if (timer < 60 * 2 * MINUTES_PER_FASE){	//until six minutes
			_phase = 1;
		} else if (timer < 60 * 3 * MINUTES_PER_FASE){	//until nine minutes
			_phase = 2;
		} else{
			_endGame = true;
		}
		_timeToFinish = 3 * MINUTES_PER_FASE * 60 - timer;
	}
}

/** Method to get when has to be launched a new wall.
*	It uses the time between walls of each phase to know
*   when a wall has to be launch.
*   The method checks if has elapsed the threshold time in
*   respect to the last launch. If yes, it updates the last
*	lauch time.
*/
bool TempoController::launchNewWall(){
	_launchNewWall = false;
	int timeController = 2;
	float timer;
	if (timeController == 2){
		time(&currentT);
		timer = difftime(currentT,lastLaunchT);
	}else{
		timer = ofGetElapsedTimef() - _lastWallLaunchTime;
	}
	int timeBetweenWalls = _timeBetweenWallsPerPhase.at(_phase);
	//if (timeController != 1) timeBetweenWalls--;
	if (timer >= WALL_TEMPO_FACTOR * timeBetweenWalls){
		_launchNewWall = true;
		//if (timeController == 2)	time(&lastLaunchT);
		if (timeController == 2)	lastLaunchT = currentT;
		else						_lastWallLaunchTime = ofGetElapsedTimef();	//save the time of wall launching
	}
	return _launchNewWall;
}

void TempoController::setTimePhases(float phase1, float phase2, float phase3){
	_timeBetweenWallsPerPhase.clear();
	_timeBetweenWallsPerPhase.push_back(phase1);
	_timeBetweenWallsPerPhase.push_back(phase2);
	_timeBetweenWallsPerPhase.push_back(phase3);
}