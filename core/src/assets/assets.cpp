#include <fstream>
#include <filesystem>

#include <assets/utils.hpp>
#include <utils/logging.hpp>

namespace core::assets::utils
{
    std::vector<char> loadBinaryFile(const std::string &filename)
    {
        L_TAG("loadBinaryFile");

        if (!std::filesystem::exists(filename))
        {
            L_THROW_RUNTIME("Could not find file to load: {}", filename);
        }

        std::ifstream     f(filename, std::ios::ate | std::ios::binary);
        std::size_t       fileSize = (std::size_t)f.tellg();
        std::vector<char> buffer(fileSize);

        f.seekg(0);
        f.read(buffer.data(), fileSize);
        f.close();

        return buffer;
    }
} // namespace core::assets::utils