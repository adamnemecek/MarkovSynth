//
//  AudioUnitsEngine.h
//  AUPlayer
//
//  Created by willpirkle on 4/18/11.
//  Copyright 2011 University of Miami. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AudioToolbox/AudioToolbox.h>
#import "CAStreamBasicDescription.h"
#import "MiniSynthVoice.h"
//For Delay
//#include "DDLModule.h"
//For Reverb
//#include "DattoroPlate.h"

//#import "MiniSynthObject.h"

// a custom struct to hold info for playing a file through AU
struct AUPlayFileInfoStruct {
    AudioFileID                     m_AudioFileID;		/* ID of audio file */
    ExtAudioFileRef                 m_MP3FileRef;	    /* for MP3/other compressed files */
    SInt64                          m_CurrentPacket;	/* packet counter */
	bool							m_bDone;			/* done reading data */
    CAStreamBasicDescription        m_CSABD;
    
    std::vector<CMiniSynthVoice*> m_VoicePtrStack1;
	std::vector<CMiniSynthVoice*>::iterator m_VoiceIterator1;
    bool                            m_bNoteOn;
    UINT                    m_uTimbreSelection;
    UINT                            MAX_VOICES;
};

@interface AudioUnitsEngine : NSObject 
{
	// struct for one track (mono or stereo) of playback from a file
	AUPlayFileInfoStruct auTrack_1_PlaybackInfo;
	
	// the AU Graph - a conceptual arrangement of plugins
	AUGraph   m_AUGraph;
	
	// the Mixer plugin
	AudioUnit m_AUMixer;
    
    double m_dLastNoteFrequency;
    
    UINT MAX_VOICES;
}

@property AUPlayFileInfoStruct auTrack_1_PlaybackInfo;
// our designated init-er
- (id)init;

// init the AUGraph with a file to playback:
- (OSStatus)initPlaybackAUGraphWithFile:(NSString *)filename
                                 ofType:(NSString *)type
                          isCompressed:(bool)extFileType;

// start rendering audio
- (void)startAUGraph;

// stop rendering audio
- (void)stopAUGraph;

-(void)setOscillatorFO:(float)fo;
-(void)setOscillatorWave:(int)wave;
-(void)setNoteOn:(int)notenumber;
-(void)setNoteOff:(int)notenumber;
-(void)setTimbreSelection:(UINT)sel;

@end
