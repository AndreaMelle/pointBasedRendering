#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <math.h>
#include <ctime>
#include <SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <resourceLoader.h>
#include <surfel.h>

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void createShaderProgram(const GLchar* vertSrc, const GLchar* fragSrc, GLuint& vertexShader, GLuint& fragmentShader, GLuint& shaderProgram)
{
	GLint result = GL_FALSE;
	
    // Create and compile the vertex shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, (const GLchar**)&vertSrc, NULL);
    glCompileShader(vertexShader);
    
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
    if (result != GL_TRUE) {
    	std::cout<<"Failed to load vertex shader"<<std::endl;
    }

    // Create and compile the fragment shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, (const GLchar**)&fragSrc, NULL);
    glCompileShader(fragmentShader);
    
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
    if (result != GL_TRUE) {
    	std::cout<<"Failed to load fragment shader"<<std::endl;
    }

    // Link the vertex and fragment shader into a shader program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
}

void specifySceneVertexAttributes(GLuint shaderProgram)
{
	
	GLint stride = 16 * sizeof(GLfloat);
	
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, stride, 0);

    GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(GLfloat)));

    GLint uAttrib = glGetAttribLocation(shaderProgram, "u");
    glEnableVertexAttribArray(uAttrib);
    glVertexAttribPointer(uAttrib, 3, GL_FLOAT, GL_FALSE, stride, (void*)(7 * sizeof(GLfloat)));
    
    GLint vAttrib = glGetAttribLocation(shaderProgram, "v");
    glEnableVertexAttribArray(vAttrib);
    glVertexAttribPointer(vAttrib, 3, GL_FLOAT, GL_FALSE, stride, (void*)(10 * sizeof(GLfloat)));
    
    GLint nAttrib = glGetAttribLocation(shaderProgram, "n");
    glEnableVertexAttribArray(nAttrib);
    glVertexAttribPointer(nAttrib, 3, GL_FLOAT, GL_FALSE, stride, (void*)(13 * sizeof(GLfloat)));
}

void createContext(GLFWwindow*& window)
{
	glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	
	window = glfwCreateWindow(640, 480, "Point-based rendering", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
		exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    glewInit();
}

void destroyContext(GLFWwindow*& window)
{
	glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

int main(void)
{
    GLFWwindow* window;
    createContext(window);
    
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_DEPTH_TEST);
    
    // Create VAOs and VBOs
    GLuint vaoScene;
    glGenVertexArrays(1, &vaoScene);

    GLuint vboScene;
    glGenBuffers(1, &vboScene);
	
	// Load surfel data
	Surfel* surfelSphere;
	GLfloat* sphereVertices;
	const GLint numpoints = 100;
	loadSurfelArray("./resources/models/spherePoints100.txt", surfelSphere);
	GLsizeiptr sphereByteSize = surfelArrayToVertexArray(sphereVertices,surfelSphere,numpoints);
	
	glBindBuffer(GL_ARRAY_BUFFER, vboScene);
    glBufferData(GL_ARRAY_BUFFER, sphereByteSize, sphereVertices, GL_STATIC_DRAW);
    
	unloadSurfelArray(surfelSphere);
	if (sphereVertices != 0) {
		delete[] sphereVertices;
	}

	sphereVertices = 0;

    // Create shader programs
    GLchar*   sceneVertexSource;
    GLchar*   sceneFragmentSource;
    
    loadShaderSource("./resources/shaders/pointVertexShader.vert", sceneVertexSource);
	loadShaderSource("./resources/shaders/pointFragmentShader.frag", sceneFragmentSource);
    
    GLuint sceneVertexShader, sceneFragmentShader, sceneShaderProgram;
    createShaderProgram(sceneVertexSource, sceneFragmentSource, sceneVertexShader, sceneFragmentShader, sceneShaderProgram);
    glUseProgram(sceneShaderProgram);

    // Specify the layout of the vertex data
    glBindVertexArray(vaoScene);
    glBindBuffer(GL_ARRAY_BUFFER, vboScene);
    specifySceneVertexAttributes(sceneShaderProgram);
    
    GLint uniModel = glGetUniformLocation(sceneShaderProgram, "model");
    
    glm::mat4 model;
    glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
    
    // Set up projection
    glm::mat4 view = glm::lookAt(
        glm::vec3(4.0f, 4.0f, 4.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f)
    );
    GLint uniView = glGetUniformLocation(sceneShaderProgram, "view");
    glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
	
	float fov = 45.0f; //degrees
	float winWidth  = 640.0f;
	float winHeight = 480.0f;
	float aspect = winWidth / winHeight;
	float near = 1.0f;
	float far = 10.0f;
	float top = tan(fov * 3.141593f / 360.0f) * near;
	float splatMult = -2.0f * ( near * winHeight / (2.0f * top) );
	
    glm::mat4 proj = glm::perspective(fov, aspect, near, far);
    GLint uniProj = glGetUniformLocation(sceneShaderProgram, "proj");
    glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

	GLint uniSplatMult = glGetUniformLocation(sceneShaderProgram, "splatMult");
	glUniform1f(uniSplatMult, splatMult);
	
	GLint uniRadius = glGetUniformLocation(sceneShaderProgram, "radius");
	// A very empirical computation of surfel radius in the case of unit sphere spampling
	// area of each sample: dA = 4 * PI / numsamples
	// square radius of each sample: r*r = dA / PI = 4 / numsamples
	// For non spherical models, radius actually depends on local sampling density! How to compute that?
	float radius = sqrt(4.0f / (float)numpoints);
	glUniform1f(uniRadius, radius);

    //GLint uniColor = glGetUniformLocation(sceneShaderProgram, "overrideColor");
	
    while (!glfwWindowShouldClose(window))
    {   
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Draw cube
        glDrawArrays(GL_POINTS, 0, numpoints);
    
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(sceneShaderProgram);
    glDeleteShader(sceneFragmentShader);
    glDeleteShader(sceneVertexShader);

    glDeleteBuffers(1, &vboScene);

    glDeleteVertexArrays(1, &vaoScene);

    destroyContext(window);
}
