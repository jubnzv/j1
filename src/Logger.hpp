#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <cstdlib>
#include <string>

#include "Config.hpp"
#include "Term.hpp"

#ifdef DEBUG_J1
#define dbg(_PRNTMSG, ...)                                                               \
    fprintf(stderr, "[DBG] %20s:%d: " _PRNTMSG "\n", __func__, __LINE__, ##__VA_ARGS__); \
    fflush(stderr);
#else
#define dbg(_PRNTMSG, ...) \
    do {                   \
    } while (0);
#endif // DEBUG_J1

#ifdef DEBUG_J1
#define warn(_PRNTMSG, ...)                                                                                           \
    fprintf(stderr, "%s[WARN]%s %20s:%d: " _PRNTMSG "\n", TCMAP.at(TC::WARN), TCMAP.at(TC::ENDC), __func__, __LINE__, \
        ##__VA_ARGS__);                                                                                               \
    fflush(stderr);
#else
#define warn(_PRNTMSG, ...)                                                                               \
    fprintf(stderr, "%s[WARN]%s  " _PRNTMSG "\n", TCMAP.at(TC::WARN), TCMAP.at(TC::ENDC), ##__VA_ARGS__); \
    fflush(stderr);
#endif // DEBUG_J1

#ifdef DEBUG_J1
#define info(_PRNTMSG, ...)                                                                                           \
    fprintf(stdout, "%s[INFO]%s %20s:%d: " _PRNTMSG "\n", TCMAP.at(TC::BLUE), TCMAP.at(TC::ENDC), __func__, __LINE__, \
        ##__VA_ARGS__);                                                                                               \
    fflush(stdout);
#else
#define info(_PRNTMSG, ...)                                                                               \
    fprintf(stdout, "%s[INFO]%s  " _PRNTMSG "\n", TCMAP.at(TC::BLUE), TCMAP.at(TC::ENDC), ##__VA_ARGS__); \
    fflush(stdout);
#endif // DEBUG_J1

#ifdef DEBUG_J1
#define err(_PRNTMSG, ...)                                                                                          \
    fprintf(stderr, "%s[ERR]%s %20s:%d: " _PRNTMSG "\n", TCMAP.at(TC::ERR), TCMAP.at(TC::ENDC), __func__, __LINE__, \
        ##__VA_ARGS__);                                                                                             \
    fflush(stderr);
#else
#define err(_PRNTMSG, ...)                                                                              \
    fprintf(stderr, "%s[ERR]%s  " _PRNTMSG "\n", TCMAP.at(TC::ERR), TCMAP.at(TC::ENDC), ##__VA_ARGS__); \
    fflush(stderr);
#endif // DEBUG_J1

#endif // LOGGER_HPP
