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


class ComputeShader {
public:
    unsigned int ID, SSBO, size;
    int elementCount;
    glm::vec3 workGroupAmount;

    void init(const char* shaderPathArg, int wGSX, int wGSY, int wGSZ)
    {
        std::string shaderPath   = elaboratePath(shaderPathArg, "comp");
        // Generate the SSBO
        glGenBuffers(1, &SSBO);
        changeWorkGroupAmount(wGSX, wGSY, wGSZ);

        // 1. retrieve the vertex/fragment source code from filePath
        std::string code;
        std::ifstream shaderFile;
        // ensure ifstream objects can throw exceptions:
        shaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // open files
            shaderFile.open(shaderPath);
            std::stringstream shaderStream;
            // read file's buffer contents into streams
            shaderStream << shaderFile.rdbuf();
            // close file handlers
            shaderFile.close();
            // convert stream into string
            code = shaderStream.str();
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        }

        const char* shaderCode = code.c_str();

        // 2. compile shaders
        unsigned int compute;
        // vertex shader
        compute = glCreateShader(GL_COMPUTE_SHADER);
        glShaderSource(compute, 1, &shaderCode, NULL);
        glCompileShader(compute);
        checkCompileErrors(compute, "COMPUTE");

        // shader Program
        ID = glCreateProgram();
        glAttachShader(ID, compute);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");

        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(compute);
    }
    // activate the shader
    // ------------------------------------------------------------------------
    void use() {
        glUseProgram(ID);
        // glMemoryBarrier(GL_ALL_BARRIER_BITS);
    }
    void dispatch() {
        glDispatchCompute(workGroupAmount.x, workGroupAmount.y, 1);
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    }
    void changeWorkGroupAmount(int wGSX, int wGSY, int wGSZ) {
        workGroupAmount.x = wGSX;
        workGroupAmount.y = wGSY;
        workGroupAmount.z = wGSZ;
        elementCount = workGroupAmount.x * workGroupAmount.y * workGroupAmount.z;
        size = elementCount * sizeof(glm::vec4);

        // Update the SSBO
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);

        glBufferData(GL_SHADER_STORAGE_BUFFER, wGSX * wGSY * 1 * sizeof(glm::vec4), nullptr, GL_DYNAMIC_READ);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBO);
    }
    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const string &name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    // ------------------------------------------------------------------------
    void setInt(const string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setFloat(const string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setVec2(const string &name, float x, float y) const
    {
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(glm::vec2(x, y)));
    }
    // ------------------------------------------------------------------------
    void setVec3(const string &name, float x, float y, float z) const
    {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(glm::vec3(x, y, z)));
    }
    // ------------------------------------------------------------------------
    void setMat4(const string &name, glm::mat4 matrix) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
    }
    // ------------------------------------------------------------------------

private:
    static std::string elaboratePath(const char* base, const std::string& suffix) {
        if (!base) return {};
        return "shaders/" + std::string(base) + "_" + suffix + ".glsl";
    }
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(GLuint shader, std::string type)
    {
        GLint success;
        GLchar infoLog[1024];

        if(type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if(!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if(!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }

    }
};
#endif // SHADER_H
