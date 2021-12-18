#include "file.h"
#include <iterator>
#include <vector>

namespace Base
{
    FileRead::FileRead(const std::string& path)
    {
        file_.open(path, std::ios::in);

        isOpen_ = file_.is_open();
    }

    void FileRead::read(std::string& buf)
    {
        if(!isOpen_) return;

        std::istreambuf_iterator<char>iter(file_);
        std::istreambuf_iterator<char>eof;

       buf.assign(iter,eof);   //这玩意能读完吗。。。

       closeFile();
    }

    FileRead::~FileRead()
    {
        if(isOpen_) file_.close();  
    }  

    void FileRead::closeFile()
    {
        if(isOpen_) file_.close(); 

        isOpen_=false;
    }
}
