#include "NormalGangMember.h"
#include <iostream>
void NormalGangMember::receiveInformation(InformationMessage message) {
    std::cout << "NormalGangMember received message" << std::endl;
    infoList.push_back(message);
}
