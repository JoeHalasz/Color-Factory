#include "ErrorCatching.h"




void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    bool noError = true;
    while (GLenum error = glGetError())
    {
        std::cout << "Error: (" << error << ")\nFunction: " << function << "\nFile: " << file << ":\nLine: " << line << std::endl;
        noError = false;
    }
    return noError;

}