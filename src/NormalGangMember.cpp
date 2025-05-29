#include "NormalGangMember.h"

void NormalGangMember::receiveInformation(InformationMessage message) {
    infoList.push_back(message);
}
