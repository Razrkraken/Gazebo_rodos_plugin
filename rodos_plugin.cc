#ifndef _VELODYNE_PLUGIN_HH_
#define _VELODYNE_PLUGIN_HH_

#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/transport/transport.hh>
#include <gazebo/msgs/msgs.hh>


namespace gazebo {
/// \brief A plugin to control a Velodyne sensor.
    class RodosPlugin : public ModelPlugin {
        /// \brief Constructor
    public:
        RodosPlugin() {}


/// \brief Handle incoming message
/// \param[in] _msg Repurpose a vector3 message. This function will
/// only use the x component.
    private:
        void OnMsgElevator(ConstVector3dPtr &_msg) {
            this->model->GetJointController()->SetVelocityTarget(this->joint1->GetScopedName(), _msg->x());
        }

        void OnMsgRudder(ConstVector3dPtr &_msg) {
            this->model->GetJointController()->SetVelocityTarget(this->joint2->GetScopedName(), _msg->x());
        }


        /// \brief The load function is called by Gazebo when the plugin is
        /// inserted into simulation
        /// \param[in] _model A pointer to the model that this plugin is
        /// attached to.
        /// \param[in] _sdf A pointer to the plugin's SDF element.
    public:
        virtual void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf) {

            // Safety check
            if (_model->GetJointCount() == 0) {
                std::cerr << "Invalid joint count, Velodyne plugin not loaded\n";
                return;
            }

            // Store the model pointer for convenience.
            this->model = _model;

            /*
             * Joint_Fuselage+Tailplane
             * Joint_Fuselage+Wing
             * Joint_Tailplane+Elevator
             * Joint_Fuselage+Rudder
             * Joint_Wing+LeftAileron
             * Joint_Wing+RightAileron
             * */


            this->joint1 = _model->GetJoints()[2]; // Joint_Tailplane+Elevator
            this->joint2 = _model->GetJoints()[3]; // Joint_Fuselage+Rudder

            //std::cout << joint->GetName() << std::endl;


            // Setup a P-controller, with a gain of 0.1.
            this->pid1 = common::PID(0.1, 0, 0);
            this->pid2 = common::PID(0.1, 0, 0);


            // Apply the P-controller to the joint.
            this->model->GetJointController()->SetPositionPID(
                    this->joint1->GetScopedName(), this->pid1);

            this->model->GetJointController()->SetPositionPID(
                    this->joint2->GetScopedName(), this->pid2);

            // Set the joint's target velocity. This target velocity is just
            // for demonstration purposes.
            //this->model->GetJointController()->SetVelocityTarget(
            //    this->joint->GetScopedName(), 10.0);

// Default to zero velocity
//            double velocity = 0;

// Check that the velocity element exists, then read the value
//            if (_sdf->HasElement("velocity"))
//                velocity = _sdf->Get<double>("velocity");

// Set the joint's target velocity. This target velocity is just
// for demonstration purposes.
            this->model->GetJointController()->SetPositionTarget(
                    this->joint1->GetScopedName(), 0.5);

            this->model->GetJointController()->SetPositionTarget(
                    this->joint2->GetScopedName(), 0.5);


// Create the node
            this->node = transport::NodePtr(new transport::Node());
#if GAZEBO_MAJOR_VERSION < 8
            this->node->Init(this->model->GetWorld()->GetName());
#else
            this->node->Init(this->model->GetWorld()->Name());
#endif

// Create a topic name
            std::string topicName1 = "~/" + this->model->GetName() + "/elevator_angle";
            std::string topicName2 = "~/" + this->model->GetName() + "/rudder_angle";

            std::cout << topicName1 << " : " << topicName2 << std::endl; // ~/Gleider/elevator_angle : ~/Gleider/rudder_angle

// Subscribe to the topic, and register a callback
            this->sub1 = this->node->Subscribe(topicName1,
                                               &RodosPlugin::OnMsgElevator, this);
            this->sub2 = this->node->Subscribe(topicName2,
                                               &RodosPlugin::OnMsgRudder, this);


        }

/// \brief Pointer to the model.
    private:
        physics::ModelPtr model;

/// \brief Pointer to the joint.
    private:
        physics::JointPtr joint1;
        physics::JointPtr joint2;


/// \brief A PID controller for the joint.
    private:
        common::PID pid1;
        common::PID pid2;

/// \brief A node used for transport
    private:
        transport::NodePtr node;

/// \brief A subscriber to a named topic.
    private:
        transport::SubscriberPtr sub1;
        transport::SubscriberPtr sub2;

    };

// Tell Gazebo about this plugin, so that Gazebo can call Load on this plugin.
    GZ_REGISTER_MODEL_PLUGIN(RodosPlugin)
}
#endif
