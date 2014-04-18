//
//  AudioUnitsEngine.m
//  AUPlayer
//
//  Created by willpirkle on 4/18/11.
//  Copyright 2011 University of Miami. All rights reserved.
//

#import "AudioUnitsEngine.h"

//#define MAX_VOICES 4

@implementation AudioUnitsEngine
@synthesize auTrack_1_PlaybackInfo;
// default initializer
- (id)init
{
	self = [super init];
    if (self) 
	{
        // set our defaults
		// end init
        // load up voices
        CMiniSynthVoice* pVoice;
        MAX_VOICES = 8;
        auTrack_1_PlaybackInfo.MAX_VOICES = 8;
        for(int i=0; i<MAX_VOICES; i++)
        {
            pVoice = new CMiniSynthVoice;
            pVoice->setSampleRate((double)44100);
            pVoice->prepareForPlay();
            pVoice->update();
            auTrack_1_PlaybackInfo.m_VoicePtrStack1.push_back(pVoice);
        }
    }
	
    return self;
}

-(void)setOscillatorFO:(float)fo{
}
-(void)setOscillatorWave:(int)wave{
}
-(void)setTimbreSelection:(UINT)sel{
        MAX_VOICES = 4;
        auTrack_1_PlaybackInfo.MAX_VOICES = 4;
}

-(void)setNoteOn:(int)notenumber{
    auTrack_1_PlaybackInfo.m_bNoteOn = TRUE;
    
    CMiniSynthVoice* pVoice;
	auTrack_1_PlaybackInfo.m_VoiceIterator1 = auTrack_1_PlaybackInfo.m_VoicePtrStack1.begin();
    
	bool bStealNote = true;
	for(int i=0; i<MAX_VOICES; i++)
	{
		pVoice =  auTrack_1_PlaybackInfo.m_VoicePtrStack1[i];
		
		// if we have a free voice, turn on
		if(!pVoice->m_bNoteOn)
		{
			auTrack_1_PlaybackInfo.m_VoiceIterator1 = auTrack_1_PlaybackInfo.m_VoicePtrStack1.erase(auTrack_1_PlaybackInfo.m_VoiceIterator1);
			auTrack_1_PlaybackInfo.m_VoicePtrStack1.push_back(pVoice);
            
			pVoice->noteOn(notenumber, 60, midiMIDIFreqTable[notenumber], m_dLastNoteFrequency);
            
			pVoice->setSustainOverride(false);
            
            
			// save
			m_dLastNoteFrequency = midiMIDIFreqTable[notenumber];
			bStealNote = false;
			break;
		}
		else {
			auTrack_1_PlaybackInfo.m_VoiceIterator1++;
		}
	}
	if(bStealNote)
	{
		// steal oldest note
		CMiniSynthVoice* pVoice = auTrack_1_PlaybackInfo.m_VoicePtrStack1[0]; // always the oldest
		
		auTrack_1_PlaybackInfo.m_VoicePtrStack1.erase(auTrack_1_PlaybackInfo.m_VoicePtrStack1.begin());
		auTrack_1_PlaybackInfo.m_VoicePtrStack1.push_back(pVoice);
        
		pVoice->noteOn(notenumber, 60, midiMIDIFreqTable[notenumber], m_dLastNoteFrequency);
		pVoice->setSustainOverride(false);
    
        
		// save
		m_dLastNoteFrequency = midiMIDIFreqTable[notenumber];
	}
}

-(void)setNoteOff:(int)notenumber{
    auTrack_1_PlaybackInfo.m_bNoteOn = FALSE;
    
    // find and turn off
	auTrack_1_PlaybackInfo.m_VoiceIterator1 = auTrack_1_PlaybackInfo.m_VoicePtrStack1.begin();
    
	for(int i=0; i<MAX_VOICES; i++)
	{
		CMiniSynthVoice* pVoice = auTrack_1_PlaybackInfo.m_VoicePtrStack1[i];
		
		// find matching source/destination pairs
		if(pVoice->canNoteOff() && pVoice->m_uMIDINoteNumber == notenumber)
		{
			pVoice->noteOff(notenumber);
			

            
			// may have multiple notes sustaining; this ensures the oldest
			// note gets the event
			break;
		}
		
        
		auTrack_1_PlaybackInfo.m_VoiceIterator1++;
	}
}

// AU Callback method
static OSStatus auReadFileCallback(void *inRefCon,				/* pointer to userdata - our custom struct for callback */
								   AudioUnitRenderActionFlags *ioActionFlags, 
								   const AudioTimeStamp *inTimeStamp, /* the time stamp of the data, can be used for playbck time counting */
								   UInt32 inBusNumber,			/* the bus number */
								   UInt32 inNumberFrames,		/* the number of frames that AU wants us to deliver to it */
								   AudioBufferList *ioData)		/* the audio data buffers */
{	
	// get and cast our custom struct; we need it for the fileID to read from
	// and its packet counter variable
	AUPlayFileInfoStruct *auInfo = (AUPlayFileInfoStruct*)inRefCon;
	
	if(auInfo->m_bDone)
	{
		// silence the audio
		memset(ioData->mBuffers[0].mData, 0.0, inNumberFrames*4);
		return noErr;
	}
	
	// Initialize it to the number we want to read; it will be changed in the function
	// to the number of packets that were actually read.
	UInt32 nPackets = inNumberFrames;
	
	// the number of bytes that were read, returned from function call
	//UInt32 numBytesRead;
	
	// AudioStreamPacketDescription needed for method call, unused otherwise
 	//AudioStreamPacketDescription    *packetDescs;
	
    if(auInfo->m_AudioFileID)
    {
        // read out the packets into the output buffer; the output buffer is located at:
        // ioData->mBuffers[0].mData
        // it will be packed by whatever you specify in the desc; in this case it is interleaved L/R/L/R
        // 44.1k Stereo Signed-Integer data, straight from the wave file
        //
//        AudioFileReadPackets (auInfo->m_AudioFileID,			/* input fileID */
//                              false,						/* no cache */
//                              &numBytesRead,				/* num bytes actually read */
//                              packetDescs,					/* low level info about the packets */
//                              auInfo->m_CurrentPacket,		/* starting packet index */
//                              &nPackets,					/* num packets to attempt to read */
//                              ioData->mBuffers[0].mData);	/* the buffer of freshly read data */
    }
    else
    {
        // --- seek to MP3 decode point
        ExtAudioFileSeek(auInfo->m_MP3FileRef, auInfo->m_CurrentPacket);
        
        // --- decode and read into buffer
        ExtAudioFileRead(auInfo->m_MP3FileRef, &nPackets, ioData);
    }

	// inc the packet counter
	auInfo->m_CurrentPacket += nPackets;
	
	if(nPackets <= 0)
	{
		auInfo->m_bDone = true;
	}
	
	// here you can process the data in the buffer with a plug-in or sig proc function... access
	// it the same way as we did in the CAPlayer app
    AudioSampleType *buffer = (AudioSampleType *)ioData->mBuffers[0].mData;
	
	// each packet has 2 channels; L/R/L/R here
	int m=0;
	
    // --- coo or retrieve plug-in rocessing information
	
	for(int i = 0; i<nPackets; i++)
	{
		// do left channel; convert to float -1.0 to +1.0
        // do the LEFT processing - here is pass-thru
        double dLeftAccum = 0.0;
        double dRightAccum = 0.0;
        
        float fMix = 1.0/(float)auInfo->MAX_VOICES*2;
        
        CVoice* pVoice;
        for(int i=0; i<auInfo->MAX_VOICES; i++)
        {
            double dLeft, dRight;
            
            pVoice =  auInfo->m_VoicePtrStack1[i];
            pVoice->doVoice(dLeft, dRight);
            dLeftAccum += fMix*dLeft;

        }
        
        float fLeftAccum = float(dLeftAccum);
        float fRightAccum = float(dRightAccum);
		
		
		// write out; convert back to -32768 to +32767
		buffer[m] = (SInt16)((float)fLeftAccum*32767.0);
		
		// write out; convert back to -32768 to +32767
		buffer[m+1] = (SInt16)((float)fRightAccum*32768.0);
		
		m+=2;
	}

	// all good
	return noErr;
}

// The initialize method with a file to open and read
- (OSStatus)initPlaybackAUGraphWithFile:(NSString *)filename
                                 ofType:(NSString *)type
                           isCompressed:(bool)extFileType;
{
	// get the Path
	NSString *musicPath = [[NSBundle mainBundle] pathForResource:filename ofType:type];
	
	// get the URL
	NSURL *fileURL;
	if(musicPath)
	{
		// URL
		fileURL = [NSURL fileURLWithPath:musicPath];
	}
	else // bail!
	{
		return -1;
	}
	
	// Open the file:
	//
    auTrack_1_PlaybackInfo.m_MP3FileRef = nil;
    auTrack_1_PlaybackInfo.m_AudioFileID = nil;
    
    OSStatus status;
    
    if(extFileType)
    {
        // NOTE: if successful, this will set AudioFileID in the variable m_MP3FileRef = m_MP3FileRef*
        ExtAudioFileOpenURL((__bridge CFURLRef)fileURL, &auTrack_1_PlaybackInfo.m_MP3FileRef);
    }
    else
    {
     	// NOTE: if successful, this will set AudioFileID in the variable aqOutputInfo.m_AudioFileID = AudioFileID*
        status = AudioFileOpenURL((__bridge CFURLRef)fileURL,
                                  kAudioFileReadPermission,
                                  kAudioFileWAVEType,
                                  &auTrack_1_PlaybackInfo.m_AudioFileID);
    }

    // init
	auTrack_1_PlaybackInfo.m_CurrentPacket = 0;
    
    // save AudioStreamBasicDescription for setting up the AUGraph
    UInt32 propertySize = sizeof(auTrack_1_PlaybackInfo.m_CSABD);
    status = AudioFileGetProperty(auTrack_1_PlaybackInfo.m_AudioFileID, kAudioFilePropertyDataFormat, &propertySize, &auTrack_1_PlaybackInfo.m_CSABD);

	if(status != 0)
		; // error message

	// our return variable
	OSStatus result = noErr;
	
	// create a new AUGraph
	result = NewAUGraph(&m_AUGraph);
	
	// 1.	Define the AUNodes; each node is a AUPlug-In
	//		We need an output node and a mixer node
	AUNode outputNode;
	AUNode mixerNode;
	
	// 2.	Create AudioComponentDescriptions for the AUs we want in the graph
	//		An AU Plugin is defined by these 5 variables on the description struct
	// Mixer Description:
	AudioComponentDescription mixer_desc;
	mixer_desc.componentType = kAudioUnitType_Mixer;
	mixer_desc.componentSubType = kAudioUnitSubType_MultiChannelMixer;
	mixer_desc.componentFlags = 0;
	mixer_desc.componentFlagsMask = 0;
	mixer_desc.componentManufacturer = kAudioUnitManufacturer_Apple;
	
	//  Ouput Description
	AudioComponentDescription output_desc;
	output_desc.componentType = kAudioUnitType_Output;
	output_desc.componentSubType = kAudioUnitSubType_RemoteIO;
	output_desc.componentFlags = 0;
	output_desc.componentFlagsMask = 0;
	output_desc.componentManufacturer = kAudioUnitManufacturer_Apple;
	
	// 3.	Add nodes to the graph to hold our AudioUnits,
	//		You pass in a reference to the  AudioComponentDescription
	//		and get back an  AudioUnit (Node)
	result = AUGraphAddNode(m_AUGraph, &output_desc, &outputNode);
	result = AUGraphAddNode(m_AUGraph, &mixer_desc, &mixerNode );
	
	// The graph looks like this: 
	//	MIXER-->OUTPUT
	//
	//  We will supply the function that will feed data into the mixer:
	//	(Callback Function reads data from file)-->MIXER-->OUTPUT
	// 
	//	The output will request data from the mixer, which will in turn
	//	request data from the callback function we write. This is called
	//	a data pull. The data is pulled from the output backwards
	//
	//	(Callback Function reads data from file)<--MIXER<--OUTPUT
	//
	//	rather than being pushed into it (forward).
	//	WindowsOS works exactly the same way
	//
	// 4.	Connect the mixer node's output to the output node's input
	result = AUGraphConnectNodeInput(m_AUGraph, mixerNode, 0, outputNode, 0);
	
	// 6.	open the graph 
	result = AUGraphOpen(m_AUGraph);
	
	// 6.	Get a pointer to the mixer AU (&m_AUMixer) so we can use it later
	result = AUGraphNodeInfo(m_AUGraph, mixerNode, NULL, &m_AUMixer);
	
	// 7.	Set the number of input busses on the Mixer Unit
	//		1 File= 1 Buss (mono or stereo is determined later)
	UInt32 numbuses = 1;
	UInt32 size = sizeof(numbuses);
	
	// we will use SetProperty often; here we set the number of busses in the Input (kAudioUnitScope_Input)
	result = AudioUnitSetProperty(m_AUMixer, kAudioUnitProperty_ElementCount, kAudioUnitScope_Input, 0, &numbuses, size);
	
	// a stream description to describe the audio data (stereo, 44.1, 16 bit, etc...)
	CAStreamBasicDescription desc;
	
	// Loop through and setup a callback for each source you want to send to the mixer.
	// You can have a separate callback function for each source into the mixer, or use one
	// callback.
	//
	// Like the AudioQueue, we define a custom struct and pass it to the system
	// which passes it back to us during the callback operation. That struct is 
	// key to making an elegant system.
	for (int i = 0; i < numbuses; ++i) 
	{
		// 8.	Setup the callback and node info:
		//
		// This struct describes the function that will be called
		// to provide a buffer of audio samples for the mixer unit.
		AURenderCallbackStruct renderCallbackStruct;
		
		// the name of our callback is auReadFileInput
		renderCallbackStruct.inputProc = &auReadFileCallback;
		
		// here is where we give it the pointer to our struct so it can pass
		// it back to us in the callback
		renderCallbackStruct.inputProcRefCon = &auTrack_1_PlaybackInfo;

		// Set a callback for the specified node's specified input (i)
		result = AUGraphSetNodeInputCallback(m_AUGraph, mixerNode, i, &renderCallbackStruct);
    
        // set the this pointer in the struct; I don't use it here, but you might
		// set up a system that needs it (be careful - this is dangerous, but doable hack)
		auTrack_1_PlaybackInfo.m_bDone = false;
		
        /* From APPLE Docs for Mixer AU ---
         
         On the input scope, manage stream formats as follows:
        
         - If an input bus is fed by an audio unit connection, it acquires its stream format from that connection.
        
         - If an input bus is fed by a render callback function, set your complete application stream format on the bus. Use the same stream format as used for the data provided by the callback.
         */
        
        if(extFileType)
        {
            // --- we have to set up manually for compressed files
            CAStreamBasicDescription inputCASBD;

            // --- setup the Mixer casbd to default wave file
            size = sizeof(inputCASBD);
            result = AudioUnitGetProperty(m_AUMixer,
                                          kAudioUnitProperty_StreamFormat,
                                          kAudioUnitScope_Input,
                                          i,
                                          &inputCASBD,
                                          &size);
            
            // setup the CAStreamBasicDescription for this particular input node (file for us)
            inputCASBD.mSampleRate = 44100;				// sample rate
            inputCASBD.mFormatID = kAudioFormatLinearPCM;		// WAV = PCM
            inputCASBD.mFormatFlags      = kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked;	// integer and packed, just the same as AudioQueue
            inputCASBD.mBitsPerChannel = 16;	// I know it's 16bit here...
            
            inputCASBD.mChannelsPerFrame = 2;	// stereo
            inputCASBD.mFramesPerPacket = 1;	// 1 = uncompressed, 0 = compressed
            inputCASBD.mBytesPerFrame = (inputCASBD.mBitsPerChannel / 8) * inputCASBD.mChannelsPerFrame;
            inputCASBD.mBytesPerPacket = inputCASBD.mBytesPerFrame * inputCASBD.mFramesPerPacket;
            
            // --- Set the CAStreamBasicDescription on the mixer input for this buss
            result = AudioUnitSetProperty(m_AUMixer,
                                          kAudioUnitProperty_StreamFormat,
                                          kAudioUnitScope_Input,
                                          i,
                                          &inputCASBD,
                                          sizeof(inputCASBD));
            
            // --- tell the MP3 decoder to use this same CASBD to make them match
            ExtAudioFileSetProperty(auTrack_1_PlaybackInfo.m_MP3FileRef,
                                    kExtAudioFileProperty_ClientDataFormat,
                                    sizeof (AudioStreamBasicDescription),
                                    &inputCASBD);

         }
        else
        {
            // Set the CAStreamBasicDescription on the mixer input for this buss that we saved from
            // the file
            result = AudioUnitSetProperty(m_AUMixer,
                                          kAudioUnitProperty_StreamFormat,
                                          kAudioUnitScope_Input,
                                          i,
                                          &auTrack_1_PlaybackInfo.m_CSABD,
                                          sizeof(auTrack_1_PlaybackInfo.m_CSABD));
        }
    }
    
    /* From APPLE Docs for Mixer AU ---
     
     On the output scope, set just the application sample rate.*/
    
    // BUT - if you have a compressed file, you need to set this manually
    if(extFileType)
    {
        // Our output bus will have the same value: stereo, 16 bit, 44.1kHz, uncompressed data
        // which is the default (and ONLY) output type in iOS
        //
        // Set the CAStreamBasicDescription on the output buss
        CAStreamBasicDescription ouutputCASBD;
        
        // setup the CAStreamBasicDescription for the output node
        ouutputCASBD.mSampleRate = 44100;					// sample rate
        ouutputCASBD.mFormatID = kAudioFormatLinearPCM;		// WAV = PCM
        ouutputCASBD.mFormatFlags      = kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked;	// integer and packed, just the same as AudioQueue
        ouutputCASBD.mBitsPerChannel = 16;	// Must be stereo, 16 bit audio
        
        ouutputCASBD.mChannelsPerFrame = 2;	// stereo
        ouutputCASBD.mFramesPerPacket = 1;		// 1 = uncompressed, 0 = compressed
        ouutputCASBD.mBytesPerFrame = ( ouutputCASBD.mBitsPerChannel / 8 ) * ouutputCASBD.mChannelsPerFrame;
        ouutputCASBD.mBytesPerPacket = ouutputCASBD.mBytesPerFrame * ouutputCASBD.mFramesPerPacket;
        
        // set it
        result = AudioUnitSetProperty(m_AUMixer,
                                      kAudioUnitProperty_StreamFormat,
                                      kAudioUnitScope_Output,
                                      0,
                                      &ouutputCASBD,
                                      sizeof(ouutputCASBD));
    }
    else
    {
        // set sample rate as per docs
        double inSampleRate = auTrack_1_PlaybackInfo.m_CSABD.mSampleRate;
        
        result = AudioUnitSetProperty(m_AUMixer,
                                      kAudioUnitProperty_SampleRate, /// <-- note!
                                      kAudioUnitScope_Output,
                                      0,
                                      &inSampleRate,
                                      sizeof(inSampleRate));
    }

	
	// Once everything is set up call initialize to validate connections
	result = AUGraphInitialize(m_AUGraph);
    
	// our return variable
	return result;
}

// Start the AU Graph
- (void)startAUGraph
{
	// Start the AUGraph
	OSStatus result = AUGraphStart(m_AUGraph);
	
	// log the result
	NSLog(@"startAUGraph  result %d %08X %4.4s\n", (int)result, (int)result, (char*)&result);
}

// stops render
- (void)stopAUGraph
{
    Boolean isRunning = false;
	
    // Check to see if the graph is running.
    OSStatus result = AUGraphIsRunning(m_AUGraph, &isRunning);
	
	// If the graph is running, stop it.
    if (isRunning) 
	{
		// stop eet
        result = AUGraphStop(m_AUGraph);
		
		// close the file
		AudioFileClose(auTrack_1_PlaybackInfo.m_AudioFileID);
		
		// log the result
		NSLog(@"stopAUGraph  result %d %08X %4.4s\n", (int)result, (int)result, (char*)&result);
    }
}



@end
