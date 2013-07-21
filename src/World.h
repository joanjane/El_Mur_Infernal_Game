#ifndef _WORLD
#define _WORLD

#define GRID_COLS 26
#define GRID_ROWS 18
#define GAME_MODE
//#define DEBUG_TOOLS
//#define DEBUGGING_WITHOUT_PLAYER
enum VIEWS{ START_MENU_VIEW, GAME_VIEW, KINECT_VIEW , CAPTURE_POSTURE_VIEW, END_GAME_VIEW, INSTRUCTIONS_VIEW, ERROR_VIEW, FIRST_PERSON_VIEW };
#define WALL_COLLISION_THRESHOLD 1

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "ofVboMesh.h"
#include "Grid.h"
#include "ResManager.h"
#include "Wall.h"
#include "Player.h"
#include "ScoreController.h" 
#include "GUIMessages.h"
#include "TempoController.h"
#include "LogGenerator.h"
#include "Instructions.h"

typedef std::vector<Wall *> Walls;

class World : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		/*~World(){
			delete _rm;
			delete _player;
			//Instructions
		}*/
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);		

		void drawScene();
		void updateCollisions();
		void wallsController();
		void addWall(float delayedTime=0);
		void loadWallsFromXML(string filePath="");
		float checkDistanceOfWallFromPlayer();
		bool isAnyWall();
		void drawMessages();
		void setTempoModulation(string m="H-M-L");
		bool isPostureRepeated(int index);
		void launchView();
		void setView(int v){ _view = v;}
		int View(){return _view;}
		void drawGame();
		bool canGameStart();
		void drawCircleProgress();
		void drawRectangleOnCoordinates(int x, int y, float w, float h, ofColor c, bool enableDepth=true);
		void drawInstructions();
		void moveCamAroundPlayer();
		void pauseResumeGame(){
			for (int i=0; i < _screenWalls.size(); i++){
				_screenWalls.at(i)->pauseResume();
			}
		}
	
	private:
        ofLight	light, light2;
		ofCamera cam;

		int prevMouseX, prevMouseY;
	
		Walls _walls;			//all the walls loaded to grab randomly them and copy to _screenWalls
		Walls _screenWalls;
		MyResourceManager* _rm;
		Player* _player;
		ScoreController _scoreC;
		GUIMessages messages;
		TempoController tempo;
		LogGenerator _log;		
		Instructions _instructions;

		bool _isKinectViewEnabled;
		ofPoint _lastCamPos;
		int _wallsCount;
		float _distanceOfWallFromPlayer;
		bool _isFullScreen;
		bool _debugMessages;
		ofColor _barColor;	//color of the topbar

		//Wall* _floor;	//not used
		//Walls _scene;	//not used

		int _lastRandomWallIndex;
		list<int> _lastRandomPostures;
		bool	_evenOdd;	//used to launch a wall on the right or left
		bool _startGame;	//used to start the game when is ready
		int _view;			//it stores the current view.
		int _lastView;		
		float _timer;
		bool _simulateEnd;
};

#endif
