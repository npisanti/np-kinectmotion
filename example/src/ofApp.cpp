#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	ofDisableAntiAliasing();
	ofBackground(0);
	
	kMotion.setup();

    gui.setup("", "settings.xml", ofGetWidth()-210, 20 );
    gui.add( kMotion.ui );
    gui.loadFromFile("settings.xml");
    
    ofSetWindowTitle( "kinect motion sensor" );
}

//--------------------------------------------------------------
void ofApp::update(){
	
}

//--------------------------------------------------------------
void ofApp::draw(){

	gui.draw();
	
	
	ofTranslate( 10, 10 );
	ofSetColor( 255 );	
	
	ofNoFill();
	ofDrawRectangle(0, 15, 640, 10 );
	ofFill();
	ofDrawBitmapString( "total motion", 20, 12 );
	ofDrawRectangle(0, 15, kMotion.motion()*480, 10 );
	
	auto & columns = kMotion.columns();
    ofTranslate(0, 35);
	ofNoFill();
	ofDrawRectangle(0, 15, 640, 100 );	
	ofDrawBitmapString( "columns motion", 20, 12 );
	ofFill();
    for( size_t i=0; i<columns.size(); ++i){
		int cw = kMotion.getColumnWidth();
		ofDrawRectangle( i*cw, 15, cw, 100*columns[i]);
	}	

}

//--------------------------------------------------------------
void ofApp::exit(){
	kMotion.exit();
}

//--------------------------------------------------------------		
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){}
void ofApp::mouseMoved(int x, int y){}
void ofApp::mouseDragged(int x, int y, int button){}
void ofApp::mousePressed(int x, int y, int button){}
void ofApp::mouseReleased(int x, int y, int button){}
void ofApp::mouseEntered(int x, int y){}
void ofApp::mouseExited(int x, int y){}
void ofApp::windowResized(int w, int h){}
void ofApp::gotMessage(ofMessage msg){}
void ofApp::dragEvent(ofDragInfo dragInfo){ }
//--------------------------------------------------------------
