#pragma once

#ifdef TIME_SLICE_FOR_SAME_PRIORITY
#  error "RODOS must be included after gazebo"
#endif

#include <gazebo/gazebo.hh>
#include <rodos.h>
#include <queue>
#include <condition_variable>
#include "rodos_plugin.h"

template<class T>
class GazeboTopic;

template<class T>
class GazeboTopicThread : public StaticThread<> {

public:
    GazeboTopicThread(GazeboTopic<T> *topic) : StaticThread(
            (topic->getName() + std::string("_publisher")).c_str(),
            DEFAULT_THREAD_PRIORITY), topic(topic) {}

    void run() override {
        std::unique_lock<std::mutex> lock(topic->queueLock);
        while (running) {
            topic->queueWait.wait(lock);
            while (!topic->toRodosQueue.empty()) {
                topic->publish(topic->toRodosQueue.front());
                topic->toRodosQueue.pop();
            }
        }
    }

    void stop() {
        running = false;
        topic->queueWait.notify_all();
    }

private:
    GazeboTopic<T> *topic;
    bool running = true;
};

template<class T>
class GazeboTopic : public Topic<T>, GzTopicInitializer {
    friend class GazeboTopicThread<T>;

public:

    GazeboTopic(int64_t id, const char *name, bool _onlyLocal = false) :
            Topic<T>(id, name, _onlyLocal), publisherThread(this) {
    }

    void load(gazebo::transport::NodePtr node) override {
        gzmsg << "Loading topic: " << this->getName() << std::endl;
        this->gazeboSub = node->Subscribe(this->getName(), &GazeboTopic<T>::onGazeboMsg, this);
    }

    ~GazeboTopic() override {
        publisherThread.stop();
    }

private:


    void onGazeboMsg(const boost::shared_ptr<T const> &msg) {
        std::unique_lock<std::mutex> uniqueLock(queueLock);
        toRodosQueue.emplace(*msg);
        queueWait.notify_one();
    }


    gazebo::transport::SubscriberPtr gazeboSub;
    GazeboTopicThread<T> publisherThread;
    std::queue<T> toRodosQueue;
    std::mutex queueLock;
    std::condition_variable queueWait;

};