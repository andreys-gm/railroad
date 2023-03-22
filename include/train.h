/* /*--------------------------------------------------------------------------
* Objectnane: Train.h                   
*
* Author: Tanya Shkinev                                                        
----------------------------------------------------------------------------
*/
#ifndef __TRAIN_H__
#define __TRAIN_H__

#include <list>

#include "tracksegment.h"

using namespace std;

/* enumerators
*/
enum class ETrainDirection
{
    Forward,
    Backward
};


class CTrain {
private:
    /* uinque train id 
    */
    int m_trainId; 

    /* current rail segment
    */
    int m_currentPoint; 
    /* segment train starts from
    */
    int m_departurePoint;

    /* finish segment*/
    int m_destinationPoint; 
    
    /* shortest path - list of rail segments
    /* between departure and destination
     */
    vector<int> m_trainPath;   

    /*list of segments contained in train shortest path
    */
    list<CTrackSegnment*> m_shortPathSegList; 

    /*iterator to monitor train progress through shortest path 
    */
    list<CTrackSegnment*>::iterator m_it;

    /*train tread
    */
    std::thread m_trainThread;

    static std::mutex m_coutMutex;
   
    /*  
    */
    ETrainDirection m_direction = ETrainDirection::Forward;

    
public: 
    CTrain(int i, int from, int to);
    virtual ~CTrain();
    
    /* get/set function
    */
    int GetTrainId(){ return m_trainId; }
    int GetTrainDepartPoint(){ return m_departurePoint; }
    int GetTrainDestPoint(){ return m_destinationPoint; }
    
    /* returns train path (list of segments IDs)
    */
    const vector<int> & GetTrainPath(){ return m_trainPath;};
    void SetTrainPath(vector<int>& vec){ m_trainPath = vec; }
    /* returns number of segments on the train path
    */
    int GetTrainPathLength(){ return m_trainPath.size();};

    /*attach calculated shortest path to train
    */
    void SetTrainShortPathSegList(list<CTrackSegnment*> seglist){ m_shortPathSegList = seglist;};
    
    /* train start moving
    */
    void RunTrain();

    /*create train thread
    */
    void StartEngine();
    
    void WaitForTrainArrival();

    void PrintTrainPath();

protected:
    static void s_RunTrain(CTrain* train)
    {
        // cout << __func__ << ": " << hex << (void*)train << " : " << this_thread::get_id() << dec << endl;  
        std::this_thread::sleep_for(1000ms);
        train->RunTrain();
    };

};
#endif // __TRAIN_H__ */