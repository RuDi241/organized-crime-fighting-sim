#ifndef UTILS_H
#define UTILS_H

#include <time.h>
#include <stdlib.h>
#include <unistd.h>

// Function prototypes

/**
 * Seeds the random number generator.
 * Should be called once at the beginning of the main process.
 */
void seed_random();

/**
 * Generates a random integer within a specified range [min, max].
 * 
 * @param min The minimum value (inclusive).
 * @param max The maximum value (inclusive).
 * @return A random integer within the range.
 */
int random_int(int min, int max);

/**
 * Generates a random float within a specified range [min, max].
 * 
 * @param min The minimum value (inclusive).
 * @param max The maximum value (inclusive).
 * @return A random float within the range.
 */
float random_float(float min, float max);

/**
 * Simulates work or waiting time by sleeping for a specified duration.
 * 
 * @param seconds The number of seconds to sleep.
 */
void sleep_seconds(int seconds);

/**
 * Simulates work or waiting time by sleeping for a specified duration in milliseconds.
 * 
 * @param milliseconds The number of milliseconds to sleep.
 */
void sleep_milliseconds(long milliseconds);


#endif // UTILS_H
