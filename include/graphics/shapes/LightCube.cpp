#include <graphics/shapes/LightCube.hpp>
#include <utils/debug.hpp>
#define MODULE_NAME "LightCube"


float LightCube::vertices[] = {
    -0.5F, -0.5F, -0.5F, 0.0F, 0.0F, 0.5F,  -0.5F, -0.5F, 1.0F, 0.0F,
    0.5F,  0.5F,  -0.5F, 1.0F, 1.0F, 0.5F,  0.5F,  -0.5F, 1.0F, 1.0F,
    -0.5F, 0.5F,  -0.5F, 0.0F, 1.0F, -0.5F, -0.5F, -0.5F, 0.0F, 0.0F,

    -0.5F, -0.5F, 0.5F,  0.0F, 0.0F, 0.5F,  -0.5F, 0.5F,  1.0F, 0.0F,
    0.5F,  0.5F,  0.5F,  1.0F, 1.0F, 0.5F,  0.5F,  0.5F,  1.0F, 1.0F,
    -0.5F, 0.5F,  0.5F,  0.0F, 1.0F, -0.5F, -0.5F, 0.5F,  0.0F, 0.0F,

    -0.5F, 0.5F,  0.5F,  1.0F, 0.0F, -0.5F, 0.5F,  -0.5F, 1.0F, 1.0F,
    -0.5F, -0.5F, -0.5F, 0.0F, 1.0F, -0.5F, -0.5F, -0.5F, 0.0F, 1.0F,
    -0.5F, -0.5F, 0.5F,  0.0F, 0.0F, -0.5F, 0.5F,  0.5F,  1.0F, 0.0F,

    0.5F,  0.5F,  0.5F,  1.0F, 0.0F, 0.5F,  0.5F,  -0.5F, 1.0F, 1.0F,
    0.5F,  -0.5F, -0.5F, 0.0F, 1.0F, 0.5F,  -0.5F, -0.5F, 0.0F, 1.0F,
    0.5F,  -0.5F, 0.5F,  0.0F, 0.0F, 0.5F,  0.5F,  0.5F,  1.0F, 0.0F,

    -0.5F, -0.5F, -0.5F, 0.0F, 1.0F, 0.5F,  -0.5F, -0.5F, 1.0F, 1.0F,
    0.5F,  -0.5F, 0.5F,  1.0F, 0.0F, 0.5F,  -0.5F, 0.5F,  1.0F, 0.0F,
    -0.5F, -0.5F, 0.5F,  0.0F, 0.0F, -0.5F, -0.5F, -0.5F, 0.0F, 1.0F,

    -0.5F, 0.5F,  -0.5F, 0.0F, 1.0F, 0.5F,  0.5F,  -0.5F, 1.0F, 1.0F,
    0.5F,  0.5F,  0.5F,  1.0F, 0.0F, 0.5F,  0.5F,  0.5F,  1.0F, 0.0F,
    -0.5F, 0.5F,  0.5F,  0.0F, 0.0F, -0.5F, 0.5F,  -0.5F, 0.0F, 1.0F};

unsigned int LightCube::indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

LightCube::LightCube(ShaderProgram* main_shader, Camera* main_camera) : shader(main_shader), main_camera(main_camera),  cube_vbo(new VBO()), cube_ebo(new EBO()){
    // Shader setup
    cube_vao = new VAO(cube_vbo);

    cube_vao->Bind();
    cube_vbo->Bind();
    cube_vbo->SetBufferData(sizeof(vertices), vertices);
    cube_ebo->Bind();
    cube_ebo->SetBufferData(sizeof(indices), indices);

    // Location 0 (Position): index=0, size=3, stride_floats=5, offset_floats=0
    cube_vao->SetBufferData(0, 3, 5, 0);
    // Location 2 (TexCoords): index=2, size=2, stride_floats=5, offset_floats=3
    cube_vao->SetBufferData(2, 2, 5, 3);

    shader->Use();

    if (!tex->Generate()) {
      Debug::Critical(MODULE_NAME, "Texture generation failed. Exiting.");
      return;
    }

    if (shader->GetId() == 0) {
      Debug::Critical(MODULE_NAME, "Invalid shader program found. Exiting.");
      return;
    }

    shader->Use();
    Texture2D::Activate(GL_TEXTURE0);
    tex->Bind();
    glUniform1i(glGetUniformLocation(shader->GetId(), "texture1"), 0); // set it manually

}

bool LightCube::Draw(glm::vec3 pos, int view_loc, int model_loc){

    shader->Use();
    Texture2D::Activate(GL_TEXTURE0);
    tex->Bind();
    cube_vao->Bind();

    glUniformMatrix4fv(view_loc, 1, GL_FALSE,
                       glm::value_ptr(main_camera->GetViewMatrix()));

    auto current_model = glm::mat4(1.0F); // Start fresh for each LightCube
    // Translate and Rotate the LightCube
    current_model = glm::translate(current_model, pos);
    float angle =
        20.0F * static_cast<float>(0); // Use the fixed rotation value
    current_model = glm::rotate(current_model, glm::radians(angle),
                                glm::vec3(1.0F, 0.3F, 0.5F));

    // Upload the Model matrix
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(current_model));


    // Draw the LightCube
    glDrawArrays(GL_TRIANGLES, 0, 36);

    return true;
}
