#include "Speed_Bonus.h"
#include <functional>
#include<set>
#include <map>



void Speed_Bonus::fixing_Adj_List_bonus(
    vector<vector<Read_file::Adj_nodes_bonus>>& adj_list,
    vector<pair<double, double>>& co_ordinates,
    double source_x, double source_y,
    double destination_x, double destination_y, double R, int co_ordinates_size)
{
    R = R / 1000.0;
    R *= R;
    for (int i = 0; i < co_ordinates_size; ++i)
    {
        double dx_source = co_ordinates[i].first - source_x;
        double dy_source = co_ordinates[i].second - source_y;
        double dis_pow_2_source = dx_source * dx_source + dy_source * dy_source;
        if (dis_pow_2_source <= R)
        {
            double dis = sqrt(dis_pow_2_source);
            adj_list[0].push_back({ i + 1,dis, {dis / 5.0f} });
        }
        double dx_dest = co_ordinates[i].first - destination_x;
        double dy_dest = co_ordinates[i].second - destination_y;
        double dis_pow_2_dest = dx_dest * dx_dest + dy_dest * dy_dest;

        if (dis_pow_2_dest <= R)
        {
            double dis = sqrt(dis_pow_2_dest);
            adj_list[i + 1].push_back({ (int)(adj_list.size() - 1), dis, {dis / 5.0f} });
			adj_list[(int)(adj_list.size() - 1)].push_back({ i + 1, dis, {dis / 5.0f} });
            added_to_destination.push_back(i + 1);
        }

    }
    /*for (int i = 0; i < adj_list.size(); ++i)
    {
        cout << "Node : " << i;
        for (int j = 0; j < adj_list[i].size(); ++j)
        {
            cout << " { adj node is " << adj_list[i][j].node << " , the distance is " <<
                adj_list[i][j].distance << " ,the time is  "<< " } " << endl << "\t";
        }
        cout << endl;

    }*/
}

void Speed_Bonus::Dijkstra_edited(
    vector<vector<Read_file::Adj_nodes_bonus>>& adj_list,
    int source, int destination,
    vector<int>& path, double& shortest_time,
    double& path_length, double& total_walking_distance,
    double& total_vehicle_distance,
    double speed_interval_hours,
    int speed_count)
{
    int N = adj_list.size();
    vector<int> parent(N, -1); // O(N)
    vector<set<int>> visited_from(N); // Track all nodes each node was visited from

    vector<vector<Read_file::Adj_nodes_bonus>>temp = adj_list;

    // Priority queue for processing nodes in order of shortest time (min-heap)
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
    vector<set<pair<double, vector<int>>>> time(N);
    time[source].insert(std::make_pair(0.0, vector<int>{source}));
    pq.push({ 0.0f, source }); // O(log N)

    // Dijkstra's algorithm loop O(V)
    while (!pq.empty()) {
        double current_time = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (adj_list[u].size() == 0) continue;

        // If we came from a node and this node only has two neighbors, remove the back edge
        if (!visited_from[u].empty() && adj_list[u].size() == 2) {
            for (int from_node : visited_from[u]) {
                adj_list[u].erase(remove_if(
                    adj_list[u].begin(), adj_list[u].end(),
                    [&](const Read_file::Adj_nodes_bonus& neighbor) {
                        return neighbor.node == from_node;
                    }),
                    adj_list[u].end());

                has_been_deleted.push_back({ u, from_node });
            }
        }

        // O(E) loop over all edges from node u
        for (int i = 0; i < adj_list[u].size(); ++i) {
            int v = adj_list[u][i].node;

            // If we've already visited this node from the same parent before, remove it to prevent loop
            if (visited_from[v].count(u)) {
                adj_list[u].erase(adj_list[u].begin() + i);
                has_been_deleted.push_back({ u, v });
                --i; // adjust index after erase
                continue;
            }

            for (const auto& entry : time[u]) {
                double t = entry.first;
                vector<int> current_path = entry.second;

                double edge_time;
                if (v == destination || u == source)
                    edge_time = adj_list[u][i].time[0];
                else
                    edge_time = adj_list[u][i].time[static_cast<int>(t / speed_interval_hours) % speed_count];

                double new_time = t + edge_time;
                vector<int> new_path = current_path;
                new_path.push_back(v);

                time[v].insert(make_pair(new_time, new_path));
                pq.push({ new_time, v });
                parent[v] = u;
                visited_from[v].insert(u);
            }
        }
    }

    // Get the shortest time and path

    shortest_time = time[destination].begin()->first;
    path = time[destination].begin()->second;

	// Calculate path length, total walking distance, and total vehicle distance
    path_length = 0.0;
    total_walking_distance = 0.0;
    total_vehicle_distance = 0.0;

    for (size_t i = 1; i < path.size(); ++i) {
        int u = path[i - 1];
        int v = path[i];

        for (const auto& edge : temp[u]) {
            if (edge.node == v) {
                path_length += edge.distance;

                if ((i == 1 && u == source) || (i == path.size() - 1 && v == destination)) {
                    total_walking_distance += edge.distance;
                }
                else {
                    total_vehicle_distance += edge.distance;
                }
            }
        }
    }
}

void Speed_Bonus::orig_list(vector<pair<int, int>>& has_been_deleted)
{
    for (int i = 0; i < has_been_deleted.size(); ++i)
    {

    }
}


void Speed_Bonus::Original_Adj_list_bonus(vector<vector<Read_file::Adj_nodes_bonus>>& adj_list)
{
    adj_list[0].clear();

    for (int node : added_to_destination) {
        adj_list[node].pop_back();
    }
    added_to_destination.clear();
}

