#include <GL/glew.h>
#include <glm/matrix.hpp>
#include <glm/ext.hpp>
#include "../headers/ShaderProgram.h"

ShaderProgram::ShaderProgram() : handle(0) {}

ShaderProgram::~ShaderProgram() {
    if (handle <= 0) {
        return;
    }

    GLint shaderAmount = 0;
    glGetProgramiv(handle, GL_ATTACHED_SHADERS, &shaderAmount);

    GLuint *shaderNames = new GLuint[shaderAmount];
    glGetAttachedShaders(handle, shaderAmount, nullptr, shaderNames);

    for (int i = 0; i < shaderAmount; i++) {
        glDeleteShader(shaderNames[i]);
    }

    glDeleteProgram(handle);

    delete [] shaderNames;
}

void ShaderProgram::compile(std::string fileName, GLuint shaderType) {
    if (handle <= 0) {
        handle = glCreateProgram();
        if (handle == 0) {
            throw ShaderProgramExeption("Can't create a shader program.");
        }
    }

    GLuint shader = glCreateShader(shaderType);
    if (shader == 0) {
        throw ShaderProgramExeption("Can't create a shader.");
    }

    std::string shaderText = loadTextFile(fileName);
    const GLchar *shaderCode = shaderText.c_str();
    const GLchar *codeArray[] = {shaderCode};
    glShaderSource(shader, 1, codeArray, NULL);
    glCompileShader(shader);

    GLint result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (GL_FALSE == result) {
        std::string text = "";
        text = text + "Can't compile a shader: " + fileName + "\n";

        GLint log_length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);

        GLchar *log_text = new GLchar[log_length];
        glGetShaderInfoLog(shader, log_length, nullptr, log_text);

        text = text +
               "----- Start Shader Log -----\n" +
               log_text + "\n" +
               "----- End Shader Log -----\n";

        delete [] log_text;
        std::cerr << text << std::endl;
        throw ShaderProgramExeption(text);
    }

    glAttachShader(handle, shader);
}

void ShaderProgram::link() {
    if (handle <= 0) {
        throw ShaderProgramExeption("You should emit the create method first.");
    }

    glLinkProgram(handle);

    GLint result;
    glGetProgramiv(handle, GL_LINK_STATUS, &result);
    if (GL_FALSE == result) {
        std::string text = "Can't link a shader program.\n";

        GLint log_length = 0;
        glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &log_length);

        if (log_length > 0) {
            GLchar *log_text = new GLchar[log_length];
            glGetProgramInfoLog(handle, log_length, nullptr, log_text);
            text = text + "----- Start Shader Program Log -----\n" +
                   log_text + "\n" +
                   "----- End Shader Program Log -----\n";
            delete [] log_text;
        }

        throw ShaderProgramExeption(text);
    }
}

void ShaderProgram::use() {
    if (handle <= 0) {
        throw ShaderProgramExeption("You should emit the create method first.");
    }

    glUseProgram(handle);
}

void ShaderProgram::bindAttribLocation(const GLuint location, const std::string name) {
    glBindAttribLocation(handle, location, name.c_str());
}

void ShaderProgram::setUniform(const std::string &name, const int value) {
    GLuint location = glGetUniformLocation(handle, name.c_str());
    glUniform1i(location, value);
}

void ShaderProgram::setUniform(const std::string &name, const float value) {
    GLuint location = glGetUniformLocation(handle, name.c_str());
    glUniform1f(location, value);
}

void ShaderProgram::setUniform(const std::string &name, const glm::mat3 &value) {
    GLuint location = glGetUniformLocation(handle, name.c_str());
    glUniformMatrix3fv(location, 1, GL_FALSE, &value[0][0]);
}

void ShaderProgram::setUniform(const std::string &name, const glm::mat4 &value) {
    GLuint location = glGetUniformLocation(handle, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
}

void ShaderProgram::setUniform(const std::string &name, const glm::vec3 &value) {
    GLuint location = glGetUniformLocation(handle, name.c_str());
    glUniform3f(location, value.x, value.y, value.z);
}

void ShaderProgram::setUniform(const std::string &name, const glm::vec4 &value) {
    GLuint location = glGetUniformLocation(handle, name.c_str());
    glUniform4f(location, value.x, value.y, value.z, value.w);
}
