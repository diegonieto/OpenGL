#include <GL/glut.h>

static void RenderSceneCB()
{
    // Cleans the color buffer
    glClear(GL_COLOR_BUFFER_BIT);
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

    // Sets the color when cleaning the frame buffer (rgba)
    glClearColor(0.0f, 1.0f, 0.0f, 0.0f); 

    // Gives the control to the GLUT loop
    glutMainLoop();
    return 0;
}
