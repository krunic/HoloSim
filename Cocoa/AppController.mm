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

- (IBAction)optimizeDrawingChanged:(id)sender
{
   NSEnumerator *e = [[[NSApplication sharedApplication] orderedDocuments] objectEnumerator];
   optimizeDrawing = [sender state] == NSOnState;
   
   HoloSimDocument *document;
   while (document = [e nextObject]) 
      [document setOptimizeDrawing:optimizeDrawing];
}

- (IBAction)optimizeDrawingThresholdChanged:(id)sender
{
   NSEnumerator *e = [[[NSApplication sharedApplication] orderedDocuments] objectEnumerator];
   optimizeDrawingThreshold = [sender intValue];
   
   HoloSimDocument *document;
   while (document = [e nextObject]) 
      [document setOptimizeDrawingThreshold:optimizeDrawingThreshold];
}

- (IBAction)logPerformanceChanged:(id)sender
{
   NSEnumerator *e = [[[NSApplication sharedApplication] orderedDocuments] objectEnumerator];
   logPerformance = [sender state] == NSOnState;
   
   HoloSimDocument *document;
   while (document = [e nextObject]) 
      [document setLogPerformance:logPerformance];
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
   [document setOptimizeDrawing:optimizeDrawing];
   [document setOptimizeDrawingThreshold:optimizeDrawingThreshold];
   [document setLogPerformance:logPerformance];
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
   
   optimizeDrawing = [[NSUserDefaults standardUserDefaults] integerForKey:OPTIMIZE_DRAWING_KEY];
   
   optimizeDrawingThreshold = [[NSUserDefaults standardUserDefaults] integerForKey:OPTIMIZE_THRESHOLD_KEY];
   if (optimizeDrawingThreshold < MIN_OPTIMIZE_THRESHOLD)
   {
      [[NSUserDefaults standardUserDefaults] setFloat:MIN_OPTIMIZE_THRESHOLD forKey:OPTIMIZE_THRESHOLD_KEY];
      optimizeDrawingThreshold = MIN_OPTIMIZE_THRESHOLD;
   }
   
   logPerformance = [[NSUserDefaults standardUserDefaults] integerForKey:LOG_PERFORMANCE_KEY];
   
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
	NSError *error = nil;
   NSDocumentController *dc = [NSDocumentController sharedDocumentController];
   HoloSimDocument *doc = [dc openDocumentWithContentsOfURL:[NSURL fileURLWithPath:fileName] display:NO error:&error];
   
   // Set parameters for document opened
   [self prepareDocumentParameters:doc];
   
   // Now when all parameters are setup correctly, show document
   [doc makeWindowControllers];
   [doc showWindows];
   
   return doc != nil;
}

@end
