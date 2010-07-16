#include "mirrorApp.h"

// init()
void mirrorApp::setup() {	 
	
	hblocks = 50;
	vblocks = hblocks * 0.625f;
	
	int numValues = hblocks * vblocks;
	
	values = (int *)malloc(numValues*sizeof(int));
	panelSounds = new ofSoundPlayer*[numValues];
	for(int i = 0; i < numValues; i++) {
		panelSounds[i] = new ofSoundPlayer;
		panelSounds[i]->loadSound("shutter.wav",false);
		panelSounds[i]->setVolume(1.f);
		panelSounds[i]->setMultiPlay(true);
	}
	
	captureWidth = 320;
	captureHeight = 200;
	
	sampleBlockWidth = captureWidth/hblocks;
	sampleBlockHeight = captureHeight/vblocks;
	
	camera.setVerbose(true);
	camera.initGrabber(captureWidth,captureHeight);
	
	ofSetFrameRate(10);
	
	debug = false;
}

// update between renderings
void mirrorApp::update() {
	ofBackground(0,0,0);
	
	blockWidth = ofGetWidth()/hblocks;
	blockHeight = ofGetHeight()/vblocks;
		
	camera.grabFrame();
	if(camera.isFrameNew()) {
		// calculate new values
		for(int i = 0; i < hblocks*vblocks; i++) {
			int blockx = i%hblocks;
			int blocky = i/hblocks;
			int blockValue = calculateBlockValue(camera.getPixels(),blockx,blocky);
			
			int diff = abs(blockValue - values[i]);
			if(diff > 15) {
				values[i] = blockValue;
				panelSounds[i]->setVolume((float)diff/255.f);
				panelSounds[i]->play();
			}
		}
	}
}

int mirrorApp::calculateBlockValue(unsigned char * pixels, int blockx, int blocky) {
	
	int starty	= blocky*sampleBlockHeight;
	int stopy	= blocky*sampleBlockHeight+sampleBlockHeight;
	int startx	= blockx*sampleBlockWidth;
	int stopx	= blockx*sampleBlockWidth+sampleBlockWidth;
	
	float blockValue = 0.f;
	int nSamples = sampleBlockWidth*sampleBlockHeight;
	
	for(int y = starty; y < stopy; y++) {
		for(int x = startx; x < stopx; x++) {
			int i = (y*captureWidth+x) * 3; // 3 bytes per pixel
			int graytone = (pixels[i]+pixels[i+1]+pixels[i+3])/3;
			blockValue += (float)graytone/(float)nSamples;
		}
	}
	return blockValue;
}

// render app state
void mirrorApp::draw() {
	
	// mirror image rendering
	ofPushMatrix();
	ofRotate(180, 0, 1, 0);
	ofTranslate(-ofGetWidth(), 0, 0);
	
	for(int y = 0; y < vblocks; y++) {
		for(int x = 0; x < hblocks; x++) {
			
			int grayLevel = values[y*hblocks+x];
			float level = (float)grayLevel/255.f;
			ofSetColor(grayLevel,grayLevel,grayLevel);
			
			/**
			 * Examples of painting using the grayscale average
			 **/
			
			/*
			ofRect(x*blockWidth,y*blockHeight,blockWidth,blockHeight);
			 */
			
			/*
			ofCircle(x*blockWidth+blockWidth/2, y*blockHeight+blockHeight/2, blockHeight/2-2);
			 */
			

			/**
			 * Monochromatic painting, grayscale average drives geometry, not color
			 **/
	
			ofSetColor(200,200,200);
			//ofCircle(x*blockWidth+blockWidth/2, y*blockHeight+blockHeight/2, (blockHeight/2-2)*level);
	
			// like this one the best so far
			ofRect(x*blockWidth,(y+1)*blockHeight,blockWidth-1,-level*blockHeight-1);
			
//			ofLine(x*blockWidth,y*blockHeight,x*blockWidth+level*blockWidth,y*blockHeight+level*blockHeight);
			
		}
	}
	
	if(debug) {
		ofSetColor(255,255,255);
		camera.draw(ofGetWidth()-captureWidth,ofGetHeight()-captureHeight);
	}
	
	ofPopMatrix();
}

void mirrorApp::keyPressed  (int key) { 
	if(key == 'd') {
		debug = !debug;
	}
}

void mirrorApp::keyReleased  (int key) { 
}

void mirrorApp::mouseMoved(int x, int y ) {
}

void mirrorApp::mouseDragged(int x, int y, int button) {

}

void mirrorApp::mousePressed(int x, int y, int button) {
}

void mirrorApp::mouseReleased() {
}

