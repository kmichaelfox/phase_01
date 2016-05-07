//
//  NoteEvent.cpp
//  phase_01
//
//  Created by Kelly Fox on 3/20/16.
//
//

#include "NoteEvent.hpp"
#include "ofMain.h"
#include "ofxOsc.h"
#include "Helpers.hpp"

long int NoteEvent::noteIDCounter = 0;
auto_ptr<ofxOscSender> NoteEvent::oscSender;
string NoteEvent::addr = string();

NoteEvent::NoteEvent(int note, int staffPos, Accidental acc, float offset, float duration)
: note(note)
, staffPos(staffPos)
, acc(acc)
, offset(offset)
, duration(duration) {
    noteID = NoteEvent::noteIDCounter++;
}

NoteEvent::~NoteEvent() {
    // send noteoff via osc
    if (note >= 0 && NoteEvent::isOscSenderInit()) {
        ofxOscMessage msg;
        msg.setAddress(NoteEvent::getOscAddr());
        msg.addInt32Arg(noteID);
        msg.addStringArg("off");
        NoteEvent::oscSender->sendMessage(msg);
    }
}

void NoteEvent::tick(float timeElapsed) {
    if (offset > 0) {
        offset -= timeElapsed;
        
        if (offset < 0) {
            duration += offset;
            offset = 0;
            
            active = true;
            if (note >= 0 && NoteEvent::isOscSenderInit()) {
                ofxOscMessage msg;
                msg.setAddress(NoteEvent::getOscAddr());
                msg.addInt32Arg(noteID);
                msg.addStringArg("on");
                msg.addIntArg(note);
                NoteEvent::oscSender->sendMessage(msg);
            }
        }
    } else {
        if (!active) {
            // send noteon via osc
            if (note >= 0 && NoteEvent::isOscSenderInit()) {
                ofxOscMessage msg;
                msg.setAddress(NoteEvent::getOscAddr());
                msg.addInt32Arg(noteID);
                msg.addStringArg("on");
                msg.addIntArg(note);
                NoteEvent::oscSender->sendMessage(msg);
            }
            active = true;
        }
        duration -= timeElapsed;
    }
}

void NoteEvent::toggleNoteOff() {
    if (note >= 0 && NoteEvent::isOscSenderInit()) {
        ofxOscMessage msg;
        msg.setAddress(NoteEvent::getOscAddr());
        msg.addInt32Arg(noteID);
        msg.addStringArg("off");
        NoteEvent::oscSender->sendMessage(msg);
    }
}

float NoteEvent::getOffset() {
    return offset;
}

float NoteEvent::getDuration() {
    return duration;
}

int NoteEvent::getNote() {
    return note;
}

Accidental NoteEvent::getAccidental() {
    return acc;
}

int NoteEvent::getStaffPosition() {
    return staffPos;
}

bool NoteEvent::isCompleted() {
    return duration <= 0;
}

void NoteEvent::setOscSender(ofxOscSender* s, string addr) {
    NoteEvent::oscSender = auto_ptr<ofxOscSender>(s);
    NoteEvent::addr = addr;
    int port = Helpers::scanForProcessPort("sclang", PortSearch::Blocking);
    if (port == 0) {
        port = 57120;
        cout << "Warning: no active port found for sclang, switching to listen on default: " <<
        port << endl;
    }
    NoteEvent::oscSender->setup("127.0.0.1", port);
}

string NoteEvent::getOscAddr() {
    return NoteEvent::addr;
}

bool NoteEvent::isOscSenderInit() {
    if (oscSender.get()) {
        return true;
    }
    
    return false;
}