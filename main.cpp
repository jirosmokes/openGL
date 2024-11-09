// main.cpp
#include <GL/glew.h>       // Include GLEW before other OpenGL headers
#include <GL/freeglut.h>
#include <iostream>

using namespace std;

void display() {
    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw a triangle
    glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f); // Red
        glVertex2f(-0.5f, -0.5f);

        glColor3f(0.0f, 1.0f, 0.0f); // Green
        glVertex2f(0.5f, -0.5f);

        glColor3f(0.0f, 0.0f, 1.0f); // Blue
        glVertex2f(0.0f,  0.5f);
    glEnd();

    // Swap the front and back buffers (double buffering)
    glutSwapBuffers();
}

void initialize() {
    // Initialize GLEW
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        cerr << "Error initializing GLEW: " << glewGetErrorString(err) << endl;
        exit(1);
    }

    // Check OpenGL and GLEW versions
    cout << "Using GLEW version: " << glewGetString(GLEW_VERSION) << std::endl;
    cout << "OpenGL version supported: " << glGetString(GL_VERSION) << endl;

    // Set background color to black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);

    // Set display mode with double buffering and RGB color
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    // Set the window size and position
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);

    // Create the window with a title
    glutCreateWindow("OpenGL Triangle with FreeGLUT and GLEW");

    // Initialize OpenGL and GLEW settings
    initialize();

    // Set the display callback function
    glutDisplayFunc(display);

    // Start the main loop
    glutMainLoop();

    return 0;
}
