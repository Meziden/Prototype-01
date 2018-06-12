#include "Camera.h"

Camera::Camera(glm::vec3 arg_pos,glm::vec3 arg_tar,glm::vec3 arg_up)
{
	position = arg_pos;
	target = arg_tar;
	up = arg_up;
	view = glm::lookAt(arg_pos,arg_tar,arg_up);
	
	//initialize yaw and pitch
	glm::vec3 tmpdir = glm::normalize(target - position);
	//57.29578 = 180 / 3.141592653590...
	yaw = 0;
	if(tmpdir.x != 0)
		yaw += 57.29578 * atan(tmpdir.z/tmpdir.x);
	else
		if(tmpdir.z > 0)
			yaw = 90;
		else
			yaw =270;
	
	if(tmpdir.x < 0)
		yaw += 180.0;
	
	pitch = 0;
	
	if(pow(tmpdir.x,2) + pow(tmpdir.z,2) != 0)
		pitch += 57.29578 * atan(tmpdir.y/sqrt(pow(tmpdir.x,2)+pow(tmpdir.z,2)));
	else
		if(tmpdir.y > 0)
			pitch = 89.0;
		else
			pitch = -89.0;
}

void Camera::Move(int direction,float step)
{
	glm::vec3 front = normalize(target - position);
	switch(direction)
	{
		case CAMERA_MOVEFRONT:
			position += front * step;
			target += front * step;
			break;
		case CAMERA_MOVEBACK:
			position -= front * step;
			target -= front * step;
			break;
		case CAMERA_MOVELEFT:
			position -= glm::normalize(glm::cross(front,up)) * step;
			target -= glm::normalize(glm::cross(front,up)) * step;
			break;
		case CAMERA_MOVERIGHT:
			position += glm::normalize(glm::cross(front,up)) * step;
			target += glm::normalize(glm::cross(front,up)) * step;
			break;
		case CAMERA_MOVEUP:
			position += glm::normalize(up) * step;
			target += glm::normalize(up) * step;
			break;
		case CAMERA_MOVEBOTTOM:
			position -= glm::normalize(up) * step;
			target -= glm::normalize(up) * step;
			break;
	}
	view = glm::lookAt(position,target,up);
}

void Camera::Rotate(float offset_x,float offset_y)
{
	yaw += offset_x * 0.05;
	pitch -= offset_y * 0.05;
	
	//comment these lines to disable the limit for looking around.
	if(pitch > CAMERA_PITCH_MAX)
		pitch = CAMERA_PITCH_MAX;
	if(pitch < CAMERA_PITCH_MIN)
		pitch = CAMERA_PITCH_MIN;
	
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	up.x = -cos(glm::radians(yaw)) * sin(glm::radians(pitch));
	up.y = cos(glm::radians(pitch));
	up.z = -sin(glm::radians(yaw)) * sin(glm::radians(pitch));
	
	target = position + front;
	view = glm::lookAt(position,target,up);
}

void Camera::Set(glm::vec3 position_arg,float yaw_arg, float pitch_arg)
{
	position = position_arg;
	yaw = yaw_arg;
	pitch = pitch_arg;
	
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	up.x = -cos(glm::radians(yaw)) * sin(glm::radians(pitch));
	up.y = cos(glm::radians(pitch));
	up.z = -sin(glm::radians(yaw)) * sin(glm::radians(pitch));
	
	target = position + front;
	view = glm::lookAt(position,target,up);
}
