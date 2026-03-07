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


// Normalize coordinates to fit the window while preserving aspect ratio, with zoom and pan
pair<double, double> normalize_coordinates(double x, double y, double min_x, double max_x,
    double min_y, double max_y, double window_width, double window_height, double zoom, double pan_x, double pan_y) {
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

    scale *= zoom; // Apply zoom factor
    double normalized_x = ((x - min_x) * scale) + offset_x + pan_x;
    double normalized_y = ((y - min_y) * scale) + offset_y + pan_y;

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
    cout << "[4] New Test Case (Sample)" << endl;
    cout << "[5] New Test Case (Medium)" << endl;
    cout << "[6] New Test Case (Hard)" << endl;
    cout << "Enter choice: ";
    int n, m;
    cin >> n;

    auto start = high_resolution_clock::now();

    if (n == 1) {
        cout << "[1] Map1" << endl << "[2] Map2" << endl << "[3] Map3" << endl << "[4] Map4" << endl << "[5] Map5" << endl;
        cout << "Enter choice: ";
        cin >> m;

        start = high_resolution_clock::now();

        string map_file = "[1] Sample Cases normal\\Input\\map" + to_string(m) + ".txt";
        string query_file = "[1] Sample Cases normal\\Input\\queries" + to_string(m) + ".txt";

        read_file.read_map(map_file, co_ordinates, adj_list, N_nodes);
        read_file.read_queries(query_file, queries, N_queries);
    }
    else if (n == 2) {
        read_file.read_map("[2] Medium Cases normal\\Input\\OLMap.txt", co_ordinates, adj_list, N_nodes);
        read_file.read_queries("[2] Medium Cases normal\\Input\\OLQueries.txt", queries, N_queries);
    }
    else if (n == 3) {
        read_file.read_map("[3] Large Cases normal\\Input\\SFMap.txt", co_ordinates, adj_list, N_nodes);
        read_file.read_queries("[3] Large Cases normal\\Input\\SFQueries.txt", queries, N_queries);
    }
    else if (n == 4) {
        read_file.read_map("[1] Sample Cases\\map8.txt", co_ordinates, adj_list, N_nodes);
        read_file.read_queries("[1] Sample Cases\\queries8.txt", queries, N_queries);
    }
    else if (n == 5) {
        read_file.read_map("[2] Medium Cases\\TGMap.txt", co_ordinates, adj_list, N_nodes);
        read_file.read_queries("[2] Medium Cases\\TGQueries.txt", queries, N_queries);
    }
    else if (n == 6) {
        read_file.read_map("[3] Large Cases\\NAMap.txt", co_ordinates, adj_list, N_nodes);
        read_file.read_queries("[3] Large Cases\\NAQueries.txt", queries, N_queries);
    }
    else {
        cout << "Invalid choice." << endl;
        return 0;
    }



    auto Time_without_IO_start = high_resolution_clock::now();

    vector<output> results(N_queries);
    Dijkstra_Algorithm dijkstra_algorithm;

    for (int i = 0; i < N_queries; i++) {
        dijkstra_algorithm.fixing_Adj_List(adj_list, co_ordinates, queries[i].X_source, queries[i].Y_source, queries[i].X_destination, queries[i].Y_destination, queries[i].R, N_nodes);
        dijkstra_algorithm.dijkstra(adj_list, 0, N_nodes + 1, results[i].Path, results[i].Shortest_time, results[i].Path_length, results[i].Total_walking_distance, results[i].Total_vehicle_distance, (N_nodes + 2));
        dijkstra_algorithm.Original_Adj_list(adj_list);
    }

    auto Time_without_IO_end = high_resolution_clock::now();
    chrono::duration<double, milli> Time_without_IO = Time_without_IO_end - Time_without_IO_start;

    string out_file;
    if (n == 1) {
        out_file = "Output\\output" + to_string(m) + ".txt";
    }
    else if (n == 2)
        out_file = "Output\\OLOutput.txt";
    else if (n == 3)
        out_file = "Output\\SFOutput.txt";
    else if (n == 4)
        out_file = "Output\\output8.txt";
    else if (n == 5)
        out_file = "Output\\TGOutput.txt";
    else if (n == 6)
        out_file = "Output\\NAOutput.txt";

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

    // GUI
    {
        sf::RenderWindow window(sf::VideoMode(1280, 720), "Dijkstra Pathfinding Visualization", sf::Style::Default);
        window.setFramerateLimit(60);

        bool is_fullscreen = false;

        sf::Font font;
        if (!font.loadFromFile("arial.ttf")) {
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

        sf::RectangleShape query_info_bg(sf::Vector2f(260, 75));
        query_info_bg.setPosition(10, 10);
        query_info_bg.setFillColor(sf::Color(0, 0, 0, 180));

        int current_query = 0;
        double zoom_level = 1.0; // Initial zoom level
        double pan_x = 0.0, pan_y = 0.0; // Initial pan offsets
        const double zoom_speed = 0.1; // Zoom sensitivity
        const double min_zoom = 1.0; // Minimum zoom level
        const double max_zoom = 100.0; // Maximum zoom level
        bool is_dragging = false;
        sf::Vector2f drag_start;

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
                    window.close();
                }
                if (event.type == sf::Event::KeyPressed) {
                    // Navigate queries with arrow keys
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
                // Zoom controls at mouse position
                if (event.type == sf::Event::MouseWheelScrolled) {
                    sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
                    double old_zoom = zoom_level;

                    // Update zoom level
                    if (event.mouseWheelScroll.delta > 0) {
                        zoom_level *= (1.0 + zoom_speed); // Zoom in
                    }
                    else if (event.mouseWheelScroll.delta < 0) {
                        zoom_level /= (1.0 + zoom_speed); // Zoom out
                    }

                    // Clamp zoom level
                    zoom_level = max(min_zoom, min(zoom_level, max_zoom));

                    // Adjust pan to keep the mouse's position fixed
                    double scale_factor = zoom_level / old_zoom;
                    pan_x = mouse_pos.x + scale_factor * (pan_x - mouse_pos.x);
                    pan_y = mouse_pos.y + scale_factor * (pan_y - mouse_pos.y);
                }
                // Pan controls with mouse drag
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    is_dragging = true;
                    drag_start = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
                }
                if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                    is_dragging = false;
                }
                if (event.type == sf::Event::MouseMoved && is_dragging) {
                    sf::Vector2f current_pos(event.mouseMove.x, event.mouseMove.y);
                    sf::Vector2f delta = current_pos - drag_start;
                    pan_x += delta.x;
                    pan_y += delta.y;
                    drag_start = current_pos;
                }
            }

            // Clamp panning to keep graph in view
            sf::Vector2u window_size = window.getSize();
            double window_width = window_size.x;
            double window_height = window_size.y;
            double map_width = (max_x - min_x) * zoom_level * (window_width - 100) / (max_x - min_x);
            double map_height = (max_y - min_y) * zoom_level * (window_height - 100) / (max_y - min_y);
            double margin = 100.0; // Allow some margin around the graph
            pan_x = max(-map_width - margin, min(pan_x, margin));
            pan_y = max(-map_height - margin, min(pan_y, margin));

            stringstream ss;
            ss << "Query " << (current_query + 1) << "/" << N_queries << "\n"
                << "Time: " << fixed << setprecision(2) << results[current_query].Shortest_time * 60 << " mins";
            query_info.setString(ss.str());

            window.clear(sf::Color::White);

            double current_window_width = static_cast<double>(window_size.x);
            double current_window_height = static_cast<double>(window_size.y);

            // Draw edges
            for (int u = 1; u < adj_list.size() - 1; ++u) {
                pair<double, double> coords1 = normalize_coordinates(co_ordinates[u - 1].first, co_ordinates[u - 1].second, min_x, max_x, min_y, max_y, current_window_width, current_window_height, zoom_level, pan_x, pan_y);
                double x1 = coords1.first;
                double y1 = coords1.second;
                for (const auto& neighbor : adj_list[u]) {
                    int v = neighbor.node;
                    if (v >= 1 && v < adj_list.size() - 1) {
                        pair<double, double> coords2 = normalize_coordinates(co_ordinates[v - 1].first, co_ordinates[v - 1].second, min_x, max_x, min_y, max_y, current_window_width, current_window_height, zoom_level, pan_x, pan_y);
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
                    pair<double, double> coords1 = normalize_coordinates(co_ordinates[u - 1].first, co_ordinates[u - 1].second, min_x, max_x, min_y, max_y, current_window_width, current_window_height, zoom_level, pan_x, pan_y);
                    double x1 = coords1.first;
                    double y1 = coords1.second;
                    pair<double, double> coords2 = normalize_coordinates(co_ordinates[v - 1].first, co_ordinates[v - 1].second, min_x, max_x, min_y, max_y, current_window_width, current_window_height, zoom_level, pan_x, pan_y);
                    double x2 = coords2.first;
                    double y2 = coords2.second;
                    draw_thick_line(window, sf::Vector2f(x1, y1), sf::Vector2f(x2, y2), 6.0f, sf::Color(255, 0, 0));
                }
            }

            // Draw source to first bus stop and last bus stop to destination
            if (results[current_query].Path.size() >= 3) {
                pair<double, double> source_coords = normalize_coordinates(queries[current_query].X_source, queries[current_query].Y_source, min_x, max_x, min_y, max_y, current_window_width, current_window_height, zoom_level, pan_x, pan_y);
                int first_bus = results[current_query].Path[1];
                pair<double, double> first_bus_coords = normalize_coordinates(co_ordinates[first_bus - 1].first, co_ordinates[first_bus - 1].second, min_x, max_x, min_y, max_y, current_window_width, current_window_height, zoom_level, pan_x, pan_y);
                draw_thick_line(window, sf::Vector2f(source_coords.first, source_coords.second), sf::Vector2f(first_bus_coords.first, first_bus_coords.second), 6.0f, sf::Color(59, 130, 246));

                int last_bus = results[current_query].Path[results[current_query].Path.size() - 2];
                pair<double, double> last_bus_coords = normalize_coordinates(co_ordinates[last_bus - 1].first, co_ordinates[last_bus - 1].second, min_x, max_x, min_y, max_y, current_window_width, current_window_height, zoom_level, pan_x, pan_y);
                pair<double, double> dest_coords = normalize_coordinates(queries[current_query].X_destination, queries[current_query].Y_destination, min_x, max_x, min_y, max_y, current_window_width, current_window_height, zoom_level, pan_x, pan_y);
                draw_thick_line(window, sf::Vector2f(last_bus_coords.first, last_bus_coords.second), sf::Vector2f(dest_coords.first, dest_coords.second), 6.0f, sf::Color(59, 130, 246));
            }

            // Draw nodes (only for small test case, n == 1)
            if (n == 1) {
                for (int i = 0; i < co_ordinates.size(); ++i) {
                    pair<double, double> coords = normalize_coordinates(co_ordinates[i].first, co_ordinates[i].second, min_x, max_x, min_y, max_y, current_window_width, current_window_height, zoom_level, pan_x, pan_y);
                    double x = coords.first;
                    double y = coords.second;
                    sf::CircleShape node(4); // All non-source/destination nodes are size 4
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
                }
            }


            // Draw source and destination nodes
            pair<double, double> source_coords = normalize_coordinates(queries[current_query].X_source, queries[current_query].Y_source, min_x, max_x, min_y, max_y, current_window_width, current_window_height, zoom_level, pan_x, pan_y);
            double src_x = source_coords.first;
            double src_y = source_coords.second;
            sf::CircleShape source_node(6); // Source node is larger (size 6)
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

            pair<double, double> dest_coords = normalize_coordinates(queries[current_query].X_destination, queries[current_query].Y_destination, min_x, max_x, min_y, max_y, current_window_width, current_window_height, zoom_level, pan_x, pan_y);
            double dest_x = dest_coords.first;
            double dest_y = dest_coords.second;
            sf::CircleShape dest_node(6); // Destination node is larger (size 6)
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

    return 0;
}