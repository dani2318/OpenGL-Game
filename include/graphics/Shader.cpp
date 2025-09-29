#include "Shader.h"

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
#include <print>


std::expected<std::filesystem::path, std::string> getExecPath(){
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

std::expected<std::string, std::string> readShaderFromfile(const std::string& shaderFileName){
    auto exe_dir = getExecPath();

    if(!exe_dir)
        return std::unexpected(exe_dir.error());
    
    const auto shaderPath = *exe_dir / shaderFileName;

    if(!std::filesystem::exists(shaderPath))
        return std::unexpected(std::format("Shader not found <{}>", shaderPath.string()));

    std::ifstream file(shaderPath,std::ios::binary);
    if(!file)
        return std::unexpected(std::format("Cannot open <{}>", shaderPath.string()));
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();

}


Shader::Shader(unsigned int type, const char* source)
    : Id(glCreateShader(type)), Type(type){

    std::string shaderPath = std::format("gamedata\\shaders\\{}", source);
    const std::expected<std::string, std::string> result = readShaderFromfile(shaderPath);
    
    if (!result.has_value()) {
        std::cerr << "Failed to load shader source: " << result.error() << std::endl;
        return;
    }

    const std::string& src = result.value();
    const char* srcPtr = src.c_str();
    glShaderSource(this->Id, 1, &srcPtr, NULL);
    glCompileShader(this->Id);

    // Check for compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(this->Id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(this->Id, 512, nullptr, infoLog);
        std::cerr << "Shader Compilation Failed:\n" << infoLog << std::endl;
    }
}

VertexShader::VertexShader(unsigned int type, const char* source) : Shader(type, source){
}


FragmentShader::FragmentShader(unsigned int type, const char* source) : Shader(type, source){

}


void ShaderProgram::use(){
    glUseProgram(this->Id);
}

ShaderProgram::ShaderProgram(){

    this->Id = glCreateProgram();

    this->Vertexshader = new VertexShader(GL_VERTEX_SHADER, "vertexShader.glsl");
    this->Fragmentshader = new FragmentShader(GL_FRAGMENT_SHADER, "fragmentShader.glsl");

    glAttachShader(this->Id, this->Vertexshader->getID());
    glAttachShader(this->Id, this->Fragmentshader->getID());
    
    glLinkProgram(this->Id);
    use();

    glDeleteShader(this->Vertexshader->getID());
    glDeleteShader(this->Fragmentshader->getID());

    delete this->Vertexshader;
    delete this->Fragmentshader;

}

