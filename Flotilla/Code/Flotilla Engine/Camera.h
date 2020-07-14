#pragma once
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

using namespace glm;

class Camera
{
private:
	vec3 position;
	vec3 forward;
	vec3 up;

public:
	Camera();
	~Camera();
	void setPosition(vec3 pos);
	void setForward(vec3 front);
	void setUp(vec3 upwards);
	void translateWorld(vec3 dPos);
	void translateLocal(vec3 strafe);

	vec3 getPosition() const;
	vec3 getForward() const;
	vec3 getRight() const;
	vec3 getUp() const;
	mat4 getView() const;
};

