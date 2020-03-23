//
// Created by alejandro on 28.01.20.
//

#ifndef RODOS_PLUGIN_VEL_H
#define RODOS_PLUGIN_VEL_H

#endif //RODOS_PLUGIN_VEL_H

#include <gazebo/gazebo_config.h>
#include <gazebo/transport/transport.hh>
#include <gazebo/msgs/msgs.hh>
//#include "rodos.h"

// Gazebo's API has changed between major releases. These changes are
// accounted for with #if..#endif blocks in this file.
#if GAZEBO_MAJOR_VERSION < 6
#include <gazebo/gazebo.hh>
#else

#include <gazebo/gazebo_client.hh>

#endif


/////////////////////////////////////////////////
int main(int _argc, char **_argv) {
    // Load gazebo as a client
#if GAZEBO_MAJOR_VERSION < 6
    gazebo::setupClient(_argc, _argv);
#else
    gazebo::client::setup(_argc, _argv);
#endif

    // Create our node for communication
    gazebo::transport::NodePtr node(new gazebo::transport::Node());
    node->Init();

    // Publish to the rodos topic
    gazebo::transport::PublisherPtr pub =
            node->Advertise<gazebo::msgs::Vector2d>("~/Schleicher_ASK_21/elevator_soll");


    // Wait for a subscriber to connect to this publisher
    pub->WaitForConnection();

    // Create a a vector3 message
    gazebo::msgs::Vector2d msg;

    while (std::cin.good()) {

        float pos;
        std::cout << "Value: ";
        std::cin >> pos;

        // Set the velocity in the x-component
#if GAZEBO_MAJOR_VERSION < 6
        gazebo::msgs::Set(&msg, gazebo::math::Vector2d(pos, 0));
#else
        gazebo::msgs::Set(&msg, ignition::math::Vector2d(pos, 0));
#endif
        // Send the message
        pub->Publish(msg);

    }

    // Make sure to shut everything down.
#if GAZEBO_MAJOR_VERSION < 6
    gazebo::shutdown();
#else
    gazebo::client::shutdown();
#endif


}



