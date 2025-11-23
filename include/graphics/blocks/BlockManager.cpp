#include <graphics/blocks/BlockManager.hpp>
#include <utils/debug.hpp>
#include <utils/json_parser.hpp>
#include <utils/utilities.hpp>

#define LOG_MODULE "BlockManager"

BlockManager::BlockManager() {
  std::filesystem::path execPath = GetExecutablePath();
  for (const auto &entry : std::filesystem::directory_iterator(
           std::string(execPath.generic_string() +
                       std::string("\\gamedata\\blockData\\")))) {
    Json root = Json::parse(read_file(entry.path().string()));
    int blockID = root.as_object().at("id").as_number();
    bool isSolid = root.as_object().at("isSolid").as_bool();
    std::string texturePath = root.as_object().at("texture").as_string();

    Debug::Info(LOG_MODULE, "Added block with id %d", blockID);
    auto *tex = new Texture2D(texturePath);
    if (!tex->Generate()) {
      Debug::Critical(LOG_MODULE, "Texture generation failed. Exiting.");
      exit(-1);
    }
    auto *block = new Block(isSolid, blockID, tex);
    blocks.push_back(block);
    BlockCount++;
    Debug::Info(LOG_MODULE, "Current block count: %d", BlockCount);
  }
}

Block *BlockManager::GetBlockByID(int id) {
  for (Block *block : blocks) {
    if (block->GetID() == id)
      return block;
  }
  return nullptr;
}
