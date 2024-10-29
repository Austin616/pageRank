#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <string>

// Structure representing a graph in CSR format
struct CSRGraph
{
    std::vector<int> row_ptr;             // Row pointer array
    std::vector<int> col_idx;             // Column index array
    std::vector<int> edge_weights;        // Edge weights array
    std::vector<std::string> node_labels; // Node labels
    int num_nodes;                        // Number of nodes
    int num_edges;                        // Number of edges
};

// Function to read a graph from a DIMACS file and build CSR representation
CSRGraph readDIMACS(const std::string &filename);

// Function to print CSR graph to DIMACS format
void printDIMACS(const CSRGraph &graph, const std::string &filename);

// Function to print node numbers and labels to a file
void printNodeLabels(const CSRGraph &graph, const std::string &filename);

#endif
