#include <graphics/shapes/Cubes.hpp>
#include <utils/debug.hpp>
#define MODULE_NAME "CUBE"


static constexpr float FACE_VERTICES[6][30] = {
    // FRONT (+Z)
    {-0.5F, -0.5F, 0.5F, 0.0F, 0.0F,
     0.5F, -0.5F, 0.5F, 1.0F, 0.0F,
     0.5F, 0.5F, 0.5F, 1.0F, 1.0F,
     0.5F, 0.5F, 0.5F, 1.0F, 1.0F,
     -0.5F, 0.5F, 0.5F, 0.0F, 1.0F,
     -0.5F, -0.5F, 0.5F, 0.0F, 0.0F},

    // BACK (-Z)
    {-0.5F, -0.5F, -0.5F, 0.0F, 0.0F,
     0.5F, 0.5F, -0.5F, 1.0F, 1.0F,
     0.5F, -0.5F, -0.5F, 1.0F, 0.0F,
     0.5F, 0.5F, -0.5F, 1.0F, 1.0F,
     -0.5F, -0.5F, -0.5F, 0.0F, 0.0F,
     -0.5F, 0.5F, -0.5F, 0.0F, 1.0F},

    // LEFT (-X)
    {-0.5F, 0.5F, 0.5F, 1.0F, 0.0F,
     -0.5F, 0.5F, -0.5F, 1.0F, 1.0F,
     -0.5F, -0.5F, -0.5F, 0.0F, 1.0F,
     -0.5F, -0.5F, -0.5F, 0.0F, 1.0F,
     -0.5F, -0.5F, 0.5F, 0.0F, 0.0F,
     -0.5F, 0.5F, 0.5F, 1.0F, 0.0F},

    // RIGHT (+X)
    {0.5F, 0.5F, 0.5F, 1.0F, 0.0F,
     0.5F, -0.5F, -0.5F, 0.0F, 1.0F,
     0.5F, 0.5F, -0.5F, 1.0F, 1.0F,
     0.5F, -0.5F, -0.5F, 0.0F, 1.0F,
     0.5F, 0.5F, 0.5F, 1.0F, 0.0F,
     0.5F, -0.5F, 0.5F, 0.0F, 0.0F},

    // TOP (+Y)
    {-0.5F, 0.5F, -0.5F, 0.0F, 1.0F,
     0.5F, 0.5F, 0.5F, 1.0F, 0.0F,
     0.5F, 0.5F, -0.5F, 1.0F, 1.0F,
     0.5F, 0.5F, 0.5F, 1.0F, 0.0F,
     -0.5F, 0.5F, -0.5F, 0.0F, 1.0F,
     -0.5F, 0.5F, 0.5F, 0.0F, 0.0F},

    // BOTTOM (-Y)
    {-0.5F, -0.5F, -0.5F, 0.0F, 1.0F,
     0.5F, -0.5F, -0.5F, 1.0F, 1.0F,
     0.5F, -0.5F, 0.5F, 1.0F, 0.0F,
     0.5F, -0.5F, 0.5F, 1.0F, 0.0F,
     -0.5F, -0.5F, 0.5F, 0.0F, 0.0F,
     -0.5F, -0.5F, -0.5F, 0.0F, 1.0F}
};

unsigned int Cube::indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

Cube::Cube(ShaderProgram* main_shader, glm::vec3 pos, Texture2D *tex, Light* main_light, Camera* main_camera, uint8_t visible_faces)
    : shader(main_shader), main_camera(main_camera), main_light(main_light),
      cube_vbo(new VBO()), cube_ebo(new EBO()), pos(pos), tex(tex), visible_faces(visible_faces){    // Shader setup
    cube_vao = new VAO(cube_vbo);

    GenerateMesh();

    cube_vao->Bind();
    cube_vbo->Bind();
    cube_vbo->SetBufferData(mesh_vertices.size() * sizeof(float), mesh_vertices.data());
    cube_ebo->Bind();
    cube_ebo->SetBufferData(sizeof(indices), indices);

    // Location 0 (Position): index=0, size=3, stride_floats=5, offset_floats=0
    cube_vao->SetBufferData(0, 3, 5, 0);
    // Location 2 (TexCoords): index=2, size=2, stride_floats=5, offset_floats=3
    cube_vao->SetBufferData(2, 2, 5, 3);

    shader->Use();
    // https://learnopengl.com/Lighting/Materials
    glUniform3fv(glGetUniformLocation(shader->GetId(), "lightPos"), 1,
                 glm::value_ptr(main_light->GetPos()));
    glUniform3fv(glGetUniformLocation(shader->GetId(), "lightColor"), 1,
                 glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));

    glUniform3fv(glGetUniformLocation(shader->GetId(), "material.ambient"), 1,
        glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.31f)));
    glUniform3fv(glGetUniformLocation(shader->GetId(), "material.diffuse"), 1,
        glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.31f)));
    glUniform3fv(glGetUniformLocation(shader->GetId(), "material.specular"), 1,
        glm::value_ptr(glm::vec3(0.5f, 0.5f, 0.5f)));

    glUniform1f(glGetUniformLocation(shader->GetId(), "material.shininess"), 32.0f);

    if (shader->GetId() == 0) {
      Debug::Critical(MODULE_NAME, "Invalid shader program found. Exiting.");
      return;
    }

    shader->Use();
    Texture2D::Activate(GL_TEXTURE0);
    tex->Bind();
    glUniform1i(glGetUniformLocation(shader->GetId(), "texture1"), 0); // set it manually

}


void Cube::GenerateMesh() {
    mesh_vertices.clear();

    // Check each face and add vertices if visible
    for (int face = 0; face < 6; face++) {
        if (visible_faces & (1 << face)) {
            // Add all 30 floats (6 vertices * 5 floats) for this face
            for (int i = 0; i < 30; i++) {
                mesh_vertices.push_back(FACE_VERTICES[face][i]);
            }
        }
    }

    needs_remesh = false;
}

void Cube::UpdateMesh() {
    if (!needs_remesh) return;

    GenerateMesh();

    cube_vbo->Bind();
    cube_vbo->SetBufferData(mesh_vertices.size() * sizeof(float), mesh_vertices.data());
}

bool Cube::Draw(int view_loc, int model_loc) {
    if (needs_remesh) {
        UpdateMesh();
    }

    if (mesh_vertices.empty()) return false; // Nothing to draw

    shader->Use();
    Texture2D::Activate(GL_TEXTURE0);
    tex->Bind();
    cube_vao->Bind();

    glUniformMatrix4fv(view_loc, 1, GL_FALSE,
                       glm::value_ptr(main_camera->GetViewMatrix()));

    auto current_model = glm::mat4(1.0F);
    current_model = glm::translate(current_model, pos);

    glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(current_model));

    // Draw only the vertices we generated
    glDrawArrays(GL_TRIANGLES, 0, mesh_vertices.size() / 5);

    return true;
}
