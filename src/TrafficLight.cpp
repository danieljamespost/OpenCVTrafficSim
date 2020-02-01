#include "TrafficLight.h"
#include <iostream>
#include <random>

template <typename T> T MessageQueue<T>::receive()
{
    std::unique_lock<std::mutex> uLock(_mutex);
    _cond.wait(uLock, [this] { return !_queue.empty(); });
    T msg = std::move(_queue.back());
    _queue.pop_back();

    return msg;
}

template <typename T> void MessageQueue<T>::send(T&& msg)
{
    std::lock_guard<std::mutex> lck(_mutex);
    _queue.push_back(std::move(msg));
    _cond.notify_one();
}

TrafficLight::TrafficLight() { _currentPhase = TrafficLightPhase::red; }

void TrafficLight::waitForGreen()
{
    while (true) {
        TrafficLightPhase tp = _mQueue.receive();
        if (tp == green) {
            return;
        }
    }
}

TrafficLightPhase TrafficLight::getCurrentPhase() { return _currentPhase; }

void TrafficLight::simulate()
{
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    // generates random number between 4 and 6
    auto start = std::chrono::high_resolution_clock::now();
    std::random_device device;
    std::mt19937 engine(device());
    std::uniform_int_distribution<> distribution(4, 6);
    int desired_dur = distribution(engine);

    while (true) {
        // measures the time between two cycles
        auto end = std::chrono::high_resolution_clock::now();
        auto duration
            = std::chrono::duration_cast<std::chrono::seconds>(end - start)
                  .count();
        // toggles current phase of traffic light if the desired_dur is reached
        if (duration >= desired_dur) {
            // toggle current phase of traffic light
            switch (_currentPhase) {
            case red:
                _currentPhase = green;
                break;
            case green:
                _currentPhase = red;
                break;
            default:
                _currentPhase = red;
                break;
            }
            // send update method to message queue using move semantics
            _mQueue.send(std::move(_currentPhase));
            // reset clock
            start = std::chrono::high_resolution_clock::now();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
