#include "light.hpp"




Light::Light(Camera* main_camera)
    : lightShader(new ShaderProgram("lightVertShader.glsl","lightFragShader.glsl")), main_camera(main_camera)
{

    lightShader->Use();
    int proj_loc = glGetUniformLocation(lightShader->GetId(), "projection");
    glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(proj));

    glUniform3fv(glGetUniformLocation(lightShader->GetId(), "objectColor"), 1, glm::value_ptr(glm::vec3(1.0f,0.5f, 0.31f)));
    glUniform3fv(glGetUniformLocation(lightShader->GetId(), "lightColor"), 1, glm::value_ptr(glm::vec3(1.0f,1.0f, 1.0f)));
    lightCube = new LightCube(lightShader, main_camera);
}
void Light::PaintLight(glm::mat4 cameraView){


    int model_loc = glGetUniformLocation(lightShader->GetId(), "model");
    int view_loc = glGetUniformLocation(lightShader->GetId(), "view");

    lightCube->Draw(GetPos(), view_loc, model_loc);

}
