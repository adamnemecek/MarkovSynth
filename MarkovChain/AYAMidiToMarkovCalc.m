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
    
//    for (int i=0; i<[sortedArray count]; i++) {
//        [sortedArray[i] setValue:@([sortedArray[i][@"noteNum"]intValue] %12) forKey:@"noteNum"];
//    }
    //create the dictionary that will store all of the note counts
    NSMutableDictionary *markovDict = [[NSMutableDictionary alloc] init];
    
    //iterate through every note event.
    for (int i =0; i< [sortedArray count]-1; i++) {
        // if the top level dictionary doesn't contain an entry for this note, we need to make a new dictionary
        if (![markovDict objectForKey:sortedArray[i][@"noteNum"]]) {
            NSMutableDictionary *probs = [[NSMutableDictionary alloc] init];
            // and add it to the top level dictionary
            [markovDict setObject:probs forKey:sortedArray[i][@"noteNum"]];
            
            // Then we need to see what the next timestamp is
            NSNumber* nextTimestamp = sortedArray[i+1][@"timestamp"];
            
            /*
             Using the next timestamp, we make a list of all the notes that happen at the next timestamp, the reasoning for this is if we have a single note that goes to a chord, we want to consider all memebers of the chord as being viable transitions from the first note.
             */
            
            //Create a predicate to filter based on the key (@%K means key) timestamp, and the value nextTimestamp
            NSPredicate *predicateString = [NSPredicate predicateWithFormat:@"%K == %@", @"timestamp", nextTimestamp];
            // and output a new array with just the filtered results
            NSArray *filteredArray = [NSMutableArray arrayWithArray:[sortedArray filteredArrayUsingPredicate:predicateString]];
            
            // Now for each note that happens next, we need to either insert it into the notes dictionary of transitions
            for (NSDictionary *dict in filteredArray) {
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
            NSMutableDictionary *probs = [markovDict objectForKey:sortedArray[i][@"noteNum"]];
            
            NSNumber* nextTimestamp = sortedArray[i+1][@"timestamp"];
            
            //Create a predicate to filter based on the key (@%K means key) timestamp, and the value nextTimestamp
            NSPredicate *predicateString = [NSPredicate predicateWithFormat:@"%K == %@", @"timestamp", nextTimestamp];
            // and output a new array with just the filtered results
            NSArray *filteredArray = [NSMutableArray arrayWithArray:[sortedArray filteredArrayUsingPredicate:predicateString]];
            
            // Now for each note that happens next, we need to either insert it into the notes dictionary of transitions
            for (NSDictionary *dict in filteredArray) {
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
