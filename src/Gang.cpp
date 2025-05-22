#include "Gang.h"
#include <iostream>

// Constructor
Gang::Gang(int ID, int capacity, int acceptance_rate)
    : ID(ID), capacity(capacity), acceptance_rate(acceptance_rate) {
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
