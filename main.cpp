// ============================================
// main.cpp - "The Model Worker's Study, 1965"
// Socialist Realist 3D Scene using OpenGL/GLUT
// ============================================

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>
#include <cstdlib>
#include <cstdio>

// stb_image for loading image files
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// ============================================
// Global variables
// ============================================
const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 800;

// Camera variables
float cameraAngleX = 0.0f;
float cameraAngleY = 0.0f;
float cameraDistance = 8.0f;
float cameraPosX = 0.0f;
float cameraPosY = 2.0f;
float cameraPosZ = 8.0f;
float cameraLookX = 0.0f;
float cameraLookY = 1.5f;
float cameraLookZ = 0.0f;
bool cameraMode = false; // false = orbital, true = FPS
float orbitalAngle = 0.0f;

// Lighting states
bool redStarLight = true;
bool deskLampLight = true;
bool deskLampLightOn = true;
float redStarIntensity = 1.0f;
float starPulseFactor = 0.0f;
bool starPulseIncreasing = true;

// Mouse control for FPS camera
int lastMouseX = 0;
int lastMouseY = 0;
bool mousePressed = false;

// Animation state
bool animationPaused = false;

// Texture IDs
// GLuint textureWood, texturePlaster, texturePaper, texturePortrait;
// Texture IDs
GLuint textureWood = 0;
GLuint texturePlaster = 0;
GLuint texturePaper = 0;
GLuint texturePortrait = 0;
GLuint textureGlass = 0;
GLuint textureGround = 0;
// Colors for Socialist Realism aesthetic
GLfloat colorRed[4] = {0.7f, 0.1f, 0.1f, 1.0f}; // Soviet Red
GLfloat colorGold[4] = {0.8f, 0.7f, 0.1f, 1.0f}; // Gold accents
GLfloat colorDarkWood[4] = {0.3f, 0.2f, 0.15f, 1.0f}; // Dark wood tones
GLfloat colorLightWood[4] = {0.5f, 0.4f, 0.3f, 1.0f}; // Light wood tones
GLfloat colorPaper[4] = {0.95f, 0.9f, 0.8f, 1.0f}; // aged paper 
GLfloat colorGray[4] = {0.4f, 0.4f, 0.4f, 1.0f}; // concrete gray

// == Function prototypes ==
void init();
void display();
void reshape(int w, int h);
void timer(int value);
void keyboard(unsigned char key, int x, int y);
void specialKeys(int key, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void drawRoom();
void drawDesk();
void drawChair();
void drawRadio();
void drawBooks();
void drawPortrait();
void drawWindowAndStar();
void drawCapAndPapers();
void loadTextures();
void createWoodTexture();
void createPlasterTexture();
void createPaperTexture();
void createStarTexture();
void createGlassTexture();
void createGroundTexture();
void setupLighting();
void updateCamera();
void drawAxes(); // for debugging

// ============================================
// Main function
// ============================================
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("The Model Worker's Study, 1965");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutTimerFunc(16, timer, 0); // 60 fps

    glutMainLoop();
    return 0;
}

// ============================================
// initialization functions
// ============================================
void init() {
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f); // dark gray background
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    // Enable smooth shading
    glShadeModel(GL_SMOOTH);

    // Setup Lighting
    setupLighting();

    // Load Textures
    loadTextures();

    // Print controls
    printf("\n=========== CONTROLS ===========\n");
    printf("C - Switch camera mode (Orbital/FPS)\n");
    printf("L - Toggle desk lamp light\n");
    printf("R - Toggle red star light\n");
    printf("P - Pause/resume star animation\n");
    printf("+/- - Adjust red star intensity\n");
    printf("Arrow Keys - Move camera (FPS mode)\n");
    printf("Mouse Drag - Look around (FPS mode)\n");
    printf("ESC - Exit\n");
    printf("===============================\n\n");
}
// === Display Function ===
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Update camera postion based on mode
    updateCamera();

    // Draw the scene
    drawRoom();
    drawDesk();
    drawChair();
    drawRadio();
    drawBooks();
    drawPortrait();
    drawWindowAndStar();
    drawCapAndPapers();

    // draw axes for debugging
    // drawAxes();

    glutSwapBuffers();
}

// == Reshape Functon ====

void reshape(int w, int h){
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)w / (float)h, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

// ==== Timer function ====
void timer(int value){
    if (!animationPaused){
        // update orbital camera angle
        orbitalAngle += 0.5f;
        if (orbitalAngle > 360.0f) orbitalAngle -= 360.0f;

        // update red star pulse
        if (starPulseIncreasing) {
            starPulseFactor += 0.02f;
            if (starPulseFactor >= 1.0f) starPulseIncreasing = false;
        } else {
            starPulseFactor -= 0.02f;
            if (starPulseFactor <= 0.5f) starPulseIncreasing = true;
        }
        // Update red star intensity with pulse
        redStarIntensity = 0.7f + starPulseFactor * 0.3f;

        // Update red star Light if it's on
        if (redStarLight) {
            GLfloat redStarDiffuse[] = {redStarIntensity * 0.7f, 0.0f, 0.0f, 1.0f};
            GLfloat redStarAmbient[] = {redStarIntensity * 0.3f, 0.0f, 0.0f, 1.0f};
            glLightfv(GL_LIGHT0, GL_DIFFUSE, redStarDiffuse);
            glLightfv(GL_LIGHT0, GL_AMBIENT, redStarAmbient);
        }
    }
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

// ============= Keyboard Controls =============
void keyboard(unsigned char key, int x, int y) {
    switch(key) {
        case 27: // ESC key
            exit(0);
            break;
            
        case 'c':
        case 'C':
            // Switch camera mode
            cameraMode = !cameraMode;
            if (cameraMode) {
                printf("Camera mode: FPS (Use arrows to move, mouse to look)\n");
            } else {
                printf("Camera mode: Orbital\n");
            }
            break;
            
        case 'l':
        case 'L':
            // Toggle desk lamp light
            deskLampLightOn = !deskLampLightOn;
            if (deskLampLightOn) {
                glEnable(GL_LIGHT1);
                printf("Desk lamp: ON\n");
            } else {
                glDisable(GL_LIGHT1);
                printf("Desk lamp: OFF\n");
            }
            break;
            
        case 'r':
        case 'R':
            // Toggle red star light
            redStarLight = !redStarLight;
            if (redStarLight) {
                glEnable(GL_LIGHT0);
                printf("Red Star: ON\n");
            } else {
                glDisable(GL_LIGHT0);
                printf("Red Star: OFF\n");
            }
            break;
            
        case 'p':
        case 'P':
            // Pause/resume animation
            animationPaused = !animationPaused;
            if (animationPaused) {
                printf("Animation: PAUSED\n");
            } else {
                printf("Animation: RESUMED\n");
            }
            break;
            
        case '+':
        case '=':
            // Increase red star intensity
            redStarIntensity += 0.1f;
            if (redStarIntensity > 2.0f) redStarIntensity = 2.0f;
            printf("Red Star intensity: %.1f\n", redStarIntensity);
            break;
            
        case '-':
        case '_':
            // Decrease red star intensity
            redStarIntensity -= 0.1f;
            if (redStarIntensity < 0.1f) redStarIntensity = 0.1f;
            printf("Red Star intensity: %.1f\n", redStarIntensity);
            break;
            
        // WASD movement for FPS camera
        case 'w':
        case 'W':
            if (cameraMode) {
                cameraPosZ -= 0.5f;
            }
            break;
        case 's':
        case 'S':
            if (cameraMode) {
                cameraPosZ += 0.5f;
            }
            break;
        case 'a':
        case 'A':
            if (cameraMode) {
                cameraPosX -= 0.5f;
            }
            break;
        case 'd':
        case 'D':
            if (cameraMode) {
                cameraPosX += 0.5f;
            }
            break;
    }
    
    glutPostRedisplay();
}
// ============= Special Keys (Arrow Keys) =============
void specialKeys(int key, int x, int y) {
    float moveSpeed = 0.5f;
    
    if (cameraMode) { // Only in FPS mode
        switch(key) {
            case GLUT_KEY_UP:
                cameraPosZ -= moveSpeed;
                break;
            case GLUT_KEY_DOWN:
                cameraPosZ += moveSpeed;
                break;
            case GLUT_KEY_LEFT:
                cameraPosX -= moveSpeed;
                break;
            case GLUT_KEY_RIGHT:
                cameraPosX += moveSpeed;
                break;
        }
    }
    
    glutPostRedisplay();
}

// ============= Mouse Controls =============
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            mousePressed = true;
            lastMouseX = x;
            lastMouseY = y;
        } else {
            mousePressed = false;
        }
    }
}
void motion(int x, int y){
    if(mousePressed && cameraMode){
        int deltaX = x - lastMouseX;
        int deltaY = y - lastMouseY;

        cameraAngleY += deltaX * 0.5f;
        cameraAngleX += deltaY * 0.5f;

        // clamp vertical rotation
        if (cameraAngleX > 89.0f) cameraAngleX = 89.0f;
        if (cameraAngleX < -89.0f) cameraAngleX = -89.0f;
        
        lastMouseX = x;
        lastMouseY = y;
        
        glutPostRedisplay();
    }
}

// ===== Lighting Setup ==================
void setupLighting(){
    // Enable Lighting
    glEnable(GL_LIGHTING);

    // light 0: red star
    GLfloat redStarPosition[] = {-1.0f, 0.5f, -1.0f, 0.0f};
    GLfloat redStarDiffuse[] = {0.7f, 0.0f, 0.0f, 1.0f};
    GLfloat redStarAmbient[] = {0.3f, 0.0f, 0.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_POSITION, redStarPosition);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, redStarDiffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT, redStarAmbient);
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
    glEnable(GL_LIGHT0);

    // Light 1: Desk Lamp (Point Light)
    GLfloat deskLampPosition[] = {0.0f, 2.5f, 0.0f, 1.0f};
    GLfloat deskLampDiffuse[] = {0.9f, 0.8f, 0.6f, 1.0f};
    GLfloat deskLampAmbient[] = {0.3f, 0.3f, 0.2f, 1.0f};
    GLfloat deskLampSpecular[] = {1.0f, 1.0f, 0.8f, 1.0f};

    glLightfv(GL_LIGHT1, GL_POSITION, deskLampPosition);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, deskLampDiffuse);
    glLightfv(GL_LIGHT1, GL_AMBIENT, deskLampAmbient);
    glLightfv(GL_LIGHT1, GL_SPECULAR, deskLampSpecular);
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.05f);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.01f);
    glEnable(GL_LIGHT1);

    // Light 2: General ambient light
    GLfloat globalAmbient[] = {0.1f, 0.1f, 0.1f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

}

// ====== Camera Update =======
void updateCamera() {
    if (cameraMode) {
        // FPS Camera Mode
        glRotatef(cameraAngleX, 1.0f, 0.0f, 0.0f);
        glRotatef(cameraAngleY, 0.0f, 1.0f, 0.0f);
        glTranslatef(-cameraPosX, -cameraPosY, -cameraPosZ);
    } else {
        // Orbital Camera Mode
        float angleRad = orbitalAngle * M_PI / 180.0f;
        float camX = cameraDistance * sin(angleRad);
        float camZ = cameraDistance * cos(angleRad);
        gluLookAt(camX, 3.0f, camZ,  // Eye position
                  0.0f, 1.5f, 0.0f,  // Look at point (center of room)
                  0.0f, 1.0f, 0.0f); // Up vector

    }
}

// =========== Texture Loading =======
// ============= Texture Loading Function =============
void loadTextures() {
    // Enable texture mapping
    glEnable(GL_TEXTURE_2D);
    
    // Generate texture IDs
    glGenTextures(6, &textureWood); // textureWood gets first ID, others follow
    
    // Load textures from files
    createWoodTexture();
    createPlasterTexture();
    createPaperTexture();
    createStarTexture();
    createGlassTexture();
    createGroundTexture();
    
    printf("Textures loaded successfully\n");
}

// ============= Texture Loading Functions =============

void createWoodTexture() {
    int width, height, channels;
    unsigned char *image = stbi_load("textures/wood.jpg", &width, &height, &channels, 0);
    
    if (!image) {
        printf("Failed to load wood.jpg texture\n");
        return;
    }
    
    glBindTexture(GL_TEXTURE_2D, textureWood);
    
    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    stbi_image_free(image);
    printf("Wood texture loaded (ID: %d)\n", textureWood);
}

void createPlasterTexture() {
    int width, height, channels;
    unsigned char *image = stbi_load("textures/plaster.jpg", &width, &height, &channels, 0);
    
    if (!image) {
        printf("Failed to load plaster.jpg texture\n");
        return;
    }
    
    texturePlaster = textureWood + 1;
    glBindTexture(GL_TEXTURE_2D, texturePlaster);
    
    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    stbi_image_free(image);
    printf("Plaster texture loaded (ID: %d)\n", texturePlaster);
}

void createPaperTexture() {
    int width, height, channels;
    unsigned char *image = stbi_load("textures/paper.jpg", &width, &height, &channels, 0);
    
    if (!image) {
        printf("Failed to load paper.jpg texture\n");
        return;
    }
    
    texturePaper = textureWood + 2;
    glBindTexture(GL_TEXTURE_2D, texturePaper);
    
    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    
    stbi_image_free(image);
    printf("Paper texture loaded (ID: %d)\n", texturePaper);
}

void createStarTexture() {
    int width, height, channels;
    unsigned char *image = stbi_load("textures/red_star.png", &width, &height, &channels, 4); // Force RGBA
    
    if (!image) {
        printf("Failed to load red_star.png texture\n");
        return;
    }
    
    texturePortrait = textureWood + 3;
    glBindTexture(GL_TEXTURE_2D, texturePortrait);
    
    // PNG with alpha channel
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    // Enable alpha blending for this texture
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    stbi_image_free(image);
    printf("Star texture loaded (ID: %d)\n", texturePortrait);
}

void createGlassTexture() {
    int width, height, channels;
    unsigned char *image = stbi_load("textures/glass.jpg", &width, &height, &channels, 0);
    
    if (!image) {
        printf("Failed to load glass.jpg texture\n");
        return;
    }
    
    textureGlass = textureWood + 4;
    glBindTexture(GL_TEXTURE_2D, textureGlass);
    
    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    stbi_image_free(image);
    printf("Glass texture loaded (ID: %d)\n", textureGlass);
}

void createGroundTexture() {
    int width, height, channels;
    unsigned char *image = stbi_load("textures/ground.jpg", &width, &height, &channels, 0);
    
    if (!image) {
        printf("Failed to load ground.jpg texture\n");
        return;
    }
    
    textureGround = textureWood + 5;
    glBindTexture(GL_TEXTURE_2D, textureGround);
    
    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    stbi_image_free(image);
    printf("Ground texture loaded (ID: %d)\n", textureGround);
}

//================= Room Drawing Functions ===========================
void drawRoom(){
    // Floor - use ground texture
    glBindTexture(GL_TEXTURE_2D, textureGround);
    glColor3f(1.0f, 1.0f, 1.0f); // White to show texture colors
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-5.0f, 0.0f, -5.0f);
    glTexCoord2f(4.0f, 0.0f); glVertex3f(5.0f, 0.0f, -5.0f);
    glTexCoord2f(4.0f, 4.0f); glVertex3f(5.0f, 0.0f, 5.0f);
    glTexCoord2f(0.0f, 4.0f); glVertex3f(-5.0f, 0.0f, 5.0f);
    glEnd();

    // walls - use plaster texture
    glBindTexture(GL_TEXTURE_2D, texturePlaster);
    glColor3f(1.0f, 1.0f, 1.0f); // White to show texture colors
    
    // Back wall
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-5.0f, 0.0f, -5.0f);
    glTexCoord2f(2.0f, 0.0f); glVertex3f(5.0f, 0.0f, -5.0f);
    glTexCoord2f(2.0f, 2.0f); glVertex3f(5.0f, 5.0f, -5.0f);
    glTexCoord2f(0.0f, 2.0f); glVertex3f(-5.0f, 5.0f, -5.0f);
    glEnd();

    // left wall with window
    glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-5.0f, 0.0f, -5.0f);
    glTexCoord2f(2.0f, 0.0f); glVertex3f(-5.0f, 0.0f, 5.0f);
    glTexCoord2f(2.0f, 2.0f); glVertex3f(-5.0f, 5.0f, 5.0f);
    glTexCoord2f(0.0f, 2.0f); glVertex3f(-5.0f, 5.0f, -5.0f);
    glEnd();

    // right wall
    glBegin(GL_QUADS);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(5.0f, 0.0f, -5.0f);
    glTexCoord2f(2.0f, 0.0f); glVertex3f(5.0f, 0.0f, 5.0f);
    glTexCoord2f(2.0f, 2.0f); glVertex3f(5.0f, 5.0f, 5.0f);
    glTexCoord2f(0.0f, 2.0f); glVertex3f(5.0f, 5.0f, -5.0f);
    glEnd();

    // ceiling
    glColor3fv(colorGray);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-5.0f, 5.0f, -5.0f);
    glVertex3f(5.0f, 5.0f, -5.0f);
    glVertex3f(5.0f, 5.0f, 5.0f);
    glVertex3f(-5.0f, 5.0f, 5.0f);
    glEnd();
}

void drawDesk() {
    // Use wood texture
    glBindTexture(GL_TEXTURE_2D, textureWood);
    glColor3f(1.0f, 1.0f, 1.0f); // White to show texture colors

    // desk top
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.0f, 1.0f, -1.0f);
    glTexCoord2f(2.0f, 0.0f); glVertex3f(2.0f, 1.0f, -1.0f);
    glTexCoord2f(2.0f, 1.0f); glVertex3f(2.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.0f, 1.0f, 1.0f);
    glEnd();
    
    glColor3fv(colorDarkWood); // Back to color for legs
    // desk legs
    float legWidth = 0.2f;
    float legHeight = 1.0f;

    // front left leg
    glBegin(GL_QUADS);
    // Front face
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.8f, 0.0f, -0.8f);
    glVertex3f(-1.8f + legWidth, 0.0f, -0.8f);
    glVertex3f(-1.8f + legWidth, legHeight, -0.8f);
    glVertex3f(-1.8f, legHeight, -0.8f);
    glEnd();

    // front right leg
    glBegin(GL_QUADS);
    glVertex3f(1.8f - legWidth, 0.0f, -0.8f);
    glVertex3f(1.8f, 0.0f, -0.8f);
    glVertex3f(1.8f, legHeight, -0.8f);
    glVertex3f(1.8f - legWidth, legHeight, -0.8f);
    glEnd();

    // back legs - simplified - just lines
    glBegin(GL_LINES);
    glVertex3f(-1.8f, 0.0f, 0.8f);
    glVertex3f(-1.8f, legHeight, 0.8f);
    glVertex3f(1.8f, 0.0f, 0.8f);
    glVertex3f(1.8f, legHeight, 0.8f);
    glEnd();
}

void drawChair() {
    glColor3fv(colorLightWood);

    // chair seat
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-0.5f, 0.6f, 0.5f);
    glVertex3f(0.5f, 0.6f, 0.5f);
    glVertex3f(0.5f, 0.6f, 1.5f);
    glVertex3f(-0.5f, 0.6f, 1.5f);
    glEnd();
    // chair back
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(-0.5f, 0.6f, 1.5f);
    glVertex3f(0.5f, 0.6f, 1.5f);
    glVertex3f(0.5f, 1.5f, 1.5f);
    glVertex3f(-0.5f, 1.5f, 1.5f);
    glEnd();
    
    // Chair legs (simplified)
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    // Front legs
    glVertex3f(-0.4f, 0.0f, 0.6f);
    glVertex3f(-0.4f, 0.6f, 0.6f);
    glVertex3f(0.4f, 0.0f, 0.6f);
    glVertex3f(0.4f, 0.6f, 0.6f);
    // Back legs
    glVertex3f(-0.4f, 0.0f, 1.4f);
    glVertex3f(-0.4f, 0.6f, 1.4f);
    glVertex3f(0.4f, 0.0f, 1.4f);
    glVertex3f(0.4f, 0.6f, 1.4f);
    glEnd();
    glLineWidth(1.0f);
}

void drawRadio() {
    glColor3fv(colorGray);

    // radio body
    glBegin(GL_QUADS);
    // Front face
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-0.6f, 1.1f, -0.8f);
    glVertex3f(0.6f, 1.1f, -0.8f);
    glVertex3f(0.6f, 1.8f, -0.8f);
    glVertex3f(-0.6f, 1.8f, -0.8f);
    glEnd();

    // speaker grills
    glColor3fv(colorGold);
    for(int i = 0; i < 4; i++){
        glBegin(GL_LINE_LOOP);
        float x = -0.4f + i * 0.2f;
        glVertex3f(x, 1.4f, -0.79f);
        glVertex3f(x + 0.1f, 1.4f, -0.79f);
        glVertex3f(x + 0.1f, 1.6f, -0.79f);
        glVertex3f(x, 1.6f, -0.79f);
        glEnd();
    }
}

void drawBooks() {
    // Stack of books
    glColor4fv(colorRed); // 1st book, red cover
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.5f, 1.01f, -0.3f);
    glVertex3f(1.0f, 1.01f, -0.3f);
    glVertex3f(1.0f, 1.2f, -0.3f);
    glVertex3f(0.5f, 1.2f, -0.3f);
    glEnd();

    glColor3fv(colorGold); // 2nd Book - gold cover
    glBegin(GL_QUADS);
    glVertex3f(0.5f, 1.21f, -0.4f);
    glVertex3f(1.0f, 1.21f, -0.4f);
    glVertex3f(1.0f, 1.35f, -0.4f);
    glVertex3f(0.5f, 1.35f, -0.4f);
    glEnd();

    // papers - use paper texture
    glBindTexture(GL_TEXTURE_2D, texturePaper);
    glColor3f(1.0f, 1.0f, 1.0f); // White to show texture colors
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.01f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, 1.01f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, 1.05f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.05f, 0.0f);
    glEnd();
}

void drawPortrait(){
    // portrait frame on the wall
    glColor3fv(colorGold);

    // Frame
    float frameSize = 1.5f;
    float frameThickness = 0.1f;


    // Outer frame
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    // Top piece
    glVertex3f(-frameSize/2, 3.0f, -4.99f);
    glVertex3f(frameSize/2, 3.0f, -4.99f);
    glVertex3f(frameSize/2, 3.0f + frameThickness, -4.99f);
    glVertex3f(-frameSize/2, 3.0f + frameThickness, -4.99f);
    // Bottom piece
    glVertex3f(-frameSize/2, 2.0f - frameThickness, -4.99f);
    glVertex3f(frameSize/2, 2.0f - frameThickness, -4.99f);
    glVertex3f(frameSize/2, 2.0f, -4.99f);
    glVertex3f(-frameSize/2, 2.0f, -4.99f);
    // Left piece
    glVertex3f(-frameSize/2, 2.0f, -4.99f);
    glVertex3f(-frameSize/2 + frameThickness, 2.0f, -4.99f);
    glVertex3f(-frameSize/2 + frameThickness, 3.0f, -4.99f);
    glVertex3f(-frameSize/2, 3.0f, -4.99f);
    // Right piece
    glVertex3f(frameSize/2 - frameThickness, 2.0f, -4.99f);
    glVertex3f(frameSize/2, 2.0f, -4.99f);
    glVertex3f(frameSize/2, 3.0f, -4.99f);
    glVertex3f(frameSize/2 - frameThickness, 3.0f, -4.99f);
    glEnd();

    // Portrait content (simplified hammer and sickle)
    glColor3fv(colorRed);
    glLineWidth(3.0f);
    glBegin(GL_LINES);
    // Hammer handle
    glVertex3f(0.0f, 2.3f, -4.98f);
    glVertex3f(0.3f, 2.6f, -4.98f);
    // Hammer head
    glVertex3f(0.4f, 2.5f, -4.98f);
    glVertex3f(0.2f, 2.7f, -4.98f);
    // Sickle arc (simplified)
    for(int i = 0; i < 5; i++) {
        float angle = M_PI * 0.8f + i * 0.1f;
        float x1 = -0.2f + 0.2f * cos(angle);
        float y1 = 2.5f + 0.2f * sin(angle);
        float x2 = -0.2f + 0.2f * cos(angle + 0.1f);
        float y2 = 2.5f + 0.2f * sin(angle + 0.1f);
        glVertex3f(x1, y1, -4.98f);
        glVertex3f(x2, y2, -4.98f);
    }
    glEnd();
    glLineWidth(1.0f);
}

void drawWindowAndStar() {
    // window on left wall
    // First draw the glass panes with texture
    glBindTexture(GL_TEXTURE_2D, textureGlass);
    glColor4f(0.9f, 0.9f, 1.0f, 0.7f); // Slightly blue tinted glass with transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Draw 4 glass panes (divided by cross)
    // Top left pane
    glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-4.985f, 2.5f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-4.985f, 2.5f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-4.985f, 3.5f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-4.985f, 3.5f, -1.0f);
    glEnd();
    
    // Top right pane
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-4.985f, 2.5f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-4.985f, 2.5f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-4.985f, 3.5f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-4.985f, 3.5f, 0.0f);
    glEnd();
    
    // Bottom left pane
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-4.985f, 1.5f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-4.985f, 1.5f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-4.985f, 2.5f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-4.985f, 2.5f, -1.0f);
    glEnd();
    
    // Bottom right pane
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-4.985f, 1.5f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-4.985f, 1.5f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-4.985f, 2.5f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-4.985f, 2.5f, 0.0f);
    glEnd();
    
    // Window frame (drawn on top of glass)
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    glColor3fv(colorDarkWood);
    float windoWidth = 2.0f;
    float windowHeight = 2.0f;

    glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f);
    // Window outline frame
    glVertex3f(-4.99f, 1.5f, -1.0f);
    glVertex3f(-4.99f, 1.5f, 1.0f);
    glVertex3f(-4.99f, 3.5f, 1.0f);
    glVertex3f(-4.99f, 3.5f, -1.0f);
    glEnd();
    
    // Window panes dividers (cross)
    glColor3fv(colorGold);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    // Vertical divider
    glVertex3f(-4.98f, 1.5f, 0.0f);
    glVertex3f(-4.98f, 3.5f, 0.0f);
    // Horizontal divider
    glVertex3f(-4.98f, 2.5f, -1.0f);
    glVertex3f(-4.98f, 2.5f, 1.0f);
    glEnd();
    glLineWidth(1.0f);
    glEnable(GL_TEXTURE_2D); // Re-enable textures for other objects

    // red star outside window - use star texture
    if (redStarLight){
        glBindTexture(GL_TEXTURE_2D, texturePortrait);
        glColor3f(redStarIntensity, redStarIntensity, redStarIntensity); // Brightness based on intensity
        glPushMatrix();
        glTranslatef(-6.0f, 2.5f, 0.0f);
        
        // Draw textured quad for star
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.0f);
        glEnd();
        glPopMatrix();
    }

}

void drawCapAndPapers() {
    // Worker's cap on the desk
    glColor3fv(colorRed);
    
    // Cap base
    glPushMatrix();
    glTranslatef(-1.0f, 1.05f, 0.5f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glutSolidCone(0.3f, 0.2f, 20, 5);
    glPopMatrix();
    
    // Cap star (simplified)
    glColor3fv(colorGold);
    glPointSize(5.0f);
    glBegin(GL_POINTS);
    glVertex3f(-1.0f, 1.15f, 0.5f);
    glEnd();

    // Scattered papers - use paper texture
    glBindTexture(GL_TEXTURE_2D, texturePaper);
    glColor3f(1.0f, 1.0f, 1.0f); // White to show texture colors
    for(int i = 0; i < 3; i++){
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f);
        float offset = i * 0.05f;
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.5f + offset, 1.01f, -0.5f + offset);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f + offset, 1.01f, -0.5f + offset);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f + offset, 1.02f, -0.3f + offset);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.5f + offset, 1.02f, -0.3f + offset);
        glEnd();
    }
}

void drawAxes() {
    glDisable(GL_LIGHTING);

    glLineWidth(2.0f);
    glBegin(GL_LINES);

    // X axis (Red)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(2.0f, 0.0f, 0.0f);
    
    // Y axis (Green)
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 2.0f, 0.0f);
    
    // Z axis (Blue)
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 2.0f);
    
    glEnd();
    glLineWidth(1.0f);
    
    glEnable(GL_LIGHTING);
}