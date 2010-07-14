OVERVIEW
========

This is HoloSim, part of the project described in the following papers:

1. Krunic, V: "SCALABLE SOFTWARE CONTROL OF MILLION-ELEMENT CYBER-PHYSICAL SYSTEMS USING A GRAPHICS PROCESSING UNIT", _PhD Thesis, Department of Computer Science_, University of Colorado at Boulder, 2010 
2. Krunic, V. and Han, R.: "Towards Cyber-Physical Holodeck Systems Via Physically Rendered Environments (PRE's)", _Proceedings of the 2008 The 28th International Conference on Distributed Computing Systems Workshops_, Volume 00 (2008) pp. 507-512
3. Krunic, V. and Han, R.: "Towards Physically Rendered Environments", Department of Computer Science, University of Colorado at Boulder, Technical Report (2007) (CU-CS-1033-07)

BUILDING
========

Tested on the Mac OS X Snow Leopard.

To compile, you will need following libraries installed and compiled on your system:

1. Colada-DOM 2.2 (Needed for Collada loading)
2. CppUnit 1.12.1 (needed for unit testing)
3. xerces-c-3.0.1-x86-macosx-gcc-4.0.tar (Needed for XML parsing)
4. Zlib 1.2.3 (needed for some aspects of ColladaDOM)
5. Doxygen++ (used for automated doc generation, currently uses hardcoded project path)

Later versions might work, but have not been tested.

Note that previous libraries are NOT distributed with this project.

Furthermore, you would need to change Doxygen file so that it points to the path in your home directory.

Will get to cleaning this build process at some point in the future. 

LICENSE
=======

HoloSim, visualization and control of the moxel based environment.

Copyright (C) 2010 Veljko Krunic

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License version 3 as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License along with this program. 
If not, see <http://www.gnu.org/licenses/>

LICENSES FOR THIRD PARTY CODE
=============================

Although none of the libraries we are linking with is distributed as a part of this project, code includes files that are originally based on the example code which license allows for 
the inclusion and use of the code (e.g. MIT license). This licenses are included in the appropriate source files, as well as acknowledged in the text of the PhD thesis document.

In the case that the license of the third party library causes the conflict that would otherwise invalidate your license, you are here granted permission to use the rest of the code in 
HoloSim under the terms of the GNU Affero General Public License version 3 upon removing third party code which license is conflicting with the GNU Affero General Public License version 3.
