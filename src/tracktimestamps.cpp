#include <tracktimestamps.h>

using namespace std;

TrackTimestamps::TrackTimestamps() {
    debugMode = false;
    elapsedTime = 0;
    nrOfMeasures = 1;
    fileName = TIMES_FILE;
    currentState = ST_CREATED;
    repMeasuresCntr = 0;
}

TrackTimestamps::~TrackTimestamps() {}

void TrackTimestamps::initTimer(Report* rep) {
    cout << "[initTimer] currentState :: [ " << currentState << " ]\n";
    //cout << "[initTimer] Report Address :: [ " << rep << " ]\n";
    if(currentState != ST_INITIALIZED && currentState != ST_FINALIZED)
    {
        // create the report
        rep->success = false;
        rep->err = NOT_ALLOW_OPERATION;
        rep->msg = "Timer can be initialized only in INITIALIZED or FINALIZED states";

        return;
    }
    //startTime = clock();
    //time(&startTime);
    if(debugMode)
        cout << "++++++++++++++++++++++++++++++++++++++++++++++++\n";
    clock_gettime(CLOCK_REALTIME, &startTime);
    if(debugMode)
        cout << "start time :: [ " << startTime.tv_sec << " ]s [ " << startTime.tv_nsec <<" ]us \n";
    currentState = ST_TRACKING;
    // create the report
    rep->success = true;
    rep->msg = "Timer correctly initialized: tracking is on going. Next State [ST_TRACKING]";
}

void TrackTimestamps::stopTimer(Report* rep) {
    cout << "[stopTimer] currentState :: [ " << currentState << " ]\n";
    if(currentState != ST_TRACKING) {
        // create the report
        rep->success = false;
        rep->err = NOT_ALLOW_OPERATION;
        rep->msg = "Timer can be stopped only in TRACKING state";

        return;
    }
    //endTime = clock();
    //time(&endTime);
    clock_gettime(CLOCK_REALTIME, &endTime);
    if(debugMode)
        cout << "  end time :: [ " << endTime.tv_sec << " ]s [ " << endTime.tv_nsec <<" ]us \n";
    currentState = ST_MEASURED;
    // create the report
    rep->success = true;
    rep->msg = "Timer correctly stopped: track to be finalized. Next State [ST_MEASURED]";
}

double TrackTimestamps::finalizeMeasure(Report* rep, const int& mesNr) {
    cout << "[finalizeMeasure]    currentState :: [ " << currentState << " ]\n";
    cout << "[finalizeMeasure]           mesNR :: [ " << mesNr << " ]\n";
    cout << "[finalizeMeasure] repMeasuresCntr :: [ " << repMeasuresCntr << " ]\n";
    if(currentState != ST_MEASURED) {
        // create the report
        rep->success = false;
        rep->err = NOT_ALLOW_OPERATION;
        rep->msg = "Timer can be finalized only in MEASURED state";

        return -1;
    }
    //measure
    timespec timestampsDiff = diffTimestamps();
    double timeToReturn = 0.0;
    long trackedTime = timestampsDiff.tv_nsec;
    if(debugMode)
        cout << "diff :: [ " << trackedTime << " ] in nanoseconds\n";
    elapsedTime += trackedTime;
    if(repMeasuresCntr == 1) {
        // finalize the operations
        timeToReturn = calcAverageAndWriteOnFile(mesNr);
        repMeasuresCntr = nrOfMeasures;
        elapsedTime = 0.0;
    }else
        repMeasuresCntr -= 1;
    if(debugMode)
        cout << "++++++++++++++++++++++++++++++++++++++++++++++++\n\n";
    currentState = ST_FINALIZED;
    // create the report
    rep->success = true;
    rep->msg = "Finalization accomplished. Next State [ST_FINALIZED]";

    return timeToReturn;
}

void TrackTimestamps::setDebugMode(Report* rep) {
    cout << "[setDebugMode] currentState :: [ " << currentState << " ]\n";
    cout << "[" << ((currentState ==  ST_CREATED)?"EUQUAL":"DIFFERENT") << "]\n";
    if(currentState != ST_CREATED && currentState != ST_INITIALIZED) {
        // create the report
        rep->success = false;
        rep->err = NOT_ALLOW_OPERATION;
        rep->msg = "Debug modality can be set only in CREATE or INITIALIZED states";

        return;
    }
     debugMode = true;
     cout << "[setDebugMode] debug :: [ " << debugMode<< " ]";
     // create the report
     rep->success = true;
     rep->msg = "Debug modality correctly set. Next State [current]";
}

void TrackTimestamps::unsetDebugMode(Report* rep) {
    cout << "[unsetDebugMode] currentState :: [ " << currentState << " ]\n";
    if(currentState != ST_CREATED && currentState != ST_INITIALIZED) {
        // create the report
        rep->success = false;
        rep->err = NOT_ALLOW_OPERATION;
        rep->msg = "Debug modality can be set only in CREATE or INITIALIZED states";

        return;
    }
    debugMode = false;
    // create the report
    rep->success = true;
    rep->msg = "Debug modality correctly set. Next State [current]";
}

void TrackTimestamps::initTracker(Report* rep) {
    cout << "[initTracker] currentState :: [ " << currentState << " ]\n";
    if(currentState != ST_CREATED) {
        // create the report
        rep->success = false;
        rep->err = NOT_ALLOW_OPERATION;
        rep->msg = "Tracker can be (re)initialized only in CREATED state";

        return;
    }
    if(debugMode)
        cout << "initializing tracker :: \n";
    string tmpStr = TIMES_FILE;
    //tmpStr += "_";
    //tmpStr += getDateAndTime();
    tmpStr += ".dat";
    if(debugMode)
        cout << "output file :: " << tmpStr << "\n";
    // assign to member variable
    fileName = tmpStr;
    currentState = ST_INITIALIZED;
    // create the report
    rep->success = true;
    rep->msg = "Tracker module correctly initialized. Next State [ST_INITIALIZED]";
}

void TrackTimestamps::resetTracker(Report* rep) {
    cout << "[resetTracker] currentState :: [ " << currentState << " ]\n";
    if(currentState != ST_FINALIZED && currentState != ST_INITIALIZED) {
        // create the report
        rep->success = false;
        rep->err = NOT_ALLOW_OPERATION;
        rep->msg = "Tracker can be reset only in FINALIZED or CREATED states";

        return;
    }
    if(debugMode)
        cout << "reset tracker :: \n";
    string tmpStr = TIMES_FILE;
    //tmpStr += "_";
    //tmpStr += getDateAndTime();
    tmpStr += ".dat";
    if(debugMode)
        cout << "output file :: " << tmpStr << "\n";
    // assign to member variable
    fileName = tmpStr;
    if(nrOfMeasures > 1)
        repMeasuresCntr = nrOfMeasures;
    currentState = ST_INITIALIZED;
    // create the report
    rep->success = true;
    rep->msg = "Tracker module correctly reset. Next State [ST_INITIALIZED]";
}

void TrackTimestamps::setNrOfMeasures(Report* rep, const int& n) {
    cout << "[setNrOfMeasures] currentState :: [ " << currentState << " ]\n";
    if(n <= 0) {
        // create the report
        rep->success = false;
        rep->err = BAD_PARAMETERS;
        rep->msg = "Measure number cannot be less or equal to 0.";

        return;
    }
    if(currentState != ST_CREATED && currentState != ST_INITIALIZED) {
        // create the report
        rep->success = false;
        rep->err = NOT_ALLOW_OPERATION;
        rep->msg = "Number of Measures can be set only in CREATED or INITIALIZED states";

        return;
    }
    nrOfMeasures = n;
    repMeasuresCntr = n;
    // create the report
    rep->success = true;
    rep->msg = "Number of Measures correctly set. Next State [current]";
}

int TrackTimestamps::getNrOfMeasures() {
    return nrOfMeasures;
}

// internal support functionalities
timespec TrackTimestamps::diffTimestamps() {
    timespec diff;
    // calculate the elapsed time in milliseconds
    if(endTime.tv_sec > startTime.tv_sec) {
        if (debugMode)
            cout << "End Time (s) > Start Time (s) [" << (endTime.tv_sec  - startTime.tv_sec) << "]s \n";
            diff.tv_sec = (endTime.tv_sec - startTime.tv_sec);
            diff.tv_nsec =  ((1000000000 - startTime.tv_nsec) + endTime.tv_nsec + ((endTime.tv_sec - startTime.tv_sec - 1)*1000000000));
    }else {
        diff.tv_sec = (endTime.tv_sec - startTime.tv_sec);
        diff.tv_nsec =  (endTime.tv_nsec - startTime.tv_nsec);
    }
    if (debugMode) {
        cout << "diff :: [ " << (diff.tv_sec ) << " ]s\n";
        cout <<"         [ " << (diff.tv_nsec) << " ]us\n";
    }
    return diff;
}

string TrackTimestamps::getDateAndTime() {
    // formatted date and time
    string str = "";
    // get time now
    time_t tmp = time(0);
    // get date and time
    dateAndTime = localtime(&tmp);
    // make the conversion to string
    ostringstream convert;
    convert << (dateAndTime->tm_year + 1900);
    str = convert.str();
    str += "_";
    convert.flush();
    convert.str("");
    convert.clear();
    convert << (dateAndTime->tm_mon + 1);
    str += convert.str();
    str += "_";
    convert.flush();
    convert.str("");
    convert.clear();
    convert << (dateAndTime->tm_mday);
    str += convert.str();
    str += "_";
    convert.flush();
    convert.str("");
    convert.clear();
    convert << (dateAndTime->tm_hour);
    str += convert.str();
    str += "_";
    convert.flush();
    convert.str("");
    convert.clear();
    convert << (dateAndTime->tm_min);
    str += convert.str();
    str += "_";
    convert.flush();
    convert.str("");
    convert.clear();
    convert << (dateAndTime->tm_sec);
    str += convert.str();
    convert.flush();
    convert.str("");
    convert.clear();
    if (debugMode)
        cout << "Formatted Date and Time ::" << str << "\n";

    return str;
}

double TrackTimestamps::calcAverageAndWriteOnFile(const int& mesNr) {
    // accumulate
    double avTime = 0.0;
    avTime = elapsedTime / nrOfMeasures;
    // write on file the results
    ofstream resFile;
    if(debugMode)
        cout << "opening :: [ " << fileName << " ]\n";
    resFile.open (fileName.c_str(), ios::out | ios::app);
    resFile << mesNr << " " << elapsedTime << "\n";
    resFile.close();

    return avTime;
}
