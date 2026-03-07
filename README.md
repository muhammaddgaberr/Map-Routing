# Map Routing System using Dijkstra Algorithm

A graph-based routing system that calculates the shortest travel path between two locations using **Dijkstra's Algorithm**.

This project simulates a road network and finds the optimal route based on distance and road speeds.

---

## Problem Statement

Finding the shortest route between two locations is a fundamental problem in computer science and navigation systems.

Given a road network represented as a graph:

- Intersections are **nodes**
- Roads are **edges**
- Each edge has a **travel time based on distance and speed**

The goal is to compute the **fastest route between a source and a destination**.

This project implements an efficient routing system that solves this problem using **Dijkstra’s Algorithm**.

---

## Algorithm Used

The system uses **Dijkstra's Algorithm** to compute the shortest travel time.

Steps:

1. Represent the road network as a graph
2. Assign weights to edges based on travel time
3. Use a priority queue to explore the shortest paths
4. Update distances dynamically until the destination is reached

This guarantees the optimal path in graphs with non-negative edge weights.

---

## Visualization

The project also visualizes the routing process using **SFML**, showing the map and the computed shortest path.

---

## Example Output

![Output](images/output.png)

---

## Technologies Used

- C++
- Dijkstra Algorithm
- SFML (for visualization)

---

## Project Presentation

You can find the detailed project presentation here:

`docs/project-presentation.pptx`

