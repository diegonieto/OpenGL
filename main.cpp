#include <math.h>
#include <GL/glew.h> 
#include <GL/glut.h>
#include <stdio.h>
#include <iostream>
#include <string.h>

// Vertex shader. This will modify the vertex attributes
static const char* pVS = "                                                    \n\
#version 330                                                                  \n\
                                                                              \n\
layout (location = 0) in vec3 Position;                                       \n\
                                                                              \n\
//uniform float glScale;                                                        \n\
uniform mat4 gWorld;                                                        \n\
                                                                              \n\
void main()                                                                   \n\
{                                                                             \n\
    //gl_Position = vec4(glScale * Position.x, Position.y, Position.z, 1.0);    \n\
    gl_Position = gWorld * vec4(Position, 1.0);    \n\
}";

// Fragment shared. This will modify the pixel colors
static const char* pFS = "                                                    \n\
#version 330                                                                  \n\
                                                                              \n\
out vec4 FragColor;                                                           \n\
                                                                              \n\
void main()                                                                   \n\
{                                                                             \n\
    FragColor = vec4(0.0, 0.0, 1.0, 0.8);                                     \n\
}";


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

struct Matrix4f
{
    float m[4][4];
};

Vertex3f vertex[3];
GLuint VBO;
GLuint gWorldLocation;
GLfloat gScaleLocation;

static void RenderSceneCB()
{
    // Cleans the color buffer
    glClear(GL_COLOR_BUFFER_BIT);

    static float scale = 0.0f;

    scale += 0.01;
    glUniform1f(gScaleLocation, sinf(scale));

    Matrix4f World;

    World.m[0][0] = 1.0f; World.m[0][1] = 0.0f; World.m[0][2] = 0.0f; World.m[0][3] = sinf(scale);
    World.m[1][0] = 0.0f; World.m[1][1] = 1.0f; World.m[1][2] = 0.0f; World.m[1][3] = cosf(scale);
    World.m[2][0] = 0.0f; World.m[2][1] = 0.0f; World.m[2][2] = 1.0f; World.m[2][3] = 0.0f;
    World.m[3][0] = 0.0f; World.m[3][1] = 0.0f; World.m[3][2] = 0.0f; World.m[3][3] = 1.0f;


    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &World.m[0][0]);

    glColor3f(0.5, 0.0, 0.3);

    // Activates the first array
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Draw a triangle
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(0);

    glutSwapBuffers(); 
}

void addShader(GLuint shaderProgram, const char* pShaderText, GLenum shaderType)
{
    GLuint shaderObj = glCreateShader(shaderType);

    if (shaderObj == 0) {
        fprintf(stderr, "Error creating shader type %d\n", shaderType);
        exit(0);
    }

    const GLchar* p[1];
    p[0] = pShaderText;
    GLint Lengths[1];
    Lengths[0]= strlen(pShaderText);

    /**
      * GLuint   : handle -> OpenGL shader object
      * GLsizei  : count  -> number of sources. In this case only one source to build the shader
      * GLchar** : string -> array of pointers to the string sources
      * GLint*   : length -> array of the string lenghts
      */
    glShaderSource(shaderObj, 1, p, Lengths);
    glCompileShader(shaderObj);

    // Verifier process
    GLint success;
    glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(shaderObj, 1024, NULL, InfoLog);
        fprintf(stderr, "Error compiling shader type %d: '%s'\n", shaderType, InfoLog);
        exit(1);
    }

   glAttachShader(shaderProgram, shaderObj);
}

void compileShaders()
{
    GLuint shaderProgram = glCreateProgram();

    if (shaderProgram == 0) {
        fprintf(stderr, "Error creating shader program\n");
        exit(1);
    }

    // Create and attach each shader processor to the program object
    addShader(shaderProgram, pVS, GL_VERTEX_SHADER);
    addShader(shaderProgram, pFS, GL_FRAGMENT_SHADER);

    GLint Success = 0;
    GLchar ErrorLog[1024] = { 0 };

    glLinkProgram(shaderProgram);

    // Verify shaders
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(shaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
        exit(1);
	}

    glValidateProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &Success);
    if (!Success) {
        glGetProgramInfoLog(shaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
        exit(1);
    }

    // Install the program object as part of current rendering state
    glUseProgram(shaderProgram);

    // Links the shader uniform variable with the defined one here which is
    // incremented on the render function
    gScaleLocation = glGetUniformLocation(shaderProgram, "glScale");
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

    // Registering callback for display
    glutDisplayFunc(RenderSceneCB); 
    glutIdleFunc(RenderSceneCB); 

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
    vertex[0] = Vertex3f(-1.0f, -1.0f, 0.0f);
    vertex[1] = Vertex3f(1.0f, -1.0f, 1.0f);
    vertex[2] = Vertex3f(0.0f, 1.0f, 0.0f);

    // This creates the vertex buffer
    // Generates the buffer
    glGenBuffers(1, &VBO);
    // Binds the type of the buffer with the buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Sets the buffer data 
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

    compileShaders();

    // Gives the control to the GLUT loop
    glutMainLoop();
    return 0;
}
