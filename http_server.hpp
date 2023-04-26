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
        // 解析HTTP请求中的body部分，获取draw命令
        std::string drawCmd;
        drawCmd = body;

        VFS::VirtualFileSystem vfs;

        // 读取文件内容
        string content = vfs.readFile(drawCmd);
        cout << content << endl;
    }
}
#endif