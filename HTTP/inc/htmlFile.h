#ifndef HTTP_FILE_H
#define HTTP_FILE_H

#include "file.h"
#include "logger.h"

#include <assert.h>
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
            if (isOpen())
            {
                Base::FileRead::read(buf_);

                LOG_HTTP << "Read html file success! read size: " << buf_.size() << "  " << path << Log::end;
            }
            else
                LOG_HTTP << "Read html file file error... " << path << Log::end;
        }
        ~HtmlFile()
        {
        }
        void display() { LOG_COUT << "html files:\n"
                                  << buf_ << std::endl; }

        const std::string &buf() const { return buf_; }

        size_t size() const { return buf_.size(); }

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