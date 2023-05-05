#pragma once
#ifdef WIN
// --------------------------------------------
// server
// --------------------------------------------
extern int servermain(int num);
namespace Serializer
{
    extern VFS::VirtualFileSystem vfs;
}
void onrequest(const std::string& msg, const std::string& body, std::string& out)
{
	PRINT("onrequest: " << msg << "\n");
    if (msg == "cmd")
    {
        PRINTV(body);
        std::vector<string> spair = STR::split(body, ':');
        string cmd = spair.front();
        string param = spair.back();
        if (cmd == "open")
        {
           // string content = GDB2d::vfs.readFile(param);
           // sendmsg_2d(content);
            VFS::VirtualFileSystem ::Folder* fd = Serializer::vfs.getFolder(param);
            if(fd)
            { 
                Serializer::vfs.show(*fd);
            }
            else
            {
                string content = Serializer::vfs.readFile(param);
                if(param.find("2d") != std::string::npos)
                    sendmsg_2d(content);
                else if (param.find("3d") != std::string::npos)
                    sendmsg_3d(content);
            }
        }
    }
}
#endif