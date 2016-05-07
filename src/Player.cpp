//
//  Player.cpp
//  phase_01
//
//  Created by Kelly Fox on 3/20/16.
//
//

#include "Player.hpp"

Player::Player() {
    fbo.allocate(1920, 1080);
    
    drawWidth = 1920;
    drawHeight = 1080;
    
    staff = new Staff(GENERIC);
}

Player::Player(Instrument instr) {
    fbo.allocate(1920, 1080);
    
    drawWidth = 1920;
    drawHeight = 1080;
    
    staff = new Staff(instr);
}

Player::Player(ClefType clef) {
    fbo.allocate(1920, 1080);
    
    drawWidth = 1920;
    drawHeight = 1080;
    
    staff = new Staff(clef);
}

Player::~Player() {
    if (staff != nullptr) {
        delete staff;
    }
}

void Player::update() {
    
}

void Player::draw() {
    draw(0,0);
}

void Player::draw(int x, int y) {
    fbo.begin();
    staff->draw();
    fbo.end();
    
    fbo.draw(x, y, drawWidth, drawHeight);
}

int Player::getWidth() {
    return drawWidth;
}

void Player::setWidth(int width) {
    drawWidth = width;
    drawHeight = drawWidth * (fbo.getHeight() / fbo.getWidth());
}

int Player::getHeight() {
    return drawHeight;
}

void Player::setHeight(int height) {
    drawHeight = height;
    drawWidth = drawHeight * (fbo.getWidth() / fbo.getHeight());
}

float Player::getTempo() {
    return tempo;
}

void Player::setTempo(float tempo) {
    this->tempo = tempo;
    staff->setTempo(tempo);
}

void Player::idle(bool immediately) {
    staff->clearSequenceQueue(immediately);
}

void Player::queueSequence(Sequence seq, float tempo) {
    staff->queueSequence(seq, tempo);
}

void Player::createSingleNote(int note) {
    staff->createNote(note);
}

float Player::getAspectRatio() {
    return fbo.getWidth() / fbo.getHeight();
}