/*--------------------------------------------------------------------------
* Objectnane: TrackSegment.h   
*                
* Author: Tanya Shkinev                                                        
----------------------------------------------------------------------------
*/
#ifndef __TRACKSEGMENT_H__
#define __TRACKSEGMENT_H__
#include <iostream>
#include <vector>
#include <map>
#include <mutex>

#include "trainsignal.h"

// forward declaration
enum class ETrainDirection;

using namespace std;

enum class EMoveStatus
{
    Waiting,
    Moved,
    Finished
};
/**
 * CTrackSegnment class reperesents rail track segment 
 * 
 * History: 2023/19/03 - Initial version
*/
class CTrackSegnment {
private:
    /*segment id 
    */
    int m_segmentId;     

    /* length of segment (use as weight in shortest path calculation)
    */
    int m_segmentLength; 
   
    /*each segment contains traffic signal 
    */
    vector<CTrainSignal> m_signals;

    /*list of connections 
    */
    vector<CTrackSegnment*> m_tsConnections; // all connections to other segments from
                                              // the segment

    /* map of trains in progress
    */
    struct CTrainRecord
    {
        ETrainDirection m_direction;
        int m_trainStep;
        int m_stop; 
        int m_current;
    };

    map<int, CTrainRecord> m_trainRecords;

    /* current direction. On a given segment
       all trains have to go in the same direction 
       until the segment is cleared
    */
   ETrainDirection m_currentDirection;

   // section mutex
   mutex m_cs;
    

public:
    /*constructor
    */
    CTrackSegnment(int i, int len);

    /*destructor
    */
    virtual ~CTrackSegnment ();

    /*get/set attributes
    */
    int GetTrackSegmentLength(){ return m_segmentLength; }
    int GetTrackSegmentId(){ return m_segmentId; }
    const vector<CTrackSegnment*> & GetTrackSegmentConnections(){return m_tsConnections;};
    void AddConnection(CTrackSegnment * connect ){ m_tsConnections.push_back(connect);};

    /* Function that moves the train (or does not)
    */
    EMoveStatus MoveTrain(ETrainDirection direction, int trainId, int& atStep);

    /* Function that releases the segment when a train left it
    */
    bool ReleaseSection(int trainId);

protected:
    //steps in segment are introduced for segments that are longer then shortest segment
    /*train should wait for green signal before enetering the segment
    */
    void WaitForGreenSignal(int stepIndex);
    /*try to take green signal before enetering the segment
    */
    bool TryToTakeGreenSignal(int stepIndex);

    /*train releases the signal 
    */
    void ReleaseSignal(int stepIndex);    
};
#endif // __TRACKSEGMENT_H__