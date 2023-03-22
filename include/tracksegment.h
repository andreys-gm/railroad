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

#include "trainsignal.h"

using namespace std;

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
    CTrainSignal m_signal;

    /*list of connections 
    */
    vector<CTrackSegnment*> m_tsConnections; // all connections to other segments from
                                              // the segment

    
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


    /*train should wait for green signal before enetering the segment
    */
    void WaitForGreenSignal();
    /*try to take green signal before enetering the segment
    */
    bool TryToTakeGreenSignal();

    /*train releases the signal 
    */
    void ReleaseSignal();

    
};
#endif // __TRACKSEGMENT_H__