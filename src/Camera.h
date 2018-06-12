/*
Provides a [view matrix] for OpenGL Applications
Perspective * [view matrix] * model matrix * vertices
*/

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const int CAMERA_MOVEFRONT  = 0;
const int CAMERA_MOVEBACK  = 1;
const int CAMERA_MOVELEFT  = 2;
const int CAMERA_MOVERIGHT  = 3;
const int CAMERA_MOVEUP  = 4;
const int CAMERA_MOVEBOTTOM = 5;

const float CAMERA_PITCH_MAX = 89.9;
const float CAMERA_PITCH_MIN = -89.9;

class Camera
{
public:
	Camera(glm::vec3 arg_pos,glm::vec3 arg_tar,glm::vec3 arg_up);
	
	void Rotate(float offset_x,float offset_y);
	
	void Move(int direction,float step);
	
	void Set(glm::vec3 position_arg,float yaw_arg,float pitch_arg);
	
	glm::vec3 position;
	glm::vec3 target;
	glm::vec3 up;
	
	float yaw;
	float pitch;
	
	glm::mat4 view;
};
