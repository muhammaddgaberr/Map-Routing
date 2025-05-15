#pragma once
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include "Read_file.h"

using namespace std;

class Dijkstra_Algorithm
{
	vector<int> added_to_destination;

public :
	// every query has different source and destination co_ordinates
	// so we need to fix the adj_list for every query
	// assuming the source is node 0 and destination is node (N_nodes+1)

	void fixing_Adj_List
	(vector<vector<Read_file::Adj_nodes>>& adj_list, vector<pair<float, float>> &co_ordinates,
		float source_x, float source_y, float destination_x, float destination_y, float R);

	void Original_Adj_list(vector<vector<Read_file::Adj_nodes>>& adj_list);

	void dijkstra(vector<vector<Read_file::Adj_nodes>>& adj_list,
		int source, int destination, vector<int>& path, float& shortest_time, float& path_length,
		float& total_walking_distance, float& total_vehicle_distance);

};

