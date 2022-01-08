#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <assert.h>
#include <filesystem>
#include "htmlFile.h"

namespace Http
{

    WebResources::WebResources(const std::string &resoutcePath)
        : num_(0)
    {
        findFile(resoutcePath);

        LOG_HTTP << "Find " << num_ << " HTML files.." << Log::end;
    }

    void WebResources::findFile(const std::string &pathStr)
    {
        std::cout << "pathStr: " <<pathStr<< std::endl;

        std::filesystem::directory_entry direct(pathStr.c_str());

        assert(direct.is_directory());

        for (const auto &entry : std::filesystem::recursive_directory_iterator(direct))
        {
            if (entry.is_regular_file())
            {
                HtmlFile *file = new HtmlFile(entry.path());  //path 可以隐式转换成 string 

                //insertToMap(file, dp->d_name);

                num_++;

                std::cout << "Find resource: " << entry << std::endl;
            }
        }

#if 0
        struct stat st;
        auto res1 = ::stat(path.c_str(), &st); //检测路径是否合法

        assert(res1 >= 0);

        auto res2 = S_ISDIR(st.st_mode); //检测路径是否是目录

        assert(res2 == true);

        auto dir = ::opendir(path.c_str());

        struct dirent *dp = NULL;

        while ((dp = ::readdir(dir)) != nullptr)
        {
            if ((!strncmp(dp->d_name, ".", 1)) || (!strncmp(dp->d_name, "..", 2))) //忽略这两个目录
                continue;

            std::string dir(path);

            dir += '/';
            dir += dp->d_name;

            HtmlFile *file = new HtmlFile(dir);

            insertToMap(file, dp->d_name);

            num_++;

            LOG_HTTP << "Find resource: " << dir << Log::end;

            //file->display();
        }
        ::closedir(dir);
#endif
    }

    void WebResources::insertToMap(HtmlFile *file, const char *path)
    {
        auto size = ::strlen(path);

        std::string ans(size - 5, '\0');

        for (int i = 0; i < size - 5; i++)
        {
            if (path[i] == '_')
                ans[i] = '/';

            else
                ans[i] = path[i];
        }
        files_[ans] = std::shared_ptr<HtmlFile>(file);
    }

    std::shared_ptr<const HtmlFile> WebResources::findHtml(const std::string &name) const
    {
        auto iter = files_.find(name);
        if (iter == files_.end())
            return std::shared_ptr<const HtmlFile>(nullptr);

        return iter->second;
    }
};
