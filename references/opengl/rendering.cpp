
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/gl.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <exception>
#include <stdexcept>
#include <vector>
#include <thread>
#include <chrono>
#include <string>
#include <fstream>
#include <filesystem>

#include <spdlog/spdlog.h>

#include <core/assets/mesh.hpp>

extern const char *vertexMVPShaderSource;
extern const char *fragmentShaderSource;

constexpr int windowWidth  = 800;
constexpr int windowHeight = 600;
const glm::mat4 default_modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f)) * glm::rotate(glm::mat4(1.0f), 0.0f ,glm::vec3(0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));

std::vector<float> g_vertex_buffer_triangle = {-1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f};
std::vector<float> g_xworld_lines_data;
std::vector<float> g_yworld_lines_data;
std::vector<float> g_vertex_buffer_cube = {
    -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,
    1.0f,  -1.0f, 1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,
    -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f,  -1.0f, 1.0f,  -1.0f, 1.0f,  -1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  -1.0f,
    1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, -1.0f, 1.0f, -1.0f, 1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,
    -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f, 1.0f,  -1.0f, 1.0f};
std::vector<float> g_texture_buffer_cube = {
    0.000059f, 1.0f - 0.000004f, 0.000103f, 1.0f - 0.336048f, 0.335973f, 1.0f - 0.335903f, 1.000023f, 1.0f - 0.000013f,
    0.667979f, 1.0f - 0.335851f, 0.999958f, 1.0f - 0.336064f, 0.667979f, 1.0f - 0.335851f, 0.336024f, 1.0f - 0.671877f,
    0.667969f, 1.0f - 0.671889f, 1.000023f, 1.0f - 0.000013f, 0.668104f, 1.0f - 0.000013f, 0.667979f, 1.0f - 0.335851f,
    0.000059f, 1.0f - 0.000004f, 0.335973f, 1.0f - 0.335903f, 0.336098f, 1.0f - 0.000071f, 0.667979f, 1.0f - 0.335851f,
    0.335973f, 1.0f - 0.335903f, 0.336024f, 1.0f - 0.671877f, 1.000004f, 1.0f - 0.671847f, 0.999958f, 1.0f - 0.336064f,
    0.667979f, 1.0f - 0.335851f, 0.668104f, 1.0f - 0.000013f, 0.335973f, 1.0f - 0.335903f, 0.667979f, 1.0f - 0.335851f,
    0.335973f, 1.0f - 0.335903f, 0.668104f, 1.0f - 0.000013f, 0.336098f, 1.0f - 0.000071f, 0.000103f, 1.0f - 0.336048f,
    0.000004f, 1.0f - 0.671870f, 0.336024f, 1.0f - 0.671877f, 0.000103f, 1.0f - 0.336048f, 0.336024f, 1.0f - 0.671877f,
    0.335973f, 1.0f - 0.335903f, 0.667969f, 1.0f - 0.671889f, 1.000004f, 1.0f - 0.671847f, 0.667979f, 1.0f - 0.335851f};
std::vector<float>    g_vertex_buffer_crate;
std::vector<uint32_t> g_indice_buffer_crate;

void loadCrateMesh()
{
    core::assets::Mesh mesh("../../assets/models/crate.obj");
    g_vertex_buffer_crate.reserve(mesh.getVertices().size() * 3);
    for (auto &s : mesh.getVertices())
    {
        g_vertex_buffer_crate.push_back(s.v.x);
        g_vertex_buffer_crate.push_back(s.v.y);
        g_vertex_buffer_crate.push_back(s.v.z);
    }
    g_indice_buffer_crate.reserve(mesh.getIndices().size());
    for (auto &s : mesh.getIndices())
    {
        g_indice_buffer_crate.push_back(s);
    }
}

std::vector<float> generateWorldLines(const int          boundsAxis,
                                      const int          stepAxis,
                                      const float        bounds,
                                      const float        step,
                                      const unsigned int lineCount)
{
    std::string boundAxisName = boundsAxis == 0 ? "X"
                              : boundsAxis == 1 ? "Y"
                              : boundsAxis == 2 ? "Z"
                                                : throw std::logic_error("Invalid boundsAxis");
    std::string stepAxisName  = stepAxis == 0 ? "X"
                              : stepAxis == 1 ? "Y"
                              : stepAxis == 2 ? "Z"
                                              : throw std::logic_error("Invalid stepAxis");
    if (boundsAxis == stepAxis) throw std::logic_error("step and bound axis cannot be same");

    std::vector<float> vertice;

    int floatCount   = lineCount * 3;
    int verticeCount = lineCount >> 1;
    spdlog::info("Generating {} floats ({} vertices) for world lines for {}{} axis",
                 floatCount,
                 verticeCount,
                 boundAxisName,
                 stepAxisName);

    float lineBounds = static_cast<float>(lineCount >> 1) * step;
    for (float i = -lineBounds; i <= lineBounds; i += step)
    {
        float x1 = (boundsAxis == 0 ? -bounds : (stepAxis == 0 ? i : 0.0f));
        float y1 = (boundsAxis == 1 ? -bounds : (stepAxis == 1 ? i : 0.0f));
        float z1 = (boundsAxis == 2 ? -bounds : (stepAxis == 2 ? i : 0.0f));
        float x2 = (boundsAxis == 0 ? bounds : (stepAxis == 0 ? i : 0.0f));
        float y2 = (boundsAxis == 1 ? bounds : (stepAxis == 1 ? i : 0.0f));
        float z2 = (boundsAxis == 2 ? bounds : (stepAxis == 2 ? i : 0.0f));

        spdlog::trace("L1: {} - {}", glm::to_string(glm::vec3(x1, y1, z1)), glm::to_string(glm::vec3(x2, y2, z2)));

        vertice.push_back(x1);
        vertice.push_back(y1);
        vertice.push_back(z1);
        vertice.push_back(x2);
        vertice.push_back(y2);
        vertice.push_back(z2);
    }

    return vertice;
}

void build_world_lines()
{
    float step   = 1.0f;
    float bounds = 1000.0f;
    float start  = 100.0f;

    {
        int vertexCount = 6 * (((2 * start) / step) + 1);
        g_xworld_lines_data.reserve(vertexCount);
        spdlog::info("Generating {} vertices for xworld lines", vertexCount);

        int vertexIndex = 0;
        for (float i = -start; i <= start; i += step)
        {
            g_xworld_lines_data.push_back(i);
            g_xworld_lines_data.push_back(bounds);
            g_xworld_lines_data.push_back(0.0f);
            g_xworld_lines_data.push_back(i);
            g_xworld_lines_data.push_back(-bounds);
            g_xworld_lines_data.push_back(0.0f);
            vertexIndex += 6;
        }
        spdlog::info("Generated {} vertices for xworld lines", g_xworld_lines_data.size());
    }

    {
        int vertexCount = 6 * (((2 * start) / step) + 1);
        g_yworld_lines_data.reserve(vertexCount);
        spdlog::info("Generating {} vertices for yworld lines", vertexCount);

        int vertexIndex = 0;
        for (float i = -start; i <= start; i += step)
        {
            g_yworld_lines_data.push_back(bounds);
            g_yworld_lines_data.push_back(i);
            g_yworld_lines_data.push_back(0.0f);
            g_yworld_lines_data.push_back(-bounds);
            g_yworld_lines_data.push_back(i);
            g_yworld_lines_data.push_back(0.0f);
            vertexIndex += 6;
        }
        spdlog::info("Generated {} vertices for yworld lines", g_yworld_lines_data.size());
    }
}

int initGLEW()
{
    const char *version = (const char *)(glGetString(GL_VERSION));
    if (version != NULL) spdlog::info("OpenGL Version: {}", version);
    const char *vendor = (const char *)(glGetString(GL_VENDOR));
    if (vendor != NULL) spdlog::info("OpenGL Vendor: {}", vendor);
    const char *renderer = (const char *)(glGetString(GL_RENDERER));
    if (renderer != NULL) spdlog::info("OpenGL Renderer: {}", renderer);

    GLenum gres = glewInit();
    if (gres != GLEW_OK) throw std::runtime_error("Could not initialize glew");

    return 0;
}

GLuint loadShaders(const char *vertexShaderBuffer, const char *fragmentShaderBuffer)
{
    GLint  res = GL_FALSE;
    int    loglength;
    GLuint vertexShaderID   = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShaderID, 1, &vertexShaderBuffer, NULL);
    glCompileShader(vertexShaderID);
    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &res);
    glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &loglength);
    if (loglength > 0)
    {
        std::vector<char> logMessage(loglength);
        glGetShaderInfoLog(vertexShaderID, loglength, NULL, &logMessage[0]);
        printf("%s\n", &logMessage[0]);
        exit(1);
    }

    glShaderSource(fragmentShaderID, 1, &fragmentShaderBuffer, NULL);
    glCompileShader(fragmentShaderID);
    glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &res);
    glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &loglength);
    if (loglength > 0)
    {
        std::vector<char> logMessage(loglength);
        glGetShaderInfoLog(fragmentShaderID, loglength, NULL, &logMessage[0]);
        printf("%s\n", &logMessage[0]);
        exit(1);
    }

    GLuint programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);

    glGetShaderiv(programID, GL_LINK_STATUS, &res);
    glGetShaderiv(programID, GL_INFO_LOG_LENGTH, &loglength);
    if (loglength > 0)
    {
        std::vector<char> logMessage(loglength);
        glGetShaderInfoLog(programID, loglength, NULL, &logMessage[0]);
        printf("%s\n", &logMessage[0]);
        exit(1);
    }

    glDetachShader(programID, vertexShaderID);
    glDetachShader(programID, fragmentShaderID);

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    return programID;
}

template <typename T>
GLuint loadBuffer(GLenum target, GLenum usage, const std::vector<T> &buffer)
{
    GLuint bufferID;
    glGenBuffers(1, &bufferID);
    glBindBuffer(target, bufferID);
    glBufferData(target, buffer.size() * sizeof(T), buffer.data(), usage);
    return bufferID;
}

GLuint loadTextureBMP(const std::string &filename)
{
    std::vector<char> fileBuffer;
    {
        if (!std::filesystem::exists(filename)) throw std::runtime_error("Could not open texture file: " + filename);
        std::ifstream f(filename, std::ios::ate | std::ios::binary);
        std::size_t   fileSize = (std::size_t)f.tellg();
        fileBuffer.reserve(fileSize);

        f.seekg(0);
        f.read(fileBuffer.data(), fileSize);
        f.close();
    }

    if (fileBuffer[0] != 'B' || fileBuffer[1] != 'M') throw std::runtime_error("Texture is not a bitmap file");

    uint32_t dataPos   = *(uint32_t *)&fileBuffer[0x0A];
    uint16_t width     = *(uint16_t *)&fileBuffer[0x12];
    uint16_t height    = *(uint16_t *)&fileBuffer[0x16];
    uint32_t imageSize = *(uint32_t *)&fileBuffer[0x22];

    if (imageSize == 0) imageSize = width * height * 3;
    if (dataPos == 0) dataPos = 54;

    /** Create gl texture */
    GLuint textureID;
    glGenTextures(1, &textureID);
    /** Bind subsequent texture function calls to this texture */
    glBindTexture(GL_TEXTURE_2D, textureID);
    /** Give image to opengl */
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, &fileBuffer[dataPos]);

    // On Magnify, use linear
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // On minify, use mipmaps
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // Tell OpenGL to generate mipmap
    glGenerateMipmap(GL_TEXTURE_2D);

    return textureID;
}

GLuint loadTextureDDS(const std::string &filename)
{
    const uint32_t FOURCC_DXT1 = 0x31545844; // Equivalent to "DXT1" in ASCII
    const uint32_t FOURCC_DXT3 = 0x33545844; // Equivalent to "DXT3" in ASCII
    const uint32_t FOURCC_DXT5 = 0x35545844; // Equivalent to "DXT5" in ASCII

    std::vector<char> fileBuffer;
    {
        if (!std::filesystem::exists(filename)) throw std::runtime_error("Could not open texture file: " + filename);
        std::ifstream f(filename, std::ios::ate | std::ios::binary);
        std::size_t   fileSize = (std::size_t)f.tellg();
        fileBuffer.reserve(fileSize);

        f.seekg(0);
        f.read(fileBuffer.data(), fileSize);
        f.close();
    }

    /** Verify signature */
    if (strncmp(&fileBuffer[0], "DDS ", 4) != 0)
    {
        throw std::runtime_error("Texture format is not DDS");
    }

    unsigned int height      = *(unsigned int *)&(fileBuffer[4 + 8]);
    unsigned int width       = *(unsigned int *)&(fileBuffer[4 + 12]);
    unsigned int linearSize  = *(unsigned int *)&(fileBuffer[4 + 16]);
    unsigned int mipMapCount = *(unsigned int *)&(fileBuffer[4 + 24]);
    unsigned int fourCC      = *(unsigned int *)&(fileBuffer[4 + 80]);

    unsigned char *dataPos = (unsigned char *)&fileBuffer[4 + 124];

    unsigned int components = fourCC == FOURCC_DXT1 ? 3 : 4;
    unsigned int format;
    switch (fourCC)
    {
    case FOURCC_DXT1: format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT; break;
    case FOURCC_DXT3: format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT; break;
    case FOURCC_DXT5: format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; break;
    default: return 0; break;
    }

    /** Create GL Texture */
    GLuint textureID;
    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    unsigned int blockSize = format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT ? 8 : 16;
    unsigned int offset    = 0;

    /** Load mipmaps */
    for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
    {
        unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
        glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height, 0, size, dataPos + offset);
        offset += size;
        width /= 2;
        height /= 2;

        if (width < 1) width = 1;
        if (height < 1) height = 1;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    return textureID;
}

void renderVertexBuffer(GLuint shaderProgramID, GLenum mode, GLuint vertexBufferID, GLsizei count, const glm::mat4 &mvp)
{
    glUseProgram(shaderProgramID);
    GLint mvpLocation = glGetUniformLocation(shaderProgramID, "u_mvp");
    if (mvpLocation >= 0) glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &mvp[0][0]);

    GLint vertexLocation = glGetAttribLocation(shaderProgramID, "v_vertexPos");
    if (vertexLocation >= 0)
    {
        glEnableVertexAttribArray(vertexLocation);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
        glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    }

    glDrawArrays(mode, 0, count);

    if (vertexLocation >= 0) glDisableVertexAttribArray(vertexLocation);
}

void renderVertexBuffer(GLuint     shaderProgramID,
                        GLenum     mode,
                        GLuint     vertexBufferID,
                        GLuint     texCoordBufferID,
                        GLuint     textureID,
                        GLsizei    count,
                        const glm::mat4 &mvp)
{
    glUseProgram(shaderProgramID);
    GLint mvpLocation = glGetUniformLocation(shaderProgramID, "u_mvp");
    if (mvpLocation >= 0) glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &mvp[0][0]);

    GLint vertexLocation = glGetAttribLocation(shaderProgramID, "v_vertexPos");
    if (vertexLocation >= 0)
    {
        glEnableVertexAttribArray(vertexLocation);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
        glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    }

    GLint textureSamplerLocation = glGetUniformLocation(shaderProgramID, "u_texSampler");
    GLint texCoordLocation       = glGetAttribLocation(shaderProgramID, "a_texCoord");
    if (textureSamplerLocation >= 0 && texCoordLocation >= 0)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glUniform1i(textureSamplerLocation, 0);

        glEnableVertexAttribArray(texCoordLocation);
        glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferID);
        glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
    }

    glDrawArrays(mode, 0, count);

    if (vertexLocation >= 0) glDisableVertexAttribArray(vertexLocation);
    if (textureSamplerLocation >= 0 && texCoordLocation >= 0) glDisableVertexAttribArray(texCoordLocation);
}

void renderMesh(GLuint     shaderProgramID,
                GLenum     mode,
                GLuint     vertexBufferID,
                GLuint     indiceBufferID,
                GLsizei    count,
                const glm::mat4 &mvp)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glUseProgram(shaderProgramID);
    GLint mvpLocation = glGetUniformLocation(shaderProgramID, "u_mvp");
    if (mvpLocation >= 0) glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &mvp[0][0]);

    GLint vertexLocation = glGetAttribLocation(shaderProgramID, "v_vertexPos");
    if (vertexLocation >= 0)
    {
        glEnableVertexAttribArray(vertexLocation);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiceBufferID);
        glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    }

    glDrawElements(mode, count, GL_UNSIGNED_INT, reinterpret_cast<const GLvoid *>(0));

    if (vertexLocation >= 0) glDisableVertexAttribArray(vertexLocation);
}

int main(int argc, const char *argv[])
{
    int res;

    spdlog::set_level(spdlog::level::trace);
    /** Initialize SDL2 */
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow("opengl-render",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          windowWidth,
                                          windowHeight,
                                          SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (SDL_GL_MakeCurrent(window, context) != 0) throw std::runtime_error("SDL_GL_MakeCurrent failed");
    initGLEW();

    /** Enable depth test */
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    build_world_lines();
    loadCrateMesh();

    /** Create vertex buffers */
    GLuint triangleVertexBuffer   = loadBuffer<float>(GL_ARRAY_BUFFER, GL_STATIC_DRAW, g_vertex_buffer_triangle);
    GLuint xWorldLineVertexBuffer = loadBuffer<float>(GL_ARRAY_BUFFER, GL_STATIC_DRAW, g_xworld_lines_data);
    GLuint yWorldLineVertexBuffer = loadBuffer<float>(GL_ARRAY_BUFFER, GL_STATIC_DRAW, g_yworld_lines_data);

    /** Load Textures */
    // GLuint cubeTextureID        = loadTextureDDS("assets/uvtemplate.DDS");
    GLuint cubeTextureID      = loadTextureBMP("assets/uvtemplate.bmp");
    GLuint cubeVertexBufferID = loadBuffer<float>(GL_ARRAY_BUFFER, GL_STATIC_DRAW, g_vertex_buffer_cube);
    GLuint cubeUVBufferID     = loadBuffer<float>(GL_ARRAY_BUFFER, GL_STATIC_DRAW, g_texture_buffer_cube);

    /** Crate */
    GLuint crateVertexBufferID = loadBuffer<float>(GL_ARRAY_BUFFER, GL_STATIC_DRAW, g_vertex_buffer_crate);
    GLuint crateIndiceBufferID = loadBuffer<uint32_t>(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, g_indice_buffer_crate);

    /** World Lines */
    std::size_t worldLineCount = 100;
    GLuint      xy_worldBufferID =
        loadBuffer<float>(GL_ARRAY_BUFFER, GL_STATIC_DRAW, generateWorldLines(0, 1, 1000.0f, 1.0f, worldLineCount));
    GLuint yx_worldBufferID =
        loadBuffer<float>(GL_ARRAY_BUFFER, GL_STATIC_DRAW, generateWorldLines(1, 0, 1000.0f, 1.0f, worldLineCount));

    /** Load Shaders */
    GLuint mvpShaderID = loadShaders(vertexMVPShaderSource, fragmentShaderSource);

    bool isRunning = true;

    glm::vec3 position(0.0f);
    glm::vec3 scale(1.0f);
    glm::quat orientation(glm::vec3(0.0f));

    glm::mat4 identity(1.0f);
    glm::vec3 modelPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 rotationAxis(0.0f, 1.0f, 0.0f);
    float     rotationDegrees = 0.0f;
    float     fieldOfView     = 90.0f;
    float     aspectRatio     = 4.0f / 3.0f;

    glm::vec3 cameraPosition(0.0f, 0.0f, 5.0f), cameraDirection(0.0f, 0.0f, 5.0f), positionInput(0.0f);
    glm::quat cameraOrientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
    glm::vec2 cameraAngle(3.14f, 0.0f), cameraInput(0.0f);
    float     cameraSpeed = 0.1f;

    auto keyFunc = [&]() {
        const uint8_t *state = SDL_GetKeyboardState(NULL);
        cameraInput.y        = (state[SDL_SCANCODE_DOWN] ? cameraSpeed : (state[SDL_SCANCODE_UP] ? -cameraSpeed : 0));
        cameraInput.x   = (state[SDL_SCANCODE_RIGHT] ? cameraSpeed : (state[SDL_SCANCODE_LEFT] ? -cameraSpeed : 0));
        positionInput.z = (state[SDL_SCANCODE_W] ? cameraSpeed : (state[SDL_SCANCODE_S] ? -cameraSpeed : 0));
        positionInput.x = (state[SDL_SCANCODE_D] ? cameraSpeed : (state[SDL_SCANCODE_A] ? -cameraSpeed : 0));
        positionInput.y = (state[SDL_SCANCODE_LSHIFT] ? cameraSpeed : (state[SDL_SCANCODE_LCTRL] ? -cameraSpeed : 0));
        cameraSpeed += state[SDL_SCANCODE_KP_PLUS] ? 1.0f : (state[SDL_SCANCODE_KP_MINUS] ? -1.0f : 0);
    };

    spdlog::set_level(spdlog::level::trace);
    float    loopCount   = 0.0f;
    uint32_t targetDelta = 1000 / 60;
    uint32_t lastTick    = SDL_GetTicks();
    uint32_t nextRender  = lastTick + targetDelta;

    while (isRunning)
    {
        cameraInput = glm::vec2(0.0f);
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_WINDOWEVENT:
                if (event.window.event != SDL_WINDOWEVENT_CLOSE) break;
            case SDL_QUIT: isRunning = false; break;
            default: break;
            }
        }
        SDL_PumpEvents();
        keyFunc();
        /** Route input to camera */
        cameraAngle += cameraInput;

        /** Camera (projection and viwe matrix) */
        cameraDirection = glm::vec3(cos(cameraAngle.y) * sin(cameraAngle.x),
                                    sin(cameraAngle.y),
                                    cos(cameraAngle.y) * cos(cameraAngle.x));
        glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), cameraDirection));
        glm::vec3 up    = glm::cross(right, cameraDirection);

        cameraPosition += cameraDirection * positionInput.z;
        cameraPosition += right * positionInput.x;
        cameraPosition += up * positionInput.y;

        /** Route input to model */
        glm::quat angleAx = glm::angleAxis(glm::radians(1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        orientation       = angleAx * orientation;

        // Model matrix
        glm::mat4 translationMatrix = glm::translate(identity, position);
        glm::mat4 rotationMatrix    = glm::mat4_cast(orientation);
        glm::mat4 scaleMatrix       = glm::scale(identity, scale);

        spdlog::trace("------------------------------");
        spdlog::trace("cameraInput:      x:       {:16}, y:      {:16}", cameraInput.x, cameraInput.y);
        spdlog::trace("Angles:              xAngle:  {:16}, yAngle: {:16}", cameraAngle.x, cameraAngle.y);
        spdlog::trace("cameraDirection      x:       {:16}, y:      {:16}, z:      {:16}",
                      cameraDirection.x,
                      cameraDirection.y,
                      cameraDirection.z);
        spdlog::trace("right:               x:       {:16}, y:      {:16}, z:      {:16}", right.x, right.y, right.z);
        spdlog::trace("up:                  x:       {:16}, y:      {:16}, z:      {:16}", up.x, up.y, up.z);

        glm::mat4 modelMatrix      = translationMatrix * rotationMatrix * scaleMatrix;
        glm::mat4 perspectiveMatrix = glm::perspective(glm::radians(fieldOfView), aspectRatio, 0.1f, 100.0f);
        glm::mat4 orthographicMatrix = glm::ortho(-100.0f, 100.0f, -100.0f, 200.0f, -10.0f, 10.0f);

        glm::mat4 projectionMatrix = perspectiveMatrix;
        glm::mat4 viewMatrix       = glm::lookAt(cameraPosition, cameraPosition + cameraDirection, up);

        if (nextRender < SDL_GetTicks())
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            renderVertexBuffer(mvpShaderID, GL_LINES, xy_worldBufferID, worldLineCount * 3, projectionMatrix * viewMatrix * glm::mat4(1.0f));
            renderVertexBuffer(mvpShaderID, GL_LINES, yx_worldBufferID, worldLineCount * 3, projectionMatrix * viewMatrix * glm::mat4(1.0f));
            
            renderMesh(mvpShaderID, GL_TRIANGLES, crateVertexBufferID, crateIndiceBufferID, g_indice_buffer_crate.size(), projectionMatrix * viewMatrix * modelMatrix);

            SDL_GL_SwapWindow(window);
            nextRender += targetDelta;
        }

        uint32_t delta = (SDL_GetTicks() - lastTick);
        if (targetDelta > delta)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(targetDelta - delta));
        }
        lastTick = SDL_GetTicks();
        // std::this_thread::sleep_for(std::chrono::milliseconds(1));
        loopCount += 0.01f;
    }
}

const char *vertexMVPShaderSource = "#version 120\n"
                                    "uniform mat4 u_mvp;\n"
                                    "attribute vec3 v_vertexPos;\n"
                                    "attribute vec2 a_texCoord;\n"
                                    "varying vec2 v_texCoord;\n"
                                    "void main()\n"
                                    "{\n"
                                    "    gl_Position = u_mvp * vec4(v_vertexPos, 1.0);\n"
                                    "    v_texCoord = a_texCoord;\n"
                                    "}\n";

const char *fragmentShaderSource = "#version 120\n"
                                   "varying vec2 v_texCoord;\n"
                                   "uniform sampler2D u_texSampler;\n"
                                   "void main()\n"
                                   "{\n"
                                   "    gl_FragColor = vec4(texture2D(u_texSampler, v_texCoord).rgb, 1.0);\n"
                                   "}\n";