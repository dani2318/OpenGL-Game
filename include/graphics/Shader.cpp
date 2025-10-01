#include "Shader.hpp"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <expected>


std::expected<std::filesystem::path, std::string> GetExecutablePath(){
    constexpr unsigned long BUFFER_SIZE = MAX_PATH;
    char path[BUFFER_SIZE];

    unsigned long result = GetModuleFileNameA(nullptr, path, BUFFER_SIZE);

    if (result == 0) {
        return std::unexpected(std::format("Failed to get executable path, error: {}", GetLastError()));
    }

    if (result >= BUFFER_SIZE) {
        return std::unexpected("Executable path too long");
    }

    return std::filesystem::path(path).parent_path();

}

std::expected<std::string, std::string> ReadShaderFromfile(const std::string& shader_file_name){
    auto exe_dir = GetExecutablePath();

    if(!exe_dir) {
        return std::unexpected(exe_dir.error());
}

    const auto SHADER_PATH = *exe_dir / shader_file_name;

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

    std::string shader_path = std::format("gamedata\\shaders\\{}", source);
    const std::expected<std::string, std::string> RESULT = ReadShaderFromfile(shader_path);

    if (!RESULT.has_value()) {
        std::cerr << "Failed to load shader source: " << RESULT.error() << '\n';
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

    glAttachShader(this->id, this->vertexshader->GetId());
    glAttachShader(this->id, this->fragmentshader->GetId());

    glLinkProgram(this->id);
    Use();

    glDeleteShader(this->vertexshader->GetId());
    glDeleteShader(this->fragmentshader->GetId());

    delete this->vertexshader;
    delete this->fragmentshader;

}
