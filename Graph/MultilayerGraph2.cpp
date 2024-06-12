// #include "MultilayerGraph.h"

// MultilayerGraph::~MultilayerGraph(){
//     delete[] graph_layers;    
// }

// void MultilayerGraph::PrintStatistics() {
//     uint max_m = 0, sum_m = 0, m;

//     for (uint i = 0; i < n_layers; i++) {
//         m = graph_layers[i].getM() >> 1;
//         max_m = std::max(max_m, m);
//         sum_m += m;
//     }

//     std::cout << "|L| = " << n_layers << ", |V| = " << n << ", |E| = " << sum_m << ", max_|E_i| = " << max_m << std::endl;
// }

// void MultilayerGraph::GetGraphFile(const std::string &input_path, std::vector<std::string> &graph_files){

//     std::string conf_file = input_path + "mlg.conf";
    
//     std::string line;

//     auto fin = std::ifstream(conf_file);
//     while (fin.peek() != EOF) {
//         getline(fin, line);

//         if (!line.empty()) {
//             graph_files.emplace_back(line);
//         }
//     }
//     fin.close();

// }


// void MultilayerGraph::LoadFromFile(const std::string &input_path){
//     std::vector<std::string> graph_files;
//     std::vector<edge *> edge_buf;
//     std::vector<uint> edge_size;

//     std::unordered_map<ll_uint, uint> vtx2id;

//     GetGraphFile(input_path, graph_files);
//     n_layers = graph_files.size(); 
//     graph_layers = new Graph[n_layers];
    
//     edge_buf.resize(n_layers);
//     edge_size.resize(n_layers);

//     for(int i = 0; i < n_layers; i ++){
//         edge_size[i] = LoadLayer(input_path + graph_files[i], edge_buf[i], vtx2id);
//     }

//     n = vtx2id.size();

//     cout << "n = " << n << endl;

//     for (uint i = 0; i < n_layers; i++) {
//         graph_layers[i].BuildFromEdgeLst(edge_buf[i], n, edge_size[i]);
//         delete[] edge_buf[i];
//     }

// }

// uint MultilayerGraph::LoadLayer(const std::string &graph_file, edge *&edge_buf, std::unordered_map<ll_uint, uint> &vtx2id){
    
//     uint uid, vid;
//     uint edge_buf_size, num_of_vtx, num_of_edge;
//     uint u, v;

//     edge *tmp_edge_buf;

//     std::basic_ifstream<char> graph_in;
//     graph_in = std::ifstream(graph_file);
//     // load graph
//     num_of_edge = 0;
//     edge_buf_size = DEFAULT_EDGE_BUF_SIZE;
//     edge_buf = new edge[edge_buf_size];

//     num_of_vtx = (uint) vtx2id.size();

//      while (graph_in.peek() != EOF) {
//          graph_in >> u >> v;
         
//          if(u != v){

//             auto iter1 = vtx2id.find(u);
//             if (iter1 != vtx2id.end()) {
//                 uid = iter1->second;
//             } else {
//                 uid = num_of_vtx++;
//                 vtx2id.emplace(u, uid);
//             }

//             auto iter2 = vtx2id.find(v);
//             if (iter2 != vtx2id.end()) {
//                 vid = iter2->second;
//             } else {
//                 vid = num_of_vtx++;
//                 vtx2id.emplace(v, vid);
//             }


//             if (num_of_edge + 2 > edge_buf_size) {
//                 edge_buf_size = edge_buf_size << 1;
//                 tmp_edge_buf = new edge[edge_buf_size];

//                 memcpy(tmp_edge_buf, edge_buf, num_of_edge * sizeof(edge));
//                 delete[] edge_buf;
//                 edge_buf = tmp_edge_buf;
//             }

//             edge_buf[num_of_edge++] = edge(u, v);
//             edge_buf[num_of_edge++] = edge(v, u);
//          }
//      }

//     cout << "num_of_vtx" << " = " << num_of_vtx << endl;
//     graph_in.close();
//     return num_of_edge;

// }

// MultilayerGraph* MultilayerGraph::Load(const string &file) {
//     auto mg = new MultilayerGraph();
//     mg->LoadFromFile(file);
//     return mg;
// }
