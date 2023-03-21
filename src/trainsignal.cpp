/* /*--------------------------------------------------------------------------
 * File name:     TrainSignal.cpp
 * Description:   Train class implementation
 *
 *
 * Author:        Tanya Shkinev
 *---------------------------------------------------------------------------
 */

#include "trainsignal.h"

/**************************************************************************
 * Function name: WaitForGreenSignalAndTake
 * Description:   wait for the signal to become GREEN
 *                and turns it RED
 *                meaning that the segment can be
 *                used by the caller
 *
 * Parameters:    none
 *
 * Return:        bool - always return true
 *
 * History: 19/03/2023 - initial version
 **************************************************************************/
bool CTrainSignal::WaitForGreenSignalAndTake()
{

    // wait for the signal to be freed (green)
    // and turn it red right away
    signal_.lock();

    // lock the object before modifying attributes
    lock_guard<std::mutex> guard(cs_);

    // keep thread id
    owners_thread_id_ = this_thread::get_id();

    // we have taken the lock,
    // change the color to red
    state_ = ESignalState::Red;

    return true;
}

/**************************************************************************
 * Function name: TryToTakeGreenSignal
 * Description:   wait for signal to be GREEN
 *                and turn it to RED
 *                
 *
 * Parameters:    none
 *
 * Return:        bool - GREEN - true, RED - false 
 *
 * History:       21/03/2023 - initial version
 **************************************************************************/
bool CTrainSignal::TryToTakeGreenSignal()
{
    // wait for the signal to be freed (green)
    // and turn it red right away
    auto green = signal_.try_lock();

    if(green)
    {
        // lock the object before modifying attributes
        lock_guard<mutex> guard(cs_);

        // keep thread id
        owners_thread_id_ = this_thread::get_id();

        // we have taken the lock,
        // change the color to red
        state_ = ESignalState::Red;
    }
    
    return green;    
}
/**************************************************************************
 * Function name: ReleaseSignal
 * Description:   Release the signal (switch it to GREEN)
 * Parameters:    none
 *
 * Return:        bool - false if other thread tries to unlock mutex
 *
 * History: 19/03/2023 - initial version
 **************************************************************************/
bool CTrainSignal::ReleaseSignal()
{
    std::lock_guard<std::mutex> guard(cs_);

    if (state_ == ESignalState::Green)
        return true;

    if (this_thread::get_id() != owners_thread_id_)
        return false;

    signal_.unlock();

    // Signal is released,
    state_ = ESignalState::Green;

    return true;
}