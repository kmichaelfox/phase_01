//
//  Player.cpp
//  phase_01
//
//  Created by Kelly Fox on 3/20/16.
//
//

#include "Player.hpp"

using namespace std;

Player::Player() {
    fbo.allocate(1920, 1080);
    
    drawWidth = 1920;
    drawHeight = 1080;
    
    staff = new Staff(GENERIC);
}

Player::Player(Instrument instr, string name) {
    fbo.allocate(1920, 1080);
    
    drawWidth = 1920;
    drawHeight = 1080;
    
    staff = new Staff(instr);
    staff->setName(name);
}

Player::Player(ClefType clef, string name) {
    fbo.allocate(1920, 1080);
    
    drawWidth = 1920;
    drawHeight = 1080;
    
    staff = new Staff(clef);
    staff->setName(name);
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

string Player::getName() {
    return staff->getName();
}

void Player::setName(string n) {
    staff->setName(n);
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

void Player::setTempo(float t) {
    tempo = t;
    staff->setTempo(t);
}

Range Player::getRange() {
    return staff->getRange();
}

bool Player::isFirstChair() {
    string n = staff->getName();
    if (n.empty() && n.size() > 2) {
        return false;
    }
    
    n = n.substr(n.size() - 2, 2);
    std::transform(n.begin(), n.end(), n.begin(), ::tolower);
    
    if (n.compare(" i") == 0 || n.compare(".i") == 0) {
        return true;
    }
    
    return false;
}

void Player::idle(bool immediately) {
    staff->clearSequenceQueue(immediately);
}

bool Player::isActive() {
    return staff->isActive();
}

void Player::activate(bool state) {
    staff->activate(state);
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