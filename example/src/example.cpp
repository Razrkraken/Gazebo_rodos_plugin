#include <GazeboTopic.h>
#include <gazebo/msgs/imu.pb.h>
#include <rodos.h>

static Application exampleApplication("ExampleApplication");

GazeboTopic<gazebo::msgs::IMU> imuTopic(10, "~/Schleicher_ASK_21/Fuselage/IMU_Fuselage/imu");
GazeboTopic<gazebo::msgs::Vector2d> rudderCmdTopic(11, "~/Schleicher_ASK_21/rudder_angle");


class IMUSub : public Subscriber {
public:
    IMUSub() : Subscriber(imuTopic, "ImuSubscriber") {}

    uint32_t put(const uint32_t topicId, const size_t len, void *data, const NetMsgInfo &netMsgInfo) override {
        gazebo::msgs::IMU *gazeboData = (gazebo::msgs::IMU *) data;
        gazebo::msgs::Vector3d acceleration = gazeboData->linear_acceleration();
        PRINTF("X:%f Y:%f Z:%f\n", acceleration.x(), acceleration.y(), acceleration.z());
        return 1;
    }
} imuSubscriber;

class RudderControlThread : public StaticThread<> {
public:
    RudderControlThread() : StaticThread("RudderControlThread") {}

    [[noreturn]] void run() override {
        gazebo::msgs::Vector2d cmd;
        while (true) {
            if (cmd.x() >= 0.8) {
                cmd.set_x(-0.8);
            } else {
                cmd.set_x(0.8);
            }
            cmd.set_y(0);
            rudderCmdTopic.publish(cmd);
            suspendCallerUntil(NOW() + SECONDS);
        }
    }
} rudderControlThread;
