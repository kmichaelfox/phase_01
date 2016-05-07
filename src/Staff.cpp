//
//  Staff.cpp
//  phase_01
//
//  Created by Kelly Fox on 3/4/16.
//
//

#include "Staff.hpp"
#include "NoteEvent.hpp"
#include "Clef.hpp"
#include "Transform.hpp"

ofTrueTypeFont Staff::notationFont;
ofTrueTypeFont Staff::accidentalsFont;

Staff::Staff(ClefType _clef, bool transposing) {
    instrument = getDataForInstrument(Instrument::GENERIC);
    clef = _clef;
    transposeFlag = transposing;
    
    if (!notationFont.isLoaded()) {
        notationFont.load("Fonts/Clefs/CLEFS.TTF", 240, true, true);
    }
    
    if (!accidentalsFont.isLoaded()) {
        accidentalsFont.load("Fonts/Accidentals/Accid.ttf", 80, true, true);
    }
    
    timer = ofGetElapsedTimef();
}

Staff::Staff(Instrument _instrument) {
    instrument = getDataForInstrument(_instrument);
    clef = instrument->clef;
    if (instrument->transposition != 0) {
        transposeFlag = true;
    } else {
        transposeFlag = false;
    }
    
    if (!notationFont.isLoaded()) {
        notationFont.load("Fonts/Clefs/CLEFS.TTF", 240, true, true);
    }
    
    if (!accidentalsFont.isLoaded()) {
        accidentalsFont.load("Fonts/Accidentals/Accid.ttf", 80, true, true);
    }
    
    timer = ofGetElapsedTimef();
}

Staff::~Staff() {
    for (NoteEvent* &ne : activeNotes) {
        ne->toggleNoteOff();
    }
    
    if (instrument != nullptr) {
        delete instrument;
    }
}

void Staff::draw() {
    draw(0, 0);
}

void Staff::draw(float x, float y) {
    
    ofClear(ofColor::black);
    
    ofPushMatrix();
    ofTranslate(100, drawHeight / 2);
    
    // draw Staff lines
    ofTranslate(0, -3);
    ofSetColor(ofColor::white);
    ofDrawRectangle(0, -staffLineDistance * 4, staffWidth, staffLineThickness);
    ofDrawRectangle(0, -staffLineDistance * 2, staffWidth, staffLineThickness);
    ofDrawRectangle(0, 0, staffWidth, staffLineThickness);
    ofDrawRectangle(0, staffLineDistance * 2, staffWidth, staffLineThickness);
    ofDrawRectangle(0, staffLineDistance * 4, staffWidth, staffLineThickness);
    
    // Draw Clef
    ofSetColor(ofColor::white);
    drawClef();
    
    // Draw Playhead
    ofPushMatrix();
    //ofTranslate(positionX,0);
    //ofSetColor(ofColor::darkGray);
    //ofDrawRectangle(0, 0 - (staffLineDistance / 4.0f), 1 * (drawWidth * scrollSpeed), staffLineDistance / 2);
    
//    ofSetColor(ofColor::black);
//    ofRotate(-20);
//    ofDrawEllipse(0, 0, staffLineDistance*2.25, staffLineDistance*1.5);
//    ofRotate(20);
    ofPopMatrix();
    //ofTranslate(-300,0);
    
    ofTranslate(playheadOffsetDistance - 2, 0);
    ofSetColor(ofColor::white);
    //    ofDrawEllipse(0, 0, staffLineDistance*3, staffLineDistance*2);
    //ofDrawEllipse(0, 0, staffLineDistance*0.5, staffLineDistance*2);
    for (NoteEvent* & e : activeNotes) {
//        if (e->getNote() != -1) {
//            ofDrawRectangle(e->getOffset()*scrollSpeed*drawWidth, 0-(staffLineDistance/4.0f), e->getDuration()*(drawWidth*scrollSpeed), staffLineDistance/2);
//            ofDrawEllipse(e->getOffset()*scrollSpeed*drawWidth, 0, staffLineDistance*0.5, staffLineDistance*2);
//        }
//        e->tick(dTime);
        drawNoteEvent(e);
    }
    ofTranslate(2, 0);
    
    ofSetColor(ofColor::red);
    ofDrawRectangle(0, -drawHeight / 2, -playheadThickness, drawHeight);
    ofPopMatrix();
    
    
    
    
    
    
    ofPushMatrix();
    ofTranslate(100, drawHeight*5/6);
    ofSetColor(ofColor::red);
    ofDrawRectangle(0, -50-5, staffWidth, 10);
    //ofDrawLine(0, 0, staffWidth, 0);
    ofDrawRectangle(0, 50-5, staffWidth, 10);
    ofPopMatrix();
    
    
    
    
    
    
    
    // Draw Active border
    if (active) {
        ofSetColor(ofColor::red);
        ofDrawRectangle(0, 0, drawWidth, activeHighlightBorderSize);
        ofDrawRectangle(0, drawHeight - activeHighlightBorderSize, drawWidth, activeHighlightBorderSize);

        ofDrawRectangle(0, 0, activeHighlightBorderSize, drawHeight);

        ofDrawRectangle(drawWidth - activeHighlightBorderSize, 0, activeHighlightBorderSize, drawHeight);
    }
    
    updateTimer();
    //positionX -= (dTime * scrollSpeed * drawWidth);
    
    // clean up completed NoteEvents in activeNotes
    for (auto i = activeNotes.begin(); i != activeNotes.end(); ++i) {
        if ((*i)->isCompleted()) {
            delete (*i);
            activeNotes.erase(i);
        }
    }
    
    // increment nextNote counter
    if (!sequences.empty()) {
        if (activeNotes.empty()) {
            activeNotes.push_back(eventFromNote(sequences.front()[nextNote++], tempo));
        } else if (activeNotes.back()->getOffset() + activeNotes.back()->getDuration() < 3) {
            activeNotes.push_back(eventFromNote(sequences.front()[nextNote++], tempo));
        }
        
        // wrap nextNote if greater than sequence length
        if (nextNote > sequences.front().size()-1) {
            nextNote = 0;
            
            // transition to new sequence, if exists
            if (sequences.size() > 1 || idleOnNextCycle) {
                sequences.pop_front();
                idleOnNextCycle = false;
            }
        }
    }
}

float Staff::getScrollSpeed() {
    return scrollSpeed;
}

void Staff::setScrollSpeed(float speed) {
    scrollSpeed = std::max(0.0f, std::min(speed, 1.0f));
}

float Staff::getTempo() {
    return tempo;
}

void Staff::setTempo(float tempo) {
    this->tempo = tempo;
}

Range Staff::getInstrumentRange() {
    return instrument->range;
}

void Staff::setInstrumentRange(Range r) {
    instrument->range = r;
}

void Staff::clearSequenceQueue(bool immediately) {
    if (immediately) {
        sequences.clear();
        cout << "cleared sequence " << sequences.size() << endl;
    } else {
        idleOnNextCycle = true;
    }
}

void Staff::queueSequence(Sequence seq, float tempo) {
    for (int i = 0; i < seq.size(); i++) {
        eventFromNote(seq[i], tempo);
    }
    
    sequences.push_back(seq);
}

bool Staff::isStartPointClear() {
    return true;
}

void Staff::createNote(int note) {
    if (sequences.empty()) {
        sequences.push_back(Sequence());
    }
    if (ofRandom(10) > 0.5) {
        sequences.front().push_back(Note((int)ofRandom(40, 70), (NoteLength)((int)ofRandom(2, 4.9))));
    } else {
        sequences.front().push_back((Note(-1, (NoteLength)((int)ofRandom(2, 4.9)))));
    }
    //activeNotes.push_back(eventFromNote(Note((int)ofRandom(40, 70), (NoteLength)((int)ofRandom(4.9))), tempo));
}

void Staff::drawNoteEvent(NoteEvent* &e) {
    if (e->getNote() != -1) {
//        int centerNote = degreeFromMidi(Clef::ClefData.at(ALTO).centerNote);
//        int staffPosition = centerNote - e->;
        //int midiNote = e->getNote();
        //if (transposeFlag) midiNote += instrument->transposition;
        //int octaveSplit = (midiNote - centerNote) / 12;
        //int intervalSplit = (degreeFromMidi(midiNote) - degreeFromMidi(centerNote)) % 8;
        //cout << intervalSplit << " " << octaveSplit << endl;
        //cout << e->getNote() - centerNote << endl;
        float offsetX = e->getOffset()*scrollSpeed*drawWidth;
        float offsetY = (e->getStaffPosition()*staffLineDistance);
        ofSetColor(ofColor::gray);
        ofDrawRectangle(
                        offsetX,
                        offsetY-(staffLineDistance/4.0f),
                        e->getDuration()*(drawWidth*scrollSpeed),
                        staffLineDistance/2
                        );
        ofSetColor(ofColor::lightGray);
        ofNoFill();
        ofDrawRectangle(offsetX,
                        offsetY-(staffLineDistance/4.0f),
                        e->getDuration()*(drawWidth*scrollSpeed),
                        staffLineDistance/2
                        );
        ofFill();
        ofSetColor(ofColor::white);
        
        string acc = "";
        if (e->getAccidental() == Accidental::SHARP) {
            acc = "#";
        } else if (e->getAccidental() == Accidental::FLAT) {
            acc = "b";
        }
        accidentalsFont.drawString(acc,
                                   offsetX-40,
                                   offsetY
                                   );
        ofDrawEllipse(offsetX,
                      offsetY,
                      staffLineDistance*0.5,
                      staffLineDistance
                      );
    }
    e->tick(dTime);
}

int Staff::degreeFromMidi(int note) {
    if (instrument->accidental == FLAT) {
        return (note/12*8)+Transform::pitchClassToNoteName(note%12, true);
    }
    
    return (note/12*8)+Transform::pitchClassToNoteName(note%12, false);
}

Accidental Staff::degreeAccidentalFromMidi(int note) {
    switch (note) {
        case 1:
        case 3:
        case 6:
        case 8:
        case 10:
            return instrument->accidental;
            
        default:
            return NATURAL;
    }
}

int Staff::transposeForInstrument(int note) {
    return note + instrument->transposition;
}

NoteEvent* Staff::eventFromNote(Note note, float tempo) {
    float dur = 60.0f / tempo;
    int midiNote = note.note;
    if (midiNote < -1 || midiNote > 127) {
        midiNote = (int)ofRandom(10, 117);
    }
    switch (note.dur) {
        case WHOLE:
            dur *= 4;
            break;
            
        case HALF:
            dur *= 2;
            break;
            
        case QUARTER:
            break;
            
        case EIGHTH:
            dur *= 0.5f;
            break;
            
        case SIXTEENTH:
            dur *= 0.25f;
            break;
            
        default:
            break;
    }
    int degree = degreeFromMidi(transposeForInstrument(note.note));
    //cout << note.note << " " << degree << " " << " " << Clef::ClefData.at(TREBLE).centerNote << endl;
    Accidental acc = degreeAccidentalFromMidi(note.note%12);
    return new NoteEvent(note.note, degreeFromMidi(Clef::ClefData.at(TREBLE).centerNote) - degree, acc, 3, dur);
}

InstrumentData* Staff::getDataForInstrument(Instrument _instrument) {
    switch (_instrument) {
        case GENERIC:
            return new InstrumentData(_instrument, 0, TREBLE, 60, 80);
            break;
            
        case PICCOLO:
            return new InstrumentData(_instrument, -12, TREBLE, 0, 0);
            break;
            
        case FLUTE:
            return new InstrumentData(_instrument, 0, TREBLE, 0, 0);
            break;
            
        case ALTO_FLUTE:
            return new InstrumentData(_instrument, 5, TREBLE, 0, 0);
            break;
            
        case OBOE:
            return new InstrumentData(_instrument, 0, TREBLE, 0, 0);
            break;
            
        case ENGLISH_HORN:
            return new InstrumentData(_instrument, 7, TREBLE, 0, 0);
            break;
            
        case CLARINET_Bb:
            return new InstrumentData(_instrument, 2, TREBLE, 0, 0);
            break;
            
        case CLARINET_A:
            return new InstrumentData(_instrument, 3, TREBLE, 0, 0);
            break;
            
        case BASS_CLARINET:
            return new InstrumentData(_instrument, 14, TREBLE, 0, 0);
            break;
            
        case SOPRANO_SAX:
            return new InstrumentData(_instrument, 2, TREBLE, 0, 0);
            break;
            
        case ALTO_SAX:
            return new InstrumentData(_instrument, 9, TREBLE, 0, 0);
            break;
            
        case TENOR_SAX:
            return new InstrumentData(_instrument, 14, TREBLE, 0, 0);
            break;
            
        case BARITONE_SAX:
            return new InstrumentData(_instrument, 21, TREBLE, 0, 0);
            break;
            
        case BASS_SAX:
            return new InstrumentData(_instrument, 26, TREBLE, 0, 0);
            break;
            
        case BASSOON:
            return new InstrumentData(_instrument, 0, BASS, 0, 0);
            break;
            
        case CONTRABASSOON:
            return new InstrumentData(_instrument, 12, TREBLE, 0, 0);
            break;
            
        case HORN:
            return new InstrumentData(_instrument, 7, TREBLE, 0, 0);
            break;
            
        case TRUMPET:
            return new InstrumentData(_instrument, 2, TREBLE, 0, 0);
            break;
            
        case CORNET:
            return new InstrumentData(_instrument, 2, TREBLE, 0, 0);
            break;
            
        case TROMBONE:
            return new InstrumentData(_instrument, 0, BASS, 0, 0);
            break;
            
        case EUPHONIUM:
            return new InstrumentData(_instrument, 0, BASS, 0, 0);
            break;
            
        case TUBA:
            return new InstrumentData(_instrument, 0, BASS, 0, 0);
            break;
            
        case VIOLIN:
            return new InstrumentData(_instrument, 0, TREBLE, 55, 100);
            break;
            
        case VIOLA:
            return new InstrumentData(_instrument, 0, ALTO, 38, 91);
            break;
            
        case VIOLONCELLO:
            return new InstrumentData(_instrument, 0, BASS, 36, 79);
            break;
            
        case CONTRABASS:
            return new InstrumentData(_instrument, 0, BASS, 0, 0);
            break;
            
        default:
            break;
    }
}

void Staff::drawClef() {
    ofSetColor(ofColor::white);
    switch (clef) {
        case TREBLE_8VA:
            notationFont.drawString("7", 25, (staffLineDistance*2));
            break;
            
        case TREBLE:
            notationFont.drawString("q", 25, (staffLineDistance*2));
            break;
            
        case TREBLE_8VB:
            notationFont.drawString("Q", 25, (staffLineDistance*2));
            break;
            
        case ALTO:
            notationFont.drawString("z", 25, 0);
            break;
            
        case BASS:
            notationFont.drawString("a", 25, -(staffLineDistance*2));
            break;
            
        default:
            break;
    }
}

void Staff::updateTimer() {
    float now = ofGetElapsedTimef();
    dTime = now - timer;
    timer = now;
}