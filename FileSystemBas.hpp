namespace FSbasic {
    inline int hash(std::string str) {
        int hash = 0;
        for (char c : str) {
            hash += c;
        }
        return hash % 8;
    }
    // 从文件中读取数据到content中
    inline void readFromFile(string filename, string& content) {
        ifstream fin(filename.c_str());
        if (fin.is_open()) {
            fin.seekg(0, ios::end);
            int size = fin.tellg();
            content.resize(size);
            fin.seekg(0, ios::beg);
            fin.read(&content[0], size);
            fin.close();
        }
    }

    // 将content中的数据写入到文件中
    inline void writeToFile(string filename, string content) {
        ofstream fout(filename.c_str());
        if (fout.is_open()) {
            fout.write(content.c_str(), content.size());
            fout.close();
        }
    }
}