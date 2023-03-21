/*--------------------------------------------------------------------------
 * File name:     Utilites.cpp
 * Description:   Utilities functions - different algorithms to
 *                calculate shortest path
 *                For unweighted graphs - Breadth First Seach Algorithm
 *                For weighted graphs - Dijkstra_WeightedGraph
 *                In current Rail Road implementation weighted
 *                graph is created and shortest path is calculated
 *                by Dijkstra's algorithm
 *
 *
 *
 * Author:        Tanya Shkinev
 *---------------------------------------------------------------------------
 */

#include "utilities.h"
#include "railroad.h"
//#define USE_UNWEIGHTED_GRAPH
//  iPair ==> Integer Pair
typedef pair<int, int> iPair;

/**************************************************************************
 * Function name: BuildRailRoad
 * Description:   Reads xml file and builds rail road model
 *                The elements in xml file represent the main objects
 *                of rail road model such as (example below):
 *                Train Id = 1 is scheduled to move from segment 0 to 6
 *                  <Train id="1" from="0" to="6"/>
 *                Rail segment - Rail Segment ID = 1 has a length 1
 *                  <TrackSegment id="0" length="1"/>
 *                Connection between segments  - segment 0 has a connection
 *                   to segment 6
 *                  <Connection source="0" target="1"/>
 *
 * Parameters:    CRailRoad * pRailRoad - pointer to RailRoad object
 *
 * Return:        void
 * History: 19/03/2023 - initial version
 **************************************************************************/
void BuildRailRoad(const string &filename, CRailRoad *pRailRoad)
{
    // Load XML file
    XMLDocument doc;
    XMLError err = doc.LoadFile(filename.c_str());
    if (err == XML_SUCCESS && pRailRoad != NULL)
    {
        XMLElement *railRoadElement = doc.FirstChildElement("RailRoad");
        if (railRoadElement != nullptr)
        {
            XMLElement *trackSegsElement = railRoadElement->FirstChildElement("TrackSegments");
            if (trackSegsElement != nullptr)
            {
                for (XMLElement *trackSegElement = trackSegsElement->FirstChildElement("TrackSegment");
                     trackSegElement != nullptr; trackSegElement = trackSegElement->NextSiblingElement("TrackSegment"))
                {
                    int trackSegmentId, length;
                    trackSegElement->QueryIntAttribute("id", &trackSegmentId);
                    trackSegElement->QueryIntAttribute("length", &length);
                    pRailRoad->AddTrackSegment(trackSegmentId, length);
                }
            }
            
            XMLElement *connectionsElement = railRoadElement->FirstChildElement("Connections");
            if (connectionsElement != nullptr)
            {

                for (XMLElement *connectionElement = connectionsElement->FirstChildElement("Connection");
                     connectionElement != nullptr; connectionElement = connectionElement->NextSiblingElement("Connection"))
                {
                    int sourceId, targetId;
                    connectionElement->QueryIntAttribute("source", &sourceId);
                    connectionElement->QueryIntAttribute("target", &targetId);
                    pRailRoad->AddConnection(sourceId, targetId);
                }
            }
            
            XMLElement *trainsElement = railRoadElement->FirstChildElement("Trains");
            if (trainsElement != nullptr)
            {

                for (XMLElement *trainElement = trainsElement->FirstChildElement("Train");
                     trainElement != nullptr; trainElement = trainElement->NextSiblingElement("Train"))
                {
                    int trainId, from, to;
                    trainElement->QueryIntAttribute("id", &trainId);
                    trainElement->QueryIntAttribute("from", &from);
                    trainElement->QueryIntAttribute("to", &to);

                    pRailRoad->AddTrain(trainId, from, to);
                }
            }
        }
    }
    else
        cout << "can't load xml file " << filename <<" TinyXML error: " << err << endl;
}

/**************************************************************************
 * Function name: PrintRailRoad
 * Description:   Print layout of rail road model
 * Parameters:    CRailRoad * pRailRoad - pointer to RailRoad object
 *
 * Return:        void
 * History: 19/03/2023 - initial version
 **************************************************************************/
void PrintRailRoad(CRailRoad *pRailRoad)
{ // Print railroad
    cout << "Rail Road Connections Map \n";
    for (auto it : pRailRoad->GetTrackSegMap())
    {
        CTrackSegnment *segment = it.second;
        cout << "Segment " << segment->GetTrackSegmentId() << ": " << "Connected to:";
        for (CTrackSegnment *connection : segment->GetTrackSegmentConnections())
        {
            cout << connection->GetTrackSegmentId() << " ";
        }
        cout << endl;
    }
    return;
}
/**************************************************************************
 * Function name: CalculateShortestPathForAllTrains
 *
 * Description:   Runs shortest path algorithm for all
 *                all trains currently in rail road model
 *
 * Parameters:     CRailRoad * pRailRoad - pointer to RailRoad object
 *
 * Return:        void
 * History: 19/03/2023 - initial version
 **************************************************************************/
void CalculateShortestPathForAllTrains(CRailRoad *prailroad)
{
    for (auto it = prailroad->GetTrainList().begin(); it != prailroad->GetTrainList().end(); it++)
    {
        list<CTrackSegnment *> shortPathSegList;
        auto trainId = it->first;
        auto *train = it->second;

        // build shortest path vector (contains only segments indexes)
        BuildShortestPathForTrain(train, prailroad->GetTrackSegMap());

        // build list of segnments objects for train shortest part
        shortPathSegList = ConvertPathToSegmentList(prailroad, train->GetTrainPath());
        // atttach the list to the train
        train->SetTrainShortPathSegList(shortPathSegList);
    }
}

/**************************************************************************
 * Function name: CalculateShortestPathForAllTrains
 *
 * Description:   Runs shortest path algorithm for a train and repopulates
 *
 *
 *
 * Parameters:    CTrain * pTrain - pointer to train object
 *                unordered_map<int, CTrackSegnment*> & trackSegmentMap
 *                      - segment map of the current model
 *
 *
 * Return:        void
 * History: 19/03/2023 - initial version
 **************************************************************************/
void BuildShortestPathForTrain(CTrain *pTrain, unordered_map<int, CTrackSegnment *> &trackSegmentMap)
{

    int source, dest, numSegments;
    if (pTrain != NULL)
    {
        source = pTrain->GetTrainDepartPoint();
        dest = pTrain->GetTrainDestPoint();
        numSegments = trackSegmentMap.size();
    
        cout << "\nCalculating path for train " << pTrain->GetTrainId() << " dept " << pTrain->GetTrainDepartPoint() << " dest " << pTrain->GetTrainDestPoint() << endl;

        FindShortestPath(trackSegmentMap, source, dest, numSegments, &pTrain->GetTrainPath());
       
        cout << "\nThe shortest path contains : " <<  pTrain->GetTrainPathLength() << " segments"<< "\n";
        for (int i = pTrain->GetTrainPathLength() -1; i >=0; i--)
            cout << pTrain->GetTrainPath()[i] << " ";
        cout << "\n" << "\n";
       
    }
}
/**************************************************************************
 * Function name: ConvertPathToSegmentList
 *
 * Description:   The algorithm returns shortest path in list of segments id
 *                This function converts the list into list of pointers of
 *                segments objects for arch train path and attach the list to
 *                the train
 *

 * Parameters:    CRailRoad * pRailRoad - pointer to RailRoad object
 *                vector<int> &trainpath - shortest path generated
 *                by algorithm
 *
 *
 * Return:        list<CTrackSegnment*>  - list of the tarck segments
 *                 representing shortest path for the train
 *
 * History: 19/03/2023 - initial version
**************************************************************************/
list<CTrackSegnment *> ConvertPathToSegmentList(CRailRoad *p_schema, vector<int> &trainpath)
{
    list<CTrackSegnment *> shortpathseglist;
    CTrackSegnment * tmpts = NULL;
    auto &mapsegment = p_schema->GetTrackSegMap();

    for (int trainid = trainpath.size() - 1; trainid >= 0; trainid--)
    {
        int segment = trainpath[trainid];
        shortpathseglist.push_back(mapsegment[segment]);
    }
    return shortpathseglist;
}

/**************************************************************************
 * Function name: BFS_UnweightedGraph
 *
 * Description:   Modified version of Breadth First Seach Algorithm (BFS) to find
 *                and save distance from src to distination in graph structure
 *                in resultant array (dist[])
 *

 * Parameters:    unordered_map<int, CTrackSegnment*>& adjlist - model
 *                adjusent list
 *                int src - source
 *                int dest - destination
 *                int v - number of vertices (in our case number of rack segments)
 *                int pred[], int dist[]
 *
 *
 *
 * Return:       bool - returns false if there is no connection/path
 *                      between src and dest
 *                      return true if path is found
 *
 * History: 19/03/2023 - initial version
**************************************************************************/
bool BFS_UnweightedGraph(unordered_map<int, CTrackSegnment *> &adjlist, int src, int dest, int v,
                         int pred[], int dist[])
{
    // a queue to maintain queue of vertices whose
    // adjacency list is to be scanned as per normal
    // DFS algorithm
    list<int> queue;

    // boolean array visited[] which stores the
    // information whether ith vertex is reached
    // at least once in the Breadth first search
    bool visited[v];

    // initially all vertices are unvisited
    // so v[i] for all i is false
    // and as no path is yet constructed
    // dist[i] for all i set to infinity
    for (int i = 0; i < v; i++)
    {
        visited[i] = false;
        dist[i] = INT_MAX;
        pred[i] = -1;
    }

    // now source is first to be visited and
    // distance from source to itself should be 0
    visited[src] = true;
    dist[src] = 0;
    queue.push_back(src);

    // standard BFS algorithm
    while (!queue.empty())
    {
        int u = queue.front();
        queue.pop_front();

        CTrackSegnment *segment = adjlist[u];
        for (CTrackSegnment *connection : segment->GetTrackSegmentConnections())
        {
            if (visited[connection->GetTrackSegmentId()] == false)
            {
                visited[connection->GetTrackSegmentId()] = true;
                dist[connection->GetTrackSegmentId()] = dist[u] + 1;
                pred[connection->GetTrackSegmentId()] = u;
                queue.push_back(connection->GetTrackSegmentId());

                // We stop BFS when we find
                // destination.
                if (connection->GetTrackSegmentId() == dest)
                    return true;
            }
        }
    }

    return false;
}

// utility function to print the shortest distance
// between source and destination
void FindShortestPath(unordered_map<int, CTrackSegnment *> &adjlist, int s,
                      int dest, int v, vector<int> *path)
{
    // predecessor[i] array stores predecessor of
    // i and distance array stores distance of i
    // from s
    int pred[v], dist[v];
#ifdef USE_UNWEIGHTED_GRAPH
    if (BFS_UnweightedGraph(adjlist, s, dest, v, pred, dist) == false)
    {
        cout << "Given source and destination"
             << " are not connected";
        return;
    }
#else // USE_WEIGHTED_GRAPH
    if (Dijkstra_WeightedGraph(adjlist, s, dest, v, pred, dist) == false)
    {
        cout << "Given source and destination"
             << " are not connected";
        return;
    }
#endif

    // vector path stores the shortest path
    int crawl = dest;
    path->clear();
    path->push_back(crawl);
    while (pred[crawl] != -1)
    {
        path->push_back(pred[crawl]);
        crawl = pred[crawl];        
    }
    // distance from source is in distance array
    cout << "Shortest path weight is : "
        << dist[dest] << " for src : " << s << " to dest : " << dest;
}

/*


*/

/**************************************************************************
 * Function name: Dijkstra_WeightedGraph
 *
 * Description:   Modified version of Dijkstra to find
 *                and save distance from src to distination in graph structure
 *                in resultant array (dist[])
 *

 * Parameters:    unordered_map<int, CTrackSegnment*>& adjlist - model
 *                adjusent list
 *                int src - source
 *                int dest - destination
 *                int v - number of vertices (in our case number of rack segments)
 *                int pred[], int dist[]
 *
 *
 *
 * Return:       bool - returns false if there is no connection/path
 *                      between src and dest
 *                      return true if path is found
 *
 * History: 19/03/2023 - initial version
**************************************************************************/
bool Dijkstra_WeightedGraph(unordered_map<int, CTrackSegnment *> &adjlist, int src, int dest, int v,
                            int pred[], int dist[])
{
    // Create a priority queue to store vertices that
    // are being preprocessed.
    priority_queue<iPair, vector<iPair>, greater<iPair>>
        pq;

    // initially all vertices are unvisited
    // so v[i] for all i is false
    // and as no path is yet constructed
    // dist[i] for all i set to infinity
    for (int i = 0; i < v; i++)
    {
        dist[i] = INT_MAX;
        pred[i] = -1;
    }

    // Insert source itself in priority queue and initialize
    // its distance as 0.
    pq.push(make_pair(0, src));
    dist[src] = 0;

    /* Looping till priority queue becomes empty (or all
    distances are not finalized) */
    while (!pq.empty())
    {
        // The first vertex in pair is the minimum distance
        // vertex, extract it from priority queue.
        // vertex label is stored in second of pair (it
        // has to be done this way to keep the vertices
        // sorted distance (distance must be first item
        // in pair)
        int u = pq.top().second;
        pq.pop();

        CTrackSegnment *segment = adjlist[u];
        for (CTrackSegnment *connection : segment->GetTrackSegmentConnections())
        {
            // Get vertex label and weight of current
            // adjacent of u.
            int v = connection->GetTrackSegmentId();
            int weight = connection->GetTrackSegmentLength();
            // If there is shorted path to v through u.
            if (dist[v] > dist[u] + weight)
            {
                // Updating distance of
                dist[v] = dist[u] + weight;
                pred[connection->GetTrackSegmentId()] = u;
                pq.push(make_pair(dist[v], v));

                if (connection->GetTrackSegmentId() == dest)
                    return true;
            }
        }
    }
    return false;
}
