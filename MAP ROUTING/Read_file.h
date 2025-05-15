#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

class Read_file
{
	

public:
	struct Query {
		float X_source, Y_source, X_destination, Y_destination, R;
	};

	struct Adj_nodes {
		int node;
		float distance;
		float time;
	};

	struct Adj_nodes_bonus
	{
		int node;
		float distance;
		vector<float> time;
	};

	// normal cases
	void read_map(string file_name, vector<pair<float, float>>& co_ordinates, vector<vector<Read_file::Adj_nodes>>& adj_list);
	void read_queries(string file_name, vector<Query>& queries,int &N_queries);

	// bonus cases
	void read_map_bonus(string file_name, vector<pair<float, float>>& co_ordinates, vector<vector<Read_file::Adj_nodes_bonus>>& adj_list,
		float &speed_interval,int &speed_count);

};

