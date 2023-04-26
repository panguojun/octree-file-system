/**                         ��VFS��
*               �����ļ�ϵͳ��Virtual File System��VFS��
*/
namespace VFS 
{
    struct VirtualFileSystem 
    {
        struct File {
            string name; // �ļ���
            string content; // �ļ�����
        };

        struct Folder {
            string name; // �ļ�����
            vector<File> files; // ���ļ�
            vector<Folder> children; // ���ļ���
        };

        Folder root; // ���ļ���
        string filePath = "VFS.raw"; // �����ļ�·��
        fstream fs; // �ļ�������

        VirtualFileSystem() {
            root.name = "root"; // ��ʼ�����ļ���
        }
        VirtualFileSystem(string filePath) {
            this->filePath = filePath;
            loadFromFile();
        }
        ~VirtualFileSystem()
        {
            saveToFile();
        }
        // �Ӵ����ļ��м���Ŀ¼�����ļ�
        void loadFromFile() {
            fs.open(filePath, ios::in);
            if (!fs) {
                PRINT("Cannot open file: " << filePath);
                return;
            }
            getline(fs, root.name);
            int filesSize;
            fs >> filesSize;
            fs.ignore();
            for (int i = 0; i < filesSize; i++) {
                File file;
                getline(fs, file.name);
                getline(fs, file.content);
                root.files.push_back(file);
            }
            int childrenSize;
            fs >> childrenSize;
            fs.ignore();
            for (int i = 0; i < childrenSize; i++) {
                Folder child;
                readFolderNode(child);
                root.children.push_back(child);
            }
            fs.close();
        }

        // ��Ŀ¼�����ļ����浽�����ļ���
        void saveToFile() {
            fs.open(filePath, ios::out);
            if (!fs) {
                PRINT("Cannot open file: " << filePath);
                return;
            }
            fs << root.name << endl;
            fs << root.files.size() << endl;
            for (int i = 0; i < root.files.size(); i++) {
                fs << root.files[i].name << endl;
                fs << root.files[i].content << endl;
            }
            fs << root.children.size() << endl;
            for (int i = 0; i < root.children.size(); i++) {
                writeFolderNode(root.children[i]);
            }
            fs.close();
        }

        // ����·����ȡ�ļ����ļ��нڵ�
        void getNode(string path, Folder** folder, File** file) {
            vector<string> paths = splitPath(path);
            Folder* current = &root;
            for (int i = 0; i < paths.size() - 1; i++) {
                string name = paths[i];
                if (current->name == name)
                    continue;
                bool found = false;
                for (int j = 0; j < current->children.size(); j++) {
                    if (current->children[j].name == name) {
                        current = &current->children[j];
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    //PRINT(path << " Folder not found!");
                    return;
                }
            }
            for (int i = 0; i < current->files.size(); i++) {
                if (current->files[i].name == paths.back()) {
                    *file = &current->files[i];
                    return;
                }
            }
            for (int i = 0; i < current->children.size(); i++) {
                if (current->children[i].name == paths.back()) {
                    *folder = &current->children[i];
                    return;
                }
            }
            *folder = &root;
            //throw "Node not found";
        }

        // �����ļ���
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

        // �����ļ�
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

        // ��ȡ�ļ�����
        string readFile(string path) {
            Folder* folder = 0;
            File* file = 0;
            getNode(path, &folder, &file);
            if (!file)
                return "";
            return file->content;
        }

        // д���ļ�����
        void writeFile(string path, string content) {
            Folder* folder = 0;
            File* file = 0;
            getNode(path, &folder, &file);
            if (file)
            {
                file->content = content;
                PRINT("writeFile: " << content)
            }
        }

        // ɾ���ļ���
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

        // ɾ���ļ�
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

        // ��·�����Ϊ�ļ��������ļ���
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
          

        // ���ļ����ж�ȡһ���ļ��нڵ�
        void readFolderNode(Folder& folder) {
            getline(fs, folder.name);
            int filesSize;
            fs >> filesSize;
            fs.ignore();
            for (int i = 0; i < filesSize; i++) {
                File file;
                getline(fs, file.name);
                getline(fs, file.content);
                folder.files.push_back(file);
            }
            int childrenSize;
            fs >> childrenSize;
            fs.ignore();
            for (int i = 0; i < childrenSize; i++) {
                Folder child;
                readFolderNode(child);
                folder.children.push_back(child);
            }
        }

        // ��һ���ļ��нڵ�д���ļ�����
        void writeFolderNode(Folder& folder) {
            fs << folder.name << endl;
            fs << folder.files.size() << endl;
            for (int i = 0; i < folder.files.size(); i++) {
                fs << folder.files[i].name << endl;
                fs << folder.files[i].content << endl;
            }
            fs << folder.children.size() << endl;
            for (int i = 0; i < folder.children.size(); i++) {
                writeFolderNode(folder.children[i]);
            }
        }

    };
    int main() {
        VirtualFileSystem vfs("VFS.raw");
        throw;
        return 0;

        // �����ļ��к��ļ�
        vfs.createFolder("/root", "folder1");
        vfs.createFile("/root/folder1", "file1", "Hello, world!");

        // ��ȡ�ļ�����
        string content = vfs.readFile("/root/folder1/file1");
        cout << content << endl;

        // д���ļ�����
        vfs.writeFile("/root/folder1/file1", "Hello, C++!");
        content = vfs.readFile("/root/folder1/file1");
        cout << content << endl;

        // ɾ���ļ�
        vfs.deleteFile("/root/folder1/file1");

        // ɾ���ļ���
        vfs.deleteFolder("/root/folder1");

        return 0;
    }
}