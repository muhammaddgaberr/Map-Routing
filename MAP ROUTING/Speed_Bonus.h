#pragma once
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include <queue>
#include "Read_file.h"


using namespace std;

class Speed_Bonus
{

    vector<int> added_to_destination;
    vector<pair<int, int>> has_been_deleted;

public :
    void fixing_Adj_List_bonus(
        vector<vector<Read_file::Adj_nodes_bonus>>& adj_list,
        vector<pair<double, double>>& co_ordinates,
        double source_x, double source_y,
        double destination_x, double destination_y, double R, int co_ordinates_size);
    void Dijkstra_edited(
        vector<vector<Read_file::Adj_nodes_bonus>> &adj_list,
        int source, int destination,
        vector<int>& path, double& shortest_time,
        double& path_length, double& total_walking_distance,
        double& total_vehicle_distance,
        double speed_interval_hours,
        int speed_count);

    void orig_list(vector<pair<int, int>>& has_been_deleted);

    void Original_Adj_list_bonus(vector<vector<Read_file::Adj_nodes_bonus>>& adj_list);
};

