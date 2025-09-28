#pragma once

class VBO{
    public:
        VBO();

        void Unbind();
        void Bind();
        void SetBufferData(int size, float data[]);

        unsigned int getID() const { return this->ID; };
    private:
        unsigned int ID;
};