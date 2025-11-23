#pragma  once
#include <graphics/textures/Texture.hpp>


class Block{
    public:
        Block(bool solid, int ID, Texture2D* texture);
        Block(bool solid, int ID);

        int GetID() const { return ID; };
        bool IsSolid() const { return isSolid; };
        Texture2D* GetTexture() const { return texture;};

    private:
        int ID = 0;
        bool isSolid = true;
        Texture2D* texture = nullptr;
};
