#include <GazeboTopic.h>
#include <gazebo/msgs/imu.pb.h>
#include <rodos.h>

static Application nameNotImportantHW("HelloWorld");

GazeboTopic<gazebo::msgs::IMU> imuTopic(10, "~/Schleicher_ASK_21/Fuselage/IMU_Fuselage/imu");


class SimpleSub : public Subscriber {
public:
    SimpleSub() : Subscriber(imuTopic, "simplesub") {}

    uint32_t put(const uint32_t topicId, const size_t len, void *data, const NetMsgInfo &netMsgInfo) override {
        gazebo::msgs::IMU *gazeboData = (gazebo::msgs::IMU *) data;
        PRINTF("%s \n", gazeboData->entity_name().c_str());
        return 1;
    }

} simpleSub;



