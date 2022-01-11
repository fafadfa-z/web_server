#include "htmlFile.h"

namespace fs = std::filesystem;

namespace Http
{
    WebResources::WebResources(const std::string &resoutcePath)
        : num_(0)
    {
        findFile(resoutcePath);

        LOG_HTTP << "Find " << num_ << " HTML files.." << Log::end;
    }

    void WebResources::findFile(const std::string &path)
    {
        fs::path filePath(path);

        fs::directory_entry direEntry(filePath);

        assert(direEntry.is_directory());

        for (auto file : fs::recursive_directory_iterator(direEntry))
        {
            if (!file.is_regular_file())
                continue;

            HtmlFile *htmlFile = new HtmlFile(file.path().string());
            num_++;
            std::string fullName = std::string("/") + file.path().filename().c_str();

            files_[fullName] = std::shared_ptr<HtmlFile>(htmlFile);
        }
    }

    std::shared_ptr<const HtmlFile> WebResources::findHtml(const std::string &name) const
    {
        auto iter = files_.find(name);

        if (iter == files_.end())
            return std::shared_ptr<const HtmlFile>(nullptr);

        return iter->second;
    }
};
