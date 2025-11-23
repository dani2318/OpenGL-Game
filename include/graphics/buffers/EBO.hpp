#pragma once

class EBO{
    public:
        EBO();

        static void Unbind();
        void Bind() const;
        static void SetBufferData(int size, unsigned int indices[]);

        [[nodiscard]] unsigned int GetId() const { return this->id; };
    private:
        unsigned int id{};
};