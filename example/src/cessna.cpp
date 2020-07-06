
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/transport/transport.hh>
#include <gazebo/msgs/msgs.hh>


namespace gazebo {
    /// \brief A plugin to control the Cessna.
    class CessnaPlugin : public ModelPlugin {
        /// \brief Constructor
    public:
        CessnaPlugin() {}

        /// \brief The load function is called by Gazebo when the plugin is
        /// inserted into simulation
        /// \param[in] _model A pointer to the model that this plugin is
        /// attached to.
        /// \param[in] _sdf A pointer to the plugin's SDF element.
        virtual void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf) {

            // Store the model pointer for convenience.
            this->model = _model;

            std::array<std::string, 6> JOINT_NAMES = {
                    "elevators_joint",
                    "rudder_joint",
                    "left_flap_joint",
                    "left_aileron_joint",
                    "right_flap_joint",
                    "right_aileron_joint",
            };

            for (auto name : JOINT_NAMES) {
                physics::JointPtr joint = _model->GetJoint("cessna_c172::" + name);
                if (joint == nullptr) {
                    gzerr << "Missing " << name << " joint\n";
                    return;
                }
                joints[name] = joint;
                this->model->GetJointController()->SetPositionPID(
                        joint->GetScopedName(), common::PID(50, 0.1, 1, 0, 0, 20000, -20000));
                this->model->GetJointController()->SetPositionTarget(
                        joint->GetScopedName(), 0);
            }

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


            // Subscribe to the topic, and register a callback
            this->sub1 = this->node->Subscribe(topicName1,
                                               &CessnaPlugin::OnMsgElevator, this);
            this->sub2 = this->node->Subscribe(topicName2,
                                               &CessnaPlugin::OnMsgRudder, this);


            //Giving initial Velocity
            if (_sdf->HasElement("initial_vel")) {
                math::Vector3 initial_vel = _sdf->GetElement("initial_vel")->Get<math::Vector3>();
                _model->GetLink("cessna_c172::body")->SetLinearVel(initial_vel);
            }

        }

    private:
        /// \brief Handle incoming message
        /// \param[in] _msg Repurpose a vector2 message. This function will
        /// only use the x component.
        void OnMsgElevator(ConstVector2dPtr &_msg) {
            this->model->GetJointController()->SetPositionTarget(joints["elevators_joint"]->GetScopedName(), _msg->x());
        }

        void OnMsgRudder(ConstVector2dPtr &_msg) {
            this->model->GetJointController()->SetPositionTarget(joints["rudder_joint"]->GetScopedName(), _msg->x());
        }

        /// \brief Pointer to the model.
        physics::ModelPtr model;

        /// \brief A node used for transport
        transport::NodePtr node;

        /// \brief A subscriber to a named topic.
        transport::SubscriberPtr sub1;
        transport::SubscriberPtr sub2;

        std::map<std::string, physics::JointPtr> joints;
    };

    // Tell Gazebo about this plugin, so that Gazebo can call Load on this plugin.
    GZ_REGISTER_MODEL_PLUGIN(CessnaPlugin)
}
