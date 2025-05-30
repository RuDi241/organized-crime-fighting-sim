#include "../include/utils.h"
#include <stdio.h>

/**
 * Seeds the random number generator.
 * Should be called once at the beginning of the main process.
 */
void seed_random() {
    srand(time(NULL) ^ getpid()); // Seed with time and process ID for better randomness in forks
}

/**
 * Generates a random integer within a specified range [min, max].
 * 
 * @param min The minimum value (inclusive).
 * @param max The maximum value (inclusive).
 * @return A random integer within the range.
 */
int random_int(int min, int max) {
    if (min > max) {
        // Swap if min > max
        int temp = min;
        min = max;
        max = temp;
    }
    if (min == max) {
        return min;
    }
    // rand() % (max - min + 1) gives range [0, max-min]
    // Add min to shift the range to [min, max]
    return min + rand() % (max - min + 1);
}

/**
 * Generates a random float within a specified range [min, max].
 * 
 * @param min The minimum value (inclusive).
 * @param max The maximum value (inclusive).
 * @return A random float within the range.
 */
float random_float(float min, float max) {
    if (min > max) {
        // Swap if min > max
        float temp = min;
        min = max;
        max = temp;
    }
    if (min == max) {
        return min;
    }
    // Generate a random float between 0.0 and 1.0
    float scale = rand() / (float) RAND_MAX; // [0, 1.0]
    // Scale and shift to the desired range
    return min + scale * (max - min); // [min, max]
}

/**
 * Simulates work or waiting time by sleeping for a specified duration.
 * 
 * @param seconds The number of seconds to sleep.
 */
void sleep_seconds(int seconds) {
    if (seconds > 0) {
        sleep(seconds);
    }
}

/**
 * Simulates work or waiting time by sleeping for a specified duration in milliseconds.
 * 
 * @param milliseconds The number of milliseconds to sleep.
 */
void sleep_milliseconds(long milliseconds) {
    if (milliseconds > 0) {
        struct timespec ts;
        ts.tv_sec = milliseconds / 1000;
        ts.tv_nsec = (milliseconds % 1000) * 1000000;
        nanosleep(&ts, NULL);
    }
}
