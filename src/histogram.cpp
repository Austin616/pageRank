#include <vector>
#include <iostream>
#include <fstream>
#include "graph.h"

// Function to compute the histogram of outgoing edges for each node
std::vector<int> computeOutgoingHistogram(const CSRGraph &graph)
{
    std::vector<int> histogram(graph.num_nodes, 0); // Initialize histogram with zeroes

    for (int node = 0; node < graph.num_nodes; ++node)
    {
        int start = graph.row_ptr[node];
        int end = graph.row_ptr[node + 1];
        histogram[node] = end - start; // Count of outgoing edges
    }

    return histogram;
}

// Function to print the histogram
void printHistogram(const std::vector<int> &histogram, const std::string &filename)
{
    std::ofstream file(filename);
    if (!file)
    {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
        return;
    }

    for (size_t i = 0; i < histogram.size(); ++i)
    {
        file << "Node " << i + 1 << ": " << histogram[i] << " outgoing edges\n"; // 1-indexed
    }

    file.close();
}
