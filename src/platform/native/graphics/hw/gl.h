#pragma once

// The OpenGL 3.3 core entry points the hardware renderer uses, loaded through SDL (nothing of macOS's or Linux's own)

#include <cstddef>
#include <cstdint>

// OpenGL's calling convention (its headers' APIENTRY): stdcall on 32 bit Windows, where the callee takes its arguments off the
// stack; the platform's own elsewhere
#if defined(_WIN32) && defined(__i386__)
#define TWIN_GL_API __attribute__((stdcall))
#else
#define TWIN_GL_API
#endif

namespace Hw
{
using GLenum = unsigned int;
using GLuint = unsigned int;
using GLint = int;
using GLsizei = int;
using GLboolean = unsigned char;
using GLchar = char;
using GLbitfield = unsigned int;
using GLfloat = float;
using GLsizeiptr = std::ptrdiff_t;
using GLuint64 = std::uint64_t;
using GLsync = struct GLsyncObject*;
constexpr GLenum GL_SYNC_GPU_COMMANDS_COMPLETE = 0x9117;
constexpr GLuint64 GL_TIMEOUT_IGNORED = ~GLuint64{0};

constexpr GLenum GL_TEXTURE_2D = 0x0DE1, GL_TEXTURE_MIN_FILTER = 0x2801, GL_TEXTURE_MAG_FILTER = 0x2800,
                 GL_TEXTURE_WRAP_S = 0x2802, GL_TEXTURE_WRAP_T = 0x2803, GL_CLAMP_TO_EDGE = 0x812F, GL_REPEAT = 0x2901,
                 GL_LINEAR = 0x2601, GL_NEAREST = 0x2600, GL_NEAREST_MIPMAP_NEAREST = 0x2700, GL_LINEAR_MIPMAP_NEAREST = 0x2701,
                 GL_NEAREST_MIPMAP_LINEAR = 0x2702, GL_LINEAR_MIPMAP_LINEAR = 0x2703, GL_TEXTURE_BASE_LEVEL = 0x813C,
                 GL_TEXTURE_MAX_LEVEL = 0x813D, GL_RGBA = 0x1908, GL_RGBA8 = 0x8058, GL_RED = 0x1903, GL_R32F = 0x822E,
                 GL_UNSIGNED_BYTE = 0x1401, GL_FLOAT = 0x1406, GL_UNSIGNED_INT = 0x1405, GL_DEPTH_STENCIL = 0x84F9,
                 GL_DEPTH_COMPONENT = 0x1902, GL_DEPTH32F_STENCIL8 = 0x8CAD, GL_FLOAT_32_UNSIGNED_INT_24_8_REV = 0x8DAD,
                 GL_FRAMEBUFFER = 0x8D40, GL_READ_FRAMEBUFFER = 0x8CA8, GL_DRAW_FRAMEBUFFER = 0x8CA9,
                 GL_COLOR_ATTACHMENT0 = 0x8CE0, GL_DEPTH_STENCIL_ATTACHMENT = 0x821A, GL_FRAMEBUFFER_COMPLETE = 0x8CD5,
                 GL_COLOR_BUFFER_BIT = 0x4000, GL_DEPTH_BUFFER_BIT = 0x100, GL_STENCIL_BUFFER_BIT = 0x400, GL_NONE = 0,
                 GL_BLEND = 0x0BE2, GL_DEPTH_TEST = 0x0B71, GL_SCISSOR_TEST = 0x0C11, GL_STENCIL_TEST = 0x0B90,
                 GL_CULL_FACE = 0x0B44, GL_PROGRAM_POINT_SIZE = 0x8642, GL_FUNC_ADD = 0x8006, GL_FUNC_SUBTRACT = 0x800A,
                 GL_FUNC_REVERSE_SUBTRACT = 0x800B, GL_ZERO = 0, GL_ONE = 1, GL_SRC1_ALPHA = 0x8589,
                 GL_ONE_MINUS_SRC1_ALPHA = 0x88FB, GL_CONSTANT_ALPHA = 0x8003, GL_ONE_MINUS_CONSTANT_ALPHA = 0x8004,
                 GL_ALWAYS = 0x0207, GL_GEQUAL = 0x0206, GL_GREATER = 0x0204, GL_NEVER = 0x0200, GL_LESS = 0x0201, GL_LEQUAL = 0x0203,
                 GL_TRIANGLES = 0x0004, GL_LINES = 0x0001, GL_POINTS = 0x0000, GL_TRIANGLE_STRIP = 0x0005,
                 GL_VERTEX_SHADER = 0x8B31, GL_FRAGMENT_SHADER = 0x8B30, GL_COMPILE_STATUS = 0x8B81, GL_LINK_STATUS = 0x8B82,
                 GL_TEXTURE0 = 0x84C0, GL_ARRAY_BUFFER = 0x8892, GL_PIXEL_UNPACK_BUFFER = 0x88EC, GL_STREAM_DRAW = 0x88E0, GL_UNPACK_ROW_LENGTH = 0x0CF2,
                 GL_PACK_ROW_LENGTH = 0x0D02, GL_UNPACK_ALIGNMENT = 0x0CF5, GL_PACK_ALIGNMENT = 0x0D05, GL_BACK = 0x0405,
                 GL_TRUE = 1, GL_FALSE = 0, GL_KEEP = 0x1E00, GL_R32UI = 0x8236, GL_R16UI = 0x8234, GL_RED_INTEGER = 0x8D94, GL_UNSIGNED_SHORT = 0x1403, GL_RGBA16F = 0x881A, GL_HALF_FLOAT = 0x140B, GL_TEXTURE_MAX_ANISOTROPY = 0x84FE, GL_REPLACE = 0x1E01, GL_EQUAL = 0x0202;

struct Gl
{
    void (TWIN_GL_API *GenTextures)(GLsizei, GLuint*);
    void (TWIN_GL_API *DeleteTextures)(GLsizei, const GLuint*);
    void (TWIN_GL_API *BindTexture)(GLenum, GLuint);
    void (TWIN_GL_API *TexImage2D)(GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const void*);
    void (TWIN_GL_API *TexSubImage2D)(GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, const void*);
    void (TWIN_GL_API *TexParameteri)(GLenum, GLenum, GLint);
    void (TWIN_GL_API *ActiveTexture)(GLenum);
    void (TWIN_GL_API *PixelStorei)(GLenum, GLint);
    void (TWIN_GL_API *GenFramebuffers)(GLsizei, GLuint*);
    void (TWIN_GL_API *DeleteFramebuffers)(GLsizei, const GLuint*);
    void (TWIN_GL_API *BindFramebuffer)(GLenum, GLuint);
    void (TWIN_GL_API *FramebufferTexture2D)(GLenum, GLenum, GLenum, GLuint, GLint);
    GLenum (TWIN_GL_API *CheckFramebufferStatus)(GLenum);
    void (TWIN_GL_API *BlitFramebuffer)(GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLbitfield, GLenum);
    void (TWIN_GL_API *ReadPixels)(GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, void*);
    void (TWIN_GL_API *ReadBuffer)(GLenum);
    void (TWIN_GL_API *DrawBuffer)(GLenum);
    void (TWIN_GL_API *Viewport)(GLint, GLint, GLsizei, GLsizei);
    void (TWIN_GL_API *Scissor)(GLint, GLint, GLsizei, GLsizei);
    void (TWIN_GL_API *Enable)(GLenum);
    void (TWIN_GL_API *Disable)(GLenum);
    void (TWIN_GL_API *BlendFuncSeparate)(GLenum, GLenum, GLenum, GLenum);
    void (TWIN_GL_API *BlendEquationSeparate)(GLenum, GLenum);
    void (TWIN_GL_API *BlendColor)(GLfloat, GLfloat, GLfloat, GLfloat);
    void (TWIN_GL_API *ColorMask)(GLboolean, GLboolean, GLboolean, GLboolean);
    void (TWIN_GL_API *DepthMask)(GLboolean);
    void (TWIN_GL_API *DepthFunc)(GLenum);
    void (TWIN_GL_API *Clear)(GLbitfield);
    void (TWIN_GL_API *ClearColor)(GLfloat, GLfloat, GLfloat, GLfloat);
    GLuint (TWIN_GL_API *CreateShader)(GLenum);
    void (TWIN_GL_API *ShaderSource)(GLuint, GLsizei, const GLchar* const*, const GLint*);
    void (TWIN_GL_API *CompileShader)(GLuint);
    void (TWIN_GL_API *GetShaderiv)(GLuint, GLenum, GLint*);
    void (TWIN_GL_API *GetShaderInfoLog)(GLuint, GLsizei, GLsizei*, GLchar*);
    void (TWIN_GL_API *DeleteShader)(GLuint);
    GLuint (TWIN_GL_API *CreateProgram)();
    void (TWIN_GL_API *AttachShader)(GLuint, GLuint);
    void (TWIN_GL_API *BindFragDataLocationIndexed)(GLuint, GLuint, GLuint, const GLchar*);
    void (TWIN_GL_API *BindAttribLocation)(GLuint, GLuint, const GLchar*);
    void (TWIN_GL_API *LinkProgram)(GLuint);
    void (TWIN_GL_API *GetProgramiv)(GLuint, GLenum, GLint*);
    void (TWIN_GL_API *GetProgramInfoLog)(GLuint, GLsizei, GLsizei*, GLchar*);
    void (TWIN_GL_API *UseProgram)(GLuint);
    GLint (TWIN_GL_API *GetUniformLocation)(GLuint, const GLchar*);
    void (TWIN_GL_API *Uniform1i)(GLint, GLint);
    void (TWIN_GL_API *Uniform1f)(GLint, GLfloat);
    void (TWIN_GL_API *Uniform2f)(GLint, GLfloat, GLfloat);
    void (TWIN_GL_API *Uniform3f)(GLint, GLfloat, GLfloat, GLfloat);
    void (TWIN_GL_API *Uniform4f)(GLint, GLfloat, GLfloat, GLfloat, GLfloat);
    void (TWIN_GL_API *Uniform1iv)(GLint, GLsizei, const GLint*);
    void (TWIN_GL_API *GenVertexArrays)(GLsizei, GLuint*);
    void (TWIN_GL_API *BindVertexArray)(GLuint);
    void (TWIN_GL_API *GenBuffers)(GLsizei, GLuint*);
    void (TWIN_GL_API *BindBuffer)(GLenum, GLuint);
    void (TWIN_GL_API *BufferData)(GLenum, GLsizeiptr, const void*, GLenum);
    void (TWIN_GL_API *VertexAttribPointer)(GLuint, GLint, GLenum, GLboolean, GLsizei, const void*);
    void (TWIN_GL_API *VertexAttribIPointer)(GLuint, GLint, GLenum, GLsizei, const void*);
    void (TWIN_GL_API *EnableVertexAttribArray)(GLuint);
    void (TWIN_GL_API *DrawArrays)(GLenum, GLint, GLsizei);
    void (TWIN_GL_API *Finish)();
    GLenum (TWIN_GL_API *GetError)();
    void (TWIN_GL_API *PointSize)(GLfloat);
    void (TWIN_GL_API *LineWidth)(GLfloat);
    void (TWIN_GL_API *Uniform4ui)(GLint, GLuint, GLuint, GLuint, GLuint);
    void (TWIN_GL_API *Uniform1ui)(GLint, GLuint);
    void (TWIN_GL_API *StencilFunc)(GLenum, GLint, GLuint);
    void (TWIN_GL_API *TexParameterf)(GLenum, GLenum, GLfloat);
    void (TWIN_GL_API *GenerateMipmap)(GLenum);
    void (TWIN_GL_API *StencilOp)(GLenum, GLenum, GLenum);
    void (TWIN_GL_API *StencilMask)(GLuint);
    void (TWIN_GL_API *ClearStencil)(GLint);
    // The render thread's hand-over of the shown picture to the window's context
    void (TWIN_GL_API *Flush)();
    GLsync (TWIN_GL_API *FenceSync)(GLenum, GLbitfield);
    void (TWIN_GL_API *WaitSync)(GLsync, GLbitfield, GLuint64);
    void (TWIN_GL_API *DeleteSync)(GLsync);
};

// The entry points (loaded the first time, with a context current); false when one is missing
bool LoadGl();
Gl& GetGl();
}
