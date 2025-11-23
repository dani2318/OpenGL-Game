#pragma once
#include <vector>

#include <graphics/blocks/Block.hpp>
#include <graphics/textures/Texture.hpp>
#include <graphics/textures/TextureAtlas.hpp>

class BlockManager{

    public:
        BlockManager();
        Block* GetBlockByID(int id);
        [[nodiscard]] int GetBlockCount() const { return BlockCount; };

    private:
        int BlockCount = 0;
        std::vector<Block*> blocks;
};
