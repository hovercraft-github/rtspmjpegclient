/*
 * Copyright 2016 Qualibit S.r.l.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http ://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef LOGHANDLER_HPP
#define LOGHANDLER_HPP

// log definitions and constants

#include <string>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/layout.h>

#define DEFAULT_OUTPUT_LOGGER "RTSPClientLibOutput"
#define DEFAULT_ERROR_LOGGER "RTSPClientLibError"

#define DEFAULT_STDOUT_APPENDER DEFAULT_OUTPUT_LOGGER "StdOutAppender"
#define DEFAULT_OUTPUT_FILE_APPENDER DEFAULT_OUTPUT_LOGGER "OutputFileAppender"
#define DEFAULT_STDERR_APPENDER DEFAULT_ERROR_LOGGER "StdErrAppender"
#define DEFAULT_ERROR_FILE_APPENDER DEFAULT_ERROR_LOGGER "ErrorFileAppender"

#define DEFAULT_OUTPUT_LOG_LEVEL INFO_LOG_LEVEL
#define DEFAULT_ERROR_LOG_LEVEL WARN_LOG_LEVEL

#define DEFAULT_IMMEDIATE_FLUSH true

#define DEFAULT_LOG_PATTERN_LAYOUT "%D{%Y-%m-%d %H:%M:%S.%q} [%c] [%H] [%t] [%p] %m%n"
#define DEFAULT_OUTPUT_LOG_FILE "rtspmjpegclient.output.log"
#define DEFAULT_ERROR_LOG_FILE "rtspmjpegclient.error.log"

#define MAX_LOG_FILE_SIZE 4194304L
#define MAX_LOG_BACKUP_INDEX 4

using namespace log4cplus;

namespace loghandlerns {

class LogHandler {
public:
    static std::string trim(const char str[]);

    static void startLog(std::string outputLogFile, std::string errorLogFile);
    static void stopLog();
    static int getLogLevel();
    static void setLogLevel(int logLevel);
};

}

#endif // LOGHANDLER_HPP
