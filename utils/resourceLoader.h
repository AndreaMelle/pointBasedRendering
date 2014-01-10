#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iostream>

// Create a texture from an image file
GLuint loadTexture(const GLchar* path)
{
    GLuint texture;
    glGenTextures(1, &texture);
    
    int width, height;
    unsigned char* image;
    
    glBindTexture(GL_TEXTURE_2D, texture);
    image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return texture;
}

GLint getFileLength(std::ifstream& file)
{
	if (!file.good()) {
		return 0;
	}
	
	unsigned long pos = file.tellg();
	file.seekg(0, std::ios::end);
	unsigned long len = file.tellg();
	file.seekg(std::ios::beg);
	
	return (GLint)len;
}

GLint loadShaderSource(const char* filename, GLchar*& shaderSource)
{
	std::ifstream file;
	file.open(filename, std::ios::in); // opens as ASCII!
	if(!file) {
		return -1;
	}
	
	GLint len = getFileLength(file);
	
	if (len==0) return -2;   // Error: Empty File 
	
	shaderSource = (GLchar*) new char[len+1];
	if (shaderSource == 0) {
		return -3;   // can't reserve memory
	}
	
	// len isn't always strlen cause some characters are stripped in ascii read...
	// it is important to 0-terminate the real length later, len is just max possible value... 
	shaderSource[len] = 0; 
	
	unsigned int i=0;
	while (file.good())
	{
		shaderSource[i] = file.get();       // get character from file.
		if (!file.eof())
		i++;
	}
	
	shaderSource[i] = 0;  // 0-terminate it at the correct position
	
	file.close();
	  
	return len; // No Error
}

GLint unloadShaderSource(GLchar*& shaderSource)
{
	if (shaderSource != 0) {
		delete[] shaderSource;
	}
	shaderSource = 0;
	return 0;
}

// return size of data in bytes, useful for glBufferData calls.
GLsizeiptr loadVertexArray(const char* filename, GLfloat*& vertices)
{
	std::ifstream file;
	file.open(filename, std::ios::in);
	if(!file) {
		return 0;
	}
	
	int len;
	file >> len;
	std::cout<<"Reading "<<len<<" vertices from file."<<std::endl;
	
	vertices = (GLfloat*) new GLfloat[len];
	if (vertices == 0) {
		return 0;
	}
	
	float x;
	int i = 0;
	while (file >> x)
	{
		vertices[i] = (GLfloat)x;
		i++;
	}
	
	file.close();
	return (GLsizeiptr)(len * sizeof(GLfloat));
}

GLint unloadVertexArray(GLfloat*& vertices)
{
	if (vertices != 0) {
		delete[] vertices;
	}
	vertices = 0;
	return 0;
}