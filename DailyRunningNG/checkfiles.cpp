#include"checkfiles.h"

#include <iostream>
#include <fstream>
#include <cstring>

bool isFileExists(char* relativePath, char* filename)
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