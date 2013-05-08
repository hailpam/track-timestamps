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

// define error codes
enum ErrorCodes {
    UNITIALIZED,
    NOT_ALLOW_OPERATION,
    BAD_PARAMETERS
};

// define a report
struct Report {
    bool success;   /* whether or not call is successful */
    ErrorCodes err; /* error code */
    string msg;  /* error message */
    void* retValue; /* actual return vaule, if any (be careful with casting) */
};

// define the states
enum State {
    ST_CREATED,     /* tacker is created */
    ST_INITIALIZED, /* tacker is initialized */
    ST_TRACKING,    /* tacker is tracking some mearures */
    ST_MEASURED,    /* tacker got a measure */
    ST_FINALIZED    /* tacker got finalized a measure */
};

class TrackTimestamps {
    private:
        timespec startTime;  /* initial timestamp */
        timespec endTime;    /* final timestamp */
        tm* dateAndTime;     /* date and time for filename */
        string fileName;     /* filename */
        bool debugMode;      /* debug modality */
        double elapsedTime;    /* elapsed time */
        int nrOfMeasures;    /* number of measures */
        int repMeasuresCntr; /* counter of repeated measures, 0 by default */
        State currentState;  /* tracker current state */

    protected:
        /**
         * Get Date and time
         */
        string getDateAndTime();
        /**
         * Calculate the Difference between two timestamps
         */
        timespec diffTimestamps();
        /**
         * Calculate the average of timestamps and returns the rounded result
         */
        double calcAverageAndWriteOnFile(const int& mesNr);

    public:
        TrackTimestamps();
        ~TrackTimestamps();
        /**
         * Init Tracker Class
         */
        void initTracker(Report* rep);
        /**
         * Reset Tracker Class
         */
        void resetTracker(Report* rep);
        /**
         * Init internal timer to tack the times
         */
        void initTimer(Report* rep);
        /**
         * Stop internal timer to tack the times
         */
        void stopTimer(Report* rep);
        /**
         * Finalize the measure, calculating the elapsed time and writing it
         * on file
         */
        double finalizeMeasure(Report* rep, const int& mesNr);
        /**
         * Set Debug Mode for this module
         */
        void setDebugMode(Report* rep);
        /**
         * Unset Debug Mode for this module
         */
        void unsetDebugMode(Report* rep);
        /**
         * Set number of repeated measures
         */
        void setNrOfMeasures(Report* rep, const int& n);
        /**
         * Get number of repeated measures
         */
        int getNrOfMeasures();
};

#endif
