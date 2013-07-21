#ifndef _SCORE_CONTROLLER
#define _SCORE_CONTROLLER

#include <vector>
#include "ofMain.h"

typedef std::vector<int> GridMap;

class ScoreController
{
	public:
		ScoreController(void);
		float score() {return _score;}
		float previewScore(){return _previewScore;}
		float level(){return _level;}
		void updateScore(float distanceOfWallFromPlayer, int collisionsNumber, int correctBlocksNumber, int holeSize);
		void updateScore(int collisions, int correctBlocksNumber, int holeSize);
		void previewNextScore(int collisions, int correctBlocksNumber, int holeSize);
		void updateLevelAndFactors();
		string outputInfo(){ return _outputInfo;}
		void isWallScored(bool b){ _isWallScored = b;}
		void updatePlayerPerformance(int collisionsNumber, int correctBlocksNumber, int holeSize);
		int playerPerformance(){ return _playerPerformance;}

	private:
		float _score;
		float _previewScore;
		float _lastScoredPoints;
		float _correctFactor;		//the value of performing a correct block
		float _incorrectFactor;		//the value of performing a collision(incorrect) block
		float _emptyBlockFactor;	//the value of not performing a correct block
		float _level;
		string _outputInfo;
		bool _isWallScored;
		int _playerPerformance;
		float _timeThreshold;
		float _startTime;
		float correctBlocksFactor;
		float collisionsFactor;
		float emptyBlocksFactor;
};
#endif
