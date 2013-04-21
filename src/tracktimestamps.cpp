#include <tracktimestamps.h>

using namespace std;

TrackTimestamps::TrackTimestamps() {
    debugMode = false;
}

TrackTimestamps::~TrackTimestamps() {}

void TrackTimestamps::initTimer() {
    //startTime = clock();
    //time(&startTime);
    if(debugMode)
        cout << "++++++++++++++++++++++++++++++++++++++++++++++++\n";
    clock_gettime(CLOCK_REALTIME, &startTime);
    if(debugMode)
        cout << "start time :: [ " << startTime.tv_sec << " ]s [ " << startTime.tv_nsec <<" ]us \n";
}

void TrackTimestamps::stopTimer() {
    //endTime = clock();
    //time(&endTime);
    clock_gettime(CLOCK_REALTIME, &endTime);
    if(debugMode)
        cout << "  end time :: [ " << endTime.tv_sec << " ]s [ " << endTime.tv_nsec <<" ]us \n";
}

float TrackTimestamps::finalizeMeasure(int mesNr) {
    timespec timestampsDiff = diffTimestamps();
    elapsedTime = timestampsDiff.tv_nsec;
    if(debugMode)
        cout << "diff :: [ " << elapsedTime << " ] in nanoseconds\n";
    // write on file the results
    ofstream resFile;
    if(debugMode)
        cout << "opening :: [ " << fileName << " ]\n";
    resFile.open (fileName.c_str(), ios::out | ios::app);
    resFile << mesNr << " " << elapsedTime << "\n";
    resFile.close();
    if(debugMode)
        cout << "++++++++++++++++++++++++++++++++++++++++++++++++\n\n";

    return elapsedTime;
}

void TrackTimestamps::setDebugMode() {
     debugMode = true;
}

void TrackTimestamps::unsetDebugMode() {
     debugMode = false;
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

void TrackTimestamps::initTracker() {
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
}

void TrackTimestamps::resetTracker() {
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
}

timespec TrackTimestamps::diffTimestamps() {
    timespec diff;
    // calculate the elapsed time in milliseconds
    if(endTime.tv_sec > startTime.tv_sec) {
        if (debugMode)
            cout << "End Time (s) > Start Time (s)" << "\n";
        diff.tv_sec = (endTime.tv_sec - startTime.tv_sec);
        diff.tv_nsec =  ((1000000000 - startTime.tv_nsec) + endTime.tv_nsec);
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
