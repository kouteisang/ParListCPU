

#include "Graph/MultilayerGraph.h"
#include "Core/FCTreeBuilder.h"
#include "Core/FCTreeBuilderLeft.h"

#include "Core/FCTreeDFS.h"
#include "Core/FCTree.h"
#include "Core/FCTreeBuilderPathParallelByk.h"
#include "Core/FCTreeBuilderPathParallelBylmd.h"

#include "CoreParallel/FCCoreTree.cpp"
#include "CoreParallel/FCTreeBuilderCoreParallel.h"
#include "CoreParallel/FCTreeBuilderCoreParallelByK.h"

#include "header.h"
#include "Util/MemoryUtils.h"

int main(int argc, char* argv[]){

    string dataset = "example";
    string method = "serial";
    int order = 0;
    uint k = 0;
    uint lmd = 0;
    uint num_thread = 1;

    for(int i = 1; i < argc; i ++){
        string arg = argv[i];
        if (arg == "-d" && i + 1 < argc) {
            dataset = argv[++i];
        }
        if(arg == "-m" && i + 1 < argc){
            method = argv[++i];
        }
        if(arg == "-o" && i + 1 < argc){
            order = std::atoi(argv[++i]);
        }
        if(arg == "-k" && i + 1 < argc){
            k = std::atoi(argv[++i]);
        }
        if(arg == "-lmd" && i + 1 < argc){
            lmd = std::atoi(argv[++i]);
        }
        if(arg == "-num_thread" && i+1 < argc){
            num_thread = std::atoi(argv[++i]);
        }
    }

    if(dataset == ""){
        cout << "Use the default dataset example as the user did not specify the dataset" << endl;
    }
    if(method == ""){
        cout << "Use the FirmCore Tree Serial method as the user did not specify the method" << endl;
    }


    // load the dataset
    MultilayerGraph mg;
    // relative path is relative to the execute file
    mg.LoadFromFile("../dataset/"+dataset+"/");
    mg.SetGraphOrder(order);
    uint *order_list = mg.GetOrder();
    for(int i = 0; i < mg.getLayerNumber(); i ++){
        cout << order_list[i] << " ";;
    }
    cout << endl;
    mg.PrintStatistics();

    ll_uint *id2vtx = new ll_uint[mg.GetN()];
    mg.LoadId2VtxMap(id2vtx);


    if(method == "serial"){
        auto start_time = omp_get_wtime();
        // This method simply traverse the tree
        // FCTreeBuilder::Execute(mg);

        // This method get the result and store the result in the tree
        FCTree tree(1, 1, mg.GetN());
        FCTreeBuilder::Execute(mg, tree);
        auto end_time = omp_get_wtime(); 
        
        double elapsed_time = end_time - start_time;
        std::cout << "Serial Elapsed time: " << elapsed_time << " seconds\n";
        long double mem = GetPeakRSSInMB();
        cout << "mem = " << mem << " MB" << endl;

        // This part to get the output
        if(k != 0 && lmd != 0){
            k = uint(k);
            lmd = uint(lmd);
            coreNode* res_node = tree.getCoreByKAndLmdByLeft(tree.getNode(), k, lmd);
            cout << k << " " << lmd << endl;
            if(res_node == nullptr){
                cout << "No statisfy result" << endl;
            }else{
                tree.saveCoreToLocal(dataset, id2vtx, res_node, "serial");
            }
        }

    }

    if(method == "serialLeft"){

        auto start_time = omp_get_wtime(); 
        FCTree tree(1, 1, mg.GetN());
        FCTreeBuilderLeft::Execute(mg, tree);
        auto end_time = omp_get_wtime(); 
        
        double elapsed_time = end_time - start_time;
        std::cout << "Serial Elapsed time: " << elapsed_time << " seconds\n";
        long double mem = GetPeakRSSInMB();
        cout << "mem = " << mem << " MB" << endl; 
    }

    if(method == "dfs"){
        auto start_time = omp_get_wtime();
        FCTreeDFS::Execute(mg);
        auto end_time = omp_get_wtime(); 
        
        double elapsed_time = end_time - start_time;
        std::cout << "DFS Elapsed time: " << elapsed_time << " seconds\n";
        long double mem = GetPeakRSSInMB();
        cout << "mem = " << mem << " MB" << endl;
 
    }


    if(method == "pathk"){
        
        omp_set_num_threads(num_thread);
        auto start_time = omp_get_wtime();
        FCTree tree(1, 1, mg.GetN());
        FCTreeBuilderPathParallelByk::Execute(mg, tree);
        auto end_time = omp_get_wtime(); 
        long double mem = GetPeakRSSInMB();
        cout << "mem = " << mem << " MB" << endl;

        double elapsed_time = end_time - start_time;
        std::cout << "Pathk Parallel Elapsed time: " << elapsed_time << " seconds\n";


        if(k != 0 && lmd != 0){
            k = uint(k);
            lmd = uint(lmd);
            coreNode* res_node = tree.getCoreByKAndLmdByLeft(tree.getNode(), k, lmd);
            if(res_node == nullptr){
                cout << "No statisfy result" << endl;
            }else{
                tree.saveCoreToLocal(dataset, id2vtx, res_node, "pathk");
            }
        }


    }

    if(method == "pathlmd"){
        
        omp_set_num_threads(num_thread);
        auto start_time = omp_get_wtime();
        FCTree tree(1, 1, mg.GetN());
        FCTreeBuilderPathParallelBylmd::Execute(mg, tree);
        auto end_time = omp_get_wtime(); 
        
        double elapsed_time = end_time - start_time;
        std::cout << "Pathlmd Parallel Elapsed time: " << elapsed_time << " seconds\n";
        long double mem = GetPeakRSSInMB();
        cout << "mem = " << mem << " MB" << endl;

         // This part to get the output
        if(k != 0 && lmd != 0){
            k = uint(k);
            lmd = uint(lmd);
            coreNode* res_node = tree.getCoreByKAndLmdByRight(tree.getNode(), k, lmd);
            if(res_node == nullptr){
                cout << "No statisfy result" << endl;
            }else{
                tree.saveCoreToLocal(dataset, id2vtx, res_node, "pathlmd");
            }
        }
        
        // For test use only
        // for(int l = 1; l < mg.getLayerNumber(); l ++){
        //     uint num_valid = 0;
        //     tree.getNumValidRight(tree.getNode(), l, num_valid);
        //     cout << "when lmd = " << l << " k max = " << num_valid << endl;
        // }


    }

    if(method == "core"){
        omp_set_num_threads(num_thread);
        auto start_time = omp_get_wtime(); 
        FCCoreTree tree(1, 1, mg.GetN());
        coreNodeP* node = tree.getNode();
        FCTreeBuilderCoreParallel::Execute(mg, tree);
        auto end_time = omp_get_wtime(); 
        
        double elapsed_time = end_time - start_time;
        std::cout << "Path left Core Parallel Elapsed time: " << elapsed_time << " seconds\n";

        long double mem = GetPeakRSSInMB();
        cout << "mem = " << mem << " MB" << endl;
    }

    if(method == "corek"){
        omp_set_num_threads(num_thread);
        auto start_time = omp_get_wtime(); 
        FCCoreTree tree(1, 1, mg.GetN());
        coreNodeP* node = tree.getNode();
        FCTreeBuilderCoreParallelByK::Execute(mg, tree);
        auto end_time = omp_get_wtime(); 

        double elapsed_time = end_time - start_time;
        std::cout << "Path right Core Parallel Elapsed time: " << elapsed_time << " seconds\n";

        long double mem = GetPeakRSSInMB();
        cout << "mem = " << mem << " MB" << endl; 
    }

    if(method == "mix"){
        omp_set_num_threads(num_thread);
        omp_set_max_active_levels(2); // Enable nested parallelism
        auto start_time = omp_get_wtime(); 
        FCCoreTree tree(1, 1, mg.GetN());
        coreNodeP* node = tree.getNode();
        FCTreeBuilderCoreParallel::ExecuteMix(mg, tree);
        auto end_time = omp_get_wtime(); 
        
        double elapsed_time = end_time - start_time;
        std::cout << "Core Parallel Elapsed time: " << elapsed_time << " seconds\n";

        long double mem = GetPeakRSSInMB();
        cout << "mem = " << mem << " MB" << endl;
    }

    if(method == "mixr"){
        omp_set_num_threads(num_thread);
        omp_set_max_active_levels(2); // Enable nested parallelism
        auto start_time = omp_get_wtime(); 
        FCCoreTree tree(1, 1, mg.GetN());
        coreNodeP* node = tree.getNode();
        FCTreeBuilderCoreParallelByK::ExecuteMix(mg, tree);
        auto end_time = omp_get_wtime(); 
        
        double elapsed_time = end_time - start_time;
        std::cout << "Core Parallel Elapsed time: " << elapsed_time << " seconds\n";

        long double mem = GetPeakRSSInMB();
        cout << "mem = " << mem << " MB" << endl;
    }


}