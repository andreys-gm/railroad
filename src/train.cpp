/* /*--------------------------------------------------------------------------
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

#define SEGMENT_LENGTH_UNIT  1
std::mutex CTrain::m_coutMutex;
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
        if (from <= to)  
            m_direction = ETrainDirection::Forward;
        else
            m_direction = ETrainDirection::Backward;

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
    int segment_length = 0;
    auto it = m_shortPathSegList.begin(); 
    m_it = it;

    {
        std::lock_guard<std::mutex> guard(m_coutMutex);
        cout << "Running train " << m_trainId << " path: ";
    
        for(auto& a : m_shortPathSegList)
            cout << a->GetTrackSegmentId() << " ";
        cout << endl;
    }

    while(it != m_shortPathSegList.end())
    {
        {
            std::lock_guard<std::mutex> guard(m_coutMutex);
            cout << "train: " << m_trainId << " at " << (*m_it)->GetTrackSegmentId() << endl;
        }
       
        it++;
        if (it != m_shortPathSegList.end())
        {
            // new segment, run until finished
            auto ts = (*it);
            auto move_status = EMoveStatus::Waiting;
            
            // loop until finished the segment
            auto step = 0;
            do
            {
                move_status = ts->MoveTrain(m_direction, m_trainId, step);

                // loop here if we got red light
                while(EMoveStatus::Waiting == move_status)
                {
                    {
                        std::lock_guard<std::mutex> guard(m_coutMutex);
                        cout << "train: " << m_trainId << " waiting on RED for " << ts->GetTrackSegmentId() << " step " << step << endl;
                    }

                    std::this_thread::sleep_for(10ms);  
                    move_status = ts->MoveTrain(m_direction, m_trainId, step);
                };


                {
                    std::lock_guard<std::mutex> guard(m_coutMutex);
                    cout << "train: " << m_trainId << " segment " << ts->GetTrackSegmentId() << " step " << step << endl;
                }

                std::this_thread::sleep_for(10ms); 

            }while(EMoveStatus::Finished != move_status);

            // release the prev section
            (*m_it)->ReleaseSection(m_trainId);

            m_it = it;
            /*{
                std::lock_guard<std::mutex> guard(m_coutMutex);
                cout << "train: " << m_trainId << " moving to " << (*m_it)->GetTrackSegmentId() << endl;
            }*/
        }
        else
        {
            // release the section, we have finished the route
            (*m_it)->ReleaseSection(m_trainId);
        }

        
        // sleep here for a bit to give other trains a chance
        std::this_thread::sleep_for(10ms); 
    }

    {
        std::lock_guard<std::mutex> guard(m_coutMutex);
        cout << "train: " << m_trainId << " arrived" << endl;
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


/**************************************************************************
 * Function name: PrintTrainPath 
 * Description:   Prints train's path
 * Parameters:    none
 * 
 * Return:        void
 * History: 22/03/2023 - initial version
**************************************************************************/
void CTrain::PrintTrainPath()
{
    cout << "\nThe shortest path contains : " <<  m_trainPath.size() << " segments"<< "\n";
    for (int i = m_trainPath.size() - 1; i >=0; i--)
        cout << m_trainPath[i] << " ";
    cout << "\n" << "\n";
}
