#include "InfoUnit.h"

// Constructor
InfoUnit::InfoUnit(int ID, int importance)
    : ID(ID), importance(importance) {}

// Getter for ID
int InfoUnit::getID() const {
    return ID;
}

// Getter for importance
int InfoUnit::getImportance() const {
    return importance;
}
