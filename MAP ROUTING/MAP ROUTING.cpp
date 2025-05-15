#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <queue>
#include <cmath>
#include <limits>
#include <fstream>
#include <algorithm>
#include <map>
#include "Read_file.h"
#include "Dijkstra_Algorithm.h"
#include "Speed_Bonus.h"

using namespace std;
using namespace chrono;

struct output
{
    vector<int> Path = {};
    float Shortest_time = 0.0;
    float Path_length = 0.0;
    float Total_walking_distance = 0.0;
    float Total_vehicle_distance = 0.0;
};


int main()
{
    cout << "[1] Normal Test Cases" << endl;
    cout << "[2] BONUS Test Cases" << endl;
    cout << "Enter choice: ";
    int choice;
    cin >> choice;
    if (choice == 1) {
        ios::sync_with_stdio(false);
        cin.tie(nullptr);
        
        Read_file read_file;
        vector<pair<float, float>> co_ordinates;
        vector<vector<Read_file::Adj_nodes>> adj_list;
        int N_queries;
        vector<Read_file::Query> queries;

        cout << "[1] Sample Test Case" << endl;
        cout << "[2] Medium Test Case" << endl;
        cout << "[3] Large Test Case" << endl;
        cout << "Enter choice: ";
        int n;
        cin >> n;


        auto program_start = high_resolution_clock::now(); 

        if (n == 1) {
            read_file.read_map("map5.txt", co_ordinates, adj_list);
            read_file.read_queries("queries5.txt", queries, N_queries);
        }
        else if (n == 2) {
            read_file.read_map("OLMap.txt", co_ordinates, adj_list);
            read_file.read_queries("OLQueries.txt", queries, N_queries);
        }
        else if (n == 3) {
            read_file.read_map("SFMap.txt", co_ordinates, adj_list);
            read_file.read_queries("SFQueries.txt", queries, N_queries);
        }
        else {
            cout << "Invalid choice." << endl;
        }


        auto query_start = high_resolution_clock::now(); 

        vector<output> results(N_queries);  

        Dijkstra_Algorithm dijkstra_algorithm;

       
        for (int i = 0; i < N_queries; i++)
        {
            dijkstra_algorithm.fixing_Adj_List(adj_list, co_ordinates, queries[i].X_source, queries[i].Y_source, queries[i].X_destination, queries[i].Y_destination, queries[i].R);
            dijkstra_algorithm.dijkstra(adj_list, 0, adj_list.size() - 1, results[i].Path, results[i].Shortest_time, results[i].Path_length, results[i].Total_walking_distance, results[i].Total_vehicle_distance);
            dijkstra_algorithm.Original_Adj_list(adj_list);

            //    for (int j = 1; j < results[i].Path.size() - 1; ++j) {
            //        cout << results[i].Path[j] - 1 << " "; // Adjust for real node indexing
            //    }
            //cout << endl;
            //cout << fixed << setprecision(2) << results[i].Shortest_time * 60 << " mins" << endl;
            //cout << fixed << setprecision(2) << results[i].Path_length << " km" << endl;
            //cout << fixed << setprecision(2) << results[i].Total_walking_distance << " km" << endl;
            //cout << fixed << setprecision(2) << results[i].Total_vehicle_distance << " km" << endl << endl;

        }



        auto query_end = chrono::high_resolution_clock::now(); 
        chrono::duration<double, milli> query_duration = query_end - query_start;

        ofstream output_file("output.txt");
        for (int i = 0; i < N_queries; i++)
        {
            for (int j = 1; j < results[i].Path.size() - 1; ++j) {
                output_file << results[i].Path[j] - 1 << " ";
            }
            output_file << endl;
            output_file << fixed << setprecision(2) << results[i].Shortest_time * 60 << " mins" << endl;
            output_file << fixed << setprecision(2) << results[i].Path_length << " km" << endl;
            output_file << fixed << setprecision(2) << results[i].Total_walking_distance << " km" << endl;
            output_file << fixed << setprecision(2) << results[i].Total_vehicle_distance << " km" << endl << endl;
        }

        output_file << fixed << setprecision(0) << query_duration.count() << " ms" << endl;
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> duration = end - program_start;
        output_file << endl << duration.count() << " ms" << endl;

        output_file.close();

        cout << endl << duration.count() << " ms" << endl;




    }
    else if (choice == 2)
    {
        ios::sync_with_stdio(false);
        cin.tie(nullptr);

        Read_file read_file;
        vector<pair<float, float>> co_ordinates;
        vector<vector<Read_file::Adj_nodes_bonus>> adj_list;
        int N_queries;
        vector<Read_file::Query> queries;
        cout << "[1] Sample Test Case" << endl;
        cout << "[2] Medium Test Case" << endl;
        cout << "Enter choice: ";
        int n;
        float speed_interval;
        int speed_count;
        cin >> n;
        auto program_start = high_resolution_clock::now(); 
        if (n == 1) {
            read_file.read_map_bonus("map1B.txt", co_ordinates, adj_list, speed_interval, speed_count);
            read_file.read_queries("queries1.txt", queries, N_queries);
        }
        else if (n == 2) {
            read_file.read_map_bonus("OLMapB.txt", co_ordinates, adj_list, speed_interval, speed_count);
            read_file.read_queries("OLQueriesB.txt", queries, N_queries);
        }
        else {
            cout << "Invalid choice." << endl;
        }

        speed_interval /= 60.0;

        //cout << speed_interval << endl << endl;

        auto query_start = high_resolution_clock::now(); 

        vector<output> results(N_queries);  

        Speed_Bonus Algo;


        for (int i = 0; i < N_queries; i++)
        {
            Algo.fixing_Adj_List_bonus(adj_list, co_ordinates, queries[i].X_source, queries[i].Y_source, queries[i].X_destination, queries[i].Y_destination, queries[i].R);
            Algo.Billman_ford_bonus(adj_list, 0, adj_list.size() - 1, results[i].Path, results[i].Shortest_time, results[i].Path_length, results[i].Total_walking_distance, results[i].Total_vehicle_distance, speed_interval, speed_count);
            Algo.Original_Adj_list_bonus(adj_list);
            for (int j = 1; j < results[i].Path.size() - 1; ++j) {
                cout << results[i].Path[j] - 1 << " "; 
            }
            cout << endl;
            cout << fixed << setprecision(2) << results[i].Shortest_time * 60 << " mins" << endl;
            cout << fixed << setprecision(2) << results[i].Path_length << " km" << endl;
            cout << fixed << setprecision(2) << results[i].Total_walking_distance << " km" << endl;
            cout << fixed << setprecision(2) << results[i].Total_vehicle_distance << " km" << endl << endl;

        }




    }

    return 0;
}