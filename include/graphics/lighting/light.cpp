#include "light.hpp"
#include <graphics/shapes/Cubes.hpp>




Light::Light()
    : lightShader(new ShaderProgram("lightVertShader.glsl","lightFragShader.glsl"))
{

    lightVAO->Bind();
    lightVBO->Bind();
    lightVBO->SetBufferData(sizeof(vertices), vertices);
    lightEBO->Bind();
    lightEBO->SetBufferData(sizeof(indices), indices);

    // Location 0 (Position): index=0, size=3, stride_floats=5, offset_floats=0
    lightVAO->SetBufferData(0, 3, 5, 0);

    lightShader->Use();
    int proj_loc = glGetUniformLocation(lightShader->GetId(), "projection");
    glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(proj));

    glUniform3fv(glGetUniformLocation(lightShader->GetId(), "objectColor"), 1, glm::value_ptr(glm::vec3(1.0f,0.5f, 0.31f)));
    glUniform3fv(glGetUniformLocation(lightShader->GetId(), "lightColor"), 1, glm::value_ptr(glm::vec3(1.0f,1.0f, 1.0f)));

}

void Light::PaintLight(glm::mat4 cameraView){

    int model_loc = glGetUniformLocation(lightShader->GetId(), "model");
    int view_loc = glGetUniformLocation(lightShader->GetId(), "view");


    model = glm::mat4(1.0f); // Reset first!
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f));
    lightShader->Use();

    glUniformMatrix4fv(view_loc, 1, GL_FALSE,
                       glm::value_ptr(cameraView));

    glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));

    lightVAO->Bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    lightVAO->Unbind();

}
