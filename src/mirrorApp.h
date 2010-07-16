#ifndef _MIRROR_APP
#define _MIRROR_APP

#include "ofMain.h"
#include "ofSoundPlayer.h"


class mirrorApp : public ofBaseApp {
	public:
		void setup();
		void update();
		void draw();
	
		void keyPressed  (int key);
		void keyReleased (int key);
		
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased();
	
	private:
		int hblocks;
		int vblocks;
		int blockWidth;
		int blockHeight;
		int sampleBlockWidth;
		int sampleBlockHeight;
		int * values;
	
		// pointer to an array of sounds
		ofSoundPlayer ** panelSounds;
	
		ofVideoGrabber camera;
		int captureWidth;
		int captureHeight;
	
		bool debug;
	
		int calculateBlockValue(unsigned char * pixels, int blockx, int blocky);
};

#endif
	
