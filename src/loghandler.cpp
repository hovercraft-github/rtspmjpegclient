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

#include "loghandler.hpp"

namespace loghandlerns {

std::string LogHandler::trim(const char str[])
{
    std::string _str(str);
    _str.at(_str.find_last_of("\r\n")) = '\0';

    return _str;
}

void LogHandler::startLog(std::string outputLogFile = "", std::string errorLogFile = "")
{
    if (outputLogFile.empty())
        outputLogFile = DEFAULT_OUTPUT_LOG_FILE;

    if (errorLogFile.empty())
        errorLogFile = DEFAULT_ERROR_LOG_FILE;

    Logger outputLogger = Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_OUTPUT_LOGGER));

    SharedAppenderPtr stdoutAppender(new log4cplus::ConsoleAppender(false, DEFAULT_IMMEDIATE_FLUSH));
    stdoutAppender->setName("stdoutAppender");
    //std::unique_ptr<Layout> stdoutLayout = std::unique_ptr<Layout>(new log4cplus::PatternLayout(DEFAULT_LOG_PATTERN_LAYOUT));
    stdoutAppender->setLayout(std::unique_ptr<Layout>(new log4cplus::PatternLayout(DEFAULT_LOG_PATTERN_LAYOUT)));

    SharedAppenderPtr outputFileAppender(new RollingFileAppender(outputLogFile, MAX_LOG_FILE_SIZE, MAX_LOG_BACKUP_INDEX, DEFAULT_IMMEDIATE_FLUSH));
    outputFileAppender->setName("outputFileAppender");
    //std::unique_ptr<Layout> outputFileLayout = std::unique_ptr<Layout>(new log4cplus::PatternLayout(DEFAULT_LOG_PATTERN_LAYOUT));
    outputFileAppender->setLayout(std::unique_ptr<Layout>(new log4cplus::PatternLayout(DEFAULT_LOG_PATTERN_LAYOUT)));

    outputLogger.addAppender(stdoutAppender);
    outputLogger.addAppender(outputFileAppender);
    outputLogger.setLogLevel(DEFAULT_OUTPUT_LOG_LEVEL);


    Logger errorLogger = Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_ERROR_LOGGER));

    SharedAppenderPtr stderrAppender(new log4cplus::ConsoleAppender(true, DEFAULT_IMMEDIATE_FLUSH));
    stderrAppender->setName("stderrAppender");
    //std::unique_ptr<Layout> stderrLayout = std::unique_ptr<Layout>(new log4cplus::PatternLayout(DEFAULT_LOG_PATTERN_LAYOUT));
    stderrAppender->setLayout(std::unique_ptr<Layout>(new log4cplus::PatternLayout(DEFAULT_LOG_PATTERN_LAYOUT)));

    SharedAppenderPtr errorFileAppender(new RollingFileAppender(errorLogFile, MAX_LOG_FILE_SIZE, MAX_LOG_BACKUP_INDEX, DEFAULT_IMMEDIATE_FLUSH));
    errorFileAppender->setName("errorFileAppender");
    //std::unique_ptr<Layout> errorFileLayout = std::unique_ptr<Layout>(new log4cplus::PatternLayout(DEFAULT_LOG_PATTERN_LAYOUT));
    errorFileAppender->setLayout(std::unique_ptr<Layout>(new log4cplus::PatternLayout(DEFAULT_LOG_PATTERN_LAYOUT)));

    errorLogger.addAppender(stderrAppender);
    errorLogger.addAppender(errorFileAppender);
    errorLogger.setLogLevel(DEFAULT_ERROR_LOG_LEVEL);

    LOG4CPLUS_DEBUG(outputLogger, "Log opened");
}

void LogHandler::stopLog()
{
    Logger outputLogger = Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_OUTPUT_LOGGER));

    LOG4CPLUS_DEBUG(outputLogger, "Log closed");

    Logger::shutdown();
}

int LogHandler::getLogLevel()
{
    return Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_OUTPUT_LOGGER)).getLogLevel();
}

void LogHandler::setLogLevel(int logLevel)
{
    Logger::getInstance(LOG4CPLUS_TEXT(DEFAULT_OUTPUT_LOGGER)).setLogLevel(logLevel);
}

}
