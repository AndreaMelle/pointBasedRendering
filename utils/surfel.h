#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <iostream>

// structure for Surfel data
struct Surfel
{
	GLfloat point[3];
	GLfloat color[4];
	GLfloat u[3];
	GLfloat v[3];
	GLfloat normal[3];
};

GLsizeiptr loadSurfelArray(const char* filename, Surfel*& surfels)
{
	std::ifstream file;
	file.open(filename, std::ios::in);
	if(!file) {
		return 0;
	}
	
	std::string line;
	std::getline(file, line);
	
	int len;
	std::istringstream in(line);
	in >> len;
	std::cout<<"Reading "<<len<<" surfels from file."<<std::endl;
	
	surfels = (Surfel*) new Surfel[len];
	if (surfels == 0) {
		return 0;
	}
	
	float x, y, z;
	float r, g, b, a;
	float ux, uy, uz;
	float vx, vy, vz;
	float nx, ny, nz;
	int i = 0;
	
	for(; std::getline(file, line); )
	{
	    std::istringstream in(line);
	    
		in >> x >> y >> z;
		in >> r >> g >> b >> a;
		in >> ux >> uy >> uz;
		in >> vx >> vy >> vz;
		in >> nx >> ny >> nz;
		
		surfels[i].point[0] = (GLfloat)x;
		surfels[i].point[1] = (GLfloat)y;
		surfels[i].point[2] = (GLfloat)z;
		
		surfels[i].color[0] = (GLfloat)r;
		surfels[i].color[1] = (GLfloat)g;
		surfels[i].color[2] = (GLfloat)b;
		surfels[i].color[3] = (GLfloat)a;
		
		surfels[i].u[0] = (GLfloat)ux;
		surfels[i].u[1] = (GLfloat)uy;
		surfels[i].u[2] = (GLfloat)uz;
		
		surfels[i].v[0] = (GLfloat)vx;
		surfels[i].v[1] = (GLfloat)vy;
		surfels[i].v[2] = (GLfloat)vz;
		
		surfels[i].normal[0] = (GLfloat)nx;
		surfels[i].normal[1] = (GLfloat)ny;
		surfels[i].normal[2] = (GLfloat)nz;
		
		i++;
	}
	
	file.close();
	return (GLsizeiptr)(len * sizeof(Surfel));
}

GLint unloadSurfelArray(Surfel*& surfels)
{
	if (surfels != 0) {
		delete[] surfels;
	}
	surfels = 0;
	return 0;
}

GLsizeiptr surfelArrayToVertexArray(GLfloat*& vertices, const Surfel* surfels, const int numsurfels)
{
	int dim = 16;
	int len = numsurfels * dim;
	
	vertices = (GLfloat*) new GLfloat[len];
	
	for(int i = 0; i < numsurfels; ++i) {
	
		vertices[dim * i +  0] = surfels[i].point[0];
		vertices[dim * i +  1] = surfels[i].point[1];
		vertices[dim * i +  2] = surfels[i].point[2];
		
		vertices[dim * i +  3] = surfels[i].color[0];
		vertices[dim * i +  4] = surfels[i].color[1];
		vertices[dim * i +  5] = surfels[i].color[2];
		vertices[dim * i +  6] = surfels[i].color[3];
		
		vertices[dim * i +  7] = surfels[i].u[0];
		vertices[dim * i +  8] = surfels[i].u[1];
		vertices[dim * i +  9] = surfels[i].u[2];
		
		vertices[dim * i + 10] = surfels[i].v[0];
		vertices[dim * i + 11] = surfels[i].v[1];
		vertices[dim * i + 12] = surfels[i].v[2];
		
		vertices[dim * i + 13] = surfels[i].normal[0];
		vertices[dim * i + 14] = surfels[i].normal[1];
		vertices[dim * i + 15] = surfels[i].normal[2];
	}
	
	return (GLsizeiptr)(len * sizeof(GLfloat));
}