#ifndef HTTP_FILE_H
#define HTTP_FILE_H

#include "file.h"
#include "logger.h"

#include <memory>
#include <map>

namespace Http
{
    class HtmlFile : public Base::FileRead
    {
    public:
        HtmlFile(const std::string& path)
            : Base::FileRead(path)
        {
            if (isOpen())
            {
                read(buf_);

                LOG_HTTP << "Read html file success! read size: " << buf_.size() << "  " << path<<log::end;
            }
            else
                LOG_HTTP << "Read html file file error... " << path << log::end;
        }

        ~HtmlFile()
        {

        }

        void display() { LOG_COUT << "html files:\n"
                                   << buf_; }

        const std::string& buf()const {return buf_;}

        size_t size()const {return buf_.size();}

    private:
        std::string buf_;
    };

    class WebResources
    {
    public:

        using fileMap=std::map<std::string,std::shared_ptr<HtmlFile>>;

        WebResources(const std::string&);

        int num()const {return num_;}

        std::shared_ptr<const HtmlFile>findHtml(const std::string&) const;

    private:
        void findFile(const std::string&);

        void insertToMap(HtmlFile*,const char*);

        fileMap files_;

        int num_;
    
    };

}

#endif