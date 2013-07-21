#include "ScoreController.h"
#define WALL_COLLISION_THRESHOLD 1
enum PLAYER_PERFORMANCE {WORST_SCORING, LOW_SCORING, MEDIUM_SCORING, HIGH_SCORING, PERFECT_SCORING};

ScoreController::ScoreController(void)
{
	_score = 0;
	_previewScore = 0;
	_correctFactor = 10;
	_incorrectFactor = 5;
	_emptyBlockFactor = 10/3;
	_level = 0;
	_isWallScored = false;
	_playerPerformance = -1;
	_timeThreshold = 0;
	_startTime = ofGetElapsedTimef();
}

void ScoreController::updateScore(float distanceOfWallFromPlayer, int collisionsNumber, int correctBlocksNumber, int holeSize)
{
	_outputInfo = "";
	holeSize += correctBlocksNumber;
	updateLevelAndFactors();

	if (distanceOfWallFromPlayer < WALL_COLLISION_THRESHOLD){
		if (ofGetElapsedTimef() - _timeThreshold > 1.5){
			_playerPerformance = -1;
		}
		previewNextScore( collisionsNumber, correctBlocksNumber, holeSize);
		_outputInfo = "Punts: " + ofToString(_score) + "\n";
		_outputInfo += "Punts de la jugada actual: " + ofToString(_previewScore) + "\n";
	} else {
		_timeThreshold = ofGetElapsedTimef();
		updateScore( collisionsNumber, correctBlocksNumber, holeSize);
		updatePlayerPerformance( collisionsNumber, correctBlocksNumber, holeSize);
		_outputInfo = "Punts: " + ofToString(_score) + "\n";
	}

	_outputInfo += "Level:" + ofToString(_level) + "\n";

}

void ScoreController::previewNextScore(int collisionsNumber, int correctBlocksNumber, int holeSize)
{
	//int correctBlocks = holeSize - correctBlocksNumber;
	_previewScore = correctBlocksNumber * _correctFactor;		//reset the preview score
	_previewScore += -collisionsNumber * _incorrectFactor;
	_previewScore += (holeSize - correctBlocksNumber) * (-_emptyBlockFactor);
}

void ScoreController::updateScore(int collisionsNumber, int correctBlocksNumber, int holeSize)
{
	if(!_isWallScored){
		_lastScoredPoints = _score;
		_score += correctBlocksNumber * _correctFactor;
		_score += -collisionsNumber * _incorrectFactor;
		int emptyBlocks = holeSize - correctBlocksNumber;
		_score += emptyBlocks * (-_emptyBlockFactor);
		_isWallScored = true;
		_lastScoredPoints = _score - _lastScoredPoints;
	}
}

void ScoreController::updateLevelAndFactors(){
	float scoreFactor = 1;
	if (_score < scoreFactor * 2000){
		_level = 0;
		_correctFactor = 10;
		_incorrectFactor = _correctFactor/2;
		_emptyBlockFactor = _correctFactor*1/3;
	} else if(_score < scoreFactor * 10000){
		_level = 1;
		_correctFactor = 20;
		_incorrectFactor = _correctFactor*2/3;
		_emptyBlockFactor = _correctFactor*1/3;
	} else if(_score < scoreFactor * 30000){
		_level = 2;
		_correctFactor = 30;
		_incorrectFactor = _correctFactor*2.5/3;
		_emptyBlockFactor = _correctFactor*1/3;
	} else if(_score < scoreFactor * 50000){
		_level = 3;
		_correctFactor = 50;
		_incorrectFactor = _correctFactor;
		_emptyBlockFactor = _correctFactor*1/3;
	} else if(_score < scoreFactor * 100000){
		_level = 4;
		_correctFactor = 75;
		_incorrectFactor = _correctFactor;
		_emptyBlockFactor = _correctFactor*1/3;
	} else {
		_level = 5;
		_correctFactor = 100;
		_incorrectFactor = _correctFactor*1.5;
		_emptyBlockFactor = _correctFactor*1/3;
	}
}

void ScoreController::updatePlayerPerformance(int collisionsNumber, int correctBlocksNumber, int holeSize){
	int emptyBlocks = holeSize - correctBlocksNumber;
	if (holeSize == 0) holeSize = 1;
	correctBlocksFactor = (float)correctBlocksNumber/(float)holeSize;
	collisionsFactor = (float)collisionsNumber/(float)(26*18);
	emptyBlocksFactor = (float)emptyBlocks/(float)holeSize;
	if (correctBlocksFactor == 0 || collisionsFactor==0){
		if (correctBlocksNumber == 0 && collisionsNumber > 0){		//if any collision and not any correct block: WORST
			_playerPerformance = WORST_SCORING;
		} else if(correctBlocksNumber <= 10){							//if 20% collisions: BAD
			_playerPerformance = LOW_SCORING;
		} else if(correctBlocksNumber <= 20 && collisionsNumber < 50){
			_playerPerformance = MEDIUM_SCORING;
		} else if(correctBlocksNumber <= 40  && collisionsNumber <= 30){					//if correct blocks are below 75% of the hole
			_playerPerformance = HIGH_SCORING;
		} else if(correctBlocksNumber <= 50 && collisionsNumber <= 5 ) {//if correct blocks are below 90% of the hole
			_playerPerformance = PERFECT_SCORING;
		} else
			_playerPerformance = -1;
	} else{
		if (correctBlocksNumber == 0 && collisionsNumber > 0){		//if any collision and not any correct block: WORST
			_playerPerformance = WORST_SCORING;
		} else if(correctBlocksFactor <= 0.3 || _lastScoredPoints < 0){							//if 30% collisions and scoring is negative: BAD
			_playerPerformance = LOW_SCORING;
		} else if(correctBlocksFactor <= 0.60 && collisionsFactor < 0.5){
			_playerPerformance = MEDIUM_SCORING;
		} else if(correctBlocksFactor >= 0.75  && collisionsFactor <= 0.05){					//if correct blocks are below 75% of the hole
			_playerPerformance = HIGH_SCORING;
		} else if(correctBlocksFactor >= 0.85 && collisionsFactor <= 0.05 ) {//if correct blocks are below 90% of the hole
			_playerPerformance = PERFECT_SCORING;
		} else
			_playerPerformance = -1;
	}
}