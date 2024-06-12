#include <iostream>

#include "Graph/MultilayerGraph.h"
#include "Core/FCTreeBuilder.h"
#include <omp.h>

int main(){

    MultilayerGraph mg;
    mg.LoadFromFile("/Users/huangcheng/Documents/firmcore_baseline/dataset/obamainisrael/");
    mg.PrintStatistics();


    auto start_time = omp_get_wtime();
    FCTreeBuilder::Execute(mg);
    auto end_time = omp_get_wtime(); 
    
    double elapsed_time = end_time - start_time;
    std::cout << "Elapsed time: " << elapsed_time << " seconds\n";
}