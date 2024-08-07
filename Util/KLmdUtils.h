#ifndef KLmdUtils
#define KLmdUtils


    std::vector<int> getK(string path){

        std::vector<int> ks;
        int cnt = 0;
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "Cannot open the file" << std::endl;
        }

        std::string line;
        while (std::getline(file, line)) {
            int number = std::stoi(line);
            ks.push_back(number);
        }

        return ks;
    }


    std::vector<int> getLmd(string path){

        std::ifstream file(path);
        std::vector<int> lmds;
        if (!file.is_open()) {
            std::cerr << "Cannot open the file" << std::endl;
        }

        std::string line;
        while (std::getline(file, line)) {
            int number = std::stoi(line);
            lmds.push_back(number);
        }

        return lmds;
    }

#endif