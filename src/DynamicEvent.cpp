//
//  DynamicEvent.cpp
//  phase_01
//
//  Created by Kelly Fox on 5/7/16.
//
//

#include "DynamicEvent.hpp"
#include "ofMain.h"
#include "ofxAnimatableFloat.h"

using namespace std;

DynamicEvent::DynamicEvent(Dynamic d, float offset)
: type(STATIC)
, dynamic(d)
, offset(offset)
{ }

DynamicEvent::DynamicEvent(float durSeconds, float offset)
: type(DYNAMIC)
, offset(offset)
, duration(durSeconds)
{ }

void DynamicEvent::tick(float timeElapsed) {
    if (offset > drawShiftX) {
        offset -= timeElapsed;
        
        if (offset < drawShiftX && type == DYNAMIC) {
            duration += offset;
            offset = drawShiftX;
            
            active = true;
        }
    } else {
        if (!active) {
            active = true;
        }
        duration -= timeElapsed;
    }
}

void DynamicEvent::toggleDynamicOff() {
    
}

float DynamicEvent::getOffset() {
    return offset;
}

Dynamic DynamicEvent::getDynamic() {
    return dynamic;
}

string DynamicEvent::getDynamicAsString() {
    switch (dynamic) {
        case niente:
            return "niente";
            
        case ppp:
            return "ppp";
            
        case pp:
            return "pp";
            
        case p:
            return "p";
            
        case mp:
            return "mp";
            
        case mf:
            return "mf";
            
        case f:
            return "f";
            
        case ff:
            return "ff";
            
        case fff:
            return "fff";
    }
}

//DynamicCurve & DynamicEvent::getCurve() {
//    return & curve;
//}

bool DynamicEvent::isCompleted() {
    return (type == DYNAMIC) && (duration <= 0);
}

Dynamic DynamicEvent::getRandomDynamic() {
    switch ((int)ofRandom(7.9)+1) {
        case 0:
            return niente;
            
        case 1:
            return ppp;
            
        case 2:
            return pp;
            
        case 3:
            return p;
            
        case 4:
            return mp;
            
        case 5:
            return mf;
            
        case 6:
            return f;
            
        case 7:
            return ff;
            
        case 8:
            return fff;
    }
}