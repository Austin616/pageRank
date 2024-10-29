#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm> // for std::max
#include "graph.h"

// Function to compute the histogram of outdegrees
std::vector<int> computeOutdegreeHistogram(const CSRGraph &graph, bool log_scale)
{
    std::vector<int> outdegrees(graph.num_nodes, 0); // Store outdegree for each node

    // Calculate outdegrees
    for (int node = 0; node < graph.num_nodes; ++node)
    {
        int start = graph.row_ptr[node];
        int end = graph.row_ptr[node + 1];
        outdegrees[node] = end - start; // Count of outgoing edges for this node
    }

    if (!log_scale) // If log_scale is false, compute the histogram without bucketing
    {
        // Find the maximum outdegree to size the histogram
        int max_outdegree = *std::max_element(outdegrees.begin(), outdegrees.end());
        std::vector<int> histogram(max_outdegree + 1, 0); // Initialize histogram for outdegree counts

        // Populate histogram: count how many nodes have each outdegree
        for (int outdegree : outdegrees)
        {
            ++histogram[outdegree];
        }

        return histogram;
    }
    else // If log_scale is true, compute the histogram with logarithmic scaling
    {
        // Find the maximum outdegree
        int max_outdegree = *std::max_element(outdegrees.begin(), outdegrees.end());
        std::vector<int> histogram; // Initialize histogram for log scale

        // Populate histogram: count how many nodes fall into logarithmic bins
        for (int outdegree : outdegrees)
        {
            int bucket_index = outdegree > 0 ? static_cast<int>(std::log2(outdegree)) : 0;
            if (bucket_index >= histogram.size())
                histogram.resize(bucket_index + 1, 0);
            ++histogram[bucket_index];
        }

        return histogram;
    }
}

// Function to print the histogram to a file
void printHistogram(const std::vector<int> &histogram, const std::string &filename, bool log)
{
    std::ofstream file(filename);
    if (!file)
    {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
        return;
    }

    if (!log) // Print histogram without bucketing
    {
        for (size_t outdegree = 0; outdegree < histogram.size(); ++outdegree)
        {
            if (histogram[outdegree] > 0) // Only print non-zero outdegrees
                file << "Outdegree " << outdegree << ": " << histogram[outdegree] << " nodes\n";
        }
    }
    else // Print histogram with log scale bucketing
    {
        for (size_t i = 0; i < histogram.size(); ++i)
        {
            // Calculate the range for log scale
            int range_start = (i == 0) ? 1 : static_cast<int>(pow(2, i - 1)); // Start at 1 for bucket 0
            int range_end = static_cast<int>(pow(2, i)); // End at 2^i for bucket i
            
            if (histogram[i] > 0) // Only print non-zero buckets
                file << "Log bucket [" << range_start << ", " << range_end << "]: " << histogram[i] << " nodes\n";
        }
    }

    file.close();
}

