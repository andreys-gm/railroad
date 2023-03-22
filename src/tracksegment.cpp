/* /*--------------------------------------------------------------------------
* File name:     TrackSegment.cpp 
* Description:   TrackSegment class implementation 
*                
*
* Author:        Tanya Shkinev                                                     
*---------------------------------------------------------------------------
*/

#include "tracksegment.h"
#include "train.h"

/**************************************************************************
 * Constructor is called RailRoad object
 * 
 * Parameters: none
 * History: 19/03/2023 - initial version
**************************************************************************/
CTrackSegnment::CTrackSegnment(int i, int len) : m_segmentId(i),  m_segmentLength(len),
m_currentDirection(ETrainDirection::Backward)
{
    m_signals = vector<CTrainSignal>(m_segmentLength);
}
/**************************************************************************
 * Destructor
 * 
 * Parameters: none
 * History: 19/03/2023 - initial version
**************************************************************************/
CTrackSegnment::~CTrackSegnment ()
{
    m_tsConnections.clear();
}

/**************************************************************************
 * Function name: WaitForGreenSignal 
 * Description:   wait for the signal to be freed (GREEN)
 *                and turn it RED right away
 * Parameters:    int index - step in section
 * 
 * Return:        void
 * History: 19/03/2023 - initial version
 *          22/02/2023 - modified to support segments internal steps 
**************************************************************************/
void CTrackSegnment::WaitForGreenSignal(int stepIndex)
{
    if (stepIndex < m_signals.size())
        m_signals[stepIndex].WaitForGreenSignalAndTake();    
}

/**************************************************************************
 * Function name: WaitForGreenSignal 
 * Description:   wait for the signal to be freed (GREEN)
 *                and turn it RED right away
 * Parameters:    int stepIndex - step in multi step segment
 * 
 * Return:        void
 * History: 19/03/2023 - initial version
 *          22/02/2023 - modified to support segments internal steps 
**************************************************************************/
bool CTrackSegnment::TryToTakeGreenSignal(int  stepIndex)
{
    if (stepIndex < m_signals.size())    
        return m_signals[stepIndex].TryToTakeGreenSignal();
    else
        return false;
   
}

/* /**************************************************************************
 * Function name: ReleaseSignal 
 * Description:   Releases the signal and change it to GREEN
 *                and turn it red right away
 * Parameters:     int stepIndex - step in multi step segment
 * 
 * Return:        void
 * History: 19/03/2023 - initial version
 *          22/02/2023 - modified to support segments internal steps 
 *                       
**************************************************************************/
void CTrackSegnment::ReleaseSignal(int stepIndex)
{
    if (stepIndex < m_signals.size())
      m_signals[stepIndex].ReleaseSignal();
     
} 


/**************************************************************************
 * Function name: MoveTrain 
 * Description:   Moves the train if possible
 * 
 * Parameters:    none
 * 
 * Return:        status of the move
 * History: 22/03/2023 - initial version
 * History: 22/03/2023 - Introduced moving train step by step in multi step segment
**************************************************************************/
EMoveStatus CTrackSegnment::MoveTrain(ETrainDirection direction, int trainId, int& atStep)
{
    lock_guard<mutex> guard(m_cs);

    
    // opposite direction trains are being held until the segment is cleared
    if(m_trainRecords.size() != 0 && m_currentDirection != direction)
    {
        atStep = 0;
        return EMoveStatus::Waiting;
    }

    // if this is a new train, add it to the trains in progress
    // set light of the current train step as RED for others
    auto moveStatus = EMoveStatus::Waiting;

    // store the current direction
    m_currentDirection = direction; 
    
    // find the train 
    auto it = m_trainRecords.find(trainId);
    CTrainRecord rec;

    // if not found, create new element
    if(it == m_trainRecords.end())
    {
        auto current = direction == ETrainDirection::Forward ? 0 : m_segmentLength + 1;
        auto train_step = direction == ETrainDirection::Forward ? 1 : -1;
        auto stop = direction == ETrainDirection::Forward ? m_segmentLength : 1;
        m_trainRecords[trainId] = {direction, train_step, stop, current};
        
        rec = m_trainRecords[trainId];
    }
    else
        rec = it->second;
    
    // check the status of next section
    auto next_step = rec.m_current + rec.m_trainStep;

    // check if we can move there
    if(TryToTakeGreenSignal(next_step - 1))
    {
        // took the next sema, move there
        if(rec.m_current != 0 && rec.m_current != m_segmentLength + 1)
            ReleaseSignal(rec.m_current - 1);

        rec.m_current = next_step;
        m_trainRecords[trainId] = rec;

        moveStatus = next_step == rec.m_stop ? EMoveStatus::Finished : EMoveStatus::Moved;
        atStep = rec.m_current - 1;
    }
    
    return moveStatus;
}

/**************************************************************************
 * Function name: ReleaseSection 
 * Description:  Function that releases the segment when a train left it
 * 
 * Parameters:    int TrainId
 * 
 * Return:        status of the move
 * History: 22/03/2023 - initial version
**************************************************************************/
bool CTrackSegnment::ReleaseSection(int trainId)
{
    lock_guard<mutex> guard(m_cs);

    // find the train 
    auto it = m_trainRecords.find(trainId);
    
    // if such train is not found
    if(it == m_trainRecords.end())
        return false;

    auto stepIndex = it->second.m_current - 1;
    m_trainRecords.erase(it);
    if (stepIndex < m_signals.size())
    {
        ReleaseSignal(stepIndex);
        return true;
    }
    else
        return false;

    
}

 