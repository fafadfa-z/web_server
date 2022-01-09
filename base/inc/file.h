#ifndef BASE_FILE_H
#define BASE_FILE_H

#include <string>
#include <fstream>
#include <filesystem>

namespace Base
{

class FileRead
{
public:
    FileRead(const std::string& path);

    FileRead(const std::filesystem::path& path)
        :FileRead(path.string()){}

    void read(std::string& buf);

    int  read(std::string& buf,int index,int maxSize=16*1024);

    ~FileRead();

    bool  isOpen()const {return isOpen_;}

protected:
    void closeFile();

private:
    bool isOpen_;

    std::ifstream file_;
};

}

#endif