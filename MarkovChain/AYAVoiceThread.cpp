//
//  AYAVoiceThread.m
//  MarkovChain
//
//  Created by Chris Bennett on 4/1/15.
//  Copyright (c) 2015 AyersAudio. All rights reserved.
//

#import "AYAVoiceThread.h"

#define MAX_QUEUE_SIZE 4096




AYAVoiceThread::AYAVoiceThread (CVoice *voice)
{
    mVoice = voice;
    circBuff = (double *)calloc(MAX_QUEUE_SIZE, sizeof(double));
    producerIndex = 0;
    consumerIndex = 0;
    

    tim.tv_sec = 0;
    tim.tv_nsec = 100;
    
    
    pthread_t voice_thread;
    int ret;
    
    ret = pthread_create(&voice_thread, NULL, AYAVoiceThread::worker_thread, this);
    
}

void *AYAVoiceThread::worker_thread(void *voiceptr)
{
    AYAVoiceThread *vThread = (AYAVoiceThread *)voiceptr;
    
    pthread_setname_np("voice_thread");
    double dLeftOutput;
    double dRightOutput;
    
    for (int i=0; i<1024; i++) {
        vThread->mVoice->doVoice(dLeftOutput, dRightOutput);
        
        vThread->circBuff[vThread->producerIndex] = dLeftOutput;
        vThread->producerIndex = vThread->incIndex(vThread->producerIndex);
    }
    
    
    while (true) {

        bool full = true;
        
        if ((vThread->producerIndex)%MAX_QUEUE_SIZE != vThread->consumerIndex) {
            full = false;
        }
        
        
        
        int diff = MAX_QUEUE_SIZE - abs(vThread->producerIndex-vThread->consumerIndex)-1;
        if (!full) {
            for (int i=0; i<diff; i++) {
                vThread->mVoice->doVoice(dLeftOutput, dRightOutput);
                
                vThread->circBuff[vThread->producerIndex] = dLeftOutput;
                vThread->producerIndex = vThread->incIndex(vThread->producerIndex);
            }
        }

    }
}


int AYAVoiceThread::incIndex(int index)
{
     return (index+1)%MAX_QUEUE_SIZE;
}


double AYAVoiceThread::getSample()
{
    if (!mVoice->m_bNoteOn) {
        return 0;
    }
    
    
    bool empty = true;
    
    
    if ((consumerIndex+1)%MAX_QUEUE_SIZE != producerIndex) {
        empty = false;
    }
    
    double output = 0;
    
    if (!empty) {
        
        output = circBuff[consumerIndex];
        consumerIndex = incIndex(consumerIndex);
        
    }

    
    return output;
}

