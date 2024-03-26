#pragma once

#include "engine_includes.h"

struct Color;
class Shader
{
public:
    // Program ID
    unsigned int ID;

    // Constructor
    Shader(const char *vertexPath, const char *fragmentPath);

    // Binding
    void Bind() const;
    static void Unbind();

    // Uniforms setting
    void SetUniform(const std::string &name, bool value) const;
    void SetUniform(const std::string &name, int value) const;
    void SetUniform(const std::string &name, float value) const;
    void SetUniform(const std::string &name, float value1, float value2, float value3) const;
    void SetUniform(const std::string &name, float value1, float value2, float value3, float value4) const;
    void SetUniform(const std::string &name, glm::mat4 matrix4x4) const;
    void SetUniform(const std::string &name, glm::vec3 vec3) const;
    void SetUniform(const std::string &name, const glm::vec3 &vec3arr, int length) const;
    void SetUniform(const std::string &name, glm::vec4 vec4) const;
    void SetUniform(const std::string &name, const Color &color) const;

private:
    void CheckShaderCompilationStatus(unsigned int shaderID);
    void CheckProgramCompilationStatus(unsigned int programID);
};
