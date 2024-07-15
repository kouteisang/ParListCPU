

#include "Graph/MultilayerGraph.h"
#include "Core/FCTreeBuilder.h"
#include "Core/FCTreeDFS.h"
#include "Core/FCTree.h"
#include "Core/FCTreeBuilderPathParallelByk.h"
#include "Core/FCTreeBuilderPathParallelBylmd.h"
#include "Core/FCTreeBuilderCoreParallel.h"
#include "header.h"

int main(int argc, char* argv[]){

    string dataset = "example";
    string method = "serial";
    int order = 0;
    uint k = 0;
    uint lmd = 0;

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

    if(method == "dfs"){
        auto start_time = omp_get_wtime();
        FCTreeDFS::Execute(mg);
        auto end_time = omp_get_wtime(); 
        
        double elapsed_time = end_time - start_time;
        std::cout << "DFS Elapsed time: " << elapsed_time << " seconds\n";
 
    }


    if(method == "pathk"){
        
        auto start_time = omp_get_wtime();
        FCTree tree(1, 1, mg.GetN());
        FCTreeBuilderPathParallelByk::Execute(mg, tree);
        auto end_time = omp_get_wtime(); 
        
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
        
        auto start_time = omp_get_wtime();
        FCTree tree(1, 1, mg.GetN());
        FCTreeBuilderPathParallelBylmd::Execute(mg, tree);
        auto end_time = omp_get_wtime(); 
        
        double elapsed_time = end_time - start_time;
        std::cout << "Pathlmd Parallel Elapsed time: " << elapsed_time << " seconds\n";

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
        for(int l = 1; l < mg.getLayerNumber(); l ++){
            uint num_valid = 0;
            tree.getNumValidRight(tree.getNode(), l, num_valid);
            cout << "when lmd = " << l << " k max = " << num_valid << endl;
        }


    }

    if(method == "core"){
        
        FCTree tree(1, 1, mg.GetN());
        FCTreeBuilderCoreParallel::Execute(mg, tree, id2vtx);

        // int a[2][2] = {{1, 1}, {1, 1}};

        // // auto origin = __sync_fetch_and_sub(&a[1][0], 1);
        // // cout << "origin = " << origin << endl;
        // // cout << "a[1][0]" << a[1][0] << endl;

    }

}