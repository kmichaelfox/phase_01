//
//  Staff.hpp
//  phase_01
//
//  Created by Kelly Fox on 3/4/16.
//
//

#ifndef Staff_hpp
#define Staff_hpp

#include "ofMain.h"
#include <iostream>
#include <Vector>
#include <deque>
#include "Note.hpp"
#include "NoteEvent.hpp"

enum ClefType : short;
enum Key : short;
enum Instrument : short;
struct InstrumentData;
struct Range;
typedef std::vector<Note> Sequence;
typedef std::deque<Sequence> SequenceQueue;
typedef std::deque<NoteEvent*> EventQueue;



class Staff {
    ClefType clef;
    InstrumentData* instrument;
    Key key;
    
    int drawWidth = 1920;
    int drawHeight = 1080;
    int staffLineDistance = 40;
    int staffLineThickness = 5;
    int staffWidth = 1700;
    int playheadThickness = 5;
    int playheadOffsetDistance = 600;
    
    bool active = true;
    bool idleOnNextCycle = false;
    int activeHighlightBorderSize = 5;
    
    float scrollSpeed = 0.2;
    float timer = 0;
    float dTime = 0;
    
    float positionX = 1920;
    
    bool transposeFlag;
    float tempo = 120.0;
    int nextNote = 0;
    
    SequenceQueue sequences;
    EventQueue activeNotes;
    
public:
    Staff(){}
    Staff(ClefType _clef, bool transposing = true);
    Staff(Instrument _instrument);
    ~Staff();
    
    void draw();
    void draw(float, float);
    
    float getScrollSpeed();
    void setScrollSpeed(float speed);
    float getTempo();
    void setTempo(float tempo);
    Range getInstrumentRange();
    void setInstrumentRange(Range r);
    
    void clearSequenceQueue(bool immediately);
    void queueSequence(Sequence seq, float tempo);
    bool isStartPointClear();
    
    void createNote(int note);
    
private:
    static ofTrueTypeFont notationFont;
    static ofTrueTypeFont accidentalsFont;
    ofShader shader;
    
    //Staff(ClefType _clef, Instrument _instrument, bool transposing);
    InstrumentData* getDataForInstrument(Instrument);
    NoteEvent* eventFromNote(Note note, float tempo);
    void drawNoteEvent(NoteEvent* &e);
    int degreeFromMidi(int note);
    Accidental degreeAccidentalFromMidi(int note);
    int transposeForInstrument(int note);
    void drawClef();
    void updateTimer();
};




//enum ClefT : short {
//    TREBLE_8VA = 0,
//    TREBLE,
//    TREBLE_8VB,
//    ALTO,
//    BASS
//};

struct Range {
    int low;
    int high;
};

enum Instrument : short {
    GENERIC = 0,
    PICCOLO,
    FLUTE,
    ALTO_FLUTE,
    OBOE,
    ENGLISH_HORN,
    CLARINET_Bb,
    CLARINET_A,
    BASS_CLARINET,
    SOPRANO_SAX,
    ALTO_SAX,
    TENOR_SAX,
    BARITONE_SAX,
    BASS_SAX,
    BASSOON,
    CONTRABASSOON,
    HORN,
    TRUMPET,
    CORNET,
    TROMBONE,
    EUPHONIUM,
    TUBA,
    PIANO,
    HARP,
    VIOLIN,
    VIOLA,
    VIOLONCELLO,
    CONTRABASS
};

enum Key : short {
    Cb = -7,
    Gb = -6,
    Db = -5,
    Eb = -4,
    Ab = -3,
    Bb = -2,
    F  = -1,
    C  =  0,
    G  =  1,
    D  =  2,
    A  =  3,
    E  =  4,
    B  =  5,
    Fs =  6,
    Cs =  7
};

//enum Accidental : short {
//    FLAT = -1,
//    NATURAL = 0,
//    SHARP = 1
//};

struct InstrumentData {
    Instrument instrument;
    Accidental accidental;
    int transposition;
    ClefType clef;
    Range range;
    
    InstrumentData(Instrument i, int t, ClefType c, int rangeLow, int rangeHigh)
    : instrument(i)
    , transposition(t)
    , clef(c)
    {
        range.low = rangeLow;
        range.high = rangeHigh;
        
        switch(instrument) {
            case VIOLIN:
            case VIOLA:
            case VIOLONCELLO:
            case CONTRABASS:
                accidental = SHARP;
                break;
                
            default:
                accidental = FLAT;
                break;
        }
    }
};

#endif /* Staff_hpp */
