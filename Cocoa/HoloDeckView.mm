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

#import <OpenGL/gl.h>

#import "HoloDeckView.h"

#import "Constants.h"
#import "HoloSimDocument.h"
#import "MouseAdapter.h"
#import "OpenGLDrawingCode.h"

@implementation HoloDeckView

/**
 * Create OpenGL drawing context compliant with the window our needs (double buffered with depth buffer).
 *
 * @return OpenGL context that is compliant with what we need
 */
- (NSOpenGLContext*)getMyContext
{
   // Specify the pixel-format attributes.
   NSOpenGLPixelFormatAttribute attrs[] =
   {
      NSOpenGLPFAWindow,
   	NSOpenGLPFADoubleBuffer,
      NSOpenGLPFAAccelerated,
      NSOpenGLPFADepthSize, (NSOpenGLPixelFormatAttribute)32,
      NSOpenGLPFASampleBuffers, (NSOpenGLPixelFormatAttribute)1,
      NSOpenGLPFASamples, (NSOpenGLPixelFormatAttribute)8,
      NSOpenGLPFANoRecovery,
      (NSOpenGLPixelFormatAttribute)nil
   };
   
   // Create the pixel-format object.
   NSOpenGLContext *myContext = nil;
   NSOpenGLPixelFormat *pixFmt = [[NSOpenGLPixelFormat alloc]
                                     initWithAttributes:attrs];
   
   // If the pixel format is valid, create the OpenGL context.
   if (pixFmt != nil)
   {
      myContext = [[NSOpenGLContext alloc] initWithFormat:pixFmt
                                           shareContext:nil];
   }
   else 
   {
      NSAssert(false, @"Creation of the pixel format failed");
   }
   
   [pixFmt release];  
   return myContext;
}  

/**
 * Get last part of the filename prior to leading slash
 *
 * @param model Model to use
 * @param moxelCalcMicroSeconds How many microseconds did last moxel calculation took
 * @param numMoxelsRendered And how many moxels did it ultimately render
 * 
 * @return last part of the filename (basename in UNIX parlance)
 */
static void logPerformance(const AbstractModel *model, double moxelCalcMicroSeconds, long numMoxelsRendered)
{
	static bool firstCall = true;
   static FILE *fp;


#ifdef FINAL_RELEASE
   if (firstCall)
   {
      fp = fopen("perf_log.dat", "w");
      CHECK(fp, "Error opening performance log");
      firstCall = false;
      fprintf(fp, "Model, Rendering_Microseconds, Num_Moxels_Rendered, Rate\n");
   }
   
   fprintf(fp, "%s, %lf, %ld, %lf\n", model->getFileName(), moxelCalcMicroSeconds, numMoxelsRendered, numMoxelsRendered/(moxelCalcMicroSeconds/1000000.0));
   fflush(fp);
   
#else
   
	// Do fast logging that leads itself to data analysis
   static int numCalls = 0;
   
   if (firstCall)
   {
      fp = fopen("perf_log.dat", "w");
      CHECK(fp, "Error opening performance log");
      firstCall = false;
      fprintf(fp, "Rendering_Microseconds, Num_Moxels, Rate\n");
   }
   
   fprintf(fp, "%lf, %ld, %lf\n", moxelCalcMicroSeconds, numMoxelsRendered, numMoxelsRendered/(moxelCalcMicroSeconds/1000000));
   
   numCalls++;
   
   if (numCalls%100 == 0) {
   	fflush(fp);
   }
   
#endif
}


/**
 * Set tracking rectangle for mouse tracking
 */
-(void)setTrackingRectangle
{
   if (trackingRectangle)
      [self removeTrackingRect:trackingRectangle];
   
   trackingRectangle = [self addTrackingRect:[self visibleRect] owner:self userData:nil assumeInside:YES];
}

/**
 * Creates drawing code abstraction layer. <b>Note that this method doesn't have anything to do with Cocoa drawers.</b>
 *
 * @return Created drawing code helper.
 */
- (AbstractDrawingCode *)createDrawer
{
   if (!drawer)
   {
      drawer = new OpenGLDrawingCode();
      mouseAdapter = new MouseAdapter();
      mouseAdapter->setDrawer(drawer);
   }
   
   return drawer;
}

/**
 * Internal initialization
 */
- (void)prepare
{
   trackingRectangle = nil;
   contextInitalized = NO;
   if (!contextInitalized)
   {
      [self setOpenGLContext:[self getMyContext]];
      contextInitalized = YES;
   }
   
   NSOpenGLContext *glContext = [self openGLContext];
   [glContext makeCurrentContext];
   
	{
		/* Sync to VBL to avoid tearing. */
		GLint VBL = 1;
		[[self openGLContext] setValues:&VBL forParameter:NSOpenGLCPSwapInterval];
	}
   
   // Add tracking rectangle for mouse move
   [self setTrackingRectangle];
   
   drawer = [self createDrawer];
}

/**
 * If we are notified that update is needed for the surface
 */
- (void) surfaceNeedsUpdate:(NSNotification*)notification 
{
   [self update];
}

/**
 * Default initializer for descendents of NSView 
 */
- (id)initWithFrame:(NSRect)frame {
   
   self = [super initWithFrame:frame];
   if (!self)
      return nil;
   
   // create and activate the context object which maintains the OpenGL state
   [self setOpenGLContext:[self getMyContext]];
   [[self openGLContext] makeCurrentContext];
   
   return self;
}

/**
 * Constructor
 *
 * @param frame Rectangle of the view
 * @param pixFmt Pixel format to use
 */
- (id)initWithFrame:(NSRect)frame
        pixelFormat:(NSOpenGLPixelFormat *)format
{
   self = [super initWithFrame:frame];
   if (self)
   {
      pixelFormat	= [format retain]; 
      
      [[NSNotificationCenter defaultCenter] addObserver:self
                                            selector:@selector(surfaceNeedsUpdate:) name:NSViewGlobalFrameDidChangeNotification object:self];
   }
   
   contextInitalized = NO;
   
   [self prepare];
   return self;
}

/**
 * Constructor. Called when we are initilized from file
 *
 * @param coder Coder to use for initialization
 */
- (id)initWithCoder:(NSCoder *)coder
{	
	self = [super initWithCoder:coder];
   contextInitalized = NO;
   [self prepare];
   return self;
}

/**
 * Prevent mouse down from moving window
 */
- (BOOL)mouseDownCanMoveWindow
{
   return NO;
}

/**
 * To stop animation when window is closing
 */
- (BOOL)windowShouldClose:(id)sender
{
   animationRunning = NO;
   return YES;
}

/**
 * Invoked on windows close
 */
- (void)cleanForClose
{
   // In Cocoa, you need to invalidate timer from the same thread that created it
   [openGLAnimationTimer invalidate];
   [openGLAnimationTimer release];
   
   // Delete OpenGL context   
   delete drawer;
   drawer = NULL;
}

/**
 * Called by GC
 */
- (void)finalize
{
   [self cleanForClose];
   [super finalize];
}

/**
 * Called by manual GC
 */
- (void) dealloc
{
   [self cleanForClose];
   [super dealloc];
}


/**
 * Callback for mouse entering the tracking rectangle
 *
 * @param theEvent NSEvent generated by the system
 */
- (void)mouseEntered:(NSEvent *)theEvent
{
}

/**
 * Callback for mouse exiting the tracking rectangle
 *
 * @param theEvent NSEvent generated by the system
 */
- (void)mouseExited:(NSEvent *)theEvent
{
}

/**
 * Callback for window resize
 */
- (void)reshape
{
   [self setTrackingRectangle];
   
   NSRect rect = [self bounds];
      
   // Resize data from drawer
   drawer->setBounds(rect.size.width, rect.size.height);
   drawer->setAspectRatio(rect.size.width/(double)rect.size.height);
   [self update];

   [self setNeedsDisplay:YES];
}

/**
 * Recalc value of the FOV from slider. Used so that we could invert zoom slider, with the higher zoom being up
 *
 * @param sender Caller that is holding FOV information
 */
- (double)recalcFOVFromSlider:(id)sender
{
   double max = [sender maxValue];
   double min = [sender minValue];
   double fov = [sender floatValue];
   return max - fov + min;
}

/**
 * Recalc value of the FOV slider based on FOV of the model. Used so that we could invert zoom slider, with the higher zoom being up
 *
 * @param value Caller that is holding FOV information
 */
- (double)recalcFOVToSlider:(double)value
{
   double max = [fovSlider maxValue];
   double min = [fovSlider minValue];
   return max - value + min;
}

/**
 * Setup animation in openGLAnimationTimer variable
 */
- (void)setupAnimation
{
   // This will set repeated animation timer to triger notification
	SEL selector;
	NSMethodSignature *signature;
	NSInvocation *invocation;
   
	// animationCallback is my animation callback method
	selector = @selector(animationCallback);
	signature = [HoloDeckView instanceMethodSignatureForSelector:selector];
	invocation = [NSInvocation invocationWithMethodSignature:signature] ;
	[invocation setSelector:selector];
	[invocation setTarget:self];
	
	openGLAnimationTimer = [NSTimer scheduledTimerWithTimeInterval:1.0/FRAMES_PER_SECOND
                                                       invocation:invocation
                                                          repeats:YES];
	[openGLAnimationTimer retain];
	
	// If single threaded, Cocoa blocks timers when popup box or move a slider or printing is invoked.
	// We will allow that blockage
	[[NSRunLoop currentRunLoop] addTimer:openGLAnimationTimer forMode:NSDefaultRunLoopMode];
   
	// Run animation even if popup box or slider are moved. 
	[[NSRunLoop currentRunLoop] addTimer:openGLAnimationTimer forMode:NSEventTrackingRunLoopMode];
   
	// Run loop for dialogs and menus
	[[NSRunLoop currentRunLoop] addTimer:openGLAnimationTimer forMode:NSModalPanelRunLoopMode];
   
   // Used for benchmarking - if you are logging output, then start animation immediately (benchmark mode).
   if ([model logPerformance]) {
   	animationRunning = YES;
   }
}

/**
 * Awake from NIB. Will set limits of control movement based on limits in other UI elements
 */
- (void)awakeFromNib
{
   drawer = [self createDrawer];
   drawer->setFOV([self recalcFOVFromSlider:fovSlider]);
   drawer->setRotationAngles([xSlider floatValue], [ySlider floatValue], [zSlider floatValue]);
   mouseAdapter->setMinFOV([fovSlider minValue]);
   mouseAdapter->setMaxFOV([fovSlider maxValue]);
   minMoxelsPerSecond = -1;
   maxMoxelsPerSecond = -1;
   animationRunning = NO;
   openGLAnimationTimer = nil;
   
   [self setupAnimation];
}

// Public method
- (IBAction)fovSliderChanged:(id)sender
{
   drawer->setFOV([self recalcFOVFromSlider:sender]);
   [self setNeedsDisplay:YES];
}

- (IBAction)xSliderChanged:(id)sender
{
   drawer->setRotationAngles([sender floatValue], 
                             drawer->getRotationAngleY(),
                             drawer->getRotationAngleZ());
                             
   [self setNeedsDisplay:YES];
}

- (IBAction)ySliderChanged:(id)sender
{
   drawer->setRotationAngles(drawer->getRotationAngleX(),
                             [sender floatValue], 
                             drawer->getRotationAngleZ());

   [self setNeedsDisplay:YES];
}

- (IBAction)zSliderChanged:(id)sender
{
   drawer->setRotationAngles(drawer->getRotationAngleX(),
                             drawer->getRotationAngleY(),
                             [sender floatValue]);
   
   [self setNeedsDisplay:YES];
}

/**
 * Stop animation
 */
- (void)stopAnimation
{
   // No need to stop the timer, just change variable
   @synchronized (self) 
   {
      animationRunning = NO;
   }

   [playPauseButton setState:NO];
}

/**
 * Start animation
 */
- (void)startAnimation
{
   // No need to stop the timer, just change variable
   @synchronized (self) 
   {
      animationRunning = YES;
   }

   [playPauseButton setState:YES];
}

- (IBAction)toggleAnimation:(id)sender
{
   @synchronized (self)
   {
      if (animationRunning)
         [self stopAnimation];
      else
         [self startAnimation];
      
   }

   // No need to refresh, this is not immediatelly changing picture   
}

- (IBAction)notifyModelChanged:(id)sender
{
   GPUInterpolatedModel *m = dynamic_cast<GPUInterpolatedModel *>([sender model]);
	[timeSliceSlider setFloatValue:m->getTimeSlice()];

   @synchronized(self)
   {
      if (animationRunning)
         [self stopAnimation];
   }
   
   [self setNeedsDisplay:YES];
}

/**
 * Helper used to update drag information during mouse move rotation
 */
- (void)rotateModelForDrag
{
   @synchronized(self)
   {
      mouseAdapter->setMouseFOVRatio([model mouseZoomSpeed]);
      mouseAdapter->setMouseMoveRatio([model mouseRotationSpeed]);
   }
   
   // Calculate distance we were dragging
   float xDistance = mouseDragCurrent.x - mouseDragStart.x;
   float yDistance = mouseDragCurrent.y - mouseDragStart.y;
   
   // And send this distance to the controller adapter to rotate model
   mouseAdapter->controlForMouseAt(xDistance, yDistance, /* lmb */true, /* rmb */false);
}

/**
 * Callback invoked when mouse button is up
 *
 * @param theEvent Generated NSEvent
 */
- (void)leftMouseUp:(NSEvent *)theEvent
{
	[self rotateModelForDrag];
   [self setNeedsDisplay:YES];
}

/**
 * Callback invoked when mouse is dragged
 *
 * @param theEvent Generated NSEvent
 */
- (void)mouseDragged:(NSEvent *)theEvent
{
   // Only rotate on left mouse button
   if ([theEvent buttonNumber] == 0) 
   {
      NSPoint point = [theEvent locationInWindow];
      mouseDragCurrent = [self convertPoint:point fromView:nil];

      [self rotateModelForDrag];
      [self setNeedsDisplay:YES];
      
      // And change initial point to be equal to the current point, as we want next mouse drag event to be from this point as opposed to initial
      mouseDragStart = mouseDragCurrent;
   }
}

/**
 * Callback invoked when left mouse button is pressed (but not released). NOTE: This is not standard Cocoa callback, but callback 
 * invoked from standard Cocoa callback mouseDown
 *
 * @param theEvent Generated NSEvent
 */
- (void)leftMouseButtonDown:(NSEvent *)theEvent
{
   NSPoint point = [theEvent locationInWindow];
   mouseDragStart = [self convertPoint:point fromView:nil];     
}

/**
 * Callback invoked when scrollwheel is moved
 *
 * @param deltaX X distance length
 * @param deltaY Y distance length
 * @param deltaZ Z distance length
 */
- (void)scrollWheel:(double)deltaX
             deltaY:(double)deltaY
             deltaZ:(double)deltaZ
{
   mouseAdapter->controlForMouseScrollChange(deltaX, deltaY, deltaZ);
   [self setNeedsDisplay:YES];
}

/**
 * Callback invoked when scrollwheel is moved
 *
 * @param theEvent Generated NSEvent
 */
- (void)scrollWheel:(NSEvent *)theEvent
{
   [self scrollWheel:[theEvent deltaX]deltaY:[theEvent deltaY]deltaZ:[theEvent deltaZ]];
}

/**
 * Emulate multiple buttons on 1 button mac mouse
 *
 * @param theEvent Event Used
 */
- (void) mouseDown:(NSEvent*)theEvent
{
	if ([theEvent modifierFlags] & NSAlternateKeyMask)
   {
		[self scrollWheel:EMULATE_MOUSE_WHEEL deltaY:EMULATE_MOUSE_WHEEL deltaZ:EMULATE_MOUSE_WHEEL];
   }
	else if ([theEvent modifierFlags] & NSControlKeyMask)
   {
		[self scrollWheel:-EMULATE_MOUSE_WHEEL deltaY:-EMULATE_MOUSE_WHEEL deltaZ:-EMULATE_MOUSE_WHEEL];
   }
	else
		[self leftMouseButtonDown:theEvent];
}

/** 
 * Calculate new interval with double the length
 *
 * @param min (IN) - min of the interval
 * @param max (IN) - max of the interval
 * @param newMin (OUT) - new min of the interval
 * @param newMax (OUT) - new max of the interval
 */
void padInterval(double min, double max, double *newMin, double *newMax)
{
	PRECONDITION(min <= max);
   
   // Find center of the old interval, add full length of the old interval left and right of it and that is your new interval   
   const double PAD_FACTOR = 1.2;
   
   double center = (min + max)/2;
   double length = (max - min) * PAD_FACTOR/2;
   
   *newMin = center - length;
   *newMax = center + length;
}

/**
 * Normalize bounds of the viewing volume
 * 
 * @param minX Min X
 * @param minY Min Y
 * @param minZ Min Z
 * @param maxX Max X
 * @param maxY Max Y
 * @param maxZ Max Z
 */
void normalizeBounds(double *minX, double *maxX, double *minY, double *maxY, double *minZ, double *maxZ)
{
   // Find max dimension. fabs is there to account for precision loss
   double xDim = fabs(*maxX - *minX);
   double yDim = fabs(*maxY - *minY);
   double zDim = fabs(*maxZ - *minZ);
   
   double maxDim = max(xDim, max(yDim, zDim));
   
   if (areEqual(xDim, 0))
   {
      // As minX == maxX
      double center = *minX;
      *minX = center - maxDim/2;
      *maxX = center + maxDim/2;      
   }

   if (areEqual(yDim, 0))
   {
      // As minY == maxY
      double center = *minY;
      *minY = center - maxDim/2;
      *maxY = center + maxDim/2;      
   }
   
   if (areEqual(zDim, 0))
   {
      // As minZ == maxZ
      double center = *minZ;
      *minZ = center - maxDim/2;
      *maxZ = center + maxDim/2;      
   }
}

/**
 * Callback called when this window needs to redisplay itself
 *
 * @param rect Rectangle that needs to be redisplayed. We always redisplay whole window
 */
- (void)drawRect:(NSRect)rect
{
   [self reshape];
   
	NSOpenGLContext *context = [self openGLContext];   
   [context makeCurrentContext]; 
   
   GPUInterpolatedModel *m = dynamic_cast<GPUInterpolatedModel *>([model model]);
   if (!m)
   {
      [context flushBuffer];
      return;      
   }

   // Set model boundaries to be a little bit bigger then the model. We in effect want to extend [minX, maxX] twice while keeping same mean. 
   double renderMinX, renderMinY, renderMinZ, renderMaxX, renderMaxY, renderMaxZ;
   
   padInterval(m->getBoundMinX(), m->getBoundMaxX(), &renderMinX, &renderMaxX);
   padInterval(m->getBoundMinY(), m->getBoundMaxY(), &renderMinY, &renderMaxY);
   padInterval(m->getBoundMinZ(), m->getBoundMaxZ(), &renderMinZ, &renderMaxZ);
   
   // Now, if any data are 0, set them up to be equal to the max dimension
   normalizeBounds(&renderMinX, &renderMaxX,
                   &renderMinY, &renderMaxY,
                   &renderMinZ, &renderMaxZ);
   
   m->setRenderedArea(renderMinX, renderMinY, renderMinZ, renderMaxX, renderMaxY, renderMaxZ);
   
   m->setOptimizeDrawing([model optimizeDrawing]);
   m->setMoxelThreshold([model optimizeDrawingThreshold]);
   
   drawer->draw(m);
   
   // Get statistics
   Statistics fpsStatistics = drawer->getAllFrameRenderingStatistics();
   Statistics moxelCalculationStatistics = m->getMoxelCalculationStatistics();
   Statistics lastRenderedFrameMoxelStatistics = m->getLastRenderedFrameMoxelStatistics();
   
   double timeRendering = fpsStatistics.getElapsedTimeInMicroSeconds();
   double timeCalulatingMoxels = moxelCalculationStatistics.getElapsedTimeInMicroSeconds();
   
   double fps = fpsStatistics.getTimeAveragedStatistics();
   
   double averageMoxelsPerSecond = moxelCalculationStatistics.getTimeAveragedStatistics();
   double moxelsPerSecond = lastRenderedFrameMoxelStatistics.getTimeAveragedStatistics();
   
   double ratioInRendering = timeCalulatingMoxels / timeRendering;
   
   long numMoxels = m->getTotalNumMoxels();
   
   // First time performance is initial min (and we know it is first time because min is < 0)
   CHECK(!(minMoxelsPerSecond >= 0  &&  maxMoxelsPerSecond < 0), "Initialization not performed correctly");
   CHECK(!(minMoxelsPerSecond < 0  &&  maxMoxelsPerSecond >= 0), "Initialization not performed correctly");
   CHECK(moxelsPerSecond > 0, "Moxels per second can't be 0");
   
   if (minMoxelsPerSecond < 0  ||  maxMoxelsPerSecond < 0)
   {
      minMoxelsPerSecond = moxelsPerSecond;
      maxMoxelsPerSecond = moxelsPerSecond;
   }
   else 
   {
      minMoxelsPerSecond = moxelsPerSecond < minMoxelsPerSecond ? moxelsPerSecond : minMoxelsPerSecond;
      maxMoxelsPerSecond = moxelsPerSecond > maxMoxelsPerSecond ? moxelsPerSecond : maxMoxelsPerSecond;
   }

   // To protect versus overflow
   CHECK(fps >= 0, "FPS overflow");
   CHECK(timeRendering >= 0, "timeRendering overflow");
   CHECK(timeCalulatingMoxels >= 0, "timeCalulatingMoxels overflow");
   CHECK(moxelsPerSecond >= 0, "moxelsPerSecond overflow");

   CHECK(areEqualInLowPrecision(numMoxels/(lastRenderedFrameMoxelStatistics.getElapsedTimeInMicroSeconds()/(double)1000000), lastRenderedFrameMoxelStatistics.getTimeAveragedStatistics()),
         "Didn't correctly calculated statistics");

   // And update UI
   [xSlider setFloatValue:drawer->getRotationAngleX()];
   [ySlider setFloatValue:drawer->getRotationAngleY()];
   [zSlider setFloatValue:drawer->getRotationAngleZ()];
   [fovSlider setFloatValue:[self recalcFOVToSlider:drawer->getFOV()]];
   [percentageLabel setFloatValue:ratioInRendering];
   [framesPerSecondCounterLabel setFloatValue:fps];
   [meanMoxelsPerSecondCounterLabel setFloatValue:averageMoxelsPerSecond];
   [minMoxelsPerSecondAchievedCounterLabel setFloatValue:minMoxelsPerSecond];
   [maxMoxelsPerSecondAchievedCounterLabel setFloatValue:maxMoxelsPerSecond];
   [lastFrameMoxelsPerSecondCounterLabel setFloatValue:moxelsPerSecond];
   [numMoxelsLabel setIntValue:numMoxels];
     
   [context flushBuffer];

	if ([model logPerformance])
   {
	   logPerformance(m, lastRenderedFrameMoxelStatistics.getElapsedTimeInMicroSeconds(), numMoxels);      
   }
}

/**
 * This method would be invoked once every time our animation callback triggers
 */
- (void)animationCallback
{
   // New value of the timeslice
   double newTimeslice;
   
   // Make sure that no paramters are changed while I am setting frame values
   @synchronized(self)
   {
      if (!animationRunning)
         return;
      
      GPUInterpolatedModel *m = dynamic_cast<GPUInterpolatedModel *>([model model]);
      if (!m)
         return;
      
      newTimeslice = m->getTimeSlice() + [model interframeDistance];
            
      if ([model loopAnimation])
      {
         // Loop at the same place from the beginning as would be if this was circular tape. Helps with avoiding discontinuity at the moment of jump
         newTimeslice = newTimeslice < GPUInterpolatedModel::MAX_TIME_SLICE ? newTimeslice : (newTimeslice - GPUInterpolatedModel::MAX_TIME_SLICE) + GPUInterpolatedModel::MIN_TIME_SLICE;
      }
		else
      {
	      newTimeslice = newTimeslice <= GPUInterpolatedModel::MAX_TIME_SLICE ? newTimeslice : GPUInterpolatedModel::MAX_TIME_SLICE;
      }
      
      m->setTimeSlice(newTimeslice);
   }

   [timeSliceSlider setFloatValue:newTimeslice];
   [self setNeedsDisplay:YES];
}

/**
 * Call update method of the OpenGL context class
 */
-(void)update
{
	[openGLContext update];
   [self setNeedsDisplay:YES];
}

/**
 * Lock focus in preparation for drawing
 */
- (void)lockFocus
{
   NSOpenGLContext *context = openGLContext;
   
   [super lockFocus];
   if ([context view] != self) 
   {
      [context setView:self];
   }
   
   [context makeCurrentContext];
}

/**
 * Get pixel format
 */
-(NSOpenGLPixelFormat *)pixelFormat
{
   return pixelFormat;
}

/**
 * Set pixel format
 */
-(void)setPixelFormat:(NSOpenGLPixelFormat*)format
{
   [pixelFormat autorelease];
	pixelFormat = [format retain];
}

/**
 * Set OpenGL context
 */
-(void)setOpenGLContext:(NSOpenGLContext*)context
{
   [openGLContext autorelease];
	openGLContext = [context retain];
}

/**
 * Get OpenGL context
 */
-(NSOpenGLContext*)openGLContext
{
   return openGLContext;
}

@end
