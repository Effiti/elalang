#pragma once

#include <algorithm>
#include <map>
#include <cctype>
#include <cstddef>
#include <cwchar>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <stdexcept>
#include <optional>
#include <memory>
#include <variant>

#ifdef DEBUG_BUILD
  const int verbosity = 5;
#else
  const int verbosity = 0;
#endif
namespace Ela {
    using std::vector;
    extern std::map<std::string, std::string> colors;
}
