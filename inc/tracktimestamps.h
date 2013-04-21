#ifndef TRACKTIMESTAMPS_H
#define TRACKTIMESTAMPS_H

#include <string>
#include <iostream>

#include <time.h>
#include <sys/time.h>

#include <fstream>
#include <sstream>

#define TIMES_FILE  "times"

using namespace std;

class TrackTimestamps {
    private:
        timespec startTime;  /* initial timestamp */
        timespec endTime;    /* final timestamp */
        tm* dateAndTime;
        string fileName;
        bool debugMode;
        long elapsedTime;  /* elapsed time */

    protected:
        /**
         * Get Date and time
         */
        string getDateAndTime();
        /**
         * Calculate the Difference between two timestamps
         */
        timespec diffTimestamps();

    public:
        TrackTimestamps();
        ~TrackTimestamps();
        /**
         * Init Tracker Class
         */
        void initTracker();
        /**
         * Reset Tracker Class
         */
        void resetTracker();
        /**
         * Init internal timer to tack the times
         */
        void initTimer();
        /**
         * Stop internal timer to tack the times
         */
        void stopTimer();
        /**
         * Finalize the measure, calculating the elapsed time and writing it
         * on file
         */
        float finalizeMeasure(int mesNr);
        /**
         * Set Debug Mode for this module
         */
        void setDebugMode();
        /**
         * Unset Debug Mode for this module
         */
        void unsetDebugMode();
};

#endif
