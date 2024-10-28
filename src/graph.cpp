// graph.c
#include "graph.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <unordered_map>
#include <tuple>

// Function to read graph from DIMACS file and build CSR
CSRGraph readDIMACS(const std::string &filename)
{
    CSRGraph graph;
    std::vector<std::tuple<int, int, int>> edges; // Store edges as (source, dest, weight)

    std::ifstream infile(filename);
    if (!infile.is_open())
    {
        throw std::runtime_error("Error: Cannot open file.");
    }

    std::string line;
    while (std::getline(infile, line))
    {
        std::istringstream iss(line);
        char line_type;
        iss >> line_type;

        if (line_type == 'c')
        {
            continue; // Skip comment lines
        }
        else if (line_type == 'p')
        {
            std::string format;
            iss >> format >> graph.num_nodes >> graph.num_edges;
            graph.row_ptr.resize(graph.num_nodes + 1, 0);  // Initialize row_ptr
            graph.node_labels.resize(graph.num_nodes + 1); // Initialize node labels (1-indexed)
            // Assign labels (1 to num_nodes)
            for (int i = 1; i <= graph.num_nodes; ++i)
            {
                graph.node_labels[i] = std::to_string(i); // Node labels from 1 to num_nodes
            }
        }
        else if (line_type == 'a')
        {
            int s, d, w;
            iss >> s >> d >> w;
            edges.emplace_back(s - 1, d - 1, w); // Nodes are zero-indexed in CSR
        }
    }
    infile.close();

    // Sort edges by source and destination nodes
    std::sort(edges.begin(), edges.end());

    // Populate col_idx, edge_weights, and row_ptr
    for (const auto &edge : edges)
    {
        int s, d, w;
        std::tie(s, d, w) = edge;
        graph.col_idx.push_back(d);      // Store destination node
        graph.edge_weights.push_back(w); // Store weight
        graph.row_ptr[s + 1]++;          // Increment the count for the source node
    }

    // Finalize row_ptr array
    for (int i = 1; i <= graph.num_nodes; i++)
    {
        graph.row_ptr[i] += graph.row_ptr[i - 1]; // Cumulative sum for row_ptr
    }

    return graph;
}



// Function to print CSR graph to DIMACS format
void printDIMACS(const CSRGraph &graph, const std::string &filename)
{
    std::ofstream outfile(filename);
    if (!outfile.is_open())
    {
        throw std::runtime_error("Error: Cannot open output file.");
    }

    outfile << "p sp " << graph.num_nodes << " " << graph.num_edges << "\n";
    for (int src = 0; src < graph.num_nodes; ++src)
    {
        for (int edge_idx = graph.row_ptr[src]; edge_idx < graph.row_ptr[src + 1]; ++edge_idx)
        {
            int dest = graph.col_idx[edge_idx];
            int weight = graph.edge_weights[edge_idx];
            outfile << "a " << src + 1 << " " << dest + 1 << " " << weight << "\n"; // Adjust back to 1-indexed
        }
    }
    outfile.close();
}

// Function to print node numbers and labels to a file
void printNodeLabels(const CSRGraph &graph, const std::string &filename)
{
    std::ofstream outfile(filename);
    if (!outfile.is_open())
    {
        throw std::runtime_error("Error: Cannot open output file.");
    }

    // Print header
    outfile << "Node Number\tNode Label\n"; // Using tab for better alignment

    // Print node labels 1-indexed
    for (int i = 1; i <= graph.num_nodes; ++i)
    {                                                           // Loop from 1 to num_nodes
        outfile << i << "\t\t" << graph.node_labels[i] << "\n"; // Ensure proper formatting
    }
    outfile.close();
}