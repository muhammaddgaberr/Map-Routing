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
		double X_source, Y_source, X_destination, Y_destination, R;
	};

	struct Adj_nodes {
		int node;
		double distance;
		double time;
	};

	struct Adj_nodes_bonus
	{
		int node;
		double distance;
		vector<double> time;
	};

	// normal cases
	void read_map(string file_name, vector<pair<double, double>>& co_ordinates,
				  vector<vector<Read_file::Adj_nodes>>& adj_list, int &N_nodes);
	void read_queries(string file_name, vector<Query>& queries,int &N_queries);
	// bonus cases
	void read_map_bonus(string file_name, vector<pair<double, double>>& co_ordinates, vector<vector<Read_file::Adj_nodes_bonus>>& adj_list,
		double&speed_interval,int &speed_count,int &N_nodes);

};

