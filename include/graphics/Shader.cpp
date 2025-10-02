#include "Shader.hpp"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif


#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <expected>
#include <utils/utilities.hpp>

std::expected<std::string, std::string> ReadShaderFromfile(const std::string& shader_file_name){
    auto exe_dir = GetExecutablePath();

    const auto SHADER_PATH = exe_dir / shader_file_name;

    if(!std::filesystem::exists(SHADER_PATH)) {
        return std::unexpected(std::format("Shader not found <{}>", SHADER_PATH.string()));
    }

    std::ifstream file(SHADER_PATH,std::ios::binary);
    if(!file) {
        return std::unexpected(std::format("Cannot open <{}>", SHADER_PATH.string()));
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();

}


Shader::Shader(unsigned int type, const char* source)
    : id(glCreateShader(type)){

    std::string shader_path;

   #if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
        shader_path = std::format("gamedata\\shaders\\{}", source);
    #else
        shader_path = std::format("gamedata/shaders/{}", source);
    #endif

    const std::expected<std::string, std::string> RESULT = ReadShaderFromfile(shader_path);

    if (!RESULT.has_value()) {
        std::cerr << "Failed to load shader source: " << RESULT.error() << '\n';
        this->id = 0;
        return;
    }

    const std::string& src = RESULT.value();
    const char* src_ptr = src.c_str();
    glShaderSource(this->id, 1, &src_ptr, nullptr);
    glCompileShader(this->id);

    // Check for compile errors
    int success = 0;
    char info_log[512];
    glGetShaderiv(this->id, GL_COMPILE_STATUS, &success);
    if (success == 0) {
        glGetShaderInfoLog(this->id, 512, nullptr, info_log);
        std::cerr << "Shader Compilation Failed:\n" << info_log << '\n';
    }
}

VertexShader::VertexShader(unsigned int type, const char* source) : Shader(type, source){
}


FragmentShader::FragmentShader(unsigned int type, const char* source) : Shader(type, source){

}


void ShaderProgram::Use() const{
    glUseProgram(this->id);
}

ShaderProgram::ShaderProgram() : id(glCreateProgram()), vertexshader(new VertexShader(GL_VERTEX_SHADER, "vertexShader.glsl")), fragmentshader(new FragmentShader(GL_FRAGMENT_SHADER, "fragmentShader.glsl")){

    if (this->vertexshader->GetId() == 0 || this->fragmentshader->GetId() == 0) {
        std::cerr << "CRITICAL: Shader objects failed to initialize. Aborting program linkage.\n";
        // Optionally, throw an exception or set this->id to 0 and return.
        this->id = 0;
        return;
    }

    glAttachShader(this->id, this->vertexshader->GetId());
    glAttachShader(this->id, this->fragmentshader->GetId());

    glLinkProgram(this->id);
    Use();

    glDeleteShader(this->vertexshader->GetId());
    glDeleteShader(this->fragmentshader->GetId());

    delete this->vertexshader;
    delete this->fragmentshader;

}
