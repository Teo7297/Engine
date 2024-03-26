#include "shader.h"
#include "colors.h"

Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
    // 1. Get Shader code from file

    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // Ensure ifstream objects can throw exceptions
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        // Open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);

        // Read
        std::stringstream vShaderStream;
        std::stringstream fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        // Close
        vShaderFile.close();
        fShaderFile.close();

        // Convert to string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }

    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();

    // 2. Compile shaders
    unsigned int vID;
    vID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vID, 1, &vShaderCode, nullptr);
    glCompileShader(vID);
    CheckShaderCompilationStatus(vID);

    unsigned int fID;
    fID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fID, 1, &fShaderCode, nullptr);
    glCompileShader(fID);
    CheckShaderCompilationStatus(fID);

    ID = glCreateProgram();
    /*glBindAttribLocation(ID, 0, "position");
    glBindAttribLocation(ID, 1, "texcoord");
    glBindAttribLocation(ID, 2, "normal");
    glBindAttribLocation(ID, 3, "color");*/
    glAttachShader(ID, vID);
    glAttachShader(ID, fID);
    glLinkProgram(ID);

    glDeleteShader(vID);
    glDeleteShader(fID);

    // std::cout << "VERTEX:\n" << vShaderCode << "\n\nFRAGMENT:\n" << fShaderCode << std::endl;
}

// ################################# PUBLIC #################################

void Shader::Bind() const
{
    glUseProgram(ID);
}

void Shader::Unbind()
{
    glUseProgram(0);
}

void Shader::SetUniform(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
}

void Shader::SetUniform(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetUniform(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetUniform(const std::string &name, float value1, float value2, float value3) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), value1, value2, value3);
}

void Shader::SetUniform(const std::string &name, float value1, float value2, float value3, float value4) const
{
    glUniform4f(glGetUniformLocation(ID, name.c_str()), value1, value2, value3, value4);
}

void Shader::SetUniform(const std::string &name, glm::vec4 vec4) const
{
    glUniform4f(glGetUniformLocation(ID, name.c_str()), vec4.x, vec4.y, vec4.z, vec4.w);
}

void Shader::SetUniform(const std::string &name, glm::mat4 matrix4x4) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, false, glm::value_ptr(matrix4x4));
}

void Shader::SetUniform(const std::string &name, glm::vec3 vec3) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), vec3.x, vec3.y, vec3.z);
}

void Shader::SetUniform(const std::string &name, const glm::vec3 &vec3arr, int length) const
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), length, glm::value_ptr(vec3arr));
}

void Shader::SetUniform(const std::string &name, const Color &color) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), color.r, color.g, color.b);
}
// ################################# END PUBLIC #################################

// ################################# PRIVATE #################################
void Shader::CheckShaderCompilationStatus(unsigned int shaderID)
{
    int success;
    char infoLog[512];
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(shaderID, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }
}

void Shader::CheckProgramCompilationStatus(unsigned int program)
{
    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }
}

// ################################# END PRIVATE #################################