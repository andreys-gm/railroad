/*--------------------------------------------------------------------------
* Objectnane: RailRoad.h 
*
* Author: Tanya Shkinev                                                     
*---------------------------------------------------------------------------
*/

#ifndef __RAILROAD_H__
#define __RAILROAD_H__
#include <iostream>
#include <unordered_map>
#include <queue>

#include "tinyxml2.h"

#include "tracksegment.h"
#include "train.h"

using namespace tinyxml2;
using namespace std;



/**
 * CRailRoad class reperesents complete railroad system 
 * 
 * History: 2023/19/03 - Initial version
*/

class CRailRoad {
public:
    // Constructor
    CRailRoad();
    //Destructor
    virtual ~CRailRoad();

    /* create new track segment and push it to the map
    */
    void AddTrackSegment(int id, int len);
    
    /* creates new train and push it to the list
    */
    void AddTrain(int id, int from, int to);
    
    /* add connection to the connection list
    */
    void AddConnection(int src, int dest);       
     
    /* get/set private attributes
    */
    unordered_map<int, CTrackSegnment*> & GetTrackSegMap(){return m_trackSegmentsMap;};
    unordered_map<int, CTrain*>& GetTrainList(){return m_trainsList;};
    
    /* returns number of trains
    */
    uint GetTrainsNumber(){ return m_trainsList.size();};
    /* returns number of segments 
    */
    uint GetSegmentsNumber(){ return m_trackSegmentsMap.size();};

private:
    /* track segment map  - conains all segements inlcuded into railroad model
    */
    unordered_map<int, CTrackSegnment*> m_trackSegmentsMap;

    /* list of all trains contained in rail road model
    */
    unordered_map<int, CTrain*> m_trainsList;

    //start train 
    void MoveTrain(int trainid);




};
#endif// __RAILROAD_H__
