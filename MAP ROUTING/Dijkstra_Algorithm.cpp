#include "Dijkstra_Algorithm.h"
#include <queue>
#include <iostream>
#include <vector>
#include <stack>
#include <cmath>
#include <climits>
#include <algorithm>

using namespace std;



void Dijkstra_Algorithm::fixing_Adj_List(
    vector<vector<Read_file::Adj_nodes>>& adj_list,
	vector<pair<double, double>>& co_ordinates,
    double source_x, double source_y,
    double destination_x, double destination_y, double R,int co_ordinates_size)
{
	R = R / 1000.0;
	R *= R;  
	for (int i = 0; i < co_ordinates_size; ++i)
	{
        double dx_source = co_ordinates[i].first - source_x;
        double dy_source = co_ordinates[i].second - source_y;
        double dis_pow_2_source = dx_source * dx_source + dy_source * dy_source;
        // The nearby nodes to the source.
		if (dis_pow_2_source <= R)
		{
            double dis = sqrt(dis_pow_2_source);
            adj_list[0].push_back({ i + 1,dis, dis / 5.0f });
		}
        double dx_dest = co_ordinates[i].first - destination_x;
        double dy_dest = co_ordinates[i].second - destination_y;
        double dis_pow_2_dest = dx_dest * dx_dest + dy_dest * dy_dest;
        // The nearby nodes to the destination.
        // We store the index of every node near to destination to delete it after applying the 
        // algorithm to return the adj_list as it was.
		if (dis_pow_2_dest <= R)
		{
            double dis = sqrt(dis_pow_2_dest);
            adj_list[i + 1].push_back({ (int)(adj_list.size() - 1), dis, dis / 5.0f });
			added_to_destination.push_back(i + 1);
		}
	}
    /*for (int i = 0; i < adj_list.size(); ++i)
    {
        cout << "Node : " << i;
        for (int j = 0; j < adj_list[i].size(); ++j)
        {
            cout << " { adj node is " << adj_list[i][j].node << " , the distance is " <<
                adj_list[i][j].distance << " ,the time is  " << adj_list[i][j].time <<" } " << endl << "\t";
        }
        cout << endl;

    }*/
}


void Dijkstra_Algorithm::dijkstra(
    vector<vector<Read_file::Adj_nodes>>& adj_list,
    int source, int destination,
    vector<int>& path, double& shortest_time,
    double& path_length, double& total_walking_distance,
    double& total_vehicle_distance, int adj_list_size)
{
    int N = adj_list_size;

    vector<double> time(N, INFINITY); // O(N)
    vector<int> parent(N, -1); // O(N)
    time[source] = 0.0f;

    // Priority queue for processing nodes in order of shortest time (min-heap)
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
    pq.push({ 0.0f, source }); // O(log N)

    // Dijkstra's algorithm loop O(V)
    while (!pq.empty()) {
        double current_time = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (u == destination) break;

        // O(E) loop over all edges from node u
        for (auto& neighbor : adj_list[u]) {
            int v = neighbor.node;
            double edge_time = neighbor.time;

            double new_time = current_time + edge_time;

            if (new_time < time[v]) {
                time[v] = new_time;
                parent[v] = u;
                pq.push({ new_time, v });
            }
        }
    }


    // Reconstruct the shortest path
    for (int v = destination; v != -1; v = parent[v]) {
        path.push_back(v);
    }
    reverse(path.begin(), path.end());

    // Calculate path length and distances
    int path_size = path.size();

    for (size_t i = 1; i < path_size; ++i) {
        int u = path[i - 1];
        int v = path[i];

        // Find the corresponding edge between u and v
        for (const auto& neighbor : adj_list[u]) {
            if (neighbor.node == v) {
                double edge_distance = neighbor.distance;
                double edge_time = neighbor.time;

                path_length += edge_distance;  
                // Walking distance for the first and last nodes
                if (i == 1 || i == path_size - 1) {
                    total_walking_distance += edge_distance;
                }
                else {
                    total_vehicle_distance += edge_distance;
                }
                break;
            }
        }
    }
    shortest_time = time[destination];
}



void Dijkstra_Algorithm::Original_Adj_list(vector<vector<Read_file::Adj_nodes>>& adj_list)
{
    // After assuming that the source node is 0 node , so we can't go from any other node to 0 
    // we can only go from source to other nodes (S nodes)
    adj_list[0].clear();
    // Same as destination but the differece is that we can't go from destination to any other node 
    // we can only go from (D nodes) to destination
    for (int node : added_to_destination) {
        adj_list[node].pop_back(); 
    }
    added_to_destination.clear();
}
