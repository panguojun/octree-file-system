/**                             【OCTVFS】
*                   八叉树空间定位的文件索引系统
*                   可以根据一个空间位置来索引对应的文件
*                   文件以列表方式保存在磁盘上，每个文件对应一个地址索引
*                   可以作为一种数据库来使用
**/
#define MAXDEPTH    8
namespace OCTVFS
{
    using FILE_GUID = __int64;

    struct OctreeNode
    {
        boundingbox aabb;// 使用一个整数就能够对应一个AABB
        vector<FILE_GUID> files; // 文件位置列表
        bool is_file;
        int size;

        std::string name;
        OctreeNode* children[8] = { 0 };
        OctreeNode() {}
        OctreeNode(std::string name) : name(name){}
    };

    class Octree {
    public:
        OctreeNode* root;

    public:
        Octree() : root(nullptr) {}
        ~Octree() { if(root) clear(root); }

        OctreeNode* findnode(OctreeNode* oct, const vec3& p, int depth = 0)
        {
            for (int ii = 0; ii < 8; ii++)
            {
                if (oct->children[ii]->aabb.intersection(p))
                {
                    if (depth + 1 == MAXDEPTH)
                        return oct->children[ii];
                    return findnode(oct->children[ii], p, depth + 1);
                }
            }
            return 0;
        }
        void walk(OctreeNode* ot, int depth)
        {
            if (depth > MAXDEPTH)
                return;

            for (int i = 0; i < 8; i++)
            {
                if (ot->children[i])
                    walk(ot->children[i], depth + 1);
            }
        }
        void clear(OctreeNode* ot)
        {
            for (int i = 0; i < 8; i++)
            {
                if (ot->children[i])
                    clear(ot->children[i]);
            }
            delete(ot);
        }
        void build(OctreeNode* ot, const boundingbox& aabb, int depth)
        {
            ot->aabb = aabb;

            if (depth < MAXDEPTH)
            {
                const vec3& a = aabb.a;
                const vec3& b = aabb.b;
                vec3 m = (aabb.a + aabb.b) / 2;
                depth++;

                ot->children[0] = new OctreeNode();
                build(ot->children[0], boundingbox(a, m), depth);

                ot->children[1] = new OctreeNode();
                build(ot->children[1], boundingbox(vec3(m.x, a.y, a.z), vec3(b.x, m.y, m.z)), depth);

                ot->children[2] = new OctreeNode();
                build(ot->children[2], boundingbox(vec3(a.x, m.y, a.z), vec3(m.x, b.y, m.z)), depth);

                ot->children[3] = new OctreeNode();
                build(ot->children[3], boundingbox(vec3(a.x, a.y, m.z), vec3(m.x, m.y, b.z)), depth);

                ot->children[4] = new OctreeNode();
                build(ot->children[4], boundingbox(m, b), depth);

                ot->children[5] = new OctreeNode();
                build(ot->children[5], boundingbox(vec3(a.x, m.y, m.z), vec3(m.x, b.y, b.z)), depth);

                ot->children[6] = new OctreeNode();
                build(ot->children[6], boundingbox(vec3(m.x, a.y, m.z), vec3(b.x, m.y, b.z)), depth);

                ot->children[7] = new OctreeNode();
                build(ot->children[7], boundingbox(vec3(m.x, m.y, a.z), vec3(b.x, b.y, m.z)), depth);
            }
        }
        void save_node(OctreeNode* node, FILE* file)
        {
            //PRINT("save_scenenode!");
            fwrite(node->name.c_str(), 128, 1, file);
            //PRINTV(node->name);
           /* int entnum = node->elist.size();
            fwrite(&entnum, 4, 1, file);
            for (int i = 0; i < entnum; i++)
            {
                fwrite(&node->elist[i], sizeof(node->elist[i]), 1, file);
            }*/
            for (auto& it : node->children)
            {
                if(it)
                    save_node(it, file);
            }
        }
        void save_octree(const std::string& filename)
        {
            PRINT("save octree ...");
            FILE* file;
            if (fopen_s(&file, filename.c_str(), "wb") == 0)
            {
                save_node(root, file);

                fclose(file);
            }

        }
    };

    int main() 
    {
        Octree tree;
        boundingbox aabb(vec3(0, 0, 0), vec3(1024, 512, 1024));
        tree.root = new OctreeNode();
        tree.root->aabb = aabb;

        tree.build(tree.root, aabb, 0);

        OctreeNode* on = tree.findnode(tree.root, vec3::ZERO);
        if (on)
        {
            on->files.push_back(5);
            on->files.push_back(1);
            on->files.push_back(8);
        }

        return 0;
    }
}