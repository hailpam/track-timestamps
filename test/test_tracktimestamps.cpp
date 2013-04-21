#include <iomanip>
#include <iostream>



#include <tracktimestamps.h>

using namespace std;


int main ( int argc, char* argv[] ) {
    TrackTimestamps* tracker = new TrackTimestamps();
    tracker->unsetDebugMode();
    tracker->initTracker();
    for (int i = 0; i < 100; i++) {
        int idx = 0;
        tracker->initTimer();
        while (idx < 100000000) {
            idx += 1;
            //cout << "idx ::" << idx;
        }
        tracker->stopTimer();
        tracker->finalizeMeasure(i);
    }
    tracker->resetTracker();
    delete tracker;
    return 0;
}
