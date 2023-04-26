namespace VFileSystem 
{
    class File {
    public:
        string name; // 文件名
        string content; // 文件内容
    };

    class Folder {
    public:
        string name; // 文件夹名
        vector<File> files; // 子文件
        vector<Folder> children; // 子文件夹
    };
    // 从文件中读取数据到content中
    void readFromFile(string filename, string& content) {
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
    void writeToFile(string filename, string content) {
        ofstream fout(filename.c_str());
        if (fout.is_open()) {
            fout.write(content.c_str(), content.size());
            fout.close();
        }
    }

    class VirtualFileSystem {
    public:
        Folder root; // 根文件夹

        VirtualFileSystem() {
            root.name = "root"; // 初始化根文件夹
        }

        // 根据路径获取文件或文件夹节点
        void getNode(string path, Folder** folder, File** file) {
            vector<string> paths = splitPath(path);
            Folder& current = root;
            for (int i = 0; i < paths.size() - 1; i++) {
                string name = paths[i];
                if (current.name == name)
                    break;
                bool found = false;
                for (int j = 0; j < current.children.size(); j++) {
                    if (current.children[j].name == name) {
                        current = current.children[j];
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    PRINT(path << " Folder not found!");
                    return;
                }
            }
            for (int i = 0; i < current.files.size(); i++) {
                if (current.files[i].name == paths.back()) {
                    *file = &current.files[i];
                    return;
                }
            }
            for (int i = 0; i < current.children.size(); i++) {
                if (current.children[i].name == paths.back()) {
                    *folder = &current.children[i];
                    return;
                }
            }
            *folder = &root;
            //throw "Node not found";
        }

        // 创建文件夹
        void createFolder(string path, string name) {
            Folder* folder = 0;
            File* file = 0;
            getNode(path, &folder, &file);
            if (folder)
            {
                for (int i = 0; i < folder->children.size(); i++) {
                    if (folder->children[i].name == name) {
                        throw "Folder already exists";
                    }
                }
                Folder newFolder;
                newFolder.name = name;
                folder->children.push_back(newFolder);
            }
        }

        // 创建文件
        void createFile(string path, string name, string content) {
            Folder* folder = 0;
            File* file = 0;
            getNode(path, &folder, &file);
            if (folder)
            {
                for (int i = 0; i < folder->files.size(); i++) {
                    if (folder->files[i].name == name) {
                        throw "File already exists";
                    }
                }
                File newFile;
                newFile.name = name;
                newFile.content = content;
                folder->files.push_back(newFile);
            }
        }

        // 读取文件内容
        string readFile(string path) {
            Folder* folder = 0;
            File* file = 0;
            getNode(path, &folder, &file);
            if (!file)
                return "";
            return file->content;
        }

        // 写入文件内容
        void writeFile(string path, string content) {
            Folder* folder = 0;
            File* file = 0;
            getNode(path, &folder, &file);
            if (file)
                file->content = content;
        }

        // 删除文件夹
        void deleteFolder(string path) {
            vector<string> paths = splitPath(path);
            Folder current = root;
            for (int i = 0; i < paths.size() - 1; i++) {
                string name = paths[i];
                bool found = false;
                for (int j = 0; j < current.children.size(); j++) {
                    if (current.children[j].name == name) {
                        current = current.children[j];
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    PRINT(path << " Folder not found");
                }
            }
            string name = paths.back();
            for (int i = 0; i < current.children.size(); i++) {
                if (current.children[i].name == name) {
                    current.children.erase(current.children.begin() + i);
                    return;
                }
            }
            PRINT("Folder not found");
        }

        // 删除文件
        void deleteFile(string path) {
            Folder* folder = 0;
            File* file = 0;
            getNode(path, &folder, &file);
            if (folder){
                for (int i = 0; i < folder->files.size(); i++) {
                    if (folder->files[i].name == file->name) {
                        folder->files.erase(folder->files.begin() + i);
                        return;
                    }
                }
            }
            PRINT("File not found");
        }

        // 将路径拆分为文件夹名和文件名
        vector<string> splitPath(string path) {
            vector<string> paths;
            int pos = 0;
            while (pos < path.size()) {
                int nextPos = path.find('/', pos + 1);
                if (nextPos == -1) {
                    paths.push_back(path.substr(pos + 1));
                    break;
                }
                paths.push_back(path.substr(pos + 1, nextPos - pos - 1));
                pos = nextPos;
            }
            return paths;
        }
    };
    int main() {
        VirtualFileSystem vfs;

        // 创建文件夹和文件
        vfs.createFolder("/root", "folder1");
        vfs.createFile("/root/folder1", "file1", "Hello, world!");

        // 读取文件内容
        string content = vfs.readFile("/root/folder1/file1");
        cout << content << endl;

        // 写入文件内容
        vfs.writeFile("/root/folder1/file1", "Hello, C++!");
        content = vfs.readFile("/root/folder1/file1");
        cout << content << endl;

        // 删除文件
        vfs.deleteFile("/root/folder1/file1");

        // 删除文件夹
        vfs.deleteFolder("/root/folder1");

        return 0;
    }

}