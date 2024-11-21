#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cmath>
#include <iostream>
#include <math.h>

// Camera parameters
float cam_radius = 5.0f;
float cam_theta = 45.0f;
float cam_phi = 45.0f;

// VBO and VAO
GLuint VBO, VAO, EBO;

// Lighting positions and colors
struct Spotlight {
    GLfloat position[4];
    GLfloat color[4];
};

Spotlight spotlights[] = {
    {{2.0f, 2.0f, 2.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},  // Red spotlight
    {{-2.0f, 2.0f, -2.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}}, // Green spotlight
    {{0.0f, -2.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}}   // Blue spotlight
};

// Octahedron vertices and indices
GLfloat vertices[] = {
    0.0f,  1.0f,  0.0f, // Top
    0.0f, -1.0f,  0.0f, // Bottom
    1.0f,  0.0f,  0.0f, // Right
   -1.0f,  0.0f,  0.0f, // Left
    0.0f,  0.0f,  1.0f, // Front
    0.0f,  0.0f, -1.0f  // Back
};

GLuint indices[] = {
    0, 2, 4, // Top-front-right
    0, 4, 3, // Top-front-left
    0, 3, 5, // Top-back-left
    0, 5, 2, // Top-back-right
    1, 4, 2, // Bottom-front-right
    1, 3, 4, // Bottom-front-left
    1, 5, 3, // Bottom-back-left
    1, 2, 5  // Bottom-back-right
};

// Function to calculate the camera position
void updateCamera() {
    float x = cam_radius * sinf(cam_theta * M_PI / 180.0f) * cosf(cam_phi * M_PI / 180.0f);
    float y = cam_radius * cosf(cam_theta * M_PI / 180.0f);
    float z = cam_radius * sinf(cam_theta * M_PI / 180.0f) * sinf(cam_phi * M_PI / 180.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(x, y, z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
}

// Initialize VBO, VAO, and EBO
void initBuffers() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// Initialize lighting
void initLighting() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    // Ambient light
    GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

    // Configure spotlights
    for (int i = 0; i < 3; i++) {
        glEnable(GL_LIGHT0 + i);
        glLightfv(GL_LIGHT0 + i, GL_POSITION, spotlights[i].position);
        glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, spotlights[i].color);
        glLightf(GL_LIGHT0 + i, GL_SPOT_CUTOFF, 45.0f);
        glLightf(GL_LIGHT0 + i, GL_SPOT_EXPONENT, 2.0f);
    }
}

// Set up projection matrix
void initProjection() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

// Draw the octahedron
void drawOctahedron() {
    // Material properties
    GLfloat matAmbient[] = { 0.0f, 0.5f, 0.5f, 1.0f };
    GLfloat matDiffuse[] = { 0.0f, 0.7f, 0.7f, 1.0f };
    GLfloat matSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat matShininess[] = { 50.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

// Display function
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    updateCamera();
    drawOctahedron();

    glutSwapBuffers();
}

// Keyboard controls
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w': cam_theta -= 5.0f; break;
        case 's': cam_theta += 5.0f; break;
        case 'a': cam_phi -= 5.0f; break;
        case 'd': cam_phi += 5.0f; break;
        case '+': cam_radius -= 0.5f; break;
        case '-': cam_radius += 0.5f; break;
    }

    if (cam_theta < 0) cam_theta = 0;
    if (cam_theta > 180) cam_theta = 180;

    glutPostRedisplay();
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Octahedron with Lighting");

    if (glewInit() != GLEW_OK) {
        std::cerr << "Error initializing GLEW" << std::endl;
        return -1;
    }

    initBuffers();
    initLighting();
    initProjection();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
