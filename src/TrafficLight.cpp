#include "TrafficLight.h"
#include <iostream>
#include <random>

/* Implementation of class "MessageQueue" */

/*
template <typename T>
T MessageQueue<T>::receive()
{
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and
_condition.wait()
    // to wait for and receive new messages and pull them from the queue using
move semantics.
    // The received object should then be returned by the receive function.
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // FP.4a : The method send should use the mechanisms
std::lock_guard<std::mutex>
    // as well as _condition.notify_one() to add a new message to the queue and
afterwards send a notification.
}
*/

TrafficLight::TrafficLight() { _currentPhase = TrafficLightPhase::red; }

void TrafficLight::waitForGreen()
{
    // FP.5b : add the implementation of the method waitForGreen, in which an
    // infinite while-loop runs and repeatedly calls the receive function on the
    // message queue. Once it receives TrafficLightPhase::green, the method
    // returns.
}

TrafficLightPhase TrafficLight::getCurrentPhase() { return _currentPhase; }

void TrafficLight::simulate()
{
    // FP.2b : Finally, the private method „cycleThroughPhases“ should be
    // started in a thread when the public method „simulate“ is called. To do
    // this, use the thread queue in the base class.
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    auto start = std::chrono::high_resolution_clock::now();
    while (true) {
        // measures the time between two cycles
        auto end = std::chrono::high_resolution_clock::now();
        auto duration
            = std::chrono::duration_cast<std::chrono::seconds>(end - start)
                  .count();

        // generates random number between 4 and 6
        std::default_random_engine generator;
        std::uniform_int_distribution<int> distribution(4, 6);
        int desired_dur = distribution(generator);

        // toggles current phase of traffic light if the desired_dur is reached
        if (duration == desired_dur) {

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
