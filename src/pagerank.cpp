// pagerank.c
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "graph.cpp" // Include your graph structure and functions

const double DAMPING_FACTOR = 0.85;        // Damping factor for PageRank
const double CONVERGENCE_THRESHOLD = 1e-4; // Convergence threshold

// Function to compute PageRank using push-style algorithm
std::vector<double> computePageRank(const CSRGraph &graph, const std::string &filename)
{
    int num_nodes = graph.num_nodes;
    std::vector<double> page_rank(num_nodes, 1.0 / num_nodes); // Initialize PageRank values
    std::vector<double> new_page_rank(num_nodes, 0.0);         // New PageRank values for the next iteration
    bool converged = false;

    while (!converged)
    {
        converged = true; // Assume convergence until proven otherwise

        // Push PageRank values
        for (int src = 0; src < num_nodes; ++src)
        {
            double current_rank = page_rank[src];

            // Distribute PageRank to neighbors
            int start_idx = graph.row_ptr[src];
            int end_idx = graph.row_ptr[src + 1];
            int out_degree = end_idx - start_idx;

            // If the current node has outgoing edges
            if (out_degree > 0)
            {
                double distributed_rank = current_rank / out_degree;
                for (int edge_idx = start_idx; edge_idx < end_idx; ++edge_idx)
                {
                    int dest = graph.col_idx[edge_idx];
                    new_page_rank[dest] += distributed_rank; // Push rank to neighbors
                }
            }
        }

        // Apply damping factor and calculate new PageRank values
        for (int i = 0; i < num_nodes; ++i)
        {
            new_page_rank[i] = (1.0 - DAMPING_FACTOR) / num_nodes + DAMPING_FACTOR * new_page_rank[i];
        }

        // Check for convergence
        for (int i = 0; i < num_nodes; ++i)
        {
            if (fabs(new_page_rank[i] - page_rank[i]) > CONVERGENCE_THRESHOLD)
            {
                converged = false;
            }
            page_rank[i] = new_page_rank[i]; // Update PageRank for the next iteration
            new_page_rank[i] = 0.0;          // Reset for the next round
        }
    }

    // Scale PageRank values so that their sum is one
    double total_rank = 0.0;
    for (double rank : page_rank)
    {
        total_rank += rank;
    }
    for (double &rank : page_rank)
    {
        rank /= total_rank; // Normalize PageRank
    }

    // Write the PageRank values to the specified file
    std::ofstream output_file(filename);
    if (output_file.is_open())
    {
        for (int i = 0; i < num_nodes; ++i)
        {
            output_file << "Node " << (i + 1) << " PageRank: " << page_rank[i] << "\n";
        }
        output_file.close();
    }
    else
    {
        std::cerr << "Error: Could not open file " << filename << " for writing.\n";
    }

    return page_rank;
}
