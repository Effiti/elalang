#pragma once

#include "cmdlib/common.hpp"
#include "cmdlib/logging.hpp"
#include <cctype>
#include <cstddef>
#include <cwchar>
#include <fmt/core.h>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#ifdef DEBUG_BUILD
  const int verbosity = 5;
#else
  const int verbosity = 0;
#endif