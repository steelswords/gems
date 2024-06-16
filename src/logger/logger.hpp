#ifndef _STEELSWORDS_GEMS_LOGGER_HPP_
#define _STEELSWORDS_GEMS_LOGGER_HPP_

#include <stddef.h>

#ifndef MAX_LOG_NAME_SIZE
#define MAX_LOG_NAME_SIZE (12)
#endif

#define DEBUG_PREFIX    ("[DEBUG]")
#define INFO_PREFIX     ("[INFO]")
#define WARNING_PREFIX  ("[WARN]")
#define ERROR_PREFIX    ("[ERROR]")
#define CRITICAL_PREFIX ("[CRITICAL]")


namespace gems{

    typedef void (*PrintCallback_t)(const char *);

    /** @brief An inheritable class that makes logging nicer. */
    class Logger
    {
    public:
        Logger(const char *name);
        Logger(const char *name, PrintCallback_t callbackToPrint);

        void debug(const char* message);
        void info(const char* message);
        void warning(const char* message);
        void error(const char* message);
        void critical(const char* message);

        enum LogLevel : unsigned char
        {
            UNKNOWN = 0,
            DEBUG = 1,
            INFO = 2,
            WARNING = 3,
            ERROR = 4,
            CRITICAL = 5,
            TOTAL_LENGTH
        };
        
    protected:
        virtual void log(LogLevel level, const char* message);
        PrintCallback_t m_printCallback {nullptr};

        static constexpr char *s_levelPrefixes[LogLevel::TOTAL_LENGTH] = {
            (char*)"[UNKNOWN]",
            (char*)"[DEBUG]",
            (char*)"[INFO]",
            (char*)"[WARN]",
            (char*)"[ERROR]",
            (char*)"[CRITICAL]"
        };

    private:
        char m_name[MAX_LOG_NAME_SIZE] = {0};
    };
}

#endif /* _STEELSWORDS_GEMS_LOGGER_HPP_ */
