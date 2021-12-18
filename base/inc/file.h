#ifndef BASE_FILE_H
#define BASE_FILE_H

#include <string>
#include <fstream>

namespace Base
{

class FileRead
{
public:
    FileRead(const std::string& path);

    void read(std::string& buf);

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