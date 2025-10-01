#pragma once

class VBO{
    public:
        VBO();

        static void Unbind();
        void Bind() const;
        static void SetBufferData(int size, float data[]);

        [[nodiscard]] unsigned int GetId() const { return this->id; };
    private:
        unsigned int id{};
};