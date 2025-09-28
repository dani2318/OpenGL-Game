#pragma once

class VBO{
    public:
        VBO(int size, float data[]);

        void Unbind();
        void Bind();
        void SetBufferData(int size, float data[]);

        unsigned int getID() const { return this->ID; };
    private:
        unsigned int ID;
};