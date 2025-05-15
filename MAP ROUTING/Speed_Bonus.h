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

public :
    void fixing_Adj_List_bonus(
        vector<vector<Read_file::Adj_nodes_bonus>>& adj_list,
        vector<pair<float, float>>& co_ordinates,
        float source_x, float source_y,
        float destination_x, float destination_y, float R);
    
    void Billman_ford_bonus(
        vector<vector<Read_file::Adj_nodes_bonus>>& adj_list,
        int source, int destination,
        vector<int>& path, float& shortest_time,
        float& path_length, float& total_walking_distance,
        float& total_vehicle_distance,
        float speed_interval, int speed_count);

    void Original_Adj_list_bonus(vector<vector<Read_file::Adj_nodes_bonus>>& adj_list);


};

