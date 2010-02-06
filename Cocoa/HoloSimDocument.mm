//
//  MyDocument.m
//  HoloSim
//
//  Created by Veljko Krunic on 4/8/07.
//  Copyright © Veljko Krunic 2007-2010. All rights reserved.
//

#import "HoloSimDocument.h"
#import "HoloDeckView.h"

#include "Constants.h"

@implementation HoloSimDocument

/**
 * Constructor
 *
 * @return Self
 */
- (id)init
{
   self = [super init];
   if (self) 
   {
      mouseRotationSpeed = MIN_MOUSE_ROTATION_SPEED;
      mouseZoomSpeed = MIN_MOUSE_WHEEL_RATIO;
      interframeDistance = DEFAULT_INTERFRAME_DISTANCE;
       
      // Add your subclass-specific initialization here.
      // If an error occurs here, send a [self release] message and return nil.
      // Create model
      model = new GPUInterpolatedModel();
   }
    
   return self;
}

/**
 * Get name of the NIB file
 *
 * @return name of the NIB file
 */
- (NSString *)windowNibName
{
    return @"HoloSimDocument";
}

/**
 * Callback notifying that loading NIB file is done
 *
 * @param aController Window controller
 */
- (void)windowControllerDidLoadNib:(NSWindowController *) aController
{
   [super windowControllerDidLoadNib:aController];
   // Add any code here that needs to be executed once the windowController has loaded the document's window.
}

/**
 * Read from given file
 *
 * @param absoluteURL Name of the file
 * @param typeName Type of the file
 * @param outError Error description that occured
 *
 * @return true if file reading was a success
 */
- (BOOL)readFromURL:(NSURL *)absoluteURL ofType:(NSString *)typeName error:(NSError **)outError
{
   // Convert input
   NSString *name = [absoluteURL path];

   // Now read model in the C style
   FILE *fp = fopen([name UTF8String], "r");
   if (!fp)
      return NO;
   
   BOOL result = model->readFromFile(fp);
   fclose(fp);
   
   if (!result)
   {
      *outError = [NSError errorWithDomain:@"Not implemented yet" code:0 userInfo:[NSDictionary dictionaryWithObject:@"Test" forKey:@"key"]];
      return NO;
   }
   
   return YES;
}

/**
 * Read from file of the given type
 *
 * @param fileName name of the file to read from
 * @param docType type of the file to read from
 *
 * @return Was load success  
 */
- (BOOL)readFromFile:(NSString *)fileName ofType:(NSString *)docType
{
   FILE *fp = fopen([fileName UTF8String], "r");
   BOOL result = model->readFromFile(fp);
   fclose(fp);

   return result;
}

/**
 * Write to given file
 *
 * @param absoluteURL Name of the file
 * @param typeName Type of the file
 * @param outError Error description that occured
 *
 * @return true if file write was a success
 */
- (BOOL)writeToURL:(NSURL *)absoluteURL ofType:(NSString *)typeName error:(NSError **)outError
{
    *outError = [NSError errorWithDomain:@"Not implemented yet" code:0 userInfo:[NSDictionary dictionaryWithObject:@"Test" forKey:@"key"]];
    return NO;
}

// Public methods

-(AbstractModel *)model
{
   return model;
}

- (IBAction)timeSliceSliderChanged:(id)sender
{
   model->setTimeSlice([sender floatValue]);
   [rodRoomWindow notifyModelChanged:self];
}

- (void)setInterframeDistance:(double)duration;
{
   interframeDistance = duration;
}

- (double)interframeDistance
{
   return interframeDistance;
}

- (void)setMouseZoomSpeed:(double)speed
{
   mouseZoomSpeed = speed;
}

- (double)mouseZoomSpeed
{
   return mouseZoomSpeed;
}

- (void)setMouseRotationSpeed:(double)speed
{
   mouseRotationSpeed = speed;
}

- (double)mouseRotationSpeed
{
   return mouseRotationSpeed;
}

- (void)setLoopAnimation:(BOOL)loop
{
   loopAnimation = loop;
}

- (BOOL)loopAnimation
{
   return loopAnimation;
}

- (IBAction)mouseZoomSpeedChanged:(id)sender
{
	[self setMouseZoomSpeed:[sender floatValue]];
}

- (IBAction)mouseRotationSpeedChanged:(id)sender
{
   [self setMouseRotationSpeed:[sender floatValue]];
}

- (IBAction)loopAnimationChanged:(id)sender
{
   [self setLoopAnimation:[sender boolValue]];
}

@end
