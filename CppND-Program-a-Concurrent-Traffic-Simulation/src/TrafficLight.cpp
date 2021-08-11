#include <iostream>
#include <random>
#include <functional>
#include "TrafficLight.h"

/* Implementation of class "MessageQueue" */


template <typename T>
T MessageQueue<T>::receive()
{
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait() 
    // to wait for and receive new messages and pull them from the queue using move semantics. 
    // The received object should then be returned by the receive function.
    std::unique_lock<std::mutex> lock(mtx);
    _condition.wait(lock);
    auto m = std::move(_queue.front());
    _queue.pop_front();
    return m;
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex> 
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.

    // Wait for mutex
    std::lock_guard<std::mutex> lock(mtx);
    // Add to queue
    _queue.push_back(msg);
    // Notify
    _condition.notify_one();
}


/* Implementation of class "TrafficLight" */


TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop 
    // runs and repeatedly calls the receive function on the message queue. 
    // Once it receives TrafficLightPhase::green, the method returns.
    while(_messages.receive() != TrafficLightPhase::green){}
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
    // FP.2b : Finally, the private method „cycleThroughPhases“ should be started in a thread when the 
    //  public method „simulate“ is called. To do this, use the thread queue in the base class. 
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    // FP.2a : Implement the function with an infinite loop that measures the time between two loop cycles 
    // and toggles the current phase of the traffic light between red and green and sends an update method 
    // to the message queue using move semantics. The cycle duration should be a random value between 4 and 6 seconds. 
    // Also, the while-loop should use std::this_thread::sleep_for to wait 1ms between two cycles. 

    // Declare: long delay, elapsed; start
    long delay, elapsed;
    std::chrono::time_point<std::chrono::system_clock> start;
    auto rand = std::bind(
        std::uniform_real_distribution<double>{4000,6000}, 
        std::default_random_engine{std::random_device()()}
        );

    // Loop forever...
    while (true)
    {
        // Select random long from 4 to 6 assign to delay
        delay = static_cast<long>(rand());
        // Capture current time as start
        start = std::chrono::system_clock::now();
        // Loop every 1ms until time since capture
        while (true)
        {
            // sleep at every iteration to reduce CPU usage
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now() - start).count();
            if (elapsed >= delay) break;
        }
        // Toggle the current phase of the traffic light `_currentPhase`
        _currentPhase = (_currentPhase == TrafficLightPhase::red) ? TrafficLightPhase::green : TrafficLightPhase::red;
        // Send an update method to the message queue using move semantics (how to create an update method?)
        _messages.send(std::move(_currentPhase));
        // Wait 1ms for next cycle
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
