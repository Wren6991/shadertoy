#include <iostream>
#include "shadertoyMain.h"

struct vert
{
    float x, y, z;
    float nx, ny, nz;
    float u, v;             //32 byte aligned :D
};

struct texture
{
    void *data;
    int width;
    int height;
};

void *fileContents(const char *filename, GLint &length);
texture readTGA(const char *filename, int &width, int &height);
GLuint makeShader(GLenum type, const char* filename);
GLuint makeShader(GLenum type, const char* filename, wxTextCtrl* output);
GLuint makeProgram(GLuint vertex_shader, GLuint fragment_shader);
GLuint makeBuffer(GLenum target, const void* buffer_data, GLsizei buffer_size);
GLuint makeTexture(const char* filename);

class error
{
    public:
    std::string errstring;
    error();
    error(std::string);
};
