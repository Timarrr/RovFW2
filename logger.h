/**
 * @file logger.h
 * @author Timarrr (tigrmango@gmail.com)
 * @brief Minimal logger implementation
 * @version 0.1
 * @date 2023-03-31
 *
 * @copyright Copyright (c) 2023
 * @todo Buffer overflow protection, partial messages, connection detection and
 * possibly optimisation(?)
 */

#ifndef LOGGER_H
#define LOGGER_H
#include <Arduino.h>

/**
 * @brief Decorator for trace() funcctions
 *
 */
#define trace_decorator F("[T]: ")

/**
 * @brief Decorator for debug() functions
 *
 */
#define debug_decorator F("[D]: ")

/**
 * @brief Decorator for info() functions
 *
 */
#define info_decorator F("[I]: ")

/**
 * @brief Decorator for warn() functions
 *
 */
#define warn_decorator F("[W]: ")

/**
 * @brief Decorator for error() functions
 *
 */
#define error_decorator F("[E]: ")

/**
 * @brief Namespace to avoid congestion and separate logger functions
 *
 */
namespace Logger {

/**
 * @brief Lowest priority output, [T] decorator
 *
 * @param string Message
 */
void trace(String string);

/**
 * @brief Lowest priority output, [T] decorator
 *
 * @param string Message
 */
void trace(const __FlashStringHelper *string);

/**
 * @brief Second lowest priority output, [D] decorator
 *
 * @param string Message
 */
void debug(String string);

/**
 * @brief Second lowest priority output, [D] decorator
 *
 * @param string Message
 */
void debug(const __FlashStringHelper *string);

/**
 * @brief Average priority output, [I] decorator
 *
 * @param string Message
 */
void info(String string);

/**
 * @brief Average priority output, [I] decorator
 *
 * @param string Message
 */
void info(const __FlashStringHelper *string);

/**
 * @brief Second highest priority output, [W] decorator
 *
 * @param string Message
 */
void warn(String string);

/**
 * @brief Second highest priority output, [W] decorator
 *
 * @param string Message
 */
void warn(const __FlashStringHelper *string);

/**
 * @brief Highest priority output, [E] decorator
 *
 * @param string Message
 */
void error(String string);

/**
 * @brief Highest priority output, [E] decorator
 *
 * @param string Message
 */
void error(const __FlashStringHelper *string);
}; // namespace Logger

#endif