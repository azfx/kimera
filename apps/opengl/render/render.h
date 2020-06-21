#ifndef RENDER_STATE_H
#define RENDER_STATE_H
/*
//  -> INPUT (RGB, RGBA, YUV420, NV12, YUV422) [in]
//      - Color Planes (1 to 3).
//  -> PROCESSING (RGBA only) [proc]
//      - Texture A.
//      - Texture B.
//  -> DISPLAY (RGBA) [disp]
//      - Display Texture with borders.
//  -> OUPUT (YUV420, YUV422, NV12) [out]
//      - Color Planes (1 to 3).
*/

//
// Global Definitions
//

#define MAX_PROC 2
#define MAX_PLANES 3

#ifdef KIMERA_MACOS
#define GLFW_EXPOSE_NATIVE_COCOA
#elif defined KIMERA_UNIX
#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_WAYLAND
#include <unistd.h>
#elif defined KIMERA_WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#include <Windows.h>
#endif

//
// Includes
//

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "glad/glad.h"
#include "glad/glad_egl.h"

#include "kimera/state.h"

#include "shaders.h"

//
// Global Constant Variables
//

const enum AVPixelFormat input_formats[] = {
    AV_PIX_FMT_YUV420P,
    AV_PIX_FMT_RGB0
};

const enum AVPixelFormat output_formats[] = {
    AV_PIX_FMT_YUV420P,
    AV_PIX_FMT_RGB0
};

const int egl_attr[] = {
    EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
    EGL_RED_SIZE, 8,
    EGL_GREEN_SIZE, 8,
    EGL_BLUE_SIZE, 8,
    EGL_ALPHA_SIZE, 8,
    EGL_DEPTH_SIZE, 16,
    EGL_NONE
};

const int egl_ctx_attr[] = {
    EGL_CONTEXT_MAJOR_VERSION, 3,
    EGL_CONTEXT_CLIENT_VERSION, 3,
    EGL_NONE
};

const int egl_pbuf_attr[] = {
    EGL_WIDTH, 640,
    EGL_HEIGHT, 480,
    EGL_NONE,
};

const float vertices[] = {
     1.0f,  1.0f, 0.0f,  1.0f, 0.0f,
     1.0f, -1.0f, 0.0f,  1.0f, 1.0f,
    -1.0f, -1.0f, 0.0f,  0.0f, 1.0f,
    -1.0f,  1.0f, 0.0f,  0.0f, 0.0f 
};

const unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3
};

//
// Typedef Variables
//

typedef struct {
    GLFWwindow*  adapter;
    GLenum       api;
    EGLConfig    config;
    EGLDisplay   display;
    EGLSurface   surface;
    EGLContext   context;
} ViewportState;

typedef struct {
    bool configured;

    ViewportState* view;
    AVFrame*       frame;

    bool use_display;
    bool use_opengles;
    bool use_input;
    bool use_output;
    bool use_process;

    int f_width;
    int f_height;

    int d_width;
    int d_height;

    double time;
    
    // add process callback pointer

    unsigned int vertex_buffer;
    unsigned int index_buffer;
    unsigned int frame_buffer;

    unsigned int in_shader;
    unsigned int out_shader;
    unsigned int proc_shader;
    unsigned int disp_shader;

    // Replace With Actual Sizes
    float in_ratio[MAX_PLANES];
    float out_ratio[MAX_PLANES];
    // -----

    unsigned int in_planes;
    unsigned int out_planes;

    unsigned int proc_index;

    unsigned int in_tex[MAX_PLANES];
    unsigned int out_tex[MAX_PLANES];
    unsigned int proc_tex[MAX_PROC];
} RenderState;

//
// Meta Methods
//

const char* egl_query(RenderState* render, int name);

const char* gl_query(GLenum name);

const char* render_mode_query(RenderState* render);
void render_print_meta(RenderState* render);
void render_print_ext(RenderState* render);

//
// Render Methods
//

RenderState* alloc_render();
bool open_render(RenderState* render, State* state);
void close_render(RenderState* render);

unsigned int punch_framebuffer(RenderState* render);
unsigned int get_framebuffer(RenderState* render);
unsigned int get_last_framebuffer(RenderState* render);
bool load_default(RenderState* render);
bool load_input(RenderState* render, AVFrame* frame);
bool load_output(RenderState* render, AVFrame* frame);
bool load_display(RenderState* render);
bool load_process(RenderState* render);
bool render_push_frame(RenderState* render, AVFrame* frame);
bool render_proc_frame(RenderState* render);
bool render_draw_frame(RenderState* render);
bool render_pull_frame(RenderState* render);
void render_print_meta2(RenderState* render);
float viewport_get_time(RenderState);

bool update_viewport(RenderState*, State*);
bool open_viewport(RenderState*);

//
// Helper Methods
//

size_t get_file_size(FILE* fp);

char* open_shader(char* filepath);

bool get_gl_error(int line);
bool get_egl_error(int line);

unsigned int compile_shader(unsigned int type, char* code_str);
unsigned int load_shader(int type, char* vs_str, char* fs_str);

bool get_planes_count(AVFrame* frame, float* ratio, unsigned int* planes);

void bind_framebuffer_tex(unsigned int atch_id, unsigned int tex_id);

void create_texture(unsigned int id, unsigned int format, int width, int height);

void set_uniform4f(int program, char* name, float v0, float v1, float v2, float v3);
void set_uniform3f(int program, char* name, float v0, float v1, float v2);
void set_uniform2f(int program, char* name, float v0, float v1);
void set_uniform1f(int program, char* name, float v0);

void set_uniform1i(int program, char* name, int v0);

void set_draw_buffer(GLenum attachment);

double mticks();

bool is_format_supported(enum AVPixelFormat, const enum AVPixelFormat[]);

#endif