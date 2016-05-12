//
//  DynamicEvent.hpp
//  phase_01
//
//  Created by Kelly Fox on 5/7/16.
//
//

#ifndef DynamicEvent_hpp
#define DynamicEvent_hpp

#include <iostream>
#include <deque>

enum Dynamic : short;
enum DynamicType : short;


//typedef std::deque<ofVec3f> DynamicCurve;

class DynamicEvent {
    DynamicType type;
    Dynamic dynamic;
    float drawShiftX = -0.5;
    float offset = 0;
    float phase = 0;
    float duration = 0;
    bool active = false;
    
    //DynamicCurve curve;
//    ofxAnimatable
    
public:
    DynamicEvent(Dynamic d, float offset);
    DynamicEvent(float durSeconds, float offset);
    ~DynamicEvent() {}
    void tick(float timeElapsed);
    
    void toggleDynamicOff();
    float getOffset();
    Dynamic getDynamic();
    std::string getDynamicAsString();
    DynamicType getDynamicType();
    //DynamicCurve & getCurve();
    bool isCompleted();
    
    static Dynamic getRandomDynamic();
};

enum Dynamic : short {
    niente = 0,
    ppp,
    pp,
    p,
    mp,
    mf,
    f,
    ff,
    fff
};

enum DynamicType : short {
    STATIC,
    DYNAMIC
};


#endif /* DynamicEvent_hpp */
