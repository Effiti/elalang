#pragma once

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <cwchar>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <variant>
#include <vector>
#include <concepts>
#include <format>
#include <source_location>
#include <string_view>
#include <type_traits>

namespace Ela {


template <typename... Fs>
struct match : Fs... {
    using Fs::operator()...;

    // constexpr match(Fs &&... fs) : Fs{fs}... {}
};
template<class... Ts> match(Ts...) -> match<Ts...>;

template <typename... Ts, typename... Fs>
constexpr decltype(auto) operator| (std::variant<Ts...> const& v, match<Fs...> const& match) {
    return std::visit(match, v);
}

template <typename... Ts, typename... Fs>
constexpr decltype(auto) operator| (std::variant<Ts...> & v, match<Fs...> const& match) {
    return std::visit(match, v);
}

  
using std::string;
using std::vector;
extern std::map<std::string, std::string> colors;
} // namespace Ela
namespace Ela::Emitter{
  class Emitter;
};
