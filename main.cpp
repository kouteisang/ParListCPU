

#include "Graph/MultilayerGraph.h"
#include "Core/FCTreeBuilder.h"
#include "Core/FCTreeDFS.h"
#include "Core/FCTree.h"
#include <omp.h>

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
        std::cout << "Elapsed time: " << elapsed_time << " seconds\n";


        // This part to get the output
        if(k != 0 && lmd != 0){
            k = uint(k);
            lmd = uint(lmd);
            coreNode* res_node = tree.getCoreByKAndLmd(tree.getNode(), k, lmd);
            if(res_node == nullptr){
                cout << "No statisfy result" << endl;
            }else{
                tree.saveCoreToLocal(dataset, id2vtx, res_node);
            }
        }

    }

    if(method == "dfs"){
        auto start_time = omp_get_wtime();
        FCTreeDFS::Execute(mg);
        auto end_time = omp_get_wtime(); 
        
        double elapsed_time = end_time - start_time;
        std::cout << "Elapsed time: " << elapsed_time << " seconds\n";
 
    }
}