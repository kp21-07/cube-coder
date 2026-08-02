#include "cube/cube.h"
#include "cube/move.h"
#include "solver/solve.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <fstream>
#include <string>

int main() {
    srand(time(NULL));
    CombinedPDBHeuristic h;
    
    std::cout << "Loading heuristic..." << std::endl;

    std::ofstream csv("benchmark/benchmark_results.csv");
    csv << "length,scramble,time_ms,nodes_expanded\n";

    for (int len = 12; len <= 13; len++) {
        double total_ms = 0;
        double max_ms = 0;
        double min_ms = 1e9;
        long total_nodes = 0;
        
        std::cout << "\n=== Benchmarking Length " << len << " ===" << std::endl;

        for (int i = 0; i < 100; i++) {
            Cube cube;
            vector<Move> scramble = cube.gen_random_scramble(len);
            cube.apply_moves(scramble);
            
            IDA_Solver solver(h);
            solver.solve(cube);
            
            double elapsed = solver.res.elapsed_ms;
            long nodes = solver.res.nodes_expanded;
            
            total_ms += elapsed;
            if (elapsed > max_ms) max_ms = elapsed;
            if (elapsed < min_ms) min_ms = elapsed;
            total_nodes += nodes;
            
            std::string scramble_str = to_string(scramble).data();
            
            std::cout << "Solve " << std::setw(3) << (i+1) << "/100 | "
                      << "Time: " << std::fixed << std::setw(8) << std::setprecision(3) << elapsed << " ms | "
                      << "Nodes: " << std::setw(8) << nodes << " | "
                      << "Scramble: " << scramble_str << std::endl;
                      
            csv << len << ",\"" << scramble_str << "\"," << elapsed << "," << nodes << "\n";
        }
        
        double avg_ms = total_ms / 100.0;
        std::cout << "--- Summary for Length " << len << " ---" << std::endl;
        std::cout << "Average Time : " << std::fixed << std::setprecision(3) << avg_ms << " ms" << std::endl;
        std::cout << "Min Time     : " << std::fixed << std::setprecision(3) << min_ms << " ms" << std::endl;
        std::cout << "Max Time     : " << std::fixed << std::setprecision(3) << max_ms << " ms" << std::endl;
        std::cout << "Avg Nodes Exp: " << (total_nodes / 100) << std::endl;
        
        csv << len << ",SUMMARY_AVG," << avg_ms << "," << (total_nodes / 100) << "\n";
        csv << len << ",SUMMARY_MIN," << min_ms << ",0\n";
        csv << len << ",SUMMARY_MAX," << max_ms << ",0\n";
    }
    
    csv.close();
    std::cout << "\nBenchmark complete. Results saved to benchmark/benchmark_results.csv." << std::endl;
    
    return 0;
}
