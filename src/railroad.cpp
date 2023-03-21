/*--------------------------------------------------------------------------
* File name:     RailRoad.cpp 
* Description:   RailRoad class implementation 
*                RailRoad is represented by biderectional graph data 
*                structure where train segments are represented as 
*                vertices and segments connections represented as edges
*
*
* Author:        Tanya Shkinev                                                     
*---------------------------------------------------------------------------
*/
#include "railroad.h"


/**************************************************************************
 * Constructor is called by main
 * 
 * Parameters: none
 * History: 19/03/2023 - initial version
**************************************************************************/
CRailRoad::CRailRoad() 
{ 

}
/**************************************************************************
 * Destructor 
 * 
 * Parameters: none
 * History: 19/03/2023 - initial version
**************************************************************************/
CRailRoad::~CRailRoad()
{
    // delete all objects
     for (int trainIdx = 0; trainIdx < m_trainsList.size(); trainIdx++)
        delete m_trainsList[trainIdx];
     for (int trackSegmentIdx = 0; trackSegmentIdx < m_trackSegmentsMap.size(); trackSegmentIdx++)
        delete m_trackSegmentsMap[trackSegmentIdx];
     m_trackSegmentsMap.clear();
     m_trainsList.clear();
}


/**************************************************************************
 * Function name: AddTrackSegment 
 * Description:   create new track segment object and push it to the map of 
 *                segments (map key is segment id)
 * Parameters:    int id - unique track segment id 
 *                int len - track segment length (weight)
 * 
 * Return:        void
 * History: 19/03/2023 - initial version
**************************************************************************/
void CRailRoad::CRailRoad::AddTrackSegment(int id, int len)
{
    m_trackSegmentsMap[id] = new CTrackSegnment(id, len);
}
/**************************************************************************
 * Function name: AddTrain 
 * Description: creates new train and push it to the list
 * Parameters: int id - unique train id
 *             int from - tark id train starts from
 *             int to   - track id trains stops at
 * 
 * Return:     void
 * History: 19/03/2023 - initial version
**************************************************************************/
void CRailRoad::AddTrain(int id, int from, int to)
{
    m_trainsList[id] = new CTrain(id, from, to);
}

/**************************************************************************
 * Function name: AddConnection 
 * Description: creates new connection and push it to the list
 * Parameters: int id - unique train id
 *             int src - connection  to previous segment
 *             int dest  - connection to next segment
 * 
 * Return:     void
 * History: 19/03/2023 - initial version
**************************************************************************/
void CRailRoad::AddConnection(int src, int dest) 
{
        if (m_trackSegmentsMap.find(src) != m_trackSegmentsMap.end() && 
        m_trackSegmentsMap.find(dest) != m_trackSegmentsMap.end()) {
             m_trackSegmentsMap[src]->GetTrackSegmentConnections().push_back(m_trackSegmentsMap[dest]);
            m_trackSegmentsMap[dest]->GetTrackSegmentConnections().push_back(m_trackSegmentsMap[src]);

        } 
 }


