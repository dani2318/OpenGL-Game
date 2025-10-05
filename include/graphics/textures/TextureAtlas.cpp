#include "TextureAtlas.hpp"
#include "Texture.hpp"

TextureAtlas::TextureAtlas() : count(0), atlas_head(nullptr) {}

TextureAtlas::~TextureAtlas() {
    TextureAtlasElement* current = atlas_head;
    while (current != nullptr) {
        TextureAtlasElement* next = current->next;
        delete current;
        current = next;
    }
}

void TextureAtlas::AddTexture(Texture2D* element){

    TextureAtlasElement* new_atlas_el = new TextureAtlasElement();
    new_atlas_el->index = this->count;
    this->count++;
    new_atlas_el->texture = element;
    new_atlas_el->next = nullptr;
    new_atlas_el->prev = nullptr;

    if (atlas_head == nullptr) {
        atlas_head = new_atlas_el;
        return;
    }

    // Find the last element - fix loop condition
    TextureAtlasElement* current = atlas_head;
    while (current->next != nullptr) {
        current = current->next;
    }

    // Add to end of list
    current->next = new_atlas_el;
    new_atlas_el->prev = current;
}

TextureAtlasElement* TextureAtlas::GetElementByID(int index){
    for (TextureAtlasElement* el = atlas_head; el != nullptr; el = el->next) {
        if (el->index == index) {
            return el;
        }
    }
    return nullptr;
}
