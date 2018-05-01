
#include "KinectMotionSensor.h"

np::KinectMotionSensor::KinectMotionSensor() {
	previous = new unsigned char [640*480];
	for(int i=0; i<640*480; ++i){ previous[i] = 0; }

	motionValue = 0.0f;

	buffers = 16;
	write=1;
	read=0;
}

void np::KinectMotionSensor::setup( int columnWidth ) {
	
	pixelsPerColumns = columnWidth;
	nColums = 640/pixelsPerColumns;

	columnsSums.resize(nColums);
	diffColumns.resize(buffers);
	for( size_t i=0; i<diffColumns.size(); ++i ){
		diffColumns[i].resize( nColums );
		for( size_t k=0; k<diffColumns[i].size(); ++k ){ diffColumns[i][k] = 0.0f; }
	}	   

	colDivisor = 1.0f / (float) nColums;

	ui.setName("kinect motion sensor");
	ui.add( scale.set("scale", 20, 16, 32) );
	ui.add( min.set("motion min", 0.0f, 0.0f, 1.0f) );
	ui.add( max.set("motion max", 1.0f, 0.0f, 1.0f) );
	ui.add( cMin.set("columns min", 0.0f, 0.0f, 1.0f ) );
	ui.add( cMax.set("columns max", 1.0f, 0.0f, 1.0f) );

	kinect.init(true);
	kinect.open();	
	
	cout<<"[kinect motion sensor] starting thread\n";
    startThread();
}


void np::KinectMotionSensor::update(){
	
	kinect.update();
		
	if( kinect.isFrameNew() ){
		
		unsigned char * pixels = kinect.getPixels().getData();
		
		int colCounter = 0;

		int c = 0;
		
		for( int x=0; x<640; x++ ){
			
			if( colCounter==0 ){
				columnsSums[c]=0;
			}
			
			for( int y=0; y<480; y++ ){
				int i = x + y*640;
				int prev = previous[i];
				int next = pixels[i];
				int diff = next - prev;
				int neg = -diff;
				diff = (diff>0) ? diff : neg;
				columnsSums[c] += diff;
				previous[i] = pixels[i];
			}
			
			colCounter++;
			if( colCounter>=pixelsPerColumns ){
				c++;
				colCounter=0;
			}
		}
		
		float scaling = pow( 2, (double)scale );
		
		float minFinal = min * scaling;
		float maxFinal = max * scaling;
		float cMinFinal = cMin * scaling * colDivisor;
		float cMaxFinal = cMax * scaling * colDivisor;
		int frameDiff = 0;
		for(int i = 0; i < (int)columnsSums.size(); i++) {
			diffColumns[write][i] = ofMap( columnsSums[i], cMinFinal, cMaxFinal, 0.0f, 1.0f, true );
			frameDiff += columnsSums[i];
		}	

		// workaround to avoid skipped frames
		if( !(frameDiff == 0 && motionValue > 0.0f) ){
			read.store( write );
			write++;
			if(write>=buffers){ write = 0; }

			float newMotion = ofMap( (float)frameDiff, minFinal, maxFinal, 0.0f, 1.0f, true ); 
			motionValue = newMotion;					
		}
	}
	
}

void np::KinectMotionSensor::threadedFunction(){
	while(isThreadRunning()) {
		update();		
		ofSleepMillis( 8 );
	}
}  

void np::KinectMotionSensor::exit() {
	cout<<"[kinect motion sensor] stopping thread...\n";
	stopThread();
	kinect.close();
	cout<<"[kinect motion sensor] ... stopped\n";
}

float np::KinectMotionSensor::motion() const{
	return motionValue.load();
}
const vector<float> & np::KinectMotionSensor::columns() const {
	return diffColumns[read];
}

int np::KinectMotionSensor::getColumnWidth() const {
	return pixelsPerColumns;
}
