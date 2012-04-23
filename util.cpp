#include <GL/glew.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

#include "util.h"



void *fileContents(const char *filename, GLint &length)
{
    FILE *f = fopen(filename, "r");
    void *buffer;

    if (!f)
    {
        fprintf(stderr, "Unable to open %s for reading\n", filename);
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    length = ftell(f);
    fseek(f, 0, SEEK_SET);

    buffer = malloc(length+1);
    length = fread(buffer, 1, length, f);
    fclose(f);
    ((char*)buffer)[length] = '\0';

    return buffer;
}

static short le_short(unsigned char *bytes)
{
    return bytes[0] | ((char)bytes[1] << 8);
}

texture readTGA(const char *filename, int &width, int &height)
{
    std::cout << "Reading TGA file: " << filename << "\n";
    texture t;
    struct tga_header
    {
        char id_length;
        char color_map_type;
        char data_type_code;
        unsigned char color_map_origin[2];
        unsigned char color_map_length[2];
        char color_map_depth;
        unsigned char x_origin[2];
        unsigned char y_origin[2];
        unsigned char width[2];
        unsigned char height[2];
        char bits_per_pixel;
        char image_descriptor;
    } header;
    int i, color_map_size, pixels_size;
    FILE *f;
    size_t read;
    void *pixels;

    f = fopen(filename, "rb");

    if (!f)
    {
        fprintf(stderr, "Unable to open %s for reading\n", filename);
        throw(error());
    }

    read = fread(&header, 1, sizeof(header), f);

    if (read != sizeof(header))
    {
        fprintf(stderr, "%s has incomplete tga header\n", filename);
        fclose(f);
        throw(error());
    }
    if (header.data_type_code != 2)
    {
        fprintf(stderr, "%s is not an uncompressed RGB tga file\n", filename);
        fclose(f);
        throw(error());
    }
    if (header.bits_per_pixel != 24)
    {
        fprintf(stderr, "%s is not a 24-bit uncompressed RGB tga file\n", filename);
        fclose(f);
        throw(error());
    }

    for (i = 0; i < header.id_length; ++i)
        if (getc(f) == EOF)
        {
            fprintf(stderr, "%s has incomplete id string\n", filename);
            fclose(f);
            throw(error());
        }

    color_map_size = le_short(header.color_map_length) * (header.color_map_depth/8);
    for (i = 0; i < color_map_size; ++i)
        if (getc(f) == EOF)
        {
            fprintf(stderr, "%s has incomplete color map\n", filename);
            fclose(f);
            throw(error());
        }

    t.width = le_short(header.width);
    t.height = le_short(header.height);
    width = t.width;
    height = t.height;
    pixels_size = width * height * (header.bits_per_pixel/8);
    pixels = malloc(pixels_size);

    read = fread(pixels, 1, pixels_size, f);

    if (read != pixels_size)
    {
        fprintf(stderr, "%s has incomplete image\n", filename);
        fclose(f);
        free(pixels);
        throw(error());
    }
    t.data = pixels;
    return t;
}
static void showInfoLog(
    GLuint object,
    PFNGLGETSHADERIVPROC glGet__iv,
    PFNGLGETSHADERINFOLOGPROC glGet__InfoLog
)
{
    GLint log_length;

    glGet__iv(object, GL_INFO_LOG_LENGTH, &log_length);
    char* log = new char[log_length];
    glGet__InfoLog(object, log_length, NULL, log);
    fprintf(stderr, "%s", log);
    free(log);
}

static void showInfoLog(
    GLuint object,
    PFNGLGETSHADERIVPROC glGet__iv,
    PFNGLGETSHADERINFOLOGPROC glGet__InfoLog,
    wxTextCtrl *output
)
{
    GLint log_length;

    glGet__iv(object, GL_INFO_LOG_LENGTH, &log_length);
    char* log = new char[log_length];
    glGet__InfoLog(object, log_length, NULL, log);
    (*output) << log;
    free(log);
}

GLuint makeShader(GLenum type, const char* source)
{
    GLint length = strlen(source);
    GLuint shader;
    GLint shader_ok;

    if (!source)
        return 0;

    shader = glCreateShader(type);
    glShaderSource(shader, 1, (const GLchar**)&source, &length);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_ok);
    if (!shader_ok)
    {
        fprintf(stderr, "Failed to compile shader.\nPrinting log:\n");
        showInfoLog(shader, glGetShaderiv, glGetShaderInfoLog);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

GLuint makeShader(GLenum type, const char* source, wxTextCtrl *output)
{
    GLint length = strlen(source);
    GLuint shader;
    GLint shader_ok;

    if (!source)
        return 0;

    shader = glCreateShader(type);
    glShaderSource(shader, 1, (const GLchar**)&source, &length);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_ok);
    if (!shader_ok)
    {
        (*output) << "\nFailed to compile shader.\nPrinting log:\n";
        showInfoLog(shader, glGetShaderiv, glGetShaderInfoLog, output);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

GLuint makeProgram(GLuint vertex_shader, GLuint fragment_shader)
{
    GLint program_ok;

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &program_ok);
    if(!program_ok)
    {
        fprintf(stderr, "Failed to link shader program.\n");
        glDeleteProgram(program);
        return 0;
    }
    return program;
}

GLuint makeTexture(const char* filename)
{
    GLuint texID;
    int width, height;
    texture t = readTGA(filename, width, height);
    void *pixels = t.data;

    if(!pixels)
        return 0;

    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_WRAP_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_WRAP_BORDER);
    glTexImage2D(
        GL_TEXTURE_2D, 0,          //target, level of detail
        GL_RGB8,                   //internal format
        t.width, t.height, 0,          //width, height, border
        GL_BGR, GL_UNSIGNED_BYTE,  //external format, type
        pixels                     //pixel data
    );
    free(pixels);
    return texID;
}

GLuint makeBuffer(
    GLenum target,
    const void* buffer_data,
    GLsizei buffer_size
)
{
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(target, buffer);
    glBufferData(target, buffer_size, buffer_data, GL_STATIC_DRAW);
    return buffer;
}


error::error()
{
    errstring = std::string("An error has occurred!\n");
}
