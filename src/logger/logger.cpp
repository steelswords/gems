#include "logger.hpp"
#include <string.h>
#include <stdlib.h>
#include <stddef.h>


namespace gems {

Logger::Logger(const char *name)
{
    strncpy(m_name, name, MAX_LOG_NAME_SIZE);
    m_name[MAX_LOG_NAME_SIZE - 1] = '\0';

    // TODO: Try to set up intelligent defaults for STL and Arduino and printf capable ones
}

Logger::Logger(const char *name, PrintCallback_t callback)
{
    strncpy(m_name, name, MAX_LOG_NAME_SIZE);
    m_name[MAX_LOG_NAME_SIZE - 1] = '\0';

    m_printCallback = callback;
}

void Logger::debug(const char *message) { log(LogLevel::DEBUG, message); }
void Logger::info(const char *message) { log(LogLevel::INFO, message); }
void Logger::warning(const char *message) { log(LogLevel::WARNING, message); }
void Logger::error(const char *message) { log(LogLevel::ERROR, message); }
void Logger::critical(const char *message) { log(LogLevel::CRITICAL, message); }


void Logger::log(LogLevel level, const char* message)
{
    // Construct a slightly bigger buffer to accomodate prefix
    size_t sizeOfMessage = strlen(message);
    // TODO: Assert valid level
    char* logPrefix = s_levelPrefixes[level];
    size_t sizeOfBytesToAllocate = strlen(logPrefix) + MAX_LOG_NAME_SIZE + sizeOfMessage;

    char* outputBuffer = (char*)calloc(sizeOfBytesToAllocate, sizeof(char));
    if (outputBuffer)
    {
        // Copy message into bigger buffer
        size_t lengthOfName = strlen(m_name);
        size_t lengthOfPrefix = strlen(s_levelPrefixes[LogLevel::DEBUG]);
        size_t lengthOfMessage = strlen(message);
        size_t lengthOfSpaces = 0;

        strncpy(outputBuffer, m_name, lengthOfName);
        strncpy(outputBuffer + lengthOfName, s_levelPrefixes[LogLevel::DEBUG], lengthOfPrefix);
        strncpy(outputBuffer + lengthOfName + lengthOfPrefix, message, sizeOfMessage);
        // Call callback on bigger buffer

        if (m_printCallback)
        {
            m_printCallback(message);
        }
        // Free our memory
        free(outputBuffer);
        outputBuffer = nullptr;
    }
}

} /* namespace gems */
