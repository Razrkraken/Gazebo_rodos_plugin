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
                T data = topic->toRodosQueue.front();
                currentMsg = &data;
                topic->publish(data);
                topic->toRodosQueue.pop();
                currentMsg = nullptr;
            }
        }
    }

    void stop() {
        running = false;
        topic->queueWait.notify_all();
    }

    bool isMsgFromGazebo(const T *msg) const {
        return msg == currentMsg;
    }

private:
    T *currentMsg = nullptr;
    GazeboTopic<T> *topic;
    bool running = true;
};


template<class T>
class GazeboTopicPutter : public Putter {
public:
    GazeboTopicPutter(GazeboTopic<T> *gazeboTopic) : topic(gazeboTopic) {}

    bool putGeneric(uint32_t topicId, size_t len, const void *msg, const NetMsgInfo &netMsgInfo) override {
        auto data = (const T *) msg;
        if (publisher == nullptr || topic->publisherThread.isMsgFromGazebo(data)) { return false; }
        std::unique_lock<std::mutex> lock(topic->eventsSentToGazeboLock);
        topic->eventsSentToGazebo.push_back(*data);
        publisher->Publish(*data, true);
        return true;
    }

    void load(gazebo::transport::PublisherPtr gazeboPublisher) {
        publisher = gazeboPublisher;
    }

private:
    GazeboTopic<T> *topic;
    gazebo::transport::PublisherPtr publisher = nullptr;
};

template<class T>
class GazeboTopic : public Topic<T>, GzTopicInitializer {
    friend class GazeboTopicThread<T>;

    friend class GazeboTopicPutter<T>;

public:

    GazeboTopic(int64_t id, const char *name, bool _onlyLocal = false) :
            Topic<T>(id, name, _onlyLocal), publisherThread(this), gazeboPutter(this),
            rodosSub(*this, gazeboPutter, (this->getName() + std::string("_subscriber")).c_str()) {
    }

    void load(gazebo::transport::NodePtr node) override {
        gzmsg << "Loading topic: " << this->getName() << std::endl;
        this->gazeboSub = node->Subscribe(this->getName(), &GazeboTopic<T>::onGazeboMsg, this);
        this->gazeboPutter.load(node->Advertise<T>(this->getName(), 50, 50));
    }

    ~GazeboTopic() override {
        publisherThread.stop();
    }

private:
    void onGazeboMsg(const boost::shared_ptr<T const> &msg) {
        std::unique_lock<std::mutex> lock(eventsSentToGazeboLock);
        const auto &dataPosition = std::find_if(eventsSentToGazebo.begin(), eventsSentToGazebo.end(), [&](T item) {
            return std::memcmp(msg.get(), &item, sizeof(T)) == 0;
        });
        if (dataPosition != eventsSentToGazebo.end()) {
            eventsSentToGazebo.erase(dataPosition);
            return;
        }
        std::unique_lock<std::mutex> uniqueLock(queueLock);
        toRodosQueue.emplace(*msg);
        queueWait.notify_one();
    }

    GazeboTopicPutter<T> gazeboPutter;
    Subscriber rodosSub;
    gazebo::transport::SubscriberPtr gazeboSub;
    GazeboTopicThread<T> publisherThread;
    std::queue<T> toRodosQueue;
    std::list<T> eventsSentToGazebo;
    std::mutex queueLock;
    std::mutex eventsSentToGazeboLock;
    std::condition_variable queueWait;
};
