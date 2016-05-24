/**********************************************************************

  Cube example - transformations and animations

  June, 8th, 2000

  This tutorial was written by Philipp Crocoll
  Contact: 
	philipp.crocoll@web.de
	www.codecolony.de

  Every comment would be appreciated.

  If you want to use parts of any code of mine:
	let me know and
	use it!

***********************************************************************/
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
#include "Shader_Loader.h"
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

using namespace std;

GLuint program;
int counter;
glm::vec3 moveCube = glm::vec3(0.0f,0.0f,0.0f);

const GLuint WIDTH=800;
const GLuint HEIGHT=600;
const float PI =3.141f;

void renderScene(void)
{ 
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glClearColor(0.0, 0.0, 0.0, 1.0);//clear red 

   //use the created program (shaders)
    glUseProgram(program);
 // Create transformations
        glm::mat4 view;
        glm::mat4 projection;
		glm::mat4 model;
		glm::mat3 normalScaleMatrix;

//        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -4.0f));
		view = glm::lookAt(glm::vec3(0.0f, 0.0f, 4.0f), 
  						glm::vec3(0.0f, 0.0f, 0.0f), 
  						glm::vec3(0.0f, 1.0f, 0.0f));

        projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);	

		//start translating to mouse pointer
        model = glm::translate(model, moveCube);

		//rotate with time		
		model = glm::rotate(model,0.05f*counter,glm::vec3(0.2f, 0.4f, 0.5f));

		//To scale normals according to world view
		normalScaleMatrix= glm::mat3(transpose(inverse(model)));

        // Get their uniform location
        GLint modelLoc = glGetUniformLocation(program, "model");
        GLint viewLoc = glGetUniformLocation(program, "view");
        GLint projLoc = glGetUniformLocation(program, "projection");		
        GLint normalScaleMatrixLoc = glGetUniformLocation(program, "normalScaleMatrix");
        // Pass the matrices to the shader
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // Note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix3fv(normalScaleMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalScaleMatrix));

		//Set the global light color
		// set object color for this object (special case, hardcoded)
        GLint LightColorLoc = glGetUniformLocation(program, "LightColor");
        GLint objectColorLoc = glGetUniformLocation(program, "objectColor");
		glUniform3f(objectColorLoc, 0.2f, 0.5f, 0.31f);
		glUniform3f(LightColorLoc,  1.0f, 1.0f, 1.0f);
		//Pass the position of light source
		GLint lightPosLoc = glGetUniformLocation(program, "lightPos");
		glUniform3f(lightPosLoc, 0.0f, 0.0f, 1.0f);

		//Pass the position of camera to determing specular lighting
		GLint viewPosLoc = glGetUniformLocation(program, "viewPos");
		glUniform3f(viewPosLoc, 0.0f, 0.0f, 4.0f);  


		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		glutSwapBuffers();    

}

void rotateCube(void){
	counter++;	
	glutPostRedisplay();
}

void moveToMouse(GLint button, GLint state, GLint x, GLint y){
	if(button==GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		//to convert screen coordinates to world coordinates
		//first, convert to NDC assuming z=0
		//added 4x scaling to ~match extended camera positions(since camera is at 0,0,4)
		GLfloat xScr = (2.0*x/WIDTH -1)*4; 
		GLfloat yScr = (-2.0*y/HEIGHT +1)*4;

		//converting NDC to world space using inverse transforms
		glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 4.0f), 
						glm::vec3(0.0f, 0.0f, 0.0f), 
  						glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
		glm::mat4 inverseWorld = glm::inverse(projection*view);
		glm::vec4 coordinateWorld = inverseWorld*glm::vec4(xScr,yScr,moveCube.z,0);

		//setting the translation vector to new coordinates
		moveCube= glm::vec3(coordinateWorld);
	}
}

int main(int argc, char **argv)
{
 
	glutInit(&argc, argv);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(WIDTH, HEIGHT); //optional

	glutCreateWindow("OpenGL First Window");
	glEnable(GL_DEPTH_TEST);

	glewInit();
	if (glewIsSupported("GL_VERSION_4_5"))
	{
	  std::cout << " GLEW Version is 4.5\n ";
	}
	else
	{
	   std::cout << "GLEW 4.5 not supported\n ";
	}
	cout << glGetString(GL_VERSION);
	
	// Define the viewport dimensions - doesnt seem to matter
//    glViewport(0, 0, 600, 800);


	Core::Shader_Loader shaderLoader;
	program = shaderLoader.CreateProgram("Vertex_Shader.glsl",
                                         "Fragment_Shader.glsl");
 
	// Set up vertex data (and buffer(s)) and attribute pointers
    GLfloat vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);	
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
	//normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	
//    glBindVertexArray(0); // Unbind VAO




		// register callbacks
	glutDisplayFunc(renderScene);
	glutIdleFunc(rotateCube);
	glutMouseFunc(moveToMouse);

	glutMainLoop();


	glDeleteProgram(program);
    glDeleteBuffers(1, &VBO);
 
	return 0;
}