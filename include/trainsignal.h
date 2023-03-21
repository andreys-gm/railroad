/* /*--------------------------------------------------------------------------
* Objectnane: TrainSignal.h                   
*
* Author: Tanya Shkinev                                                        
----------------------------------------------------------------------------
*/
#ifndef __TRAINSIGNAL_H__
#define __TRAINSIGNAL_H__

#include <mutex>
#include <thread>
#include <atomic>

using namespace std;

/* enumerators
*/
enum class ESignalState
{
    Red,
    Green
};


class CTrainSignal
{
public:
    /* constructor
    */
    CTrainSignal(){state_ = ESignalState::Red;};
    /* destructor
    */
    virtual ~CTrainSignal(){}

    // read current state
    ESignalState GetState() const {return state_;}
    
    /* wait for the signal to become GREEN
       and turns it RED
       meaning that the segment can be 
       used by the caller
    */
    bool WaitForGreenSignalAndTake();

    /* check if the signal is GREEN
       if yes turns it RED
       meaning that the segment can be 
       used by the caller,
       exit otherwise
    */
    bool TryToTakeGreenSignal();

    /* release the signal (switch it to GREEN)
    */
    bool ReleaseSignal();

private:
    // attributes
    /* signal state 
    */
    ESignalState state_ = ESignalState::Red;

    // lock
    std::mutex signal_;
    std::mutex cs_;
    std::thread::id owners_thread_id_;

};

#endif //__TRAINSIGNAL_H__ */