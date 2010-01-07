/*
 * Copyright © Veljko Krunic 2007-2010. All rights reserved.
 *
 * Modification of the original code, which license is below:
 * 
 * Copyright 2006 Sony Computer Entertainment Inc.
 *
 * Licensed under the MIT Open Source License, for details please see license.txt or the website
 * http://www.opensource.org/licenses/mit-license.php
 *
 */ 
#ifndef COLLADA_H_
#define COLLADA_H_

#include <list>
#include <vector>
#include <iostream>

#define Check(x) if (!(x)) throw exception();

using namespace std;

// Our material structure, which we create by converting a domMaterial object
class Material {
public:
	vector<float> diffuseColor;
	string diffuseTexture;
	// ... and lots of other parameters
	
	Material(domMaterial& mtl) {
		// Grab the <effect> from the <material> and initalize the parameters
	}
};


// Our mesh structure, which we create by converting a domGeometry object
class Mesh {
public:
	Material* mtl;
	// Vertex info, etc
   
	Mesh(domGeometry& geom) {
		// Parse the <geometry> element, extract vertex data, etc
	}
};


// Our node structure, which we create by converting a domNode object
class Node {
public:
	list<Mesh*> meshes;
	list<Node*> childNodes;
   
	// This is defined later to work around a circular dependency on the lookup function
	Node(domNode& node);
};

#endif
