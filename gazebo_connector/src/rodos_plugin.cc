#ifndef _VELODYNE_PLUGIN_HH_
#define _VELODYNE_PLUGIN_HH_

#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/transport/transport.hh>
#include <gazebo/msgs/msgs.hh>


namespace gazebo {
/// \brief A plugin to control joint of Glider.
    class RodosPlugin : public ModelPlugin {
        /// \brief Constructor
    public:
        RodosPlugin() {}


/// \brief Handle incoming message
/// \param[in] _msg Repurpose a vector2 message. This function will
/// only use the x component.
    private:
        void OnMsgElevator(ConstVector2dPtr &_msg) {
            this->model->GetJointController()->SetPositionTarget(joints[2]->GetScopedName(), _msg->x());
        }
        /*void OnMsgRudder(ConstVector2dPtr &_msg) {
            double s =
                    (_msg->x() > joints[3]->GetUpperLimit(0)) ? joints[3]->GetUpperLimit(0) : (_msg->x() <
                                                                                       joints[3]->GetLowerLimit(0))
                                                                                      ? joints[3]->GetLowerLimit(0)
                                                                                      : _msg->x();
            this->model->GetJointController()->SetPositionTarget(joints[3]->GetScopedName(), s);
        }

        void OnMsgRightAileron(ConstVector2dPtr &_msg) {
            double s =
                    (_msg->x() > joints[5]->GetUpperLimit(0)) ? joints[5]->GetUpperLimit(0) : (_msg->x() <
                                                                                       joints[5]->GetLowerLimit(0))
                                                                                      ? joints[5]->GetLowerLimit(0)
                                                                                      : _msg->x();
            this->model->GetJointController()->SetPositionTarget(joints[5]->GetScopedName(), s);
        }

        void OnMsgLeftAileron(ConstVector2dPtr &_msg) {
            double s =
                    (_msg->x() > joints[4]->GetUpperLimit(0) ? joints[4]->GetUpperLimit(0) : (_msg->x() <
                                                                                       joints[4]->GetLowerLimit(0))
                                                                                      ? joints[4]->GetLowerLimit(0)
                                                                                      : _msg->x();
            this->model->GetJointController()->SetPositionTarget(joints[4]->GetScopedName(), s);
        }*/
        /// \brief The load function is called by Gazebo when the plugin is
        /// inserted into simulation
        /// \param[in] _model A pointer to the model that this plugin is
        /// attached to.
        /// \param[in] _sdf A pointer to the plugin's SDF element.
    public:
        virtual void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf) {

            // Store the model pointer for convenience.
            this->model = _model;

            /*
             * [0] Joint_Fuselage+Tailplane (Fixed)
             * [1] Joint_Fuselage+Wing      (Fixed)
             * [2] Joint_Tailplane+Elevator (Revolute)
             * [3] Joint_Fuselage+Rudder    (Revolute)
             * [4] Joint_Wing+LeftAileron   (Revolute)
             * [5] Joint_Wing+RightAileron  (Revolute)
             * */

            /***Configuring Joint***/

            //get pointer to Elevator
            this->pid = common::PID(0.01, 0, 0);

            this->joints = model->GetJoints();

            /**Set PID Values**/
            for (int j = 0; j < model->GetJointCount(); ++j) {
                physics::JointPtr joint = joints[j];
                // Safety check
                if (joint == nullptr) {
                    std::cerr << "Invalid joint count, rodos plugin not loaded\n";
                    return;
                }
                // Apply the P-controller to the joint
                if (joint->GetType() == 576) {
                    this->model->GetJointController()->SetPositionPID(
                            joint->GetScopedName(), pid);
                    // Set the joint's target initial Position.
                    this->model->GetJointController()->SetPositionTarget(
                            joint->GetScopedName(), 0);
                }
            }

            /***Create Subscribers***/
            // Create the node
            this->node = transport::NodePtr(new transport::Node());

#if GAZEBO_MAJOR_VERSION < 8
            this->node->Init(this->model->GetWorld()->GetName());
#else
            this->node->Init(this->model->GetWorld()->Name());
#endif
            // Create a topic name
            std::string topicName1 = "~/" + this->model->GetName() + "/elevator_soll";
            std::string topicName2 = "~/" + this->model->GetName() + "/rudder_soll";
            std::string topicName3 = "~/" + this->model->GetName() + "/leftA_soll";
            std::string topicName4 = "~/" + this->model->GetName() + "/rightA_soll";

            std::string pubName1 = "~/" + this->model->GetName() + "/elevator_ist";
            std::string pubName2 = "~/" + this->model->GetName() + "/rudder_ist";
            std::string pubName3 = "~/" + this->model->GetName() + "/leftA_ist";
            std::string pubName4 = "~/" + this->model->GetName() + "/rightA_ist";

            // Subscribe to the topic, and register a callback
            this->sub1 = this->node->Subscribe(topicName1,
                                               &RodosPlugin::OnMsgElevator, this);


//            this->pub1 = this->node->Advertise<msgs::Vector3d>(pubName1,50,50);
//            this->pub2 = this->node->Advertise<msgs::Vector3d>(pubName2,50,50);
//            this->pub3 = this->node->Advertise<msgs::Vector3d>(pubName3,50,50);
//            this->pub4 = this->node->Advertise<msgs::Vector3d>(pubName4,50,50);
//
//            gazebo::msgs::Vector3d msg;
//
//            this->pub1->Publish()
        }

/// \brief Pointer to the model.
    private:
        physics::ModelPtr model;

/// \brief A PID controller for the joint.
    private:
        common::PID pid;

/// \brief A node used for transport
    private:
        transport::NodePtr node;

/// \brief A subscriber to a named topic.
    private:
        transport::SubscriberPtr sub1;
    private:
        physics::Joint_V joints;
    };

// Tell Gazebo about this plugin, so that Gazebo can call Load on this plugin.
    GZ_REGISTER_MODEL_PLUGIN(RodosPlugin)
}
#endif
