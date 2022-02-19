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

    class BigHtmlFile : public Base::FileRead
    {
    public:
        BigHtmlFile(std::filesystem::path path)
            : Base::FileRead(path),entry_(path),size_(entry_.file_size())
        {
            assert(size_>=minFileSize_);
        }

    private:
        std::filesystem::directory_entry entry_;
        uintmax_t size_;
        static const auto minFileSize_=16385;
    };

    class WebResources
    {
    public:
        using fileMap = std::map<std::string, std::shared_ptr<HtmlFile>>;

        WebResources(const std::string &);

        int num() const { return num_; }

        std::shared_ptr<const HtmlFile> findHtml(const std::string &) const;

    private:
        void findFile(const std::string &);

        fileMap files_;

        int num_;
    };
}

#endif