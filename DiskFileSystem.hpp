/**                 �������ļ�ϵͳ��
* 
*/
namespace DiskFileSystem 
{
#define BLOCK_SIZE 512 // ���С
#define BLOCK_NUM 1024 // ���̿���

    struct SuperBlock { // ������
        int block_size; // ���С
        int block_num; // ���̿���
        int inode_num; // i�ڵ���
        int data_block_num; // ���ݿ���
        int inode_bitmap_block; // i�ڵ�λͼ���
        int data_bitmap_block; // ���ݿ�λͼ���
        int inode_table_block; // i�ڵ����
        int data_block_start; // ���ݿ���ʼ���
    };

    struct Inode { // i�ڵ�
        int inode_no; // i�ڵ���
        int file_size; // �ļ���С
        int block_num; // �ļ�ռ�õĿ���
        int blocks[10]; // �ļ�ռ�õĿ��
    };

    struct Directory { // Ŀ¼��
        char name[20]; // �ļ���
        int inode_no; // i�ڵ���
    };

    int main() {
        // �����ļ�
        ofstream ofs("disk.img", ios::binary);
        if (!ofs) {
            cout << "Create file failed!" << endl;
            return -1;
        }

        // ��ʼ��������
        SuperBlock sb;
        sb.block_size = BLOCK_SIZE;
        sb.block_num = BLOCK_NUM;
        sb.inode_num = BLOCK_NUM / 8;
        sb.data_block_num = BLOCK_NUM - 1 - sb.inode_num;
        sb.inode_bitmap_block = 1;
        sb.data_bitmap_block = 2;
        sb.inode_table_block = 3;
        sb.data_block_start = sb.inode_table_block + sb.inode_num;

        // д�볬����
        ofs.write((char*)&sb, sizeof(SuperBlock));

        // ��ʼ��i�ڵ�λͼ
        char inode_bitmap[BLOCK_SIZE] = { 0 };
        inode_bitmap[0] = 0x80; // ��0��i�ڵ�Ϊ��Ŀ¼
        ofs.write(inode_bitmap, BLOCK_SIZE);

        // ��ʼ�����ݿ�λͼ
        char data_bitmap[BLOCK_SIZE] = { 0 };
        data_bitmap[0] = 0x80; // ��0�����ݿ�Ϊ��Ŀ¼
        ofs.write(data_bitmap, BLOCK_SIZE);

        // ��ʼ��i�ڵ��
        Inode inode_table[BLOCK_NUM / 8] = { 0 };
        inode_table[0].inode_no = 0; // ��Ŀ¼��i�ڵ���Ϊ0
        inode_table[0].file_size = 2 * sizeof(Directory); // ��Ŀ¼��СΪ����Ŀ¼��
        inode_table[0].block_num = 1; // ��Ŀ¼ռ��һ����
        inode_table[0].blocks[0] = 0; // ��Ŀ¼�Ŀ��Ϊ0
        ofs.write((char*)inode_table, BLOCK_NUM / 8 * sizeof(Inode));

        // ��ʼ����Ŀ¼
        Directory root[2] = { {".", 0}, {"..", 0} };
        ofs.seekp(sb.data_block_start * BLOCK_SIZE);
        ofs.write((char*)root, sizeof(root));

        // �ر��ļ�
        ofs.close();

        cout << "Create file success!" << endl;

        return 0;
    }
}