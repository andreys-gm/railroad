/*--------------------------------------------------------------------------
* File name:     Train.cpp 
* Description:   Train class implementation 
*                
*
* Author:        Tanya Shkinev                                                     
*---------------------------------------------------------------------------
*/

#include <iostream>
#include <thread>
#include "train.h"

using namespace std;

/**************************************************************************
 * Constructor is called RailRoad object
 * 
 * Parameters: int i - unique train id 
 *             int from - departure trail segment
 *             int to -  destination trail segment
 * 
 * History: 19/03/2023 - initial version
**************************************************************************/
CTrain::CTrain(int i, int from, int to) : 
    m_trainId(i) , m_departurePoint(from) , m_destinationPoint (to) {
        m_currentPoint = m_departurePoint; 
        m_it =  m_shortPathSegList.begin();       
}
/**************************************************************************
 * Destructor
 * 
 * Parameters: none
 * History: 19/03/2023 - initial version
 *          20/02/2023 - free memory
**************************************************************************/
CTrain::~CTrain()
{
    m_shortPathSegList.clear();
    
}

/**************************************************************************
 * Function name: RunTrain 
 * Description:   wait for the signal to be freed (GREEN)
 *                and turn it RED right away
 * Parameters:    none
 * 
 * Return:        void
 * History: 19/03/2023 - initial version
**************************************************************************/
void CTrain::RunTrain()
{
    auto it = m_shortPathSegList.begin(); 
    m_it = it;

    cout << "Running train " << m_trainId << " path: ";
    for(auto& a : m_shortPathSegList)
        cout << a->GetTrackSegmentId() << " ";
    cout << '\n';

    while(it != m_shortPathSegList.end())
    {
        cout << "train: " << m_trainId << " at " << (*m_it)->GetTrackSegmentId() << endl;
                  
        it++;
        if (it != m_shortPathSegList.end())
        {
            auto* ts = (*it);
            while(!ts->TryToTakeGreenSignal())
            {
               cout << "train: " << m_trainId << " waiting on RED at " << ts->GetTrackSegmentId() << endl;
               std::this_thread::sleep_for(10ms);  
            };
            (*m_it)->ReleaseSignal();
            m_it = it;
            cout << "train: " << m_trainId << " at " << (*m_it)->GetTrackSegmentId() << endl;

        }
        else
        {
            (*m_it)->ReleaseSignal();
        }
        
        // sleep here for a bit to give other trains a chance
        std::this_thread::sleep_for(10ms); 
    }
    return;
    
}

/**************************************************************************
 * Function name: StartEngine 
 * Description:   Starts the train thread, effectively 
 *                staring its jorney
 * Parameters:    none
 * 
 * Return:        void
 * History: 19/03/2023 - initial version
 *          20/03/203 -  member name changes
**************************************************************************/
void CTrain::StartEngine()
{
    m_trainThread = std::thread(CTrain::s_RunTrain, this);
}

/**************************************************************************
 * Function name: WaitForTrainArrival 
 * Description:   wait for the train task to finish
 * Parameters:    none
 * 
 * Return:        void
 * History: 19/03/2023 - initial version
 *          20/03/203 -  member name changes
**************************************************************************/
void CTrain::WaitForTrainArrival()
{  
    m_trainThread.join();    
}