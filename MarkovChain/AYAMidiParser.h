//
//  AYAMidiParser.h
//  MarkovChain
//
//  Created by Andrew Ayers on 4/25/14.
//  Copyright (c) 2014 AyersAudio. All rights reserved.
//

// MidiParser.h

#import <Foundation/Foundation.h>

typedef enum tagMidiTimeFormat
{
    MidiTimeFormatTicksPerBeat,
    MidiTimeFormatFramesPerSecond
} MidiTimeFormat;

@interface AYAMidiParser : NSObject
{
    NSMutableString *log;
    NSData *data;
    NSUInteger offset;
    
    UInt16 format;
    UInt16 trackCount;
    MidiTimeFormat timeFormat;
    
    UInt16 ticksPerBeat;
    UInt16 framesPerSecond;
    UInt16 ticksPerFrame;
    
    NSMutableArray *noteEvents;

}

@property (nonatomic, retain) NSMutableString *log;

@property (readonly) UInt16 format;
@property (readonly) UInt16 trackCount;
@property (readonly) MidiTimeFormat timeFormat;

- (NSArray *) parseData: (NSData *) midiData;

@end
