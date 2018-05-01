
#pragma once

#include "ofMain.h"
#include "ofxKinect.h"
#include <atomic>

namespace np {
	
class KinectMotionSensor : public ofThread {

public:
	KinectMotionSensor();
	void setup( int columnWidth=8 );
	void exit();
	
	void update();
	
	float motion() const;
	const vector<float> & columns() const;
	int getColumnWidth() const;
	

	ofParameterGroup ui;
		ofParameter<float> min;
		ofParameter<float> max;
		ofParameter<float> cMin;
		ofParameter<float> cMax;
		ofParameter<int> scale;

private:

	ofxKinect kinect;
    void threadedFunction();
    
	float					colDivisor;
	int						nColums;
	int						pixelsPerColumns;

	atomic<float> 			motionValue;
	vector<vector<float>> 	diffColumns; // columns
	int buffers;
	int 		write;
	atomic<int> read;
	
	vector<int> columnsSums;
		
	unsigned char * previous;

};

}

		
