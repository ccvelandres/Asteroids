#pragma once

#include <vector>
#include <fstream>
#include <filesystem>

#include <utils/logging.hpp>

namespace assets::model
{
    static std::vector<char> loadOBJ(const std::string &filename)
    {
        L_TAG( "loadBinaryFile");

        if (!std::filesystem::exists(filename)) {
            L_THROW_RUNTIME("Could not find file to load: {}", filename);
        }

        std::ifstream f(filename, std::ios::ate | std::ios::binary);
        std::size_t fileSize = (std::size_t) f.tellg();
        std::vector<char> buffer(fileSize);

        f.seekg(0);
        f.read(buffer.data(), fileSize);
        f.close();

        return buffer;
    }
}