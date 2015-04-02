//
//  AudioUnitsEngine.h
//  AUPlayer
//
//  Created by willpirkle on 4/18/11.
//  Copyright 2011 University of Miami. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MiniSynthVoice.h"
#import "AYAVoiceThread.h"
#import "RLAudioEffect.h"
#import "AEAudioController.h"
#import "AEBlockChannel.h"


@interface AudioUnitsEngine : NSObject 
{
    @public
    
    std::vector<CMiniSynthVoice*> m_VoicePtrStack1;
    std::vector<CMiniSynthVoice*>::iterator m_VoiceIterator1;
    
    std::vector<AYAVoiceThread *> m_VoiceThreadStack;
    
    double m_dLastNoteFrequency;
    
    UINT MAX_VOICES;
    
    // Array to hold all of the plugins
    NSMutableArray* effectsArray;
    
    // Dictionary to help with organization of effects
    NSMutableDictionary* effectsDictionary;
    
    // This is maybe faster then asking the array for it's count every callback?
    float numEffectsInArray;
}


@property (nonatomic) AEAudioController *audioController;
// our designated init-er
- (id)init;

+ (id)getAudioUnitsEngine;
// init the AUGraph with a file to playback:

// start rendering audio
- (void)startAUGraph;

// stop rendering audio
- (void)stopAUGraph;

-(void)setNoteOn:(int)notenumber;
-(void)setNoteOff:(int)notenumber;

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
