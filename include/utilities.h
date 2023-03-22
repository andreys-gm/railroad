/* /*--------------------------------------------------------------------------
* Objectnane: Utilities.h                   
*
* Author: Tanya Shkinev                                                        
----------------------------------------------------------------------------
*/
#ifndef UTILITIES_H
#define UTILITIES_H

#include "railroad.h"

using namespace std;

/* Build Rail Road model. Rails track is implemented 
*/
void BuildRailRoad(const std::string& filename, CRailRoad * pRailRoad);

/* Prints Rail Road layout 
*  
*/
void PrintRailRoad (CRailRoad * pRailRoad);

/* Runs shortest path algorithm for all
*  all trains currently in rail road model
*/
void CalculateShortestPathForAllTrains(CRailRoad * pRailRoad);

/* Runs shortest path algorithm for a train
*/
void BuildShortestPathForTrain(CTrain * pTrain, CRailRoad * pShema);

/* The algorithm returns shortest path in list of segments id 
*  This function converts the list into list of pointers of 
*  segments objects for arch train path and attach the list to 
*  the train
*/
list<CTrackSegnment*> ConvertPathToSegmentList(CRailRoad * p_schema, const vector<int> &trainpath);


/* Modified version of Breadth First Seach Algorithm (BFS) to find 
   and save distance from src to distination in graph structure
  in resultant array (dist[]) 
*/
bool BFS_UnweightedGraph(CRailRoad * p_schema, int src, int dest, int v,
         int pred[], int dist[]);

/* Modified version of Dijkstra to find 
and save distance from src to distination in graph structure
in resultant array (dist[])
*/ 

bool Dijkstra_WeightedGraph(CRailRoad * p_schema, int src,  int dest, int v,
         int pred[], int dist[]);
                                            

/* utility function to find the shortest distance
   between source and destination 
*/
void FindShortestPath(CRailRoad * p_schema, int s,
                           int dest, int v,  vector<int> * path);


#endif // __UTILITIES_H__