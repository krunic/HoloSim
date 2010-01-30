/*
 *  GPUCalculationEngineTest.cpp
 *  HoloSim
 *
 *  Created by Veljko Krunic on 1/28/10.
 *  Copyright 2010 Veljko Krunic. All rights reserved.
 *
 */

#include <cppunit/extensions/HelperMacros.h>

#include "GPUCalculationEngine.h"
#include "GPUCalculationEngineTest.h"

using namespace hdsim;

CPPUNIT_TEST_SUITE_REGISTRATION(GPUCalculationEngineTest);

GPUCalculationEngineTest::GPUCalculationEngineTest() 
{
   
}

GPUCalculationEngineTest::~GPUCalculationEngineTest()
{
   
}

void GPUCalculationEngineTest::setUp()
{
   
}

void GPUCalculationEngineTest::tearDown()
{
   
}

void GPUCalculationEngineTest::testFindWholeWorldSubstring()
{
   CPPUNIT_ASSERT_MESSAGE("SIMPLE MATCH NOT FOUND", isWholeWorldSubstring("abc 123 de", "123"));
   CPPUNIT_ASSERT_MESSAGE("BEGIN MATCH NOT FOUND", isWholeWorldSubstring("123 de", "123"));
   CPPUNIT_ASSERT_MESSAGE("END MATCH NOT FOUND", isWholeWorldSubstring("abc 123", "123"));
   
	CPPUNIT_ASSERT_MESSAGE("MATCHED WHAT IS NOT WHOLE WORLD", !isWholeWorldSubstring("abc123de", "123"));
   CPPUNIT_ASSERT_MESSAGE("MATCHED INCORRECTLY WITH PREFIX", !isWholeWorldSubstring("abc123 de", "123"));
   CPPUNIT_ASSERT_MESSAGE("MATCHED INCORRECTLY WITH SUFFIX", !isWholeWorldSubstring("abc 123de", "123"));
}

void GPUCalculationEngineTest::testOpenGLExtensions()
{
   const char *extensions = "GL_ARB_transpose_matrix GL_ARB_vertex_program GL_ARB_vertex_blend GL_ARB_window_pos "
   								 "GL_ARB_shader_objects GL_ARB_vertex_shader GL_ARB_shading_language_100 GL_EXT_multi_draw_arrays "
   								 "GL_EXT_clip_volume_hint GL_EXT_rescale_normal GL_EXT_draw_range_elements GL_EXT_fog_coord "
   								 "GL_EXT_gpu_program_parameters GL_EXT_geometry_shader4 GL_EXT_transform_feedback GL_APPLE_client_storage "
   								 "GL_APPLE_specular_vector GL_APPLE_transform_hint GL_APPLE_packed_pixels GL_APPLE_fence "
                            "GL_APPLE_vertex_array_object GL_APPLE_vertex_program_evaluators GL_APPLE_element_array GL_APPLE_flush_render "
   								 "GL_APPLE_aux_depth_stencil GL_NV_texgen_reflection GL_NV_light_max_exponent GL_IBM_rasterpos_clip "
   								 "GL_SGIS_generate_mipmap GL_ARB_imaging GL_ARB_point_parameters GL_ARB_texture_env_crossbar "
   								 "GL_ARB_texture_border_clamp GL_ARB_multitexture GL_ARB_texture_env_add GL_ARB_texture_cube_map "
   								 "GL_ARB_texture_env_dot3 GL_ARB_multisample GL_ARB_texture_env_combine GL_ARB_texture_compression "
   								 "GL_ARB_texture_mirrored_repeat GL_ARB_shadow GL_ARB_depth_texture GL_ARB_shadow_ambient GL_ARB_fragment_program "
                            "GL_ARB_fragment_program_shadow GL_ARB_fragment_shader GL_ARB_occlusion_query GL_ARB_point_sprite "
   								 "GL_ARB_texture_non_power_of_two GL_ARB_vertex_buffer_object GL_ARB_pixel_buffer_object GL_ARB_draw_buffers "
   								 "GL_ARB_shader_texture_lod GL_ARB_color_buffer_float GL_ARB_half_float_vertex GL_ARB_texture_rg "
   								 "GL_ARB_texture_compression_rgtc GL_EXT_draw_buffers2 GL_EXT_framebuffer_object GL_EXT_framebuffer_blit "
   								 "GL_EXT_texture_rectangle GL_ARB_texture_rectangle GL_EXT_texture_env_add GL_EXT_blend_color GL_EXT_blend_minmax "
   								 "GL_EXT_blend_subtract GL_EXT_texture_lod_bias GL_EXT_abgr GL_EXT_bgra GL_EXT_stencil_wrap "
                            "GL_EXT_texture_filter_anisotropic GL_EXT_separate_specular_color GL_EXT_secondary_color GL_EXT_blend_func_separate "
                            "GL_EXT_shadow_funcs GL_EXT_stencil_two_side GL_EXT_texture_compression_s3tc GL_EXT_texture_compression_dxt1 "
                            "GL_EXT_texture_sRGB GL_EXT_blend_equation_separate GL_EXT_texture_mirror_clamp GL_EXT_bindable_uniform GL_EXT_texture_integer "
   								 "GL_EXT_gpu_shader4 GL_EXT_framebuffer_sRGB GL_APPLE_flush_buffer_range GL_APPLE_ycbcr_422 GL_APPLE_rgb_422 GL_APPLE_float_pixels "
                            "GL_ATI_texture_float GL_ARB_texture_float GL_ARB_half_float_pixel GL_APPLE_pixel_buffer GL_NV_blend_square GL_NV_fog_distance "
   								 "GL_NV_conditional_render GL_ATI_texture_mirror_once GL_ATI_texture_env_combine3 GL_ATI_separate_stencil GL_SGIS_texture_edge_clamp GL_SGIS_texture_lod";
   
   CPPUNIT_ASSERT_MESSAGE("Didn't metch existing extension", isWholeWorldSubstring(extensions, "GL_EXT_framebuffer_object"));
}
