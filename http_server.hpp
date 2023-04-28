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
        PRINTV(body);
        std::vector<string> spair = STR::split(body, ':');
        string cmd = spair.front();
        string param = spair.back();
        if (cmd == "open")
        {
            string content = GDB2d::vfs.readFile(param);
            PRINT("readFile: " << content);

            sendmsg_2d(content);
        }
    }
}
#endif