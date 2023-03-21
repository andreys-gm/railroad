/* /*--------------------------------------------------------------------------
* File name:     TrainCtrlSim.cpp 
* Description:   Train control simulation implementation 
*                The application builds rail road model defined 
*                in xml file
*                It builds rail segments, connections and train 
*                objects
*                
*
* Author:        Tanya Shkinev                                                     
*---------------------------------------------------------------------------
*/
#include <stdlib.h>
#include <unistd.h> 

#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <climits>
#include <string>
#include <list>
#include "tinyxml2.h"

#include "train.h"
#include "railroad.h"
#include "utilities.h"

using namespace tinyxml2;
using namespace std;

int main(int argc, char* argv[])
{
    int opt = 0;
    std::string filename = "../resources/railroadlayout1.xml";
    
    while ((opt = getopt(argc, argv, "f:")) != -1) {
        switch (opt) {
        case 'f': filename = optarg; break;
        default:
            fprintf(stderr, "Usage: %s [-f file.xml]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    CRailRoad *p_schema = new CRailRoad();
    if (p_schema != NULL)
    {
        BuildRailRoad(filename, p_schema);
        PrintRailRoad(p_schema);
        CalculateShortestPathForAllTrains(p_schema);
        auto trainlist = p_schema->GetTrainList();

        // run all trains
        for (auto &el : trainlist)
        {
            auto train = el.second;
            train->StartEngine();
        }

        // wait for all trains to arrive
        for (auto &el : trainlist)
        {
            auto train = el.second;
            train->WaitForTrainArrival();
        }

        delete p_schema;
    }
    return 0;
}

