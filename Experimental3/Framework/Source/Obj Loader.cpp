// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
// Include GLEW - OpenGL Extension Wrangler
#define GLEW_STATIC
#include <GL/glew.h>

// Include GLFW - OpenGL Framework (Multi Platform framework to Setup a window and OpenGL Context
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM - OpenGL Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//using namespace glm;
using namespace std;
//#include "shader.hpp"
#include"sceneLoader.h"

sceneLoader* scene;

int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Texture Mapping Tutorial", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "../Source/Shaders/SimpleTransform.vertexshader", "../Source/Shaders/SingleColor.fragmentshader" );

	// Get a handle for our Transformation Matrices uniform
	GLuint WorldMatrixID = glGetUniformLocation(programID, "WorldTransform");
	GLuint ViewMatrixID = glGetUniformLocation(programID, "ViewTransform");
	GLuint ProjMatrixID = glGetUniformLocation(programID, "ProjectonTransform");

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
	// Or, for an ortho camera :
	//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates
	
	// World Matrix - Identity - Draw Sphere at the origin
	glm::mat4 World(1.0f);

	// Camera matrix
	glm::mat4 View       = glm::lookAt(
								glm::vec3(5,5,5), // Camera is at (4,3,3), in World Space
								glm::vec3(0,0,0), // and looks at the origin
								glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
						   );
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model      = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP        = Projection * View * Model; // Remember, matrix multiplication is the other way around

	// Load the texture 
	//GLuint Texture = loadDDS("../Source/Models/uvmap.DDS");
		
	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");

	string filePath = "../Source/Models/wolf/Wolf.obj";
	//string filePath = "../Source/Models/house/house.obj";
	//string filePath = "../Source/Models/Thor/thor.obj";
	scene = new sceneLoader(&filePath[0]);
	
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	do{
		// Clear the screen and the depth buffer
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		// Use our shader
		glUseProgram(programID);

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		//glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(WorldMatrixID, 1, GL_FALSE, &World[0][0]);
		glUniformMatrix4fv(ViewMatrixID,  1, GL_FALSE, &View[0][0]);
		glUniformMatrix4fv(ProjMatrixID,  1, GL_FALSE, &Projection[0][0]);
		
		scene->draw(programID);
		
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO and shader
	/*glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);*/
	delete scene;
	glDeleteProgram(programID);
	/*glDeleteTextures(1, &TextureID);
	glDeleteVertexArrays(1, &VertexArrayID);*/

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

