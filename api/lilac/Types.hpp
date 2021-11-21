#pragma once

#include "Macros.hpp"
#include <string>

namespace lilac {
    /**
     * Describes the severity of the log 
     * message.
     * @enum Severity
     */
    enum class Severity {
        /**
         * The message contains information 
         * only relevant to the developer / 
         * other mod developers.
         */
        Debug,

        /**
         * The message contains general 
         * information about that logger's 
         * state. The logger is still 
         * working correctly.
         */
        Info,

        /**
         * The message contains information 
         * about the logger's state that 
         * may require special attention.
         */
        Notice,

        /**
         * The message contains information 
         * about the logger's state that 
         * is abnormal and may result in 
         * errors if not handled properly.
         */
        Warning,

        /**
         * The message contains information 
         * about a general error, such as 
         * inability to read files.
         */
        Error,
        
        /**
         * The message contains information 
         * about a severe error that may 
         * cause the logger to be partly or 
         * fully practically unusable.
         */
        Critical,

        /**
         * The message contains information 
         * about a very severe error that 
         * requires immediate attention,
         * and renders the logger completely 
         * unusable.
         */
        Alert,
        
        /**
         * The logger has encountered an 
         * error so severe it is utterly 
         * unusable and can not proceed 
         * its execution. This error level 
         * should never be seen in-game, 
         * as an error this severe will 
         * most likely cause an immediate 
         * unrequested exit of the 
         * application, also known as a 
         * crash.
         */
        Emergency,
    };

    /**
     * Get the string representation of 
     * a Severity value.
     * @param severity Severity
     * @returns String representation
     */
    static std::string SeverityToString(Severity severity) {
        switch (severity) {
            case Severity::Debug:     return "Debug";
            case Severity::Info:      return "Info";
            case Severity::Notice:    return "Notice";
            case Severity::Warning:   return "Warning";
            case Severity::Error:     return "Error";
            case Severity::Critical:  return "Critical";
            case Severity::Alert:     return "Alert";
            case Severity::Emergency: return "Emergency";
            default:                  return "Unknown";
        }
    }
}
