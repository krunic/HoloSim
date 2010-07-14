/*
 * HoloSim, visualization and control of the moxel based environment.
 *
 * Copyright (C) 2010 Veljko Krunic
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
 * Get associated app controller
 * 
 * @return Application controller
 */
- (AppController *)appController
{
   return appController;
}

/**
 * Set associated app controller
 *
 * @param controller Associated app controller
 */
- (void)setAppController:(AppController *)controller
{
   appController = controller;
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

   BOOL result = model->readFromFile([name UTF8String]);
   if (!result)
   {
      *outError = [NSError errorWithDomain:@"Error opening file" code:0 userInfo:[NSDictionary dictionaryWithObject:@"Test" forKey:@"key"]];
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
   BOOL result = model->readFromFile([fileName UTF8String]);

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
   float value = [sender floatValue];
   model->setTimeSlice(value);
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

- (void)setOptimizeDrawingThreshold:(int)threshold
{
   optimizeDrawingThreshold = threshold;
}

- (int)optimizeDrawingThreshold
{
   return optimizeDrawingThreshold;
}

- (void)setLoopAnimation:(BOOL)loop
{
   loopAnimation = loop;
}

- (BOOL)loopAnimation
{
   return loopAnimation;
}

- (void)setOptimizeDrawing:(BOOL)optimize
{
   optimizeDrawing = optimize;
}

- (BOOL)optimizeDrawing
{
   return optimizeDrawing;
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

- (void)setLogPerformance:(BOOL)log
{
   logPerformance = log;
}

- (BOOL)logPerformance
{
   return logPerformance;
}

@end
