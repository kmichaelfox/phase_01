//
//  NoteEvent.hpp
//  phase_01
//
//  Created by Kelly Fox on 3/20/16.
//
//

#ifndef NoteEvent_hpp
#define NoteEvent_hpp

#include <iostream>

class ofxOscSender;
//class ofTrueTypeFont;

enum Accidental : short;

class NoteEvent {
    int note;
    long int noteID;
    int staffPos;
    Accidental acc;
    float offset = 0;
    float duration = 0;
    bool active = false;
    
    static long int noteIDCounter;
    static std::auto_ptr<ofxOscSender> oscSender;
    static std::string addr;
    
public:
    NoteEvent(int note, int staffPos, Accidental acc, float offset, float duration);
    ~NoteEvent();
    void tick(float timeElapsed);
    
    void toggleNoteOff();
    float getOffset();
    float getDuration();
    int getNote();
    Accidental getAccidental();
    int getStaffPosition();
    bool isCompleted();
    
    static void setOscSender(ofxOscSender* s, std::string addr);
    static std::string getOscAddr();
    static bool isOscSenderInit();
};

enum Accidental : short {
    FLAT = -1,
    NATURAL = 0,
    SHARP = 1
};

#endif /* NoteEvent_hpp */
