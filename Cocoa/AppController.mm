//
//  AppController.m
//  HoloSim
//
//  Created by Veljko Krunic on 8/6/07.
//  Copyright 2007 Veljko Krunic. All rights reserved.
//

#import "AppController.h"
#import "HoloSimDocument.h"

#include "Constants.h"

@implementation AppController

- (IBAction)showPreferencePanel:(id)sender
{
	[preferenceController showWindow:self];
}

/**
 * Calculate distance between two frames in animation
 *
 * @param normalizedValue Interpolation factor between 0 (for start keyframe) and 1 (for end keyframe)
 *
 * @return Interframe distance between two frames for the given frame rate (distance that [0, 1] interpolation parameter
                                                                            * changes between frames)
 */
- (double)calcInterframeDistanceFromValue:(double)normalizedValue
{
   // Find duration that corresponds to this value. This are values that must be synchronized with redraw
   double animationDuration =  MIN_ANIMATION_DURATION + normalizedValue*ANIMATION_DURATION_RANGE;
   return 1.0/(animationDuration * FRAMES_PER_SECOND);
}

/**
 * Calculate distance between two frames in animation
 *
 * @param slider Slider to use
 *
 * @return Interframe distance between two frames for the given frame rate (distance that [0, 1] interpolation parameter
 * changes between frames)
 */
- (double)calcInterframeDistance:(id)slider
{
   // Normalize sender duration on [0, 1] interval where 1 means "fastest" 0 means "slowest"
   double normalizedValue = ([slider maxValue] - [slider floatValue] + [slider minValue])/([slider maxValue] - [slider minValue]);
	return [self calcInterframeDistanceFromValue:normalizedValue];
}

- (IBAction)animationDurationChanged:(id)sender
{
   NSEnumerator *e = [[[NSApplication sharedApplication] orderedDocuments] objectEnumerator];
   
   interframeDistance = [self calcInterframeDistance:sender];

   HoloSimDocument *document;
   while (document = [e nextObject]) 
      [document setInterframeDistance:interframeDistance];
}

- (IBAction)mouseZoomRatioChanged:(id)sender
{
   NSEnumerator *e = [[[NSApplication sharedApplication] orderedDocuments] objectEnumerator];
   zoomRatio = [sender floatValue];
   
   HoloSimDocument *document;
   while (document = [e nextObject]) 
      [document setMouseZoomSpeed:zoomRatio];
}

- (IBAction)mouseRotationRatioChanged:(id)sender
{
   NSEnumerator *e = [[[NSApplication sharedApplication] orderedDocuments] objectEnumerator];
   rotationRatio = [sender floatValue];
   
   HoloSimDocument *document;
   while (document = [e nextObject]) 
      [document setMouseRotationSpeed:rotationRatio];
}

- (IBAction)loopAnimationChanged:(id)sender
{
   NSEnumerator *e = [[[NSApplication sharedApplication] orderedDocuments] objectEnumerator];
   loopAnimation = [sender state] == NSOnState;
   
   HoloSimDocument *document;
   while (document = [e nextObject]) 
      [document setLoopAnimation:loopAnimation];
}

/**
* Prepare document with all parameters in the view
 *
 * @param document Document to use
 */
- (void)prepareDocumentParameters:(HoloSimDocument *)document
{
   [document setLoopAnimation:loopAnimation];
   [document setMouseZoomSpeed:zoomRatio];
   [document setMouseRotationSpeed:rotationRatio];
   [document setInterframeDistance:interframeDistance];
}

/**
 * Awake from NIB. Will set limits of control movement based on limits in other UI elements
 */
- (void)awakeFromNib
{
   zoomRatio = [[NSUserDefaults standardUserDefaults] floatForKey:MOUSE_WHEEL_RATIO_KEY];
   // Deal with no key present
   if (zoomRatio < MIN_MOUSE_WHEEL_RATIO)
   {
		[[NSUserDefaults standardUserDefaults] setFloat:MIN_MOUSE_WHEEL_RATIO forKey:MOUSE_WHEEL_RATIO_KEY];
     	zoomRatio = MIN_MOUSE_WHEEL_RATIO;
   }
  
   rotationRatio = [[NSUserDefaults standardUserDefaults] floatForKey:MOUSE_ROTATION_SPEED_KEY];
   if (rotationRatio < MIN_MOUSE_ROTATION_SPEED)
   {
      [[NSUserDefaults standardUserDefaults] setFloat:MIN_MOUSE_ROTATION_SPEED forKey:MOUSE_ROTATION_SPEED_KEY];
      rotationRatio = MIN_MOUSE_ROTATION_SPEED;
   }
     
	loopAnimation = [[NSUserDefaults standardUserDefaults] integerForKey:LOOP_ANIMATION_KEY];
   
   double animationDurationOnScale = [[NSUserDefaults standardUserDefaults] floatForKey:ANIMATION_SPEED_KEY];
   if (animationDurationOnScale < MIN_ANIMATION_DURATION_ON_SCALE)
   {
      [[NSUserDefaults standardUserDefaults] setFloat:MIN_ANIMATION_DURATION_ON_SCALE forKey:ANIMATION_SPEED_KEY];
      animationDurationOnScale = MIN_ANIMATION_DURATION_ON_SCALE;
   }
   
   // Renormalize between 0 and 1
   double normalizedValue = (MAX_ANIMATION_DURATION_ON_SCALE - animationDurationOnScale + MIN_ANIMATION_DURATION_ON_SCALE)/(MAX_ANIMATION_DURATION_ON_SCALE - MIN_ANIMATION_DURATION_ON_SCALE);
   normalizedValue = normalizedValue > 1 ? 1.0 : normalizedValue;
   normalizedValue = normalizedValue < 0 ? 0 : normalizedValue;
   
   interframeDistance = [self calcInterframeDistanceFromValue:normalizedValue];
   
   NSEnumerator *e = [[[NSApplication sharedApplication] orderedDocuments] objectEnumerator];
   HoloSimDocument *document;
   
   while (document = [e nextObject]) 
      [self prepareDocumentParameters:document];
}

/**
 * Delegate method saying should we open untitled file
 *
 * @param theApplication Application object that has invoked us
 *
 * @return NO to prevent creation of untitled file
 */
- (BOOL)applicationShouldOpenUntitledFile:(NSApplication *)theApplication
{
   return NO;
}

/**
 * Delegate method that would be invoked when new document is created. That way, new 
 * document would immediatelly be notified of the parameters that are in database
 *
 * @param theApplication Application object that has invoked us
 * @param fileName File to open
 *
 * @return NO to prevent creation of untitled file
 */
- (BOOL)application:(NSApplication *)theApplication
           openFile:(NSString *)fileName
{
   NSDocumentController *dc = [NSDocumentController sharedDocumentController];
   HoloSimDocument *doc = [dc openDocumentWithContentsOfFile:fileName display:YES];
   
   // Set parameters for document opened
   [self prepareDocumentParameters:doc];
   
   return doc != nil;
}

@end
