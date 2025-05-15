#include "Speed_Bonus.h"
#include <functional>
#include<set>



void Speed_Bonus::fixing_Adj_List_bonus(
    vector<vector<Read_file::Adj_nodes_bonus>>& adj_list,
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
            adj_list[0].push_back({ i + 1,dis, {dis / 5.0f} });
        }
        float dx_dest = co_ordinates[i].first - destination_x;
        float dy_dest = co_ordinates[i].second - destination_y;
        float dis_pow_2_dest = dx_dest * dx_dest + dy_dest * dy_dest;

        if (dis_pow_2_dest <= R)
        {
            float dis = sqrt(dis_pow_2_dest);
            adj_list[i + 1].push_back({ (int)(adj_list.size() - 1), dis, {dis / 5.0f} });
            added_to_destination.push_back(i + 1);
        }

    }
}

void Speed_Bonus::Billman_ford_bonus(
    vector<vector<Read_file::Adj_nodes_bonus>>& adj_list,
    int source_node_idx, int destination_node_idx,
    vector<int>& path,
    float& shortest_time_hours,
    float& path_length_km,
    float& total_walking_distance_km,
    float& total_vehicle_distance_km,
    float speed_interval_hours,
    int speed_count)
{
    int N = adj_list.size();
    if (N == 0 || source_node_idx < 0 || source_node_idx >= N || destination_node_idx < 0 || destination_node_idx >= N) {
        shortest_time_hours = std::numeric_limits<float>::max();
        path.clear();
        path_length_km = 0.0f;
        total_walking_distance_km = 0.0f;
        total_vehicle_distance_km = 0.0f;
        return;
    }

    vector<float> dist_hours(N, std::numeric_limits<float>::max());
    vector<int> parent(N, -1);

    dist_hours[source_node_idx] = 0.0f;

    for (int iter = 0; iter < N - 1; ++iter) { // Renamed loop variable for clarity
        bool changed_in_iteration = false;
        for (int u = 0; u < N; ++u) {
            if (dist_hours[u] == std::numeric_limits<float>::max()) {
                continue;
            }

            for (const auto& edge : adj_list[u]) {
                int v = edge.node; // Corrected: Use .node instead of .to
                if (v < 0 || v >= N) continue;

                float actual_travel_time_uv_hours;
                if (edge.time.empty()) {
                    actual_travel_time_uv_hours = std::numeric_limits<float>::max();
                }
                else if (edge.time.size() == 1 || speed_count <= 0 || speed_interval_hours <= 0.0f) {
                    actual_travel_time_uv_hours = edge.time[0];
                }
                else {
                    float time_to_u_hours = dist_hours[u];
                    int current_interval_idx = 0;
                    if (speed_interval_hours > 1e-9) {
                        current_interval_idx = static_cast<int>(time_to_u_hours / speed_interval_hours);
                    }
                    int selected_time_idx = current_interval_idx % speed_count;

                    if (selected_time_idx < 0 || static_cast<size_t>(selected_time_idx) >= edge.time.size()) {
                        actual_travel_time_uv_hours = edge.time[0];
                    }
                    else {
                        actual_travel_time_uv_hours = edge.time[selected_time_idx];
                    }
                }

                if (dist_hours[u] != std::numeric_limits<float>::max() && actual_travel_time_uv_hours != std::numeric_limits<float>::max() && dist_hours[u] + actual_travel_time_uv_hours < dist_hours[v]) {
                    dist_hours[v] = dist_hours[u] + actual_travel_time_uv_hours;
                    parent[v] = u;
                    changed_in_iteration = true;
                }
            }
        }
        if (!changed_in_iteration) {
            break;
        }
    }

    path.clear();
    shortest_time_hours = dist_hours[destination_node_idx];
    path_length_km = 0.0f;
    total_walking_distance_km = 0.0f;
    total_vehicle_distance_km = 0.0f;

    if (shortest_time_hours == std::numeric_limits<float>::max()) {
        return;
    }

    int curr = destination_node_idx;
    while (curr != -1) {
        path.push_back(curr);
        if (parent[curr] == curr && curr != source_node_idx) {
            path.clear();
            shortest_time_hours = std::numeric_limits<float>::max();
            return;
        }
        curr = parent[curr];
    }
    std::reverse(path.begin(), path.end());

    if (path.empty() || path[0] != source_node_idx) {
        shortest_time_hours = std::numeric_limits<float>::max();
        path.clear();
        return;
    }

    for (size_t i = 0; i < path.size() - 1; ++i) {
        int u_node = path[i];
        int v_node = path[i + 1];
        bool edge_found_in_adj = false;

        for (const auto& edge_data : adj_list[u_node]) {
            if (edge_data.node == v_node) { // Corrected: Use .node instead of .to
                path_length_km += edge_data.distance;

                bool is_walking_edge = false;
                // An edge is walking if it starts from the virtual source or ends at the virtual destination.
                // source_node_idx and destination_node_idx are the virtual nodes.
                if (u_node == source_node_idx || v_node == destination_node_idx) {
                    is_walking_edge = true;
                }

                if (is_walking_edge) {
                    total_walking_distance_km += edge_data.distance;
                }
                else {
                    total_vehicle_distance_km += edge_data.distance;
                }
                edge_found_in_adj = true;
                break;
            }
        }
        if (!edge_found_in_adj) {
            path_length_km = 0.0f; total_walking_distance_km = 0.0f; total_vehicle_distance_km = 0.0f;
            shortest_time_hours = std::numeric_limits<float>::max();
            path.clear();
            return;
        }
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

