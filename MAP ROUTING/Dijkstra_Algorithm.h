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
	(vector<vector<Read_file::Adj_nodes>>& adj_list, vector<pair<double, double>> &co_ordinates,
		double source_x, double source_y, double destination_x, double destination_y, double R, int co_ordinates_size);

	void Original_Adj_list(vector<vector<Read_file::Adj_nodes>>& adj_list);

	void dijkstra(vector<vector<Read_file::Adj_nodes>>& adj_list,
		int source, int destination, vector<int>& path, double& shortest_time, double& path_length,
		double& total_walking_distance, double& total_vehicle_distance, int adj_list_size);
};

