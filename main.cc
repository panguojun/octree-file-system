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

#include    "com.hpp"
using namespace std;

#include    "vec_math.hpp"
#include    "geom_math.hpp"
#include    "geom_math2d.hpp"
#include    "scene/scene.hpp"
#include    "STR.hpp"

#undef      real

#include    "sqlite3/sqlite3.h"
#include    "filesystembas.hpp"
#include    "DiskFileSystem.hpp"
#include    "OCTFileSystem.hpp"
#include    "VFileSystem.hpp"
#include    "GDB.hpp"
#include    "http_server.hpp"

int main0() {
    sqlite3* db;
    char* zErrMsg = 0;
    int rc;

    // 打开数据库文件
    rc = sqlite3_open("C:/Users/18858/Desktop/example.db", &db);

    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return 1;
    }

    sqlite3_stmt* stmt;
    const char* sql = "SELECT * FROM t_plat_scene_node";

    // 编译SQL语句
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return 1;
    }

    // 执行查询语句
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        // 获取每一行的数据
        int id = sqlite3_column_int(stmt, 0);
        const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        double value = sqlite3_column_double(stmt, 2);

        // 打印每一行的数据
        std::cout << "ID: " << id << ", Name: " << name << ", Value: " << value << std::endl;
    }

    // 释放资源
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}

int main() {
    GDB::main();
    OCTFileSystem::main();
    // DiskFileSystem::main();
}
/* EOF */
