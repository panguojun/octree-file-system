/**                 【磁盘文件系统】
* 
*/
namespace DiskFileSystem 
{
#define BLOCK_SIZE 512 // 块大小
#define BLOCK_NUM 1024 // 磁盘块数

    struct SuperBlock { // 超级块
        int block_size; // 块大小
        int block_num; // 磁盘块数
        int inode_num; // i节点数
        int data_block_num; // 数据块数
        int inode_bitmap_block; // i节点位图块号
        int data_bitmap_block; // 数据块位图块号
        int inode_table_block; // i节点表块号
        int data_block_start; // 数据块起始块号
    };

    struct Inode { // i节点
        int inode_no; // i节点编号
        int file_size; // 文件大小
        int block_num; // 文件占用的块数
        int blocks[10]; // 文件占用的块号
    };

    struct Directory { // 目录项
        char name[20]; // 文件名
        int inode_no; // i节点编号
    };

    int main() {
        // 创建文件
        ofstream ofs("disk.img", ios::binary);
        if (!ofs) {
            cout << "Create file failed!" << endl;
            return -1;
        }

        // 初始化超级块
        SuperBlock sb;
        sb.block_size = BLOCK_SIZE;
        sb.block_num = BLOCK_NUM;
        sb.inode_num = BLOCK_NUM / 8;
        sb.data_block_num = BLOCK_NUM - 1 - sb.inode_num;
        sb.inode_bitmap_block = 1;
        sb.data_bitmap_block = 2;
        sb.inode_table_block = 3;
        sb.data_block_start = sb.inode_table_block + sb.inode_num;

        // 写入超级块
        ofs.write((char*)&sb, sizeof(SuperBlock));

        // 初始化i节点位图
        char inode_bitmap[BLOCK_SIZE] = { 0 };
        inode_bitmap[0] = 0x80; // 第0个i节点为根目录
        ofs.write(inode_bitmap, BLOCK_SIZE);

        // 初始化数据块位图
        char data_bitmap[BLOCK_SIZE] = { 0 };
        data_bitmap[0] = 0x80; // 第0个数据块为根目录
        ofs.write(data_bitmap, BLOCK_SIZE);

        // 初始化i节点表
        Inode inode_table[BLOCK_NUM / 8] = { 0 };
        inode_table[0].inode_no = 0; // 根目录的i节点编号为0
        inode_table[0].file_size = 2 * sizeof(Directory); // 根目录大小为两个目录项
        inode_table[0].block_num = 1; // 根目录占用一个块
        inode_table[0].blocks[0] = 0; // 根目录的块号为0
        ofs.write((char*)inode_table, BLOCK_NUM / 8 * sizeof(Inode));

        // 初始化根目录
        Directory root[2] = { {".", 0}, {"..", 0} };
        ofs.seekp(sb.data_block_start * BLOCK_SIZE);
        ofs.write((char*)root, sizeof(root));

        // 关闭文件
        ofs.close();

        cout << "Create file success!" << endl;

        return 0;
    }
}