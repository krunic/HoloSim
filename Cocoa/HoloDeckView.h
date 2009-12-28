/* HoloDeckView */

#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>

#import "AbstractDrawingCode.h"
#import "MouseAdapter.h"
#import "HoloSimDocument.h"

using namespace hdsim;

/**
 * Room view that is used to show 3D view of the room in the Holodeck
 */
@interface HoloDeckView : NSOpenGLView
{
   /**
    * Model associated with this view. 
    */
   IBOutlet HoloSimDocument *model;
   
   /**
    * FOV slider associated with this view. 
    */
   IBOutlet id fovSlider;
    
   /**
    * FOV slider associated with x rotation in this view.                                                                                                                
    */
   IBOutlet id xSlider;

   /**
    * FOV slider associated with x rotation in this view.                                                                                                                
    */
   IBOutlet id ySlider;

   /**
    * FOV slider associated with x rotation in this view.                                                                                                                
    */
   IBOutlet id zSlider;
   
   /**
    * Slider for model timeslice
    */
   IBOutlet id timeSliceSlider;
   
   /**
    * play/pause button. 
    */
   IBOutlet id playPauseButton;
   
   /**
    * Tracking rectangle for mouse view
    */
   NSTrackingRectTag trackingRectangle;
   
   /**
    * Custom animation timer
    */
   NSTimer *openGLAnimationTimer;
    
   /**
    * Is context initialized
    */
   Boolean contextInitalized;
   
   /**
    * Is animation supposed to be continues
    */
   Boolean loopAnimation;
   
   /**
    * Should we progress with animation to next frame
    */
   BOOL animationRunning;
   
   /**
    * Where did mouse drag started
    */
   NSPoint mouseDragStart;
   
   /**
    * Where is current point of the drag
    */
   NSPoint mouseDragCurrent;
    
   /**
    * This is a pointer to C++ object is where all OpenGL drawing would happen for portability reasons
    */
   AbstractDrawingCode *drawer;
   
   /**
    * Controller adapter that is controlling model in this view
    */
   MouseAdapter *mouseAdapter;
}

/**
 * Callback to be called when fovSlider has changed
 *
 * @param sender Object that made a call
 */
- (IBAction)fovSliderChanged:(id)sender;

/**
 * Callback to be called when xSlider has changed
 *
 * @param sender Object that made a call
 */
- (IBAction)xSliderChanged:(id)sender;

/**
 * Callback to be called when ySlider has changed
 *
 * @param sender Object that made a call
 */
- (IBAction)ySliderChanged:(id)sender;

/**
 * Callback to be called when zSlider has changed
 *
 * @param sender Object that made a call
 */
- (IBAction)zSliderChanged:(id)sender;

/**
 * Callback to be invoked when start animation was requested
 *
 * @param sender Who invoked this callback
 */
-(IBAction)toggleAnimation:(id)sender;

/**
 * Callback to be invoked when model got changed
 *
 * @param sender Who invoked this callback
 */
-(IBAction)notifyModelChanged:(id)sender;

@end
