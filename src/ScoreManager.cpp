//
//  ScoreManager.cpp
//  phase_01
//
//  Created by Kelly Fox on 5/9/16.
//
//

#include "ScoreManager.hpp"
#include "ofMain.h"
#include "ofEvents.h"
#include "ofxOsc.h"
#include "Helpers.hpp"

typedef struct analogRange {
    int low;
    int high;
} AnalogRange;

const AnalogRange b1 = {911, 912};
const AnalogRange b2 = {952, 953};
const AnalogRange b3 = {962, 963};
const AnalogRange b4 = {965, 966};

ScoreManager::ScoreManager() {
    //NoteEvent::setOscSender(new ofxOscSender(), "/phase01");
    
    // TODO: Setup arduino listener
    bSetupArduino = false;
    if (!ard.connect("/dev/tty.usbmodem1421", 57600)) return;
    ofAddListener(ard.EInitialized, this, &ScoreManager::setupArduino);
}

ScoreManager::~ScoreManager() {
    if (!players.empty()) {
        for (auto it = players.begin(); it != players.end(); ) {
            delete (*it);
            ++it;
        }
        players.clear();
    }
}

void ScoreManager::draw() {
    int i = 0;
    for (Player *& p : players) {
        p->draw(ofGetWindowWidth()/dimensions * (i%dimensions),
                p->getHeight()*(i/dimensions));
        ++i;
    }
}

void ScoreManager::update() {
    if (bSetupArduino) updateArduino();
    
    scene.update();
    if (scene.hasAvailableTriggers()) {
        vector<int> triggers = scene.getTriggers();
        for (int t : triggers) {
            Player * p = players[t];
            p->queueSequence(scene.getSequence(t), p->getTempo());
        }
    }
}

void ScoreManager::resize(int w, int h) {
    windowAspect = w / h;
    
    for (Player * &p : players) {
        if (windowAspect < p->getAspectRatio()) {
            p->setWidth(ofGetWindowWidth()/dimensions);
        } else {
            p->setHeight(ofGetWindowHeight()/dimensions);
        }
    }
}

void ScoreManager::addPlayer(Player * p) {
    players.push_back(p);
    
    if (players.size() != 1) {
        while (players.size() > pow(2, dimensions)) {dimensions++;}
    }
    
    for (Player *& p : players) {
        if (windowAspect < p->getAspectRatio()) {
            p->setWidth(ofGetWindowWidth()/dimensions);
        } else {
            p->setHeight(ofGetWindowHeight()/dimensions);
        }
    }
}

void ScoreManager::newSequence() {
    bool isOnePlayerActivated = false;
    for (Player *& p : players) {
        isOnePlayerActivated = isOnePlayerActivated || p->isActive();
    }
    if (!isOnePlayerActivated) {
        players[(int)ofRandom(players.size())]->activate(true);
    }
    
    bool firstSeq = scene.isEmpty();
    
    if (firstSeq) scene.setNoteType((ofRandom(2) < 1) ? PIZZ : ARCO);
    
    Sequence s;
    int sequenceLength = ofRandom(15) + 3;
    int lowerBound = 0;
    int upperBound = 15;
    s.push_back(Note((int)ofRandom(lowerBound, upperBound),
                     (NoteLength)((int)ofRandom(2, 5)),
                     scene.getNoteType()));
    for (int i = 0; i < sequenceLength; i++) {
        int prev = s.back().note;
        if (prev == -1) {
            prev = s.front().note;
        }
        bool ascend = (ofRandom(2) > 1);
        if ((ascend && prev == upperBound) || (!ascend && prev == lowerBound)) {
            ascend = !ascend;
        }
        int next = prev;
        if (ascend) {
            next += ((int)ofRandom(7));
        } else {
            next -= ((int)ofRandom(7));
        }
        next = Helpers::clip(next, lowerBound, upperBound);
        if (ofRandom(10) > 1.6) {
            s.push_back(Note((int)next, (NoteLength)((int)ofRandom(2, 5)), scene.getNoteType()));
        } else {
            s.push_back(Note(-1,
                             (NoteLength)((int)ofRandom(2, 5)),
                             scene.getNoteType())
                        );
        }
    }
    
    // TODO: switch to permutation functions from single operator below
    
    if (!firstSeq) {
        NoteType t = scene.getNoteType();
        scene = ScoreScene();
        // 60% chance to use same technique, else switch to the other
        scene.setNoteType((ofRandom(10) < 6) ? t : (NoteType)((t+1)%2));
    }
    
    for (Player *& p : players) {
        //p->queueSequence(Helpers::rangedMidiFromPitchClass(s, p->getRange(), p->isFirstChair()), p->getTempo());
        scene.addPlayerSequence(Helpers::rangedMidiFromPitchClass(s, p->getRange(), p->isFirstChair()));
    }
    //if (!firstSeq) scene.generateTimers();
    scene.generateTimers();
}

void ScoreManager::activate(int playerNum) {
    playerNum--;
    if (playerNum < players.size() && playerNum >= 0) {
        if (players[playerNum]->isActive()) {
            players[playerNum]->activate(false);
            int newActivePlayer;
            do {
                newActivePlayer = ofRandom(players.size());
            } while (newActivePlayer == playerNum);
            players[newActivePlayer]->activate(true);
            
            
            // TODO: activation action
            newSequence();
        }
    }
}

void ScoreManager::pause() {
    for (Player *& p : players) {
        p->idle(false);
        p->activate(false);
    }
}

void ScoreManager::resume() {
    if (!seq.empty()) {
        newSequence();
    } else {
        scene.generateTimers();
    }
}

void ScoreManager::stop() {
    for (Player *& p : players) {
        p->idle(true);
        p->activate(false);
    }
}

int ScoreManager::getNumPlayers() {
    return players.size();
}

void ScoreManager::setPlayerTempo(int playerNum, float tempo) {
    if (playerNum >= 0 && playerNum < players.size()) {
        players[playerNum]->setTempo(tempo);
    }
}


void ScoreManager::setupArduino(const int & version) {
    ofRemoveListener(ard.EInitialized, this, &ScoreManager::setupArduino);
    bSetupArduino = true;
    
    ofLogNotice() << ard.getFirmwareName();
    ofLogNotice() << "firmata v" << ard.getMajorFirmwareVersion() << "." << ard.getMinorFirmwareVersion();
    
    ard.sendAnalogPinReporting(0, ARD_ANALOG);
    ofAddListener(ard.EAnalogPinChanged, this, &ScoreManager::analogPinChanged);
}

void ScoreManager::analogPinChanged(const int & pinNum) {
    int val = ard.getAnalog(pinNum);
    if (val >= b1.low && val <= b1.high) {
        cout << 1 << endl;
        val = 1;
    } else if (val >= b2.low && val <= b2.high) {
        cout << 2 << endl;
        val = 2;
    } else if (val >= b3.low && val <= b3.high) {
        cout << 3 << endl;
        val = 3;
    } else if (val >= b4.low && val <= b4.high) {
        cout << 4 << endl;
        val = 4;
    } else {
        return;
    }
    
    activate(val);
}

void ScoreManager::updateArduino() {
    ard.update();
}