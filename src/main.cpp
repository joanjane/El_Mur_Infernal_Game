
#include "ofMain.h"
#include "World.h"
#include "ofAppGlutWindow.h"

int main(int argc, char *argv[]){
	int opt = 3;			//change this to quickly change
	if(argc > 1){
		opt = *argv[1] - 48;
		//std::cout << endl << "--------------Mode " << *argv[1] << " - " << opt << " arguments: " << argc << endl << endl;
	}
	int windowMode;
	switch(opt){
		case 1:
			windowMode = OF_WINDOW;
			std::cout << "Opening in window mode" << endl;
			break;
		case 2:
			windowMode = OF_FULLSCREEN;
			std::cout << "Opening in fullscreen mode" << endl;
			break;
		case 3:
			windowMode = OF_GAME_MODE;
			std::cout << "Opening in game mode" << endl;
			break;
		default:
			windowMode = OF_GAME_MODE;
			std::cout << "Opening in default game mode" << endl;
			break;
	}
	
    ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1024,768, windowMode);			// <-------- setup the GL context
	ofSetWindowTitle("El Mur Infernal - Joan Jané");

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new World());

}