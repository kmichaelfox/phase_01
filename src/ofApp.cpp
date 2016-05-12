#include "ofApp.h"
#include "NoteEvent.hpp"
#include "ofxOscSender.h"


//--------------------------------------------------------------
void ofApp::setup(){
    //windowAspect = ofGetWindowWidth() / ofGetWindowHeight();
    ofSetBackgroundColor(ofColor::black);
    manager.resize(ofGetWindowWidth(), ofGetWindowHeight());
    
//    if (numPlayers != 1) {
//        while (numPlayers > pow(2, dimensions)) {dimensions++;}
//    }
    
    //s = new Staff(BASS);
    manager.addPlayer(new Player(VIOLIN, "vn. i"));
    manager.addPlayer(new Player(VIOLIN, "vn. ii"));
    manager.addPlayer(new Player(VIOLA, "va."));
    manager.addPlayer(new Player(VIOLONCELLO, "vc."));
    for (int i = 0; i < manager.getNumPlayers(); i++) {
        manager.setPlayerTempo(i, 60+(i*0.05));
    }
    
    
    //cout << NoteEvent::getOscAddr() << endl;
}

//--------------------------------------------------------------
void ofApp::update(){
    manager.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    manager.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key) {
        case '1':
        case '2':
        case '3':
        case '4':
            manager.activate(key - 48);
            break;
            
        case 'i': {
            if (cmd) {
                manager.stop();
            } else {
                manager.pause();
            }
            break;
        }
            
        case 's':
            manager.newSequence();
            
        case OF_KEY_COMMAND:
            cmd = true;
            break;
    }
//    if (key == 'a') {
//        active = !active;
//    } else if (key == 'i') {
//        if (cmd) {
//            manager.stop();
//        } else {
//            manager.pause();
//        }
////        for (Player *& p : players) {
////            p->idle(cmd); // if cmd=true, kill seq immediately
////        }
//    } else if (key == 'n') {
//        
//        
//        
//        //p->createSingleNote(0);
//    } else if (key == OF_KEY_COMMAND) {
//        cmd = true;
//        //cout << shift << endl;
//    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if (key == OF_KEY_COMMAND) {
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
//    windowAspect = w / h;
//    
//    for (Player * &p : players) {
//        if (windowAspect < p->getAspectRatio()) {
//            //        fbo.draw(0, 0, ofGetWindowWidth(), ofGetWindowWidth() * fbo.getHeight() / fbo.getWidth());
//            p->setWidth(ofGetWindowWidth()/dimensions);
//        } else {
//            //        fbo.draw(0, 0, ofGetWindowHeight() * fbo.getWidth() / fbo.getHeight(), ofGetWindowHeight());
//            p->setHeight(ofGetWindowHeight()/dimensions);
//        }
//    }
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
//    if (!players.empty()) {
//        for (auto it = players.begin(); it != players.end(); ) {
//            delete (*it);
//            ++it;
//        }
//        players.clear();
//    }
}