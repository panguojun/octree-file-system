#define WIN

#include <map>
#include <functional>
#include <string>
#include <vector>
#include <stdio.h>
#include <time.h>
#include <string>
#include <stdlib.h> 
#include <stdarg.h>
#include <math.h>
#include <tgmath.h>
#include <stdio.h>
#include <algorithm>
#include <list>
#include <sstream>
#include <fstream>
#include <iostream >
#include <set>
#include <regex>
#include <thread>

#include <windows.h>

#define		IMAGESCALE		512				// 画布尺寸
#define		real			float			// 精度类型
#define		EPSINON			1e-3			// 精度
#define		MAXZ			100.0
#define		EXPORT_SCALE	1.0
#define		USIZE0			0.01f
#define		USIZE			(USIZE0 / EXPORT_SCALE)
#define		SCREENX		    IMAGESCALE
#define		SCREENY		    IMAGESCALE
extern void sendmsg(const std::string& msg);
extern void sendmsg_2d(const std::string& msg);

#include    "com.hpp"
using namespace std;

#include    "vec_math.hpp"
#include    "geom_math.hpp"
#include    "geom_math2d.hpp"
#include    "STR.hpp"

#undef      real

#include    "filesystembas.hpp"
#include    "DiskFileSystem.hpp"
#include    "OCTVFS.hpp"
#include    "VFS.hpp"
#include    "GDB.hpp"

#include    "http_server.hpp"


int main() {
    PRINT("######### start http-server: 127.0.0.1:8081 #########");
    std::thread serverthread = std::thread{ servermain, 100 };

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    if (!CreateProcess(L"C:\\Users\\18858\\Documents\\_LAB\\ZEXE\\vis.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        std::cerr << "CreateProcess failed: " << GetLastError() << std::endl;
        return 1;
    }
    Sleep(1000);

    sendmsg("clear");
    sendmsg("rect 350, 680, 550, 50");
    GDB::main();

    //OCTVFS::main();
    //VFS::main();

    getchar();

    CloseHandle(pi.hProcess);   
    CloseHandle(pi.hThread);
    return 0;
}
/* EOF */
