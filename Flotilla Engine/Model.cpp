#include "Model.h"


Model::Model()
	:transform{ glm::mat4{ 1 } }
{

}

Model::~Model()
{
}

void Model::Draw(Shader shader)
{
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "modelToWorld"), 1, GL_FALSE, glm::value_ptr(transform));
	mesh->Draw(shader);
}
