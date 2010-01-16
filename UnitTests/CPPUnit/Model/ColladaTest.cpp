/*
 *  ColladaTest.cpp
 *  HoloSim
 *
 *  Created by Veljko Krunic on 1/16/10.
 *  Copyright 2010 Veljko Krunic. All rights reserved.
 *
 */

#include "ColladaTest.h"

#include <cppunit/extensions/HelperMacros.h>

#include "Collada.h"
#include "GPUGeometryModel.h"

using namespace hdsim;

static const char *XML_FOR_FILE = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>"
"<COLLADA xmlns=\"http://www.collada.org/2005/11/COLLADASchema\" version=\"1.4.1\">"
	"<asset>"
		"<contributor>"
			"<authoring_tool>Google SketchUp 7.1.5803</authoring_tool>"
		"</contributor>"
		"<created>2010-01-15T05:13:06Z</created>"
		"<modified>2010-01-15T05:13:06Z</modified>"
		"<unit meter=\"0.0254000\" name=\"inch\" />"
		"<up_axis>Z_UP</up_axis>"
	"</asset>"
	"<library_visual_scenes>"
      "<visual_scene id=\"ID1\">"
         "<node name=\"SketchUp\">"
            "<instance_geometry url=\"#ID2\">"
               "<bind_material>"
                  "<technique_common>"
                     "<instance_material symbol=\"Material2\" target=\"#ID4\">"
                        "<bind_vertex_input semantic=\"UVSET0\" input_semantic=\"TEXCOORD\" input_set=\"0\" />"
                     "</instance_material>"
                  "</technique_common>"
               "</bind_material>"
            "</instance_geometry>"
         "</node>"
      "</visual_scene>"
	"</library_visual_scenes>"
	"<library_geometries>"
		"<geometry id=\"ID2\">"
			"<mesh>"
				"<source id=\"ID5\">"
					"<float_array id=\"ID8\" count=\"12\">104.5792364 121.9253048 0.0000000 61.1417364 67.5503048 0.0000000 61.1417364 121.9253048 0.0000000 104.5792364 67.5503048 0.0000000</float_array>"
					"<technique_common>"
						"<accessor count=\"4\" source=\"#ID8\" stride=\"3\">"
							"<param name=\"X\" type=\"float\" />"
							"<param name=\"Y\" type=\"float\" />"
							"<param name=\"Z\" type=\"float\" />"
						"</accessor>"
					"</technique_common>"
				"</source>"
				"<source id=\"ID6\">"
					"<float_array id=\"ID9\" count=\"12\">0.0000000 0.0000000 -1.0000000 0.0000000 0.0000000 -1.0000000 0.0000000 0.0000000 -1.0000000 0.0000000 0.0000000 -1.0000000</float_array>"
					"<technique_common>"
						"<accessor count=\"4\" source=\"#ID9\" stride=\"3\">"
							"<param name=\"X\" type=\"float\" />"
							"<param name=\"Y\" type=\"float\" />"
							"<param name=\"Z\" type=\"float\" />"
						"</accessor>"
					"</technique_common>"
				"</source>"
				"<vertices id=\"ID7\">"
					"<input semantic=\"POSITION\" source=\"#ID5\" />"
					"<input semantic=\"NORMAL\" source=\"#ID6\" />"
				"</vertices>"
				"<triangles count=\"2\" material=\"Material2\">"
					"<input offset=\"0\" semantic=\"VERTEX\" source=\"#ID7\" />"
					"<p>0 1 2 1 0 3</p>"
            "</triangles>"
			"</mesh>"
		"</geometry>"
   "</library_geometries>"
	"<library_materials>"
		"<material id=\"ID4\" name=\"material_0\">"
			"<instance_effect url=\"#ID3\" />"
		"/material>"
	"</library_materials>"
	"<library_effects>"
		"<effect id=\"ID3\">"
			"<profile_COMMON>"
				"<technique sid=\"COMMON\">"
					"<lambert>"
						"<diffuse>"
							"<color>1.0000000 1.0000000 1.0000000 1.0000000</color>"
						"</diffuse>"
					"</lambert>"
				"</technique>"
         "</profile_COMMON>"
      "</effect>"
	"</library_effects>"
	"<scene>"
		"<instance_visual_scene url=\"#ID1\" />"
	"</scene>"
"</COLLADA>";

CPPUNIT_TEST_SUITE_REGISTRATION(ColladaTest);

ColladaTest::ColladaTest() : testFileName_("testColladaLoad.tmp")
{

}

ColladaTest::~ColladaTest()
{

}

void ColladaTest::setUp()
{
   FILE *fp = fopen(testFileName_, "w");
   fprintf(fp, "%s", XML_FOR_FILE);
   fclose(fp);
}

void ColladaTest::tearDown()
{
   unlink(testFileName_);
}

void ColladaTest::testLoadQuad()
{
   GPUGeometryModel model;
   
   CPPUNIT_ASSERT_MESSAGE("Can't load collada file", loadCollada(testFileName_, model));
}
