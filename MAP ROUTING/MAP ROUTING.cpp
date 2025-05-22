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
#include <sstream>
#include "Read_file.h"
#include "Dijkstra_Algorithm.h"
#include "Speed_Bonus.h"
#include <SFML/Graphics.hpp>

using namespace std;
using namespace chrono;

struct output {
    vector<int> Path = {};
    double Shortest_time = 0.0;
    double Path_length = 0.0;
    double Total_walking_distance = 0.0;
    double Total_vehicle_distance = 0.0;
};

// Normalize coordinates to fit the window while preserving aspect ratio
pair<double, double> normalize_coordinates(double x, double y, double min_x, double max_x,
    double min_y, double max_y, double window_width, double window_height) {
    double map_width = max_x - min_x;
    double map_height = max_y - min_y;
    double map_aspect = map_width / map_height;
    double window_aspect = window_width / window_height;

    double scale;
    double offset_x = 50, offset_y = 50;
    double draw_width = window_width - 100;
    double draw_height = window_height - 100;

    if (map_aspect > window_aspect) {
        scale = draw_width / map_width;
        draw_height = draw_width / map_aspect;
        offset_y = (window_height - draw_height) / 2;
    }
    else {
        scale = draw_height / map_height;
        draw_width = draw_height * map_aspect;
        offset_x = (window_width - draw_width) / 2;
    }

    double normalized_x = ((x - min_x) * scale) + offset_x;
    double normalized_y = ((y - min_y) * scale) + offset_y;

    return { normalized_x, normalized_y };
}
void draw_thick_line(sf::RenderWindow& window, sf::Vector2f start,
    sf::Vector2f end, float thickness, sf::Color color) {
    sf::Vector2f direction = end - start;
    float length = sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length == 0) return;
    sf::Vector2f unit_direction = direction / length;
    sf::Vector2f unit_perpendicular(-unit_direction.y, unit_direction.x);
    sf::Vector2f offset = (thickness / 2.0f) * unit_perpendicular;

    sf::VertexArray quad(sf::Quads, 4);
    quad[0].position = start - offset;
    quad[1].position = start + offset;
    quad[2].position = end + offset;
    quad[3].position = end - offset;

    for (int i = 0; i < 4; ++i) {
        quad[i].color = color;
    }

    window.draw(quad);
}

int main() {
    cout << "[1] Normal Test Cases" << endl;
    cout << "[2] BONUS Test Cases" << endl;
    cout << "Enter choice: ";
    int choice;
    cin >> choice;

    if (choice == 1) {
        ios::sync_with_stdio(false);
        cin.tie(nullptr);

        Read_file read_file;
        vector<pair<double, double>> co_ordinates;
        vector<vector<Read_file::Adj_nodes>> adj_list;
        int N_queries;
        int N_nodes;
        vector<Read_file::Query> queries;

        cout << "[1] Sample Test Case" << endl;
        cout << "[2] Medium Test Case" << endl;
        cout << "[3] Large Test Case" << endl;
        cout << "Enter choice: ";
        int n, m;
        cin >> n;

        auto start = high_resolution_clock::now();

        if (n == 1) {
            cout << "[1] Map1" << endl << "[2] Map2" << endl << "[3] Map3" << endl << "[4] Map4" << endl << "[5] Map5" << endl;
			cout << "Enter choice: ";
            cin >> m;

            start = high_resolution_clock::now();

            string map_file = "map" + to_string(m) + ".txt";

            string query_file = "queries" + to_string(m) + ".txt";

            read_file.read_map(map_file, co_ordinates, adj_list, N_nodes);
            read_file.read_queries(query_file, queries, N_queries);
        }
        else if (n == 2) {
            read_file.read_map("OLMap.txt", co_ordinates, adj_list, N_nodes);
            read_file.read_queries("OLQueries.txt", queries, N_queries);
        }
        else if (n == 3) {
            read_file.read_map("SFMap.txt", co_ordinates, adj_list, N_nodes);
            read_file.read_queries("SFQueries.txt", queries, N_queries);
        }
        else {
            cout << "Invalid choice." << endl;
            return 1;
        }
        
        auto Time_without_IO_start = high_resolution_clock::now();

        vector<output> results(N_queries);
        Dijkstra_Algorithm dijkstra_algorithm;

        for (int i = 0; i < N_queries; i++) {
            dijkstra_algorithm.fixing_Adj_List(adj_list, co_ordinates, queries[i].X_source, queries[i].Y_source, queries[i].X_destination, queries[i].Y_destination, queries[i].R, N_nodes);
            dijkstra_algorithm.dijkstra(adj_list, 0, N_nodes + 1, results[i].Path, results[i].Shortest_time, results[i].Path_length, results[i].Total_walking_distance, results[i].Total_vehicle_distance, (N_nodes + 2));
            dijkstra_algorithm.Original_Adj_list(adj_list);

            /*for (int j = 1; j < results[i].Path.size() - 1; ++j) {
                cout << results[i].Path[j] - 1 << " ";
            }
            cout << endl;
            cout << fixed << setprecision(2) << results[i].Shortest_time * 60 << " mins" << endl << endl;
            cout << fixed << setprecision(2) << results[i].Path_length << " km" << endl;
            cout << fixed << setprecision(2) << results[i].Total_walking_distance << " km" << endl;
            cout << fixed << setprecision(2) << results[i].Total_vehicle_distance << " km" << endl << endl;*/
        }

        auto Time_without_IO_end = high_resolution_clock::now();
        chrono::duration<double, milli> Time_without_IO = Time_without_IO_end - Time_without_IO_start;

        string out_file;
        if (n == 1)
        {
            out_file = "output" + to_string(m) + ".txt";
        }
        else if (n == 2)
            out_file = "OLOutput.txt";
        else if (n == 3)
            out_file = "SFOutput.txt";

        ofstream output_file(out_file);
        
        for (int i = 0; i < N_queries; i++) {
			int path_size = results[i].Path.size();
            for (int j = 1; j < path_size - 1; ++j) {
                output_file << results[i].Path[j] - 1 << " ";
            }
            
            output_file << endl;
            output_file << fixed << setprecision(2) << results[i].Shortest_time * 60 << " mins" << endl;
            output_file << fixed << setprecision(2) << results[i].Path_length << " km" << endl;
            output_file << fixed << setprecision(2) << results[i].Total_walking_distance << " km" << endl;
            output_file << fixed << setprecision(2) << results[i].Total_vehicle_distance << " km" << endl << endl;
        }

        output_file << fixed << setprecision(0) << Time_without_IO.count() << " ms" << endl;
        auto end = high_resolution_clock::now();
        chrono::duration<double, milli> duration = end - start;
        output_file << endl << duration.count() << " ms" << endl;

        output_file.close();

        cout << endl << duration.count() << " ms" << endl;

        //GUI
        {
            sf::RenderWindow window(sf::VideoMode(1280, 720), "Dijkstra Pathfinding Visualization", sf::Style::Default);
            window.setFramerateLimit(60);

            bool is_fullscreen = false;

            sf::Font font;
            if (!font.loadFromFile("C:\\Users\\DELL\\OneDrive\\Desktop\\MAP ROUTING2\\MAP ROUTING\\MAP ROUTING\\x64\\Release\\arial.ttf")) {
                cout << "Error: Could not load arial.ttf. Ensure the file is in the project directory." << endl;
                return 1;
            }

            if (co_ordinates.empty()) {
                cout << "Error: No coordinates loaded from map file!" << endl;
                return 1;
            }

            double min_x = co_ordinates[0].first, max_x = co_ordinates[0].first;
            double min_y = co_ordinates[0].second, max_y = co_ordinates[0].second;
            for (const auto& coord : co_ordinates) {
                min_x = min(min_x, coord.first);
                max_x = max(max_x, coord.first);
                min_y = min(min_y, coord.second);
                max_y = max(max_y, coord.second);
            }

            sf::Text query_info;
            query_info.setFont(font);
            query_info.setCharacterSize(24);
            query_info.setFillColor(sf::Color::White);
            query_info.setPosition(20, 20);
            query_info.setStyle(sf::Text::Bold);

            sf::RectangleShape query_info_bg(sf::Vector2f(250, 60));
            query_info_bg.setPosition(10, 10);
            query_info_bg.setFillColor(sf::Color(0, 0, 0, 180));

            int current_query = 0;

            while (window.isOpen()) {
                sf::Event event;
                while (window.pollEvent(event)) {
                    if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
                        window.close();
                    }
                    if (event.type == sf::Event::KeyPressed) {
                        if (event.key.code == sf::Keyboard::Right && current_query < N_queries - 1) {
                            current_query++;
                        }
                        if (event.key.code == sf::Keyboard::Left && current_query > 0) {
                            current_query--;
                        }
                        if (event.key.code == sf::Keyboard::F11) {
                            if (!is_fullscreen) {
                                window.create(sf::VideoMode::getDesktopMode(), "Dijkstra Pathfinding Visualization", sf::Style::Fullscreen);
                                is_fullscreen = true;
                            }
                            else {
                                window.create(sf::VideoMode(1280, 720), "Dijkstra Pathfinding Visualization", sf::Style::Default);
                                is_fullscreen = false;
                            }
                            window.setFramerateLimit(60);
                        }
                    }
                }

                stringstream ss;
                ss << "Query " << (current_query + 1) << "/" << N_queries << "\n"
                    << "Time: " << fixed << setprecision(2) << results[current_query].Shortest_time * 60 << " mins";
                query_info.setString(ss.str());

                window.clear(sf::Color::White);

                sf::Vector2u window_size = window.getSize();
                double current_window_width = static_cast<double>(window_size.x);
                double current_window_height = static_cast<double>(window_size.y);

                // Draw edges
                for (int u = 1; u < adj_list.size() - 1; ++u) {
                    pair<double, double> coords1 = normalize_coordinates(co_ordinates[u - 1].first, co_ordinates[u - 1].second, min_x, max_x, min_y, max_y, current_window_width, current_window_height);
                    double x1 = coords1.first;
                    double y1 = coords1.second;
                    for (const auto& neighbor : adj_list[u]) {
                        int v = neighbor.node;
                        if (v >= 1 && v < adj_list.size() - 1) {
                            pair<double, double> coords2 = normalize_coordinates(co_ordinates[v - 1].first, co_ordinates[v - 1].second, min_x, max_x, min_y, max_y, current_window_width, current_window_height);
                            double x2 = coords2.first;
                            float y2 = coords2.second;
                            sf::Vertex line[] = {
                                sf::Vertex(sf::Vector2f(x1, y1), sf::Color(156, 163, 175)),
                                sf::Vertex(sf::Vector2f(x2, y2), sf::Color(156, 163, 175))
                            };
                            window.draw(line, 2, sf::Lines);
                        }
                    }
                }

                // Draw shortest path
                for (size_t i = 1; i < results[current_query].Path.size() - 1; ++i) {
                    int u = results[current_query].Path[i];
                    int v = results[current_query].Path[i + 1];
                    if (u >= 1 && u < adj_list.size() - 1 && v >= 1 && v < adj_list.size() - 1) {
                        pair<double, double> coords1 = normalize_coordinates(co_ordinates[u - 1].first, co_ordinates[u - 1].second, min_x, max_x, min_y, max_y, current_window_width, current_window_height);
                        double x1 = coords1.first;
                        double y1 = coords1.second;
                        pair<double, double> coords2 = normalize_coordinates(co_ordinates[v - 1].first, co_ordinates[v - 1].second, min_x, max_x, min_y, max_y, current_window_width, current_window_height);
                        double x2 = coords2.first;
                        double y2 = coords2.second;
                        draw_thick_line(window, sf::Vector2f(x1, y1), sf::Vector2f(x2, y2), 6.0f, sf::Color(255, 0, 0));
                    }
                }

                // Draw source to first bus stop and last bus stop to destination
                if (results[current_query].Path.size() >= 3) {
                    pair<double, double> source_coords = normalize_coordinates(queries[current_query].X_source, queries[current_query].Y_source, min_x, max_x, min_y, max_y, current_window_width, current_window_height);
                    int first_bus = results[current_query].Path[1];
                    pair<double, double> first_bus_coords = normalize_coordinates(co_ordinates[first_bus - 1].first, co_ordinates[first_bus - 1].second, min_x, max_x, min_y, max_y, current_window_width, current_window_height);
                    draw_thick_line(window, sf::Vector2f(source_coords.first, source_coords.second), sf::Vector2f(first_bus_coords.first, first_bus_coords.second), 6.0f, sf::Color(59, 130, 246));

                    int last_bus = results[current_query].Path[results[current_query].Path.size() - 2];
                    pair<double, double> last_bus_coords = normalize_coordinates(co_ordinates[last_bus - 1].first, co_ordinates[last_bus - 1].second, min_x, max_x, min_y, max_y, current_window_width, current_window_height);
                    pair<double, double> dest_coords = normalize_coordinates(queries[current_query].X_destination, queries[current_query].Y_destination, min_x, max_x, min_y, max_y, current_window_width, current_window_height);
                    draw_thick_line(window, sf::Vector2f(last_bus_coords.first, last_bus_coords.second), sf::Vector2f(dest_coords.first, dest_coords.second), 6.0f, sf::Color(59, 130, 246));
                }

                // Draw nodes (only for small test case, n == 1)
                if (n == 1) {
                    for (int i = 0; i < co_ordinates.size(); ++i) {
                        pair<double, double> coords = normalize_coordinates(co_ordinates[i].first, co_ordinates[i].second, min_x, max_x, min_y, max_y, current_window_width, current_window_height);
                        double x = coords.first;
                        double y = coords.second;
                        sf::CircleShape node(4);
                        bool is_path_node = false;
                        for (int path_node : results[current_query].Path) {
                            if (i + 1 == path_node) {
                                is_path_node = true;
                                break;
                            }
                        }
                        if (is_path_node) {
                            node.setFillColor(sf::Color(34, 197, 94));
                        }
                        else {
                            node.setFillColor(sf::Color(34, 197, 94, 100));
                        }
                        node.setOutlineColor(sf::Color::Black);
                        node.setOutlineThickness(1);
                        node.setPosition(x - 4, y - 4);
                        window.draw(node);

                        sf::Text node_label;
                        node_label.setFont(font);
                        node_label.setString(to_string(i));
                        node_label.setCharacterSize(10);
                        node_label.setFillColor(sf::Color::Black);
                        node_label.setPosition(x + 8, y - 8);
                        window.draw(node_label);
                    }
                }

                // Draw bus stops on the path (excluding source and destination)
                for (size_t i = 1; i < results[current_query].Path.size() - 1; ++i) {
                    int bus_node = results[current_query].Path[i];
                    if (bus_node >= 1 && bus_node < adj_list.size() - 1) {
                        pair<double, double> coords = normalize_coordinates(co_ordinates[bus_node - 1].first, co_ordinates[bus_node - 1].second, min_x, max_x, min_y, max_y, current_window_width, current_window_height);
                        double x = coords.first;
                        double y = coords.second;
                        sf::CircleShape bus_node_circle(4);
                        bus_node_circle.setFillColor(sf::Color(34, 197, 94));
                        bus_node_circle.setOutlineColor(sf::Color::Black);
                        bus_node_circle.setOutlineThickness(1);
                        bus_node_circle.setPosition(x - 4, y - 4);
                        window.draw(bus_node_circle);

                        sf::Text bus_label;
                        bus_label.setFont(font);
                        bus_label.setString(to_string(bus_node - 1));
                        bus_label.setCharacterSize(10);
                        bus_label.setFillColor(sf::Color::Black);
                        bus_label.setPosition(x + 8, y - 8);
                        window.draw(bus_label);
                    }
                }

                // Draw source and destination nodes
                pair<double, double> source_coords = normalize_coordinates(queries[current_query].X_source, queries[current_query].Y_source, min_x, max_x, min_y, max_y, current_window_width, current_window_height);
                double src_x = source_coords.first;
                double src_y = source_coords.second;
                sf::CircleShape source_node(6);
                source_node.setFillColor(sf::Color(59, 130, 246));
                source_node.setOutlineColor(sf::Color::Black);
                source_node.setOutlineThickness(1);
                source_node.setPosition(src_x - 6, src_y - 6);
                window.draw(source_node);

                sf::Text source_label;
                source_label.setFont(font);
                source_label.setString("S");
                source_label.setCharacterSize(12);
                source_label.setFillColor(sf::Color::Black);
                source_label.setPosition(src_x + 10, src_y - 10);
                window.draw(source_label);

                pair<double, double> dest_coords = normalize_coordinates(queries[current_query].X_destination, queries[current_query].Y_destination, min_x, max_x, min_y, max_y, current_window_width, current_window_height);
                double dest_x = dest_coords.first;
                double dest_y = dest_coords.second;
                sf::CircleShape dest_node(6);
                dest_node.setFillColor(sf::Color(234, 179, 8));
                dest_node.setOutlineColor(sf::Color::Black);
                dest_node.setOutlineThickness(1);
                dest_node.setPosition(dest_x - 6, dest_y - 6);
                window.draw(dest_node);

                sf::Text dest_label;
                dest_label.setFont(font);
                dest_label.setString("D");
                dest_label.setCharacterSize(12);
                dest_label.setFillColor(sf::Color::Black);
                dest_label.setPosition(dest_x + 10, dest_y - 10);
                window.draw(dest_label);

                window.draw(query_info_bg);
                window.draw(query_info);

                window.display();
            }
        }


    }
    else if (choice == 2)
    {
        ios::sync_with_stdio(false);
        cin.tie(nullptr);

        Read_file read_file;
        vector<pair<double, double>> co_ordinates;
        vector<vector<Read_file::Adj_nodes_bonus>> adj_list;
        int N_queries;
        int N_nodes;
        vector<Read_file::Query> queries;
        cout << "[1] Sample Test Case" << endl;
        cout << "[2] Medium Test Case" << endl;
        cout << "Enter choice: ";
        int n;
        double speed_interval;
        int speed_count;
        cin >> n;
        auto program_start = high_resolution_clock::now();
        if (n == 1) {
            read_file.read_map_bonus("map1B.txt", co_ordinates, adj_list, speed_interval, speed_count, N_nodes);
            read_file.read_queries("queries1B.txt", queries, N_queries);
        }
        else if (n == 2) {
            read_file.read_map_bonus("OLMapB.txt", co_ordinates, adj_list, speed_interval, speed_count, N_nodes);
            read_file.read_queries("OLQueriesB.txt", queries, N_queries);
        }
        else {
            cout << "Invalid choice." << endl;
        }

        speed_interval /= 60.0f;

        //cout << speed_interval << endl << endl;

        auto query_start = high_resolution_clock::now();

        vector<output> results(N_queries);

        Speed_Bonus Algo;

        for (int i = 0; i < N_queries; i++)
        {
            vector<vector<Read_file::Adj_nodes_bonus>> adj_list_temp = adj_list;
            Algo.fixing_Adj_List_bonus(adj_list_temp, co_ordinates, queries[i].X_source, queries[i].Y_source, queries[i].X_destination, queries[i].Y_destination, queries[i].R, N_nodes);
            Algo.Dijkstra_edited(adj_list_temp, 0, N_nodes + 1,
                results[i].Path, results[i].Shortest_time, results[i].Path_length,
                results[i].Total_walking_distance, results[i].Total_vehicle_distance,
                speed_interval, speed_count);
            //Algo.Original_Adj_list_bonus(adj_list);


            /*for (int j = 1; j < results[i].Path.size() - 1; ++j) {
                cout << results[i].Path[j] - 1 << " ";
            }
            cout << endl;
            cout << fixed << setprecision(2) << results[i].Shortest_time * 60 << " mins" << endl << endl;
            cout << fixed << setprecision(2) << results[i].Path_length << " km" << endl;
            cout << fixed << setprecision(2) << results[i].Total_walking_distance << " km" << endl;
            cout << fixed << setprecision(2) << results[i].Total_vehicle_distance << " km" << endl << endl;*/
        }

        auto query_end = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> query_duration = query_end - query_start;

        string out_file;
        if (n == 1)
            out_file = "output1B.txt";
        else if (n == 2)
            out_file = "OLOutputB.txt";

        ofstream output_file(out_file);

        for (int i = 0; i < N_queries; i++)
        {
            int Path_size = results[i].Path.size();
            for (int j = 1; j < Path_size - 1; ++j) {
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
    else
    {
        cout << "Invalid choice !";
    }

    return 0;
}
