#include <GL/glut.h>
#include <math.h>

// Camera parameters
float angle = 0.0;
float cameraDistance = 5.0;
float cameraAngleX = 0.0;
float cameraAngleY = 0.0;

// Octahedron Vertices
GLfloat vertices[][3] = {
    { 1.0f, 0.0f, 0.0f},  // v0
    {-1.0f, 0.0f, 0.0f},  // v1
    { 0.0f, 1.0f, 0.0f},  // v2
    { 0.0f,-1.0f, 0.0f},  // v3
    { 0.0f, 0.0f, 1.0f},  // v4
    { 0.0f, 0.0f,-1.0f}   // v5
};

// Octahedron Faces
GLubyte faces[][3] = {
    { 0, 2, 4 },
    { 0, 4, 3 },
    { 0, 3, 5 },
    { 0, 5, 2 },
    { 1, 2, 4 },
    { 1, 4, 3 },
    { 1, 3, 5 },
    { 1, 5, 2 }
};

// Material and lighting parameters
GLfloat materialColor[] = {0.8f, 0.1f, 0.1f, 1.0f}; // Red Material
GLfloat ambientLightColor[] = {0.2f, 0.2f, 0.2f, 1.0f}; // Soft ambient light
GLfloat spotlightColor[] = {1.0f, 1.0f, 0.0f, 1.0f}; // Yellow spotlight
GLfloat spotlightPosition1[] = {1.0f, 0.0f, 0.0f, 1.0f};
GLfloat spotlightPosition2[] = {-1.0f, 0.0f, 0.0f, 1.0f};
GLfloat spotlightPosition3[] = {0.0f, 1.0f, 0.0f, 1.0f};

GLfloat spotlightDirection[] = {0.0f, 0.0f, -1.0f}; // Pointing towards the center of the octahedron

void drawOctahedron() {
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < 8; i++) {
        glVertex3fv(vertices[faces[i][0]]);
        glVertex3fv(vertices[faces[i][1]]);
        glVertex3fv(vertices[faces[i][2]]);
    }
    glEnd();
}

void setupLighting() {
    // Ambient light
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLightColor);
    
    // Spotlight 1
    glLightfv(GL_LIGHT1, GL_POSITION, spotlightPosition1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, spotlightColor);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotlightDirection);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0f); // Narrow spotlight

    // Spotlight 2
    glLightfv(GL_LIGHT2, GL_POSITION, spotlightPosition2);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, spotlightColor);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spotlightDirection);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 30.0f);

    // Spotlight 3
    glLightfv(GL_LIGHT3, GL_POSITION, spotlightPosition3);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, spotlightColor);
    glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, spotlightDirection);
    glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 30.0f);
    
    // Enable lights
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0); // Ambient light
    glEnable(GL_LIGHT1); // Spotlight 1
    glEnable(GL_LIGHT2); // Spotlight 2
    glEnable(GL_LIGHT3); // Spotlight 3
}

void setupMaterial() {
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColor); // Apply material color
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialColor);
    glMaterialf(GL_FRONT, GL_SHININESS, 100.0f); // Set shininess
}

void setupCamera() {
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -cameraDistance);
    glRotatef(cameraAngleX, 1.0f, 0.0f, 0.0f);
    glRotatef(cameraAngleY, 0.0f, 1.0f, 0.0f);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    setupCamera();
    setupLighting();
    setupMaterial();
    drawOctahedron();
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'w') cameraAngleX += 5.0f;
    if (key == 's') cameraAngleX -= 5.0f;
    if (key == 'a') cameraAngleY -= 5.0f;
    if (key == 'd') cameraAngleY += 5.0f;
    if (key == 'q') cameraDistance -= 0.1f;
    if (key == 'e') cameraDistance += 0.1f;
    glutPostRedisplay();
}

void init() {
    glEnable(GL_DEPTH_TEST); // Enable depth testing for 3D
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Background color
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 1.0f, 1.0f, 100.0f);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Octahedron with Spotlights");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
