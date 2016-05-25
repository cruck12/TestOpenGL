//Cube.h
#pragma once
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
#include "Shader_Loader.h"
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

using namespace std;

		class Cube
		{
		public:
			Cube();
			~Cube();

			int drawCube();
			int drawCube(glm::mat4 translate);
		};