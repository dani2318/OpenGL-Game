#include "Block.hpp"


Block::Block(bool solid, int ID,  Texture2D* texture)
    : isSolid(solid), texture(texture), ID(ID)
{

}

Block::Block(bool solid, int ID)
    : isSolid(solid), ID(ID)
{

}
