#pragma once
#include <graphics/shapes/Cubes.hpp>
#include <graphics/textures/Texture.hpp>
#include <graphics/textures/TextureAtlas.hpp>
#include <graphics/Shader.hpp>
#include <graphics/blocks/BlockManager.hpp>
#include <utils/utilities.hpp>
#include <glm/vec3.hpp>

class Chunk{
    public:
        Chunk(glm::vec3 position, BlockManager* blockManager, ShaderProgram* shader, Camera* camera, Light* lighting);
        void Initialize();
        void Draw(int view_loc,int model_loc);

        glm::vec3 GetChunkPosition() const { return chunkPosition; };
        glm::vec3 GetChunkSize() const { return chunkSize; };
        std::vector<Cube *> GetCubes() const { return chunkCubes; };

    private:
        glm::vec3 chunkPosition = { 0,0,0 };
        glm::vec3 chunkSize = { 16, 16, 16 };
        std::vector<Cube *> chunkCubes;
        BlockManager* blockManager = nullptr;
        ShaderProgram* shader;
        Camera* camera;
        Light* lighting;
};
