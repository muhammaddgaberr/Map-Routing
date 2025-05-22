#include "Read_file.h"
#include <sstream>

using namespace std;


void Read_file::read_map(string file_name, vector<pair<double, double>>& co_ordinates,
    vector<vector<Read_file::Adj_nodes>> &adj_list, int &N_nodes)
{
    ifstream file(file_name);

    if (!file.is_open())
    {
        cout << "Error opening file" << endl;
        exit(EXIT_FAILURE);
    }

    file >> N_nodes;

    
    co_ordinates.reserve(N_nodes);
    adj_list.resize(N_nodes + 2); 

    for (int i = 0; i < N_nodes; ++i)
    {
        int id;
        double x, y;
        file >> id >> x >> y;
        co_ordinates.push_back(make_pair(x, y));  
    }

    int N_edges;
    file >> N_edges;

    for (int i = 0; i < N_edges; ++i)
    {
        int node1, node2;
        double distance, speed;
        file >> node1 >> node2 >> distance >> speed;
        adj_list[node1 + 1].push_back({ node2 + 1, distance, distance / speed });
        adj_list[node2 + 1].push_back({ node1 + 1, distance, distance / speed });
    }

    file.close();
}


void Read_file::read_map_bonus(string file_name, vector<pair<double, double>>& co_ordinates,
    vector<vector<Read_file::Adj_nodes_bonus>>& adj_list, double&speed, int &speed_count, int &N_nodes)
{
    ifstream file(file_name);

    if (!file.is_open())
    {
        cout << "Error opening file" << endl;
        exit(EXIT_FAILURE);
    }

    file >> N_nodes;


    co_ordinates.reserve(N_nodes);
    adj_list.resize(N_nodes + 2);

    for (int i = 0; i < N_nodes; ++i)
    {
        int id;
        double x, y;
        file >> id >> x >> y;
        co_ordinates.push_back(make_pair(x, y));
    }

    
    int N_edges;
    double speed_interval;
    file >> N_edges >> speed_count >> speed_interval;
    speed = speed_interval;

    for (int i = 0; i < N_edges; ++i)
    {
        int node1, node2;
        double distance, speed;
        file >> node1 >> node2 >> distance;
		vector<double> times(speed_count);
		for (int j = 0; j < speed_count; j++)
		{
			file >> times[j];
            times[j] = distance / times[j];
		}
        adj_list[node1 + 1].push_back({ node2 + 1, distance, times });
        adj_list[node2 + 1].push_back({ node1 + 1, distance, times });
    }

    file.close();
}




void Read_file::read_queries(string file_name, vector<Query>& queries, int& N_queries)
{
    ifstream file(file_name);
    if (!file.is_open())
    {
        cout << "Error opening file" << endl;
        exit(EXIT_FAILURE);
    }

    file >> N_queries;

    
    queries.resize(N_queries);

    for (int i = 0; i < N_queries; ++i)
    {
        double X_source, Y_source, X_destination, Y_destination, R;
        file >> X_source >> Y_source >> X_destination >> Y_destination >> R;

        
        queries[i] = { X_source, Y_source, X_destination, Y_destination, R };
    }

    file.close();
}
