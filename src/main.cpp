#include <iostream>
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

const int SCREEN_HEIGHT = 720;
const int SCREEN_WIDTH = 1280;

//Camera
//Notice:using OpenGL native coordinates
glm::vec3 view_pos(0.0f,0.0f,7.0f);
glm::vec3 view_target(0.0f,0.0f,0.0f);
glm::vec3 view_up(0.0f,1.0f,0.0f);
Camera camera1(view_pos,view_target,view_up);

//Mouse
int firstpos = 1;
double xpos_s,ypos_s = 0;

void mouseproc_callback(GLFWwindow* window,double xpos,double ypos);

int main(int argc, char** argv)
{
	//Initialization
	if(!glfwInit())
	{
		std::cout<<"Error:GLFW init failed."<<std::endl;
		return -1;
	}
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	
	GLFWwindow* window = glfwCreateWindow(	SCREEN_WIDTH,
											SCREEN_HEIGHT,
											"S&S Project / OpenGL 4.5",
											NULL,
											NULL);
	if(!window)
	{
		glfwTerminate();
		std::cout<<"Error:Failed to Create Window."<<std::endl;
		return -1;
	}
	
	glfwMakeContextCurrent(window);
	
	glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
    	std::cout<<"GLEW_ERROR"<<std::endl;
    	std::cout<<glewGetErrorString(err)<<std::endl;
        return -1;
    }
    
    std::cout<<"Init Passed"<<std::endl;
    
    //Shader(s)
	Shader shader_obj("./GLSL/Vertex.glsl","./GLSL/Fragment.glsl");
	Shader shader_ssp("./GLSL/Vertex_ssp.glsl","./GLSL/Fragment_ssp.glsl");
	shader_obj.use();
    
    //Configs
    glfwSetInputMode(window, GLFW_CURSOR,GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseproc_callback);
    glEnable(GL_DEPTH_TEST);
    //model transformation
    glm::mat4 trans_mod = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0,0.0,0.0));
    //projection transformaion
    glm::mat4 trans_proj = glm::perspective(glm::radians(60.0f),(float)SCREEN_WIDTH/(float)SCREEN_HEIGHT,0.1f,100.0f);
	//send config to shader
	shader_obj.setMat4("trans_mod",trans_mod);
	shader_obj.setMat4("trans_proj",trans_proj);
	shader_obj.setMat4("trans_view",camera1.view);
	
	shader_obj.setVec3("lightPosV",glm::vec3(14.0f,26.0f,22.4f));
	shader_obj.setVec3("ViewPos",camera1.position);
    
    //Objects
    Model cube("res/hex.obj");
    
    //FBO
    GLuint FBO;
    glGenFramebuffers(1,&FBO);
    glBindFramebuffer(GL_FRAMEBUFFER,FBO);
    
    //RGBA Buffer
    GLuint texture_fbo;
	glGenTextures(1, &texture_fbo);
	glBindTexture(GL_TEXTURE_2D, texture_fbo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0,
				GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_MIRRORED_REPEAT);
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_fbo,0);
	
	//RBO
	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCREEN_WIDTH, SCREEN_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER, rbo);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	
	//External VAO VBO EBO
	float vbo_array[]={	-1.0f,  1.0f,0.0f,  0.0f, 1.0f,
        				-1.0f, -1.0f,0.0f,  0.0f, 0.0f,
         				1.0f, -1.0f,0.0f,  1.0f, 0.0f,

        				-1.0f,  1.0f,0.0f  ,0.0f, 1.0f,
        				1.0f, -1.0f, 0.0f ,1.0f, 0.0f,
        				1.0f,  1.0f, 0.0f ,1.0f, 1.0f};
	
	unsigned int ebo_array[] = {0,1,2,
								3,4,5};
	GLuint VAO,VBO,EBO;
	glCreateVertexArrays(1,&VAO);
	glBindVertexArray(VAO);
	
	glCreateBuffers(1,&VBO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glNamedBufferStorage(VBO,sizeof(vbo_array),vbo_array,0);
	
	glCreateBuffers(1,&EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
	glNamedBufferStorage(EBO,sizeof(ebo_array),ebo_array,0);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)(3*sizeof(float)));
	
	//END
    
    //Render Loop
    glm::vec3 modelpos(0.0f,0.0f,0.15f);
    while(!glfwWindowShouldClose(window))
    {
    	glBindFramebuffer(GL_FRAMEBUFFER,FBO);
    	glEnable(GL_DEPTH_TEST);
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    	
    	shader_obj.use();
    	shader_obj.setMat4("trans_mod",trans_mod);
    	shader_obj.setMat4("trans_view",camera1.view);
    	shader_obj.setVec3("ViewPos",camera1.position);
    	shader_obj.setVec4("NodeColor",glm::vec4(0.7f,0.7f,0.7f,1.0f));
    	
    	cube.render();
    	
    	glBindFramebuffer(GL_FRAMEBUFFER,0);
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
    	
    	shader_ssp.use();
    	glBindVertexArray(VAO);
		glBindTexture(GL_TEXTURE_2D,texture_fbo);
    	glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
    	
    	glfwSwapBuffers(window);
    	
    	//Events processing
    	glfwPollEvents();
    	
    	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera1.Move(CAMERA_MOVEFRONT,0.05f);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera1.Move(CAMERA_MOVEBACK,0.05f);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera1.Move(CAMERA_MOVELEFT,0.05f);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    		camera1.Move(CAMERA_MOVERIGHT,0.05f);
    	
    	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        	glfwSetWindowShouldClose(window, true);
    }
	return 0;
}

void mouseproc_callback(GLFWwindow* window,double xpos,double ypos)
{
	if(firstpos)
	{
		firstpos = 0;
		xpos_s = xpos;
		ypos_s = ypos;
	}
	double xoffset = xpos - xpos_s;
	double yoffset = ypos - ypos_s;
	xpos_s = xpos;
	ypos_s = ypos;
	
	camera1.Rotate(xoffset,yoffset);
}
