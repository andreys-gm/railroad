/* /*--------------------------------------------------------------------------
* File name:     TrackSegment.cpp 
* Description:   TrackSegment class implementation 
*                
*
* Author:        Tanya Shkinev                                                     
*---------------------------------------------------------------------------
*/

#include "tracksegment.h"

/**************************************************************************
 * Constructor is called RailRoad object
 * 
 * Parameters: none
 * History: 19/03/2023 - initial version
**************************************************************************/
CTrackSegnment::CTrackSegnment(int i, int len) : m_segmentId(i),  m_segmentLength(len)
{
    
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
 * Parameters:    none
 * 
 * Return:        void
 * History: 19/03/2023 - initial version
**************************************************************************/
void CTrackSegnment::WaitForGreenSignal()
{
    m_signal.WaitForGreenSignalAndTake();
}

/**************************************************************************
 * Function name: WaitForGreenSignal 
 * Description:   wait for the signal to be freed (GREEN)
 *                and turn it RED right away
 * Parameters:    none
 * 
 * Return:        void
 * History: 19/03/2023 - initial version
**************************************************************************/
bool CTrackSegnment::TryToTakeGreenSignal()
{
    return m_signal.TryToTakeGreenSignal();
}
/**************************************************************************
 * Function name: ReleaseSignal 
 * Description:   Releases the signal and change it to GREEN
 *                and turn it red right away
 * Parameters:    none
 * 
 * Return:        void
 * History: 19/03/2023 - initial version
**************************************************************************/
void CTrackSegnment::ReleaseSignal()
{
    m_signal.ReleaseSignal();
}
