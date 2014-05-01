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

#import "RLAudioEffect.h"


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
    
    NSMutableArray*                 m_effectsArrayCopy;

};

@interface AudioUnitsEngine : NSObject 
{
    @public
	// struct for one track (mono or stereo) of playback from a file
	AUPlayFileInfoStruct auTrack_1_PlaybackInfo;
	
	// the AU Graph - a conceptual arrangement of plugins
	AUGraph   m_AUGraph;
	
	// the Mixer plugin
	AudioUnit m_AUMixer;
    
    double m_dLastNoteFrequency;
    
    UINT MAX_VOICES;
    
    // Array to hold all of the plugins
    NSMutableArray* effectsArray;
    
    // Dictionary to help with organization of effects
    NSMutableDictionary* effectsDictionary;
    
    // This is maybe faster then asking the array for it's count every callback?
    float numEffectsInArray;
}

@property AUPlayFileInfoStruct auTrack_1_PlaybackInfo;
// our designated init-er
- (id)init;

+ (id)getAudioUnitsEngine;
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


/** Add a effect or effect group to the array of effects to be processed in Series
 @param effect - the object to add to the array
 @param key - a key to retrieve the object from the group
 */
- (void) addEffectorEffectGroupToArray:(RLAudioEffect *)effect forKey:(NSString *)key;

/** Remove a effect or effect group to the array of effects to be processed in Series
 @param key - the key used to add the object to the array;
 */
- (void) removeEffectFromArrayForKey:(NSString *)key;

/** Get the pointer from the Array for a given Key used to add the effect or effect group to the array
 @param ket - the key used to add the object to the array;
 */
-(RLAudioEffect *)retrieveEffectFromGroupForKey:(NSString *)key;

@end
