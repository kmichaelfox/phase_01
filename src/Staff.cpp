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
ofTrueTypeFont Staff::dynamicsFont;
ofTrueTypeFont Staff::labelFont;

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
    
    if (!dynamicsFont.isLoaded()) {
        dynamicsFont.load("Fonts/Dynamics/Times_it.ttf", 60, true, true);
    }
    
    if (!labelFont.isLoaded()) {
        labelFont.load("Fonts/Dynamics/Times_it.ttf", 80, true, true);
    }
    
    createDynamic(ppp);
    
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
    
    if (!dynamicsFont.isLoaded()) {
        dynamicsFont.load("Fonts/Dynamics/Times_it.ttf", 60, true, true);
    }
    
    if (!labelFont.isLoaded()) {
        labelFont.load("Fonts/Dynamics/Times_it.ttf", 80, true, true);
    }
    
    createDynamic(ppp);
    
    timer = ofGetElapsedTimef();
}

Staff::~Staff() {
    for (NoteEvent* &ne : activeNotes) {
        ne->toggleNoteOff();
        delete ne;
    }
    activeNotes.clear();
    
    for (DynamicEvent* &de : dynamics) {
        delete de;
    }
    dynamics.clear();
    
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
    
    // draw label
    labelFont.drawString(name, 0, -staffLineDistance * 8);
    
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
    
    // Move draw position to the right of the clef
    ofTranslate(playheadOffsetDistance - 2, 0);
    
    // Draw Dynamics
    for (DynamicEvent* & d : dynamics) {
        drawDynamicEvent(d);
    }
    
    // Draw NoteEvents
    ofSetColor(ofColor::white);
    for (NoteEvent* & e : activeNotes) {
        drawNoteEvent(e);
    }
    
    // Draw Playhead
    ofTranslate(2, 0);
    ofSetColor(ofColor::red);
    ofDrawRectangle(0, -drawHeight / 2, -playheadThickness, drawHeight);
    ofPopMatrix();
    
    
    
    
    
    // Draw dynamics curve pipeline
//    ofPushMatrix();
//    ofTranslate(100, drawHeight*5/6);
//    ofSetColor(ofColor::red);
//    ofDrawRectangle(0, -50-5, staffWidth, 10);
//    //ofDrawLine(0, 0, staffWidth, 0);
//    ofDrawRectangle(0, 50-5, staffWidth, 10);
//    ofPopMatrix();
    
    
    
    
    
    
    
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
    while(!activeNotes.empty() && activeNotes.front()->isCompleted()) {
        delete activeNotes.front();
        activeNotes.pop_front();
    }
//    for (auto it = activeNotes.begin(); it != activeNotes.end();) {
//        if ((*it)->isCompleted()) {
//            delete (*it);
//            it = activeNotes.erase(it);
//        } else {
//            ++it;
//        }
//    }
    
    // clean up inactive DynamicEvents in dynamics
    if (dynamics.size() > 1) {
        DynamicEvent * d = dynamics.front();
        dynamics.pop_front();
        
        if (dynamics.front()->getOffset() < 0.5) {
            delete d;
        } else {
            dynamics.push_front(d);
        }
    }
//    for (auto it = dynamics.begin(); it != dynamics.end();) {
//        if ((it+1) != dynamics.end()) {
//            if ((*(it+1))->getOffset() < 0.5) {
//                delete (*it);
//                it = dynamics.erase(it);
//            }
//        } else {
//            ++it;
//        }
//    }
    
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

string Staff::getName() {
    return name;
}

void Staff::setName(string n) {
    name = n;
}

Range Staff::getRange() {
    return instrument->range;
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

bool Staff::isActive() {
    return active;
}

void Staff::activate(bool state) {
    active = state;
    cout << "activating" << endl;
}

void Staff::createNote(int note) {
    if (sequences.empty()) {
        sequences.push_back(Sequence());
    }
    if (ofRandom(10) > 0.5) {
        sequences.front().push_back(Note((int)ofRandom(40, 70), (NoteLength)((int)ofRandom(2, 4.9)), PIZZ));
    } else {
        sequences.front().push_back((Note(-1, (NoteLength)((int)ofRandom(2, 4.9)), PIZZ)));
    }
    //activeNotes.push_back(eventFromNote(Note((int)ofRandom(40, 70), (NoteLength)((int)ofRandom(4.9))), tempo));
}

void Staff::createDynamic(Dynamic d) {
     dynamics.push_back(new DynamicEvent(d, 3));
}

void Staff::drawNoteEvent(NoteEvent* &e) {
    if (e->getNote() != -1) {
        float offsetX = e->getOffset()*scrollSpeed*drawWidth;
        float offsetY = (e->getStaffPosition()*staffLineDistance);
        
        // draw extra ledger lines if needed
        int dist = abs(e->getStaffPosition());
        if (dist > 5) {
            int sign = e->getStaffPosition() / dist;
            for (int i = 6; i <= dist; i++) {
                
                if (i%2 == 0) {
                    ofSetColor(ofColor::white);
                    ofDrawRectangle(offsetX-(staffLineDistance),
                                    i * sign * staffLineDistance,
                                    staffLineDistance*2,
                                    staffLineThickness
                                    );
                }
            }
        }
        
        // Draw trailing duration box
        if (e->getNoteType() != PIZZ) {
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
        }
        
        
        // draw accidental
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
        
        // draw note head
        ofDrawEllipse(offsetX,
                      offsetY,
                      staffLineDistance*0.5,
                      staffLineDistance
                      );
    }
    e->tick(dTime);
}

void Staff::drawDynamicEvent(DynamicEvent* &d) {
    float offsetX = d->getOffset()*scrollSpeed*drawWidth;
    float offsetY = staffLineDistance*10;
    
    ofSetColor(ofColor::white);
    dynamicsFont.drawString(d->getDynamicAsString(),
                            offsetX - 50,
                            offsetY
                            );
    
    d->tick(dTime);
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
    float dot = dur;
    for (int i = 0; i < note.dots; i++) {
        dur += dot /= 2;
    }
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
    
    if (midiNote == -1) {
        cout << "creating a rest" << endl;
        return new NoteEvent(note.note, note.type, -1, NATURAL, 3, dur);
    }
    
    if (ofRandom(1) < 0.05) {
        Dynamic randDynamic = DynamicEvent::getRandomDynamic();
        dynamics.push_back(new DynamicEvent(randDynamic, 3));
    }
    
    int degree = degreeFromMidi(transposeForInstrument(note.note));
    Accidental acc = degreeAccidentalFromMidi(note.note%12);
    return new NoteEvent(note.note, note.type, degreeFromMidi(Clef::ClefData.at(clef).centerNote) - degree, acc, 3, dur);
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
            return new InstrumentData(_instrument, 0, ALTO, 48, 91);
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