#pragma once
#ifdef WIN
// --------------------------------------------
// server
// --------------------------------------------
extern int servermain(int num);
void onrequest(const std::string& msg, const std::string& body, std::string& out)
{
	PRINT("onrequest: " << msg << "\n");
    if (msg == "cmd")
    {
        // ����HTTP�����е�body���֣���ȡdraw����
        std::string drawCmd;
        drawCmd = body;

        VFS::VirtualFileSystem vfs;

        // ��ȡ�ļ�����
        string content = vfs.readFile(drawCmd);
        cout << content << endl;
    }
}
#endif