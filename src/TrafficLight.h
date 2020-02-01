#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include "TrafficObject.h"
#include <condition_variable>
#include <deque>
#include <mutex>

class Vehicle;
enum TrafficLightPhase { red, green };

template <class T> class MessageQueue {
public:
    void send(TrafficLightPhase&&);
    TrafficLightPhase receive();

private:
    std::deque<TrafficLightPhase> _queue;
    std::condition_variable _cond;
    std::mutex _mutex;
};

class TrafficLight : public TrafficObject {
public:
    TrafficLight();
    ~TrafficLight();

    TrafficLightPhase getCurrentPhase();
    void waitForGreen();
    void simulate();
    void cycleThroughPhases();

private:
    std::condition_variable _condition;
    std::mutex _mutex;
    TrafficLightPhase _currentPhase;
    MessageQueue<TrafficLightPhase> _mQueue;
};

#endif
