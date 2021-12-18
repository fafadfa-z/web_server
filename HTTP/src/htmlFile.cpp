#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <assert.h>

#include "htmlFile.h"

namespace Http
{

    WebResources::WebResources(const std::string &resoutcePath)
        : num_(0)
    {
        findFile(resoutcePath);

        LOG_HTTP << "Find " << num_ << " HTML files.." << log::end;
    }

    void WebResources::findFile(const std::string &path)
    {
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

            LOG_HTTP << "Find resource: " << dir << log::end;

            //file->display();
        }
        ::closedir(dir);
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
