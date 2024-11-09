#include <GL/glut.h>
#include <GL/freeglut.h>
#include <cstdlib>
#include <ctime>

float boatPositionX = 0.0f;
float waterOffset1 = 0.0f;
float waterOffset2 = 0.0f;
float waterOffset3 = 0.0f;
bool isAnimating = false;
GLubyte sailColor[3] = {242, 242, 197};

float boatVertices[8]; // Boat vertices
float flagVertices[6]; // Sail vertices

float waterLines[3][8]; // Three water lines, each with 4 points (2 vertices)

// Index buffer for boat and sail
GLuint boatIndices[6] = {0, 1, 2, 2, 3, 0}; // Indices for the boat (two triangles)
GLuint sailIndices[3] = {0, 1, 2}; // Indices for the sail (one triangle)

void initializeWaterLines() {
    for (int j = 0; j < 3; ++j) {
        for (int i = 0; i < 5; ++i) {
            waterLines[j][i * 2] = -0.8f + i * 0.2f; // x-coordinates
            waterLines[j][i * 2 + 1] = -0.38f - j * 0.1f; // y-coordinates
        }
    }
}

void updateBoatVertices() {
    boatVertices[0] = -0.7f + boatPositionX; // Left vertex
    boatVertices[1] = 0.1f; // Top vertex
    boatVertices[2] = -0.4f + boatPositionX; // Bottom left vertex
    boatVertices[3] = -0.2f; // Bottom left vertex
    boatVertices[4] = 0.4f + boatPositionX; // Bottom right vertex
    boatVertices[5] = -0.2f; // Bottom right vertex
    boatVertices[6] = 0.7f + boatPositionX; // Right vertex
    boatVertices[7] = 0.1f; // Top vertex
}

void updateFlagVertices() {
    float sailBaseWidth = 0.4f; // Width of the sail at the bottom
    float sailHeight = 0.6f; // Height of the sail

    // Set sail vertices
    flagVertices[0] = -0.2f + boatPositionX; // Bottom left vertex
    flagVertices[1] = 0.2f; // Bottom left vertex (y-coordinate remains fixed)
    flagVertices[2] = -0.2f + boatPositionX; // Left peak of the sail
    flagVertices[3] = 0.2f + sailHeight; // Top peak of the sail
    flagVertices[4] = 0.2f + boatPositionX; // Bottom right vertex
    flagVertices[5] = 0.2f; // Bottom right vertex (y-coordinate remains fixed)
}

void displayWaterStipple() {
    glEnable(GL_LINE_STIPPLE);
    glLineWidth(1.0f);
    glLineStipple(7, 0x00ff);
    glColor3ub(73, 207, 255);

    for (int j = 0; j < 3; ++j) {
        for (int i = 0; i < 5; ++i) {
            waterLines[j][i * 2] += (j == 0) ? 0.01f : (j == 1) ? 0.008f : 0.012f; // Apply offsets
            if (waterLines[j][i * 2] > 1.0f) {
                waterLines[j][i * 2] = -0.8f; // Reset position
            }
        }
        glVertexPointer(2, GL_FLOAT, 0, waterLines[j]);
        glDrawArrays(GL_LINE_STRIP, 0, 5); // Draw each water line
    }

    glDisable(GL_LINE_STIPPLE);
}

void displayBoatBody() {
    updateBoatVertices(); // Update boat vertices based on current position
    glColor3ub(171, 126, 68);
    glVertexPointer(2, GL_FLOAT, 0, boatVertices);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, boatIndices); // Draw the boat using glDrawElements
}

void displayBoatFlag() {
    updateFlagVertices(); // Update flag vertices based on boat's position
    glColor3ub(sailColor[0], sailColor[1], sailColor[2]);
    glVertexPointer(2, GL_FLOAT, 0, flagVertices);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, sailIndices); // Draw the sail using glDrawElements
}

void displayBoat() {
    glClear(GL_COLOR_BUFFER_BIT);
    displayWaterStipple();
    displayBoatBody();
    displayBoatFlag();
    glFlush();
}

void changeSailColor() {
    sailColor[0] = rand() % 256;
    sailColor[1] = rand() % 256;
    sailColor[2] = rand() % 256;
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'c') {
        changeSailColor();
        glutPostRedisplay();
    }
}

void specialKeys(int key, int x, int y) {
    if (key == GLUT_KEY_LEFT) {
        boatPositionX -= 0.05f; // Move boat left
    } else if (key == GLUT_KEY_RIGHT) {
        boatPositionX += 0.05f; // Move boat right
    }
    glutPostRedisplay(); // Request redraw
}

void moveWater() {
    waterOffset1 += 0.01f;
    waterOffset2 += 0.008f;
    waterOffset3 += 0.012f;

    if (waterOffset1 > 1.0f) waterOffset1 = 0.0f;
    if (waterOffset2 > 1.0f) waterOffset2 = 0.0f;
    if (waterOffset3 > 1.0f) waterOffset3 = 0.0f;
}

void timer(int) {
    if (isAnimating) {
        moveWater();
        glutPostRedisplay();
    }
    glutTimerFunc(16, timer, 0);
}

int main(int argc, char** argv) {
    srand(static_cast<unsigned>(time(0)));
    glutInit(&argc, argv);
    glutInitWindowSize(300, 300);
    glutInitWindowPosition(550, 250);
    glutCreateWindow("FourthSim - Boat with Flag");
    glEnableClientState(GL_VERTEX_ARRAY);

    initializeWaterLines();

    glutDisplayFunc(displayBoat);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutTimerFunc(0, timer, 0);
    isAnimating = true;
    glutMainLoop();
    return 0;
}
