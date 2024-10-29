#include "graph.h"
#include <iostream>
#include "pagerank.cpp"
#include "histogram.cpp"

bool boolVal;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }

    try
    {
        std::string input_file = "../txtfiles/" + std::string(argv[1]);
        std::cout << "Loading graph from: " << input_file << std::endl;

        // Set boolVal based on input file
        if (std::string(argv[1]) == "wiki.dimacs.txt")
        {
            boolVal = false;
        }
        else if (std::string(argv[1]) == "rmat15.dimacs.txt" || std::string(argv[1]) == "road-NY.dimacs.txt")
        {
            boolVal = true;
        }

        // Load graph from DIMACS format
        CSRGraph graph = readDIMACS(input_file);

        // Print graph back to DIMACS format for verification
        printDIMACS(graph, "output.dimacs");

        // Print node labels to a file
        printNodeLabels(graph, "node_labels.txt");

        // Compute and save PageRank results
        std::vector<double> page_rank = computePageRank(graph, "pageRank_results.txt");

        // Compute and save the out-degree histogram
        std::vector<int> histogram = computeOutdegreeHistogram(graph, boolVal);
        printHistogram(histogram, "out_degree_histogram.txt", boolVal);

        std::cout << "Graph loaded, PageRank and histogram computed, and output files created successfully.\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
