#pragma once

#include "../include/cmdlib/common.hpp"
#include "../include/cmdlib/logging.hpp"
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

using namespace std::string_view_literals;
#ifdef DEBUG_BUILD
  const int verbosity = 5;
#else
  const int verbosity = 0;
#endif
