#include "Gang.h"
#include <iostream>

// Constructor
Gang::Gang(const Config &config, int ID, int capacity, int acceptance_rate)
    :config(config), ID(ID), capacity(capacity), acceptance_rate(acceptance_rate) {
    GangMembers.reserve(capacity);  // Optional: optimize memory allocation
}

// Accept a new member into the gang
void Gang::acceptMember(GangMember gangMember) {
    if (GangMembers.size() >= static_cast<size_t>(capacity)) {
        std::cout << "Gang is full. Member rejected.\n";
        return;
    }

    // Optional: simulate acceptance probability (basic version)
    GangMembers.push_back(gangMember);
    std::cout << "GangMember accepted. Total now: " << GangMembers.size() << "\n";
}

void Gang::informGangMembers(){
    InformationMessage message;
    message.MessageID = messageIdGenerator++;
    message.gangID = ID;
    message.weight = random_int(config.info.weight_min, config.info.weight_max);
    float trueProb = random_float(0, 1.0f);
    if(trueProb > config.info.p_true){
        message.weight = -message.weight;
    }
    
    //TODO: ADD SPREAD PROBABILITY TO CONFIG
    float spread = 0.5f;

    for(GangMember& member: GangMembers){
        float spreadProb = random_float(0, 1.0f);
        if (spreadProb < spread) {
            member.receiveInformation(message);
        }
    }
}

// Simulate the gang starting operations
void Gang::startOperation() {
    std::cout << "Gang " << ID << " starting operation with " << GangMembers.size() << " members.\n";
}

// Simulate gang members leaving jail
void Gang::leaveJail() {
    std::cout << "Gang " << ID << " members leaving jail.\n";
    // Placeholder logic
}

// High-level orchestration
void Gang::run() {
    std::cout << "Running Gang #" << ID << "\n";
    startOperation();
    leaveJail();
}

int Gang::getID() const {
    return ID;
}

int Gang::getCapacity() const {
    return capacity;
}

int Gang::getAcceptanceRate() const {
    return acceptance_rate;
}