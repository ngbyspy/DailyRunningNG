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
//    const std::string relativePath = ".\\res\\failure";  // ���·��
//    const std::string filename = "0012.jpg";  // Ҫ�����ļ���
//
//    if (isJpgFileExists(relativePath, filename))
//    {
//        std::cout << "JPG �ļ����ڣ�" << std::endl;
//    }
//    else
//    {
//        std::cout << "JPG �ļ������ڣ�" << std::endl;
//    }
//
//    return 0;
//}