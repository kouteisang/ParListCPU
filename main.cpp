

#include "Graph/MultilayerGraph.h"
#include "Core/FCTree.h"
#include "header.h"
#include "Util/MemoryUtils.h"
#include "Util/KLmdUtils.h"
#include "Util/CheckMLCDTime.h"
#include "Util/CoreIndexTime.h"

// Naive
#include "Core/FCTreeDFS.h"

// serial
#include "Core/FCTreeBuilderRight.h"
#include "Core/FCTreeBuilderLeft.h"

// new path level
#include "Core/FCPathLevelLeft.h"
#include "Core/FCPathLevelRight.h"

// path level
#include "Core/FCTreeBuilderPathParallelByk.h"
#include "Core/FCTreeBuilderPathParallelBylmd.h"

// core level
#include "CoreParallel/FCCoreTree.cpp"
#include "CoreParallel/FCTreeBuilderCoreParallel.h"
#include "CoreParallel/FCTreeBuilderCoreParallelByK.h"

// A new test
#include "CoreParallelNew/CoreParallelNew.h"

// A new algorithm
#include "CoreIndexParallel/CoreIndex.h"



uint* getLmd(){

}

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

    if(method == "naive"){
        auto start_time = omp_get_wtime();
        FCTreeDFS::Execute(mg);
        auto end_time = omp_get_wtime(); 
        
        double elapsed_time = end_time - start_time;
        std::cout << "DFS Elapsed time: " << elapsed_time << " seconds\n";
        long double mem = GetPeakRSSInMB();
        cout << "mem = " << mem << " MB" << endl;
 
    }

    if(method == "OptimizedLeft"){

        auto start_time = omp_get_wtime(); 
        FCTree tree(1, 1, mg.GetN());
        FCTreeBuilderLeft::Execute(mg, tree);
        // FCTreeBuilderLeft::Execute(mg);
        auto end_time = omp_get_wtime(); 
        
        double elapsed_time = end_time - start_time;
        std::cout << "Serial Elapsed time: " << elapsed_time << " seconds\n";
        long double mem = GetPeakRSSInMB();
        cout << "mem = " << mem << " MB" << endl; 
    }

    if(method == "OptimizedRight"){
        auto start_time = omp_get_wtime();
        // This method simply traverse the tree
        // FCTreeBuilder::Execute(mg);

        // This method get the result and store the result in the tree
        FCTree tree(1, 1, mg.GetN());
        FCTreeBuilderRight::Execute(mg, tree);
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

    if(method == "PathParallel"){
        omp_set_num_threads(num_thread);
        auto start_time = omp_get_wtime();
        FCTree tree(1, 1, mg.GetN());
        FCPathLevelLeft::Execute(mg, tree);
        auto end_time = omp_get_wtime(); 
        
        double elapsed_time = end_time - start_time;
        std::cout << "PathLevelLeft Elapsed time: " << elapsed_time << " seconds\n";
        long double mem = GetPeakRSSInMB();
        cout << "mem = " << mem << " MB" << endl;



         // This part to get the output
        std::vector<int> ks = getK("/home/cheng/fctree/dataset/homo/k.txt");
        std::vector<int> lmds = getLmd("/home/cheng/fctree/dataset/homo/lmd.txt");

   
        auto start_time_query = omp_get_wtime();
        for(int i = 0; i < ks.size(); i ++){
            uint k = uint(ks[i]);
            uint lmd = uint(lmds[i]); 
            coreNode* res_node = tree.getCoreByKAndLmdByRight(tree.getNode(), k, lmd);
            // cout << res_node->k << " " << res_node->lmd << " " << res_node->length << endl;
        }
        auto end_time_query = omp_get_wtime();
        auto total_time_query = end_time_query - start_time_query;
        cout << "total time query = "<< total_time_query << endl;
        cout << "average time query = "<< total_time_query/ks.size() << endl;
        
        // if(k != 0 && lmd != 0){
        //     k = uint(k);
        //     lmd = uint(lmd);
        //     coreNode* res_node = tree.getCoreByKAndLmdByRight(tree.getNode(), k, lmd);
        //     if(res_node == nullptr){
        //         cout << "No statisfy result" << endl;
        //     }else{
        //         tree.saveCoreToLocal(dataset, id2vtx, res_node, "pathlmd");
        //     }
        // }
    }

    if(method == "CoreParallel"){
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

    if(method == "CoreIndex"){
        omp_set_num_threads(num_thread);
        auto start_time = omp_get_wtime();
        CoreIndex::Execute(mg, id2vtx);
        auto end_time = omp_get_wtime(); 
        double elapsed_time = end_time - start_time;
        std::cout << "CoreIndex Parallel Elapsed time: " << elapsed_time << " seconds\n"; 

    }

    if(method == "wds"){
        FCTree tree(1, 1, mg.GetN());
        FCPathLevelLeft::Execute(mg, tree);
    
        float beta = 2.0;
        float maximum_density = 0.0f;

        // tree.WeightDenestSubgraph();

        // tree.WeightDenestSubgraph(tree.getNode(), mg, beta, maximum_density);
        uint count = 0;
        tree.traversal(tree.getNode(), count);
    }

    // if(method == "CoreParallelNew"){

    //     omp_set_num_threads(num_thread);
    //     auto start_time = omp_get_wtime();  
    //     FCCoreTree tree(1, 1, mg.GetN());
    //     coreNodeP* node = tree.getNode();
    //     CoreParallelNew::Execute(mg,tree);
    //     auto end_time = omp_get_wtime(); 

         
    //     double elapsed_time = end_time - start_time;
    //     std::cout << "Elapsed time: " << elapsed_time << " seconds\n";

    //     long double mem = GetPeakRSSInMB();
    //     cout << "mem = " << mem << " MB" << endl;
    // }


    if(method == "MLCDTime"){
        auto core_path = "/home/cheng/MlcDec/leaf/"+ dataset +"_cores.txt";
        auto klmd_path = "/home/cheng/fctree/klmd/"+ dataset +"_klmd.txt";

        vector<int> ks;
        vector<int> lmds;
        
        vector<vector<int>> cores;

        std::ifstream inputFile(klmd_path); 
        int kk, lmdd;
        while (inputFile >> kk >> lmdd) {
            ks.push_back(kk);
            lmds.push_back(lmdd);
        }

        string line;
        std::ifstream coreFile(core_path); 
        while (std::getline(coreFile, line)) {
            std::istringstream stream(line);  // 创建字符串流用于拆分
            std::vector<int> row;             // 存储一行的整数
            int number;

            // 将一行中的整数依次提取到row中
            while (stream >> number) {
                row.push_back(number);
            }

            // 将完整的一行（vector<int>）添加到data中
            cores.push_back(row);
        }

        auto start_time = omp_get_wtime();   
        checktime(ks, lmds, cores);
        auto end_time = omp_get_wtime();
        double elapsed_time = end_time - start_time;
        std::cout << "Elapsed time: " << elapsed_time << " seconds\n"; 

    }

    // if(method == "CoreIndexTime"){

    //     auto klmd_path = "/home/cheng/fctree/klmd/"+ dataset +"_klmd.txt";

    //     vector<vector<int>> klmd(mg.getLayerNumber()+1);
        
    //     vector<vector<int>> cores;

    //     std::ifstream inputFile(klmd_path); 
    //     int kk, lmdd;
    //     while (inputFile >> kk >> lmdd) {
    //         klmd[lmdd].push_back(kk);
    //     }

    //     vector<vector<pair<int, int>>> results(mg.getLayerNumber()+1);
    //     int key, value;

    //     for(uint l = 1; l <= mg.getLayerNumber(); l ++){
    //         std::ifstream coreFile("/home/cheng/fctree/Output/"+dataset+"-"+std::to_string(l)+".txt");

    //         while (coreFile >> key) {           // 读取第一个整数
    //             coreFile.ignore(1, ':');        // 忽略冒号
    //             coreFile >> std::ws >> value;    // 忽略空格并读取第二个整数
    //             results[l].push_back(std::make_pair(key, value)); // 存入pair并加入vector
    //         }

    //         coreFile.close();
    //     }


    //     auto start_time = omp_get_wtime(); 

    //     getResCore(klmd, results, mg.getLayerNumber());

    //     auto end_time = omp_get_wtime();  
        
    //     double retrieve_time = end_time - start_time;
    //     std::cout << "Elapsed time: " << retrieve_time << " seconds\n";
    // }
    // if(method == "PathParallelRight"){
    //     omp_set_num_threads(num_thread);
    //     auto start_time = omp_get_wtime();
    //     FCTree tree(1, 1, mg.GetN());
    //     FCPathLevelRight::Execute(mg, tree);
    //     auto end_time = omp_get_wtime(); 
        
    //     double elapsed_time = end_time - start_time;
    //     std::cout << "PathLevelRight Elapsed time: " << elapsed_time << " seconds\n";
    //     long double mem = GetPeakRSSInMB();
    //     cout << "mem = " << mem << " MB" << endl;

    //      // This part to get the output
    //     if(k != 0 && lmd != 0){
    //         k = uint(k);
    //         lmd = uint(lmd);
    //         coreNode* res_node = tree.getCoreByKAndLmdByRight(tree.getNode(), k, lmd);
    //         if(res_node == nullptr){
    //             cout << "No statisfy result" << endl;
    //         }else{
    //             tree.saveCoreToLocal(dataset, id2vtx, res_node, "pathlmd");
    //         }
    //     }
    // }

    // if(method == "mix"){
    //     omp_set_num_threads(num_thread);
    //     omp_set_max_active_levels(2); // Enable nested parallelism
    //     auto start_time = omp_get_wtime(); 
    //     FCCoreTree tree(1, 1, mg.GetN());
    //     coreNodeP* node = tree.getNode();
    //     FCTreeBuilderCoreParallel::ExecuteMix(mg, tree);
    //     auto end_time = omp_get_wtime(); 
        
    //     double elapsed_time = end_time - start_time;
    //     std::cout << "Core Parallel Elapsed time: " << elapsed_time << " seconds\n";

    //     long double mem = GetPeakRSSInMB();
    //     cout << "mem = " << mem << " MB" << endl;
    // }

    // if(method == "pathk"){
        
    //     omp_set_num_threads(num_thread);
    //     auto start_time = omp_get_wtime();
    //     FCTree tree(1, 1, mg.GetN());
    //     FCTreeBuilderPathParallelByk::Execute(mg, tree);
    //     auto end_time = omp_get_wtime(); 
    //     long double mem = GetPeakRSSInMB();
    //     cout << "mem = " << mem << " MB" << endl;

    //     double elapsed_time = end_time - start_time;
    //     std::cout << "Pathk Parallel Elapsed time: " << elapsed_time << " seconds\n";


    //     if(k != 0 && lmd != 0){
    //         k = uint(k);
    //         lmd = uint(lmd);
    //         coreNode* res_node = tree.getCoreByKAndLmdByLeft(tree.getNode(), k, lmd);
    //         if(res_node == nullptr){
    //             cout << "No statisfy result" << endl;
    //         }else{
    //             tree.saveCoreToLocal(dataset, id2vtx, res_node, "pathk");
    //         }
    //     }
    // 
    // 
    // }

    // if(method == "pathlmd"){
        
    //     omp_set_num_threads(num_thread);
    //     auto start_time = omp_get_wtime();
    //     FCTree tree(1, 1, mg.GetN());
    //     FCTreeBuilderPathParallelBylmd::Execute(mg, tree);
    //     auto end_time = omp_get_wtime(); 
        
    //     double elapsed_time = end_time - start_time;
    //     std::cout << "Pathlmd Parallel Elapsed time: " << elapsed_time << " seconds\n";
    //     long double mem = GetPeakRSSInMB();
    //     cout << "mem = " << mem << " MB" << endl;

    //      // This part to get the output
    //     if(k != 0 && lmd != 0){
    //         k = uint(k);
    //         lmd = uint(lmd);
    //         coreNode* res_node = tree.getCoreByKAndLmdByRight(tree.getNode(), k, lmd);
    //         if(res_node == nullptr){
    //             cout << "No statisfy result" << endl;
    //         }else{
    //             tree.saveCoreToLocal(dataset, id2vtx, res_node, "pathlmd");
    //         }
    //     }
        
    //     // For test use only
    //     // for(int l = 1; l < mg.getLayerNumber(); l ++){
    //     //     uint num_valid = 0;
    //     //     tree.getNumValidRight(tree.getNode(), l, num_valid);
    //     //     cout << "when lmd = " << l << " k max = " << num_valid << endl;
    //     // }


    // }

    // if(method == "corek"){
    //     omp_set_num_threads(num_thread);
    //     auto start_time = omp_get_wtime(); 
    //     FCCoreTree tree(1, 1, mg.GetN());
    //     coreNodeP* node = tree.getNode();
    //     FCTreeBuilderCoreParallelByK::Execute(mg, tree);
    //     auto end_time = omp_get_wtime(); 

    //     double elapsed_time = end_time - start_time;
    //     std::cout << "Path right Core Parallel Elapsed time: " << elapsed_time << " seconds\n";

    //     long double mem = GetPeakRSSInMB();
    //     cout << "mem = " << mem << " MB" << endl; 
    // }

    // if(method == "mixr"){
    //     omp_set_num_threads(num_thread);
    //     omp_set_max_active_levels(2); // Enable nested parallelism
    //     auto start_time = omp_get_wtime(); 
    //     FCCoreTree tree(1, 1, mg.GetN());
    //     coreNodeP* node = tree.getNode();
    //     FCTreeBuilderCoreParallelByK::ExecuteMix(mg, tree);
    //     auto end_time = omp_get_wtime(); 
        
    //     double elapsed_time = end_time - start_time;
    //     std::cout << "Core Parallel Elapsed time: " << elapsed_time << " seconds\n";

    //     long double mem = GetPeakRSSInMB();
    //     cout << "mem = " << mem << " MB" << endl;
    // }

}