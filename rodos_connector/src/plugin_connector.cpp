//
// Created by alejandro on 15.06.20.
//

#include <rodos.h>

Topic<long> counter1(10, "counter1");

class SimpleSub : public Subscriber {
public:
    SimpleSub() : Subscriber(counter1, "simplesub") {}

    uint32_t put(const uint32_t topicId, const size_t len, void *data, const NetMsgInfo &netMsgInfo) {
        return 1;
    }
} simpleSub;
