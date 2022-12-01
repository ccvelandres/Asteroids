#pragma once

#include <vector>
#include <string>

#include "../utils/logging.hpp"

namespace core::assets::utils
{
    std::vector<char> loadBinaryFile(const std::string &filename);

}