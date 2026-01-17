#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
    // Store paths safely
    std::string vertexPath;
    std::string fragmentPath;
    std::string tessControlPath;
    std::string tessEvalPath;

    unsigned int ID = 0;

    void init(
        const char* vertexPathArg,
        const char* fragmentPathArg,
        const char* tessControlPathArg = nullptr,
        const char* tessEvalPathArg = nullptr)
    {
        // Build full paths
        vertexPath   = elaboratePath(vertexPathArg, "vert");
        fragmentPath = elaboratePath(fragmentPathArg, "frag");

        if (tessControlPathArg && tessEvalPathArg) {
            tessControlPath = elaboratePath(tessControlPathArg, "tc");
            tessEvalPath    = elaboratePath(tessEvalPathArg, "te");
        }

        // === Load shader source ===
        std::string commonCode;
        std::string vertexCode;
        std::string fragmentCode;
        std::string tControlCode;
        std::string tEvalCode;

        try {
            commonCode   = loadFile("shaders/common.glsl");
            vertexCode   = loadFile(vertexPath);
            fragmentCode = loadFile(fragmentPath);


            if (!tessControlPath.empty()) {
                tControlCode = loadFile(tessControlPath);
                tEvalCode    = loadFile(tessEvalPath);
            }
        }
        catch (const std::exception& e) {
            std::cerr << "ERROR::SHADER::FILE_READ_FAILED\n"
                      << e.what() << std::endl;
            return;
        }

        // Prepend common code
        vertexCode   = commonCode + "\n" + vertexCode;
        fragmentCode = commonCode + "\n" + fragmentCode;

        if (!tessControlPath.empty()) {
            tControlCode = commonCode + "\n" + tControlCode;
            tEvalCode    = commonCode + "\n" + tEvalCode;
        }

        // === Compile shaders ===
        GLuint vertex = compileShader(GL_VERTEX_SHADER, vertexCode, "VERTEX");
        GLuint fragment = compileShader(GL_FRAGMENT_SHADER, fragmentCode, "FRAGMENT");

        GLuint tessControl = 0, tessEval = 0;
        if (!tessControlPath.empty()) {
            tessControl = compileShader(GL_TESS_CONTROL_SHADER, tControlCode, "TESSELLATION CONTROL");
            tessEval    = compileShader(GL_TESS_EVALUATION_SHADER, tEvalCode, "TESSELLATION EVALUATION");
        }

        // === Link program ===
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);

        if (tessControl) {
            glAttachShader(ID, tessControl);
            glAttachShader(ID, tessEval);
        }

        glLinkProgram(ID);
        checkLinkErrors(ID);

        // Cleanup
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        if (tessControl) {
            glDeleteShader(tessControl);
            glDeleteShader(tessEval);
        }
    }

    // Activate shader
    void use() const {
        glUseProgram(ID);
    }

    // Uniform helpers
    void setBool(const std::string& name, bool value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }

    void setInt(const std::string& name, int value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    void setFloat(const std::string& name, float value) const {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

    void setVec2(const std::string& name, float x, float y) const {
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1,
                     glm::value_ptr(glm::vec2(x, y)));
    }

    void setVec3(const std::string& name, float x, float y, float z) const {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1,
                     glm::value_ptr(glm::vec3(x, y, z)));
    }

    void setVec3(const std::string& name, glm::vec3 xyz) const {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1,
                     glm::value_ptr(glm::vec3(xyz.x, xyz.y, xyz.z)));
    }

    void setVec4(const std::string& name, float x, float y, float z, float w) const {
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1,
                     glm::value_ptr(glm::vec4(x, y, z, w)));
    }

    void setMat4(const std::string& name, const glm::mat4& matrix) const {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1,
                           GL_FALSE, glm::value_ptr(matrix));
    }

private:
    // Build shader file path
    static std::string elaboratePath(const char* base, const std::string& suffix) {
        if (!base) return {};
        return "shaders/" + std::string(base) + "_" + suffix + ".glsl";
    }

    // Load file into string
    static std::string loadFile(const std::string& path) {
        std::ifstream file(path);
        if (!file)
            throw std::runtime_error("Failed to open file: " + path);

        std::stringstream ss;
        ss << file.rdbuf();
        return ss.str();
    }

    // Compile shader
    static GLuint compileShader(GLenum type, const std::string& source, const std::string& name) {
        GLuint shader = glCreateShader(type);
        const char* src = source.c_str();
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);

        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLchar infoLog[1024];
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR (" << name << ")\n"
                      << infoLog << std::endl;
        }
        return shader;
    }

    // Link error check
    static void checkLinkErrors(GLuint program) {
        GLint success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            GLchar infoLog[1024];
            glGetProgramInfoLog(program, 1024, nullptr, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR\n"
                      << infoLog << std::endl;
        }
    }
};

#endif // SHADER_H
