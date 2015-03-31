//
//  AYAMidiToMarkovCalc.m
//  MarkovChain
//
//  Created by Andrew Ayers on 4/25/14.
//  Copyright (c) 2014 AyersAudio. All rights reserved.
//

#import "AYAMidiToMarkovCalc.h"

@implementation AYAMidiToMarkovCalc

+(NSMutableDictionary *)calcProbabilityForFile:(NSString *)filename{
    // Create our MIDI Parsing Object
    AYAMidiParser* midiParse = [[AYAMidiParser alloc] init];
    // get back the array of midi Note Events
    NSArray* noteEvents = [midiParse parseData:[NSData dataWithContentsOfFile:[[NSBundle mainBundle] pathForResource:filename ofType:@".mid"]]];
    // Sort the events by timestamp.
    // Create a sort descriptor
    NSSortDescriptor *sortByName = [NSSortDescriptor sortDescriptorWithKey:@"timestamp"
                                                                 ascending:YES];
    // Sort the array with the descriptor
    NSArray *sortedArray = [noteEvents sortedArrayUsingDescriptors:@[sortByName]];
    
    // Sort the modulo 12 to put all the notes in the same octave
    for (int i=0; i<[sortedArray count]; i++) {
        [sortedArray[i] setValue:@([sortedArray[i][@"noteNum"]intValue] %12) forKey:@"noteNum"];
    }
    
    // create a set with all the distinct times in the array
    NSOrderedSet *timeStamps = [NSOrderedSet  orderedSetWithArray:[sortedArray valueForKey:@"timestamp"]];

    //create the dictionary that will store all of the note counts
    NSMutableDictionary *markovDict = [[NSMutableDictionary alloc] init];
    
    //iterate through every note event.
    for (int i=0; i<[timeStamps count]-1; i++) {
        // Get the current timestamp
        NSNumber *currentTimestamp = timeStamps[i];
        // create a predicate to sort them
        NSPredicate *currentTimePredicate = [NSPredicate predicateWithFormat:@"%K == %@", @"timestamp", currentTimestamp];
        // Get a list of notes for the current timestamp
        NSArray *notesHappeningAtCurrentTimestamp = [NSArray arrayWithArray:[sortedArray filteredArrayUsingPredicate:currentTimePredicate]];
        
        // Get the next timestamp
        NSNumber *nextTimestamp = timeStamps[i+1];
        //Create a predicate to filter based on the key (@%K means key) timestamp, and the value nextTimestamp
        NSPredicate *nextTimePredicate = [NSPredicate predicateWithFormat:@"%K == %@", @"timestamp", nextTimestamp];
        // and output a new array with just the filtered results
        NSArray *notesHappeningAtNextTimestamp = [NSMutableArray arrayWithArray:[sortedArray filteredArrayUsingPredicate:nextTimePredicate]];
        
        //iterate over the set of notes happening right now
        for (int j=0; j<[notesHappeningAtCurrentTimestamp count]; j++) {
            int fromNote = [notesHappeningAtCurrentTimestamp[j][@"noteNum"] intValue];
            // if the top level dictionary doesn't contain an entry for this note, we need to make a new dictionary
            if (![markovDict objectForKey:@(fromNote)]) {
                NSMutableDictionary *probs = [[NSMutableDictionary alloc] init];
                // and add it to the top level dictionary
                [markovDict setObject:probs forKey:@(fromNote)];
                
                for (NSDictionary *dict in notesHappeningAtNextTimestamp) {
                    if (![probs objectForKey:dict[@"noteNum"]]) {
                        [probs setObject:@(1) forKey:dict[@"noteNum"]];
                    }else{
                        // or if we've already seen this note, we need to update the count plus one.
                        NSNumber *currentCount = [probs objectForKey:dict[@"noteNum"]];
                        NSNumber *newCount = @([currentCount intValue] + 1);
                        [probs setObject:newCount forKey:dict[@"noteNum"]];
                    }
                }
            }else{
                NSMutableDictionary *probs = [markovDict objectForKey:@(fromNote)];
                
                for (NSDictionary *dict in notesHappeningAtNextTimestamp) {
                    if (![probs objectForKey:dict[@"noteNum"]]) {
                        [probs setObject:@(1) forKey:dict[@"noteNum"]];
                    }else{
                        // or if we've already seen this note, we need to update the count plus one.
                        NSNumber *currentCount = [probs objectForKey:dict[@"noteNum"]];
                        NSNumber *newCount = @([currentCount intValue] + 1);
                        [probs setObject:newCount forKey:dict[@"noteNum"]];
                    }
                }
            }
        }
    }
    for (NSNumber *note in markovDict) {
        NSDictionary *dict = markovDict[note];
        float sum = 0;
        for (NSString *note in [dict allKeys]) {
            sum += [dict[note] intValue];
        }
        for (NSString *note in [dict allKeys]) {
            [dict setValue:@([dict[note] floatValue]/sum) forKey:note];
        }
    }
    
    
    return markovDict;
}

@end
