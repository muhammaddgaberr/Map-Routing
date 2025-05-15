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
	vector<pair<float, float>>& co_ordinates,
	float source_x, float source_y,
	float destination_x, float destination_y, float R)
{
	R = R / 1000.0;
	R *= R;  
	int sz = co_ordinates.size();

	for (int i = 0; i < sz; ++i)
	{
		float dx_source = co_ordinates[i].first - source_x;
		float dy_source = co_ordinates[i].second - source_y;
		float dis_pow_2_source = dx_source * dx_source + dy_source * dy_source;
		if (dis_pow_2_source <= R)
		{
			float dis = sqrt(dis_pow_2_source);
            adj_list[0].push_back({ i + 1,dis, dis / 5.0f });
		}

		float dx_dest = co_ordinates[i].first - destination_x;
		float dy_dest = co_ordinates[i].second - destination_y;
		float dis_pow_2_dest = dx_dest * dx_dest + dy_dest * dy_dest;

		
		if (dis_pow_2_dest <= R)
		{
			float dis = sqrt(dis_pow_2_dest);
            adj_list[i + 1].push_back({ (int)(adj_list.size() - 1), dis, dis / 5.0f });
			added_to_destination.push_back(i + 1);
		}
	}


    /*for (int i = 0; i < sz; ++i)
    {
        cout << "Node : " << i;
        for (int j = 0; j < adj_list[i].size(); ++i)
        {
            cout<< " { adj node is "<<adj_list[i][j].first
        }

    }*/


}


void Dijkstra_Algorithm::dijkstra(
    vector<vector<Read_file::Adj_nodes>>& adj_list,
    int source, int destination,
    vector<int>& path, float& shortest_time,
    float& path_length, float& total_walking_distance,
    float& total_vehicle_distance)
{
    int N = adj_list.size();

    
    vector<float> time(N, INFINITY); // O(N)
    vector<int> parent(N, -1); // O(N)
    time[source] = 0.0f;

    // Priority queue for processing nodes in order of shortest time (min-heap)
    priority_queue<pair<float, int>, vector<pair<float, int>>, greater<pair<float, int>>> pq;
    pq.push({ 0.0f, source }); // O(log N)

    // Dijkstra's algorithm loop O(V)
    while (!pq.empty()) {
        float current_time = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (u == destination) break;

        // O(E) loop over all edges from node u
        for (auto& neighbor : adj_list[u]) {
            int v = neighbor.node;
            float edge_time = neighbor.time;

            float new_time = current_time + edge_time;

            if (new_time < time[v]) {
                time[v] = new_time;
                parent[v] = u;
                pq.push({ new_time, v });
            }
        }
    }

    // Reconstruct the shortest path
    path.clear();
    for (int v = destination; v != -1; v = parent[v]) {
        path.push_back(v);
    }

    reverse(path.begin(), path.end());

    // Calculate path length and distances
    path_length = 0;
    total_walking_distance = 0;
    total_vehicle_distance = 0;

    for (size_t i = 1; i < path.size(); ++i) {
        int u = path[i - 1];
        int v = path[i];

        // Find the corresponding edge between u and v
        for (const auto& neighbor : adj_list[u]) {
            if (neighbor.node == v) {
                float edge_distance = neighbor.distance;
                float edge_time = neighbor.time;

                path_length += edge_distance;  
                // Walking distance for the first and last nodes
                if (i == 1 || i == path.size() - 1) {
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

    adj_list[0].clear();

    for (int node : added_to_destination) {
        adj_list[node].pop_back(); 
    }
    added_to_destination.clear();
}

