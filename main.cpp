#include <GL/glew.h> 
#include <GL/glut.h>
#include <stdio.h>
#include <iostream>

// g++ main.cpp -lglut -lGL

class Vertex3f {
    public:
    float x;
    float y;
    float z;

    Vertex3f() :
            x(0), y(0), z(0) {}

    Vertex3f(float x, float y, float z) :
            x(x), y(y), z(z) {}
};

Vertex3f vertex[3];
GLuint VBO;

static void RenderSceneCB()
{
    // Cleans the color buffer
    glClear(GL_COLOR_BUFFER_BIT);

    // Activates the first array
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Draw point
    glDrawArrays(GL_POINTS, 0, 1);

    glDisableVertexAttribArray(0);

    glutSwapBuffers(); 
}

int main(int argc, char **argv)
{
    // Initialize library
    glutInit(&argc, argv); 

    // Setting double buffering + rgba colors
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); 

    // Set window dimensions, position and open it
    glutInitWindowSize(1024, 768);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Tutorial 01"); 

    // Registering callback
    glutDisplayFunc(RenderSceneCB); 

    // Initialize GLew
    GLenum res = glewInit();
    if (res != GLEW_OK)
    {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        return 1;
    }

    // Sets the color when cleaning the frame buffer (rgba)
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); 

    /**
     * X, Y and Z coordinates have the range
     * [-1.0.,1.0], so 0.0,0.0 is the center
     * X -> abcissa. From left to right
     * Y -> ordinate. From bottom to top
     * Z -> deep. From front to back
     */
    vertex[0] = Vertex3f(0.5f, 0.5f, 0.0f);

    // This creates the vertex buffer
    // Generates the buffer
    glGenBuffers(1, &VBO);
    // Binds the type of the buffer with the buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Sets the buffer data 
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

    // Gives the control to the GLUT loop
    glutMainLoop();
    return 0;
}
