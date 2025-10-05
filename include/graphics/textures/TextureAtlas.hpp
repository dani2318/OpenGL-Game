#pragma once

#include "Texture.hpp"
#include <graphics/textures/Texture.hpp>

struct TextureAtlasElement {
    int index = -1;
    Texture2D* texture = nullptr;
    TextureAtlasElement* next = nullptr;
    TextureAtlasElement* prev = nullptr;
};

class TextureAtlas {

    public:
        TextureAtlas();
        ~TextureAtlas();
        void AddTexture(Texture2D* element);
        TextureAtlasElement* GetElementByID(int index);

    private:
        TextureAtlasElement* atlas_head;
        int count;
};
