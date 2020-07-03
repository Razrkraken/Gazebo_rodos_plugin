#pragma once
#include <gazebo/gazebo.hh>

class GzTopicInitializer {
public:
    GzTopicInitializer();

    virtual void load(const gazebo::transport::NodePtr node) = 0;

    static const std::list<GzTopicInitializer *> &getGzTopicList();
private:
    static std::list<GzTopicInitializer *> gzTopicList;
};
typedef int (*rodosMainType)(int, char **);

namespace gazebo {

    /// \brief A plugin to control joint of Glider.
    class RodosPlugin : public WorldPlugin {
        /// \brief Constructor
    public:
        RodosPlugin();
        /// \brief The load function is called by Gazebo when the plugin is
        /// inserted into simulation
        /// \param[in] _model A pointer to the model that this plugin is
        /// attached to.
        /// \param[in] _sdf A pointer to the plugin's SDF element.
        void Load(physics::WorldPtr world, sdf::ElementPtr sdf) override;

    private:
        static void signalHandler(int signal);

        static void rodosSystemMain();

        void initConnectors();

        /// \brief A node used for transport
        transport::NodePtr node;

        static pthread_t rodosThreadId;

    };
}
