//
// Created by rxwyun on 2022/7/23.
//

#ifndef ARM_FACE_IDENTIFY_SUBSCRIBERABSTRACT_H
#define ARM_FACE_IDENTIFY_SUBSCRIBERABSTRACT_H

#include <sstream>

using namespace std;

class SubscriberAbstract {
public:
    SubscriberAbstract();
    ~SubscriberAbstract();

    virtual string getTopic();
    virtual void onSubscribe();
};


#endif //ARM_FACE_IDENTIFY_SUBSCRIBERABSTRACT_H
