#include "Ela.hpp"

namespace Ela {
    std::map<std::string, std::string> colors{{"black", "\x1B[30m"},
                                                    {"red", "\x1B[31m"},
                                                    {"green", "\x1B[32m"},
                                                    {"yellow", "\x1B[33m"},
                                                    {"blue", "\x1B[34m"},
                                                    {"magenta", "\x1B[35m"},
                                                    {"cyan", "\x1B[36m"},
                                                    {"white", "\x1B[37m"},
                                                    {"end", "\033[0m"},
                                                    {"underline", "\x1B[4m"},
                                                    {"bg:black", "\x1B[40m"},
                                                    {"bg:red", "\x1B[41m"},
                                                    {"bg:green", "\x1B[42m"},
                                                    {"bg:yellow", "\x1B[43m"},
                                                    {"bg:blue", "\x1B[44m"},
                                                    {"bg:purple", "\x1B[45m"},
                                                    {"bg:cyan", "\x1B[46m"},
                                                    {"bg:white", "\x1B[47m"}};
}
