#include <graphics/map/Chunk.hpp>

Chunk::Chunk(glm::vec3 position, BlockManager* blockManager, ShaderProgram* shader, Camera* camera, Light* lighting)
    : chunkPosition(position), blockManager(blockManager), shader(shader), camera(camera), lighting(lighting)
{

}

void Chunk::Initialize(){
    for (int y = 0; y < y_size; y++) {
      for (int x = 0; x < x_size; x++) {
        for (int z = 0; z < z_size; z++) {
          int index = (y * x_size * z_size) + (x * z_size) + z;

          uint8_t visible_faces = CalculateVisibleFaces(x, y, z);
          chunkCubes.push_back(new Cube(shader, glm::vec3(x,y,z),
                                   blockManager->GetBlockByID(0)->GetTexture(),
                                   lighting, camera, visible_faces));
        }
      }
    }
}

void Chunk::Draw(int view_loc,int model_loc){
    for (Cube *cube : chunkCubes) {
      cube->Draw(view_loc, model_loc);
    }
}
