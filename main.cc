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

// ################################################################################################
// PM 
// ################################################################################################
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
extern void sendmsg_3d(const std::string& msg);

#include    "com.hpp"
#include    "vecmath_imp.hpp"

using namespace std;

#include    "vec_math.hpp"
#include    "geom_math.hpp"
#include    "geom_math2d.hpp"
#include    "STR.hpp"

// LUA
#ifdef WIN
extern "C" {
#include "thirdpart/lua53.h"
}
#else
#include "lua53.h"
#endif
extern EXPORT_API lua_State* L;
#undef      real

#pragma comment(lib, "libPMDLL")

// VFS 头文件
#include    "filesystembas.hpp"
#include    "DiskFileSystem.hpp"
#include    "OCTVFS.hpp"
#include    "VFS.hpp"
#include    "GDB.hpp"
#include    "GDB2d.hpp"

#include    "http_server.hpp"

namespace Serializer
{
    extern int main();
}
// ################################################################################################

static int test(lua_State* L)
{
    MSGBOX("BAGA")
    return 0;
}

// ################################################################################################
int main() {
    PRINT("######### start http-server: 127.0.0.1:8081 #########");
    std::thread serverthread = std::thread{ servermain, 100 };

    {
        STARTUPINFO si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));
        if (!CreateProcess(L"vis.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
            std::cerr << "CreateProcess failed: " << GetLastError() << std::endl;
            return 1;
        }
        Sleep(1000);
    }

    sendmsg("clear");
    sendmsg("rect 350, 680, 550, 50");
    sendmsg_2d("loc 500, 500");

   // GDB::main();
    Serializer::main();
    //OCTVFS::main();
    //VFS::main();

    getchar();

  //  CloseHandle(pi.hProcess);   
  //  CloseHandle(pi.hThread);

    lua_register(L, "vfs_test", test);
    return 0;
}
/* EOF */