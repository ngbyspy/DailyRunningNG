#include"checkfiles.h"

#include <iostream>
#include <fstream>
#include <cstring>

bool isJpgFileExists(char* relativePath, char* filename)
{
    char filePath[256];
    strcpy(filePath, relativePath);
    strcat(filePath, "/");
    strcat(filePath, filename);

    std::ifstream file(filePath);

    if (file.is_open())
    {
        file.close();
        return true;
    }

    return false;
}

//int main()
//{
//    const std::string relativePath = ".\\res\\failure";  // 相对路径
//    const std::string filename = "0012.jpg";  // 要检查的文件名
//
//    if (isJpgFileExists(relativePath, filename))
//    {
//        std::cout << "JPG 文件存在！" << std::endl;
//    }
//    else
//    {
//        std::cout << "JPG 文件不存在！" << std::endl;
//    }
//
//    return 0;
//}