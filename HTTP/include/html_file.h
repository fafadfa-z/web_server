#ifndef _HTML_FILE_H
#define _HTML_FILE_H

#include "file_read.h"
#include "logger.h"

#include <cassert>
#include <filesystem>
#include <memory>
#include <map>

namespace Http
{
    class HtmlFile : public Base::FileRead //
    {
    public:
        HtmlFile(const std::string &path)
            : Base::FileRead(path)
        {

        }
        ~HtmlFile()
        {
        }

       

    private:
        std::string buf_;
    };

    class WebResources
    {
    public:
        using fileMap = std::map<std::string, std::shared_ptr<HtmlFile>>;

        WebResources();

        int num() const { return num_; }

        std::shared_ptr<const HtmlFile> findHtml(const std::string &) const;

    private:
        void findFile();

        fileMap files_;

        int num_;
    };
}

#endif