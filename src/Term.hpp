#ifndef TERM_HPP
#define TERM_HPP

#include <map>

#include "Config.hpp"

enum class TC {
    HEADER,
    BLUE,
    GREEN,
    WARN,
    ERR,
    ENDC,
    BOLD,
    UNDERLINE,
};

#ifdef TERM_COLORS
const std::map<TC, const char *> TCMAP = {
    {TC::HEADER, "\033[95m"},
    {TC::BLUE, "\033[94m"},
    {TC::GREEN, "\033[92m"},
    {TC::WARN, "\033[93m"},
    {TC::ERR, "\033[91m"},
    {TC::ENDC, "\033[0m"},
    {TC::BOLD, "\033[1m"},
    {TC::UNDERLINE, "\033[4m"},
};
#else  // no TERM_COLORS
const std::map<TC, const char *> TCMAP = {
    {TC::HEADER, ""},
    {TC::BLUE, ""},
    {TC::GREEN, ""},
    {TC::WARN, ""},
    {TC::ERR, ""},
    {TC::ENDC, ""},
    {TC::BOLD, ""},
    {TC::UNDERLINE, ""},
};
#endif // TERM_COLORS

#endif // TERM_HPP
