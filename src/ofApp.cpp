#include "ofApp.h"
#include "NoteEvent.hpp"
#include "ofxOscSender.h"


//--------------------------------------------------------------
void ofApp::setup(){
    windowAspect = ofGetWindowWidth() / ofGetWindowHeight();
    
    if (numPlayers != 1) {
        while (numPlayers > pow(2, dimensions)) {dimensions++;}
    }
    cout << dimensions << endl;
    
    //s = new Staff(BASS);
    for (int i = 0; i < numPlayers; i++) {
        Player *p = new Player(VIOLIN);
        p->setTempo(60+(i*0.1));
        if (windowAspect < p->getAspectRatio()) {
            p->setWidth(ofGetWindowWidth()/dimensions);
        } else {
            p->setHeight(ofGetWindowHeight()/dimensions);
        }
        
        players.push_back(p);
    }
    
    NoteEvent::setOscSender(new ofxOscSender(), "/phase_1");
    //cout << NoteEvent::getOscAddr() << endl;
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    int i = 0;
    for (Player *& p : players) {
        p->draw(ofGetWindowWidth()/dimensions * (i%dimensions),
                p->getHeight()*(i/dimensions));
        ++i;
    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'a') {
        active = !active;
    } else if (key == 'i') {
        cout << "idle " << cmd << endl;
        for (Player *& p : players) {
            p->idle(cmd); // if shift=true, kill seq immediately
        }
    } else if (key == 'n') {
        Sequence s;
        int sequenceLength = ofRandom(15) + 3;
        for (int i = 0; i < sequenceLength; i++) {
            if (ofRandom(10) > 0.5) {
                s.push_back(Note((int)ofRandom(55, 80), (NoteLength)((int)ofRandom(2, 4.9))));
            } else {
                s.push_back((Note(-1, (NoteLength)((int)ofRandom(2, 4.9)))));
            }
            //s.push_back(Note(0, (NoteLength)((int)ofRandom(3))));
        }
        for (Player *& p : players) {
            p->queueSequence(s, p->getTempo());
        }
        
        
        //p->createSingleNote(0);
    } else if (key == OF_KEY_COMMAND) {
        cmd = true;
        //cout << shift << endl;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if (key == OF_KEY_SHIFT) {
        cmd = false;
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    windowAspect = w / h;
    
    for (Player * &p : players) {
        if (windowAspect < p->getAspectRatio()) {
            //        fbo.draw(0, 0, ofGetWindowWidth(), ofGetWindowWidth() * fbo.getHeight() / fbo.getWidth());
            p->setWidth(ofGetWindowWidth()/dimensions);
        } else {
            //        fbo.draw(0, 0, ofGetWindowHeight() * fbo.getWidth() / fbo.getHeight(), ofGetWindowHeight());
            p->setHeight(ofGetWindowHeight()/dimensions);
        }
    }
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}

void ofApp::exit() {
//    if (p != nullptr) {
//        delete p;
//    }
    if (!players.empty()) {
        for (auto it = players.begin(); it != players.end(); ) {
            delete (*it);
            ++it;
        }
        players.clear();
    }
}