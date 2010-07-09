/*
 *  NullOpFragmentShader.fs
 *  HoloSim
 *
 *  Created by Veljko Krunic on 7/29/07.
 *  Copyright © 2007-2010 Veljko Krunic. All rights reserved.
 *
 */
 
/**
 * Don't do anything - this shader is NullOp shader used for unit tests only
 */
void main()
{
	 gl_FragDepth = gl_FragCoord.z;
}
