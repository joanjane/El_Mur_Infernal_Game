#ifndef _INSTRUCTIONS
#define _INSTRUCTIONS

#include "ofMain.h"

class Instructions
{
	public:
		Instructions(void);
		~Instructions(void){
			//deletes
			for (vector<ofImage*>::iterator it = _slides.begin(); it != _slides.end(); it++){
				delete *it;
			}
		}
		void draw();
		void nextSlide(){
			if( _currentNumberOfSlide < _slides.size())
				_currentNumberOfSlide++;
		}
		void prevSlide(){
			if( _currentNumberOfSlide > 0)
				_currentNumberOfSlide--;
		}
		void swapViewHide(){ _hideSlides = !_hideSlides;}
		bool isEnd(){ 
			int totalSlides = _slides.size();
			if(_currentNumberOfSlide == totalSlides)	return true;
			else										return false;
		}
		void skipAll(){
			_currentNumberOfSlide = _slides.size();
		}
	private:
		int _currentNumberOfSlide;
		vector<ofImage*> _slides;
		bool _hideSlides;
};
#endif
