#pragma once

#include "VBO.h"

class VAO{
    public:
        VAO(VBO* vbo);

        void Unbind();
        void Bind();
        void SetBufferData(int size);

        unsigned int getID() const { return this->Id; };
    private:
        unsigned int Id;
        VBO* vbo = nullptr;
};