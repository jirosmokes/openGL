#include <GL/glew.h>
#include <GL/glut.h>
#include <cstdlib>
#include <ctime>
#include <cmath>  // for math functions like sin

float boatPositionX = 0.0f;
float waterOffset1 = 0.0f;
float waterOffset2 = 0.0f;
float waterOffset3 = 0.0f;
bool isAnimating = false;
GLubyte sailColor[3] = {242, 242, 197};  // initial sail color

float boatVertices[8]; // boat vertices
float flagVertices[6]; // sail vertices

float waterLines[3][10]; // three water lines

GLuint boatIndices[6] = {0, 1, 2, 2, 3, 0}; // boat indices (two triangles)
GLuint sailIndices[3] = {0, 1, 2}; // sail indices (one triangle)

GLuint boatVBO, flagVBO, waterVBO, boatVAO, sailVAO;

void initializeWaterLines() {
    // initialize water lines with x and y coordinates
    for (int j = 0; j < 3; ++j) {
        for (int i = 0; i < 5; ++i) {
            waterLines[j][i * 2] = -0.8f + i * 0.2f; // x-coordinates
            waterLines[j][i * 2 + 1] = -0.6f - j * 0.1f; // y-coordinates (below the boat)
        }
    }
}

void updateBoatVertices() {
    // update boat position based on boatPositionX
    boatVertices[0] = -0.7f + boatPositionX; 
    boatVertices[1] = 0.1f; 
    boatVertices[2] = -0.4f + boatPositionX; 
    boatVertices[3] = -0.2f; 
    boatVertices[4] = 0.4f + boatPositionX; 
    boatVertices[5] = -0.2f; 
    boatVertices[6] = 0.7f + boatPositionX; 
    boatVertices[7] = 0.1f;
}

void updateFlagVertices() {
    // set sail vertices
    float sailBaseWidth = 0.4f; 
    float sailHeight = 0.6f;

    flagVertices[0] = -0.2f + boatPositionX; 
    flagVertices[1] = 0.2f; 
    flagVertices[2] = -0.2f + boatPositionX; 
    flagVertices[3] = 0.2f + sailHeight; 
    flagVertices[4] = 0.2f + boatPositionX; 
    flagVertices[5] = 0.2f; 
}

void initVBOs() {
    // create and bind VBOs for boat, sail, and water lines
    glGenBuffers(1, &boatVBO);
    glBindBuffer(GL_ARRAY_BUFFER, boatVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(boatVertices), boatVertices, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &flagVBO);
    glBindBuffer(GL_ARRAY_BUFFER, flagVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(flagVertices), flagVertices, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &waterVBO);
    glBindBuffer(GL_ARRAY_BUFFER, waterVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(waterLines), waterLines, GL_DYNAMIC_DRAW);

    glGenVertexArrays(1, &boatVAO);
    glBindVertexArray(boatVAO);
    glBindBuffer(GL_ARRAY_BUFFER, boatVBO);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glGenVertexArrays(1, &sailVAO);
    glBindVertexArray(sailVAO);
    glBindBuffer(GL_ARRAY_BUFFER, flagVBO);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);
}

void displayWaterStipple() {
    glEnable(GL_LINE_STIPPLE);
    glLineWidth(1.0f);
    glLineStipple(7, 0x00ff); // water stipple pattern

    glColor3ub(73, 207, 255); // water color

    glBindBuffer(GL_ARRAY_BUFFER, waterVBO);

    // move water lines with sine wave effect
    for (int j = 0; j < 3; ++j) {
        for (int i = 0; i < 5; ++i) {
            waterLines[j][i * 2] += (j == 0) ? 0.01f : (j == 1) ? 0.008f : 0.012f;
            waterLines[j][i * 2 + 1] = -0.6f - j * 0.1f + 0.05f * sin(waterOffset1 + i);

            if (waterLines[j][i * 2] > 1.0f) {
                waterLines[j][i * 2] = -0.8f; 
                waterLines[j][i * 2 + 1] = -0.6f - j * 0.1f;
            }
        }
        glBufferData(GL_ARRAY_BUFFER, sizeof(waterLines), waterLines, GL_DYNAMIC_DRAW);
        glDrawArrays(GL_LINE_STRIP, 0, 5);
    }

    glDisable(GL_LINE_STIPPLE);
}

void displayBoatBody() {
    updateBoatVertices(); // update boat vertices
    glColor3ub(171, 126, 68); // brown color for boat
    glBindVertexArray(boatVAO); 
    glBindBuffer(GL_ARRAY_BUFFER, boatVBO); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(boatVertices), boatVertices, GL_DYNAMIC_DRAW);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, boatIndices); 
}

void displayBoatFlag() {
    updateFlagVertices(); // update flag vertices
    glColor3ub(sailColor[0], sailColor[1], sailColor[2]);
    glBindVertexArray(sailVAO);
    glBindBuffer(GL_ARRAY_BUFFER, flagVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(flagVertices), flagVertices, GL_DYNAMIC_DRAW);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, sailIndices); 
}

void displayBoat() {
    glClear(GL_COLOR_BUFFER_BIT);  // clear screen
    
    displayWaterStipple(); // render water first
    displayBoatBody();  // render boat body
    displayBoatFlag();  // render sail

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
        boatPositionX -= 0.05f; // move boat left
    } else if (key == GLUT_KEY_RIGHT) {
        boatPositionX += 0.05f; // move boat right
    }
    glutPostRedisplay(); // request redraw
}

void moveWater() {
    waterOffset1 += 0.01f;
    waterOffset2 += 0.008f;
    waterOffset3 += 0.012f;

    // reset water offsets if they exceed a certain range
    if (waterOffset1 > 1.0f) waterOffset1 = -0.8f;
    if (waterOffset2 > 1.0f) waterOffset2 = -0.8f;
    if (waterOffset3 > 1.0f) waterOffset3 = -0.8f;
}

void timer(int value) {
    if (isAnimating) {
        moveWater(); 
        glutPostRedisplay();
        glutTimerFunc(25, timer, 0); 
    }
}

int main(int argc, char** argv) {
    srand(static_cast<unsigned int>(time(0)));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Boat with Water Animation");

    glewInit();
    initializeWaterLines();
    initVBOs();

    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);  // dark blue background
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glutDisplayFunc(displayBoat); 
    glutKeyboardFunc(keyboard); 
    glutSpecialFunc(specialKeys); 
    glutTimerFunc(25, timer, 0); 
    glutMainLoop(); 

    return 0;
}
