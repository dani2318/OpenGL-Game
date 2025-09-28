#pragma once

class EBO{
    public:
        EBO();

        void Unbind();
        void Bind();
        void SetBufferData(int size, unsigned int indices[]);

        unsigned int getID() const { return this->Id; };
    private:
        unsigned int Id;
};