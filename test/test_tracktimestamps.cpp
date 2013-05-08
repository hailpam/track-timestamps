#include <iomanip>
#include <iostream>

#include <tracktimestamps.h>

using namespace std;


int main ( int argc, char* argv[] ) {
    int repMeasure = 1;
    if(argc >= 1) {
        istringstream iss( argv[1] );
        if(!(iss >> repMeasure)) {
            cerr << "[main] Error Occurred while parsing argv[0] [" << argv[1] << "]\n**********\n";
            return -1;
        }
        cerr << "[main] Nr of Repeated Measure\n-   argv[0] :: [" << argv[1] << "]\n\n**********\n";
    }
    //
    TrackTimestamps* tracker = new TrackTimestamps();
    // report: inout parameter to be passed in
    Report* r = new Report;
    tracker->setNrOfMeasures(r, repMeasure);
    tracker->setDebugMode(r);
    tracker->initTracker(r);
    int idx =0;
    for (int i = 0; i < 10; i++) {
        idx = 0;
        for(int j = 0; j < repMeasure; j++) {
            tracker->initTimer(r);
            if(r->success)
                cout << "[main][initTimer][Report] " << r->msg << endl;
            while (idx < 100000) {
                idx += 1;
                //cout << "idx ::" << idx;
            }
            tracker->stopTimer(r);
            tracker->finalizeMeasure(r, i);
        }
    }
    tracker->resetTracker(r);

    // clean heap memory allocated for report
    delete r;
    delete tracker;

    return 0;
}
