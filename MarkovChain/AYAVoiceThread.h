//
//  AYAVoiceThread.h
//  MarkovChain
//
//  Created by Chris Bennett on 4/1/15.
//  Copyright (c) 2015 AyersAudio. All rights reserved.
//

#import "Voice.h"
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

class AYAVoiceThread {
    
    
public:
    AYAVoiceThread (CVoice*);
    double getSample();
    CVoice *mVoice;
    double *circBuff;
    int producerIndex;
    int consumerIndex;
    
private:
    int incIndex(int);
    void workerLoop();

    struct timespec tim;
    static void *worker_thread(void *);
    
};