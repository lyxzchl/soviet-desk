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
bool deskLampLight = true;
bool deskLampLightOn = true;
// Day / Night state
bool isDaytime = true; // true = sunlight on, lamp off; false = sunlight off, lamp on

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
GLuint textureWallpaper = 0;
GLuint textureCarpet = 0;
GLuint textureCurtains = 0;
GLuint textureCouch = 0;
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
void drawWindow();
void drawSunlight();
void drawCapAndPapers();
void drawShelves();
void drawCurtains();
void drawDeskLamp();
void drawDocuments();
void drawCarpet();
void drawCouch();
void loadTextures();
void createWoodTexture();
void createPlasterTexture();
void createPaperTexture();
void createWallpaperTexture();
void createCarpetTexture();
void createCurtainsTexture();
void createCouchTexture();
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
    glClearColor(0.4f, 0.35f, 0.3f, 1.0f); // warm late afternoon tone
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
    printf("L - Toggle Day/Night (Sun <-> Lamp)\n");
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
    drawCarpet();
    drawDesk();
    drawChair();
    drawRadio();
    drawBooks();
    //drawPortrait();
    drawWindow();
    drawSunlight();
    drawCapAndPapers();
    drawShelves();
    drawCurtains();
    drawDeskLamp();
    drawDocuments();
    drawCouch();

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
            // Toggle Day/Night: daytime -> sun on, lamp off; nighttime -> sun off, lamp on
            isDaytime = !isDaytime;
            if (isDaytime) {
                // Daytime: enable sun, disable desk lamp
                glEnable(GL_LIGHT0);
                glDisable(GL_LIGHT1);
                deskLampLightOn = false;
                printf("Daytime: SUN ON, Lamp OFF\n");
            } else {
                // Nighttime: disable sun, enable desk lamp
                glDisable(GL_LIGHT0);
                glEnable(GL_LIGHT1);
                deskLampLightOn = true;
                printf("Nighttime: SUN OFF, Lamp ON\n");
            }
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

    // light 0: warm window light (late afternoon sun)
    GLfloat sunPosition[] = {-1.0f, 2.0f, -1.0f, 0.0f};
    GLfloat sunDiffuse[] = {0.9f, 0.75f, 0.5f, 1.0f}; // warm golden light
    GLfloat sunAmbient[] = {0.4f, 0.3f, 0.2f, 1.0f};

    glLightfv(GL_LIGHT0, GL_POSITION, sunPosition);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, sunDiffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT, sunAmbient);
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
    if (isDaytime) glEnable(GL_LIGHT0); else glDisable(GL_LIGHT0);

    // Light 1: Desk Lamp (Point Light)
    GLfloat deskLampPosition[] = {0.0f, 2.5f, 0.0f, 1.0f};
    GLfloat deskLampDiffuse[] = {1.0f, 0.95f, 0.8f, 1.0f}; // warmer white
    GLfloat deskLampAmbient[] = {0.4f, 0.4f, 0.3f, 1.0f}; // brighter ambient from lamp
    GLfloat deskLampSpecular[] = {1.0f, 1.0f, 0.9f, 1.0f};

    glLightfv(GL_LIGHT1, GL_POSITION, deskLampPosition);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, deskLampDiffuse);
    glLightfv(GL_LIGHT1, GL_AMBIENT, deskLampAmbient);
    glLightfv(GL_LIGHT1, GL_SPECULAR, deskLampSpecular);
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.05f);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.01f);
    if (!isDaytime) glEnable(GL_LIGHT1); else glDisable(GL_LIGHT1);

    // Light 2: General ambient light - brighter for late afternoon
    GLfloat globalAmbient[] = {0.35f, 0.3f, 0.25f, 1.0f}; // warm overall ambient
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
    
    // Generate texture IDs into a local array and assign named variables
    GLuint texIds[10];
    glGenTextures(10, texIds);
    textureWood = texIds[0];
    texturePlaster = texIds[1];
    texturePaper = texIds[2];
    textureWallpaper = texIds[3];
    textureCarpet = texIds[4];
    textureCurtains = texIds[5];
    textureCouch = texIds[6];
    textureGlass = texIds[7];
    textureGround = texIds[8];
    texturePortrait = texIds[9];

    // Load textures from files (each function will bind the named texture ID)
    createWoodTexture();
    createPlasterTexture();
    createPaperTexture();
    createWallpaperTexture();
    createCarpetTexture();
    createCurtainsTexture();
    createCouchTexture();
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

void createGlassTexture() {
    int width, height, channels;
    unsigned char *image = stbi_load("textures/glass.jpg", &width, &height, &channels, 0);
    
    if (!image) {
        printf("Failed to load glass.jpg texture\n");
        return;
    }
    
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

void createWallpaperTexture() {
    int width, height, channels;
    unsigned char *image = stbi_load("textures/wallpaper.jpg", &width, &height, &channels, 0);
    
    if (!image) {
        printf("Failed to load wallpaper.jpg texture - using procedural pattern\n");
        // Create a simple procedural wallpaper pattern if file doesn't exist
        glBindTexture(GL_TEXTURE_2D, textureWallpaper);
        
        // Create a simple beige/cream colored texture with subtle pattern
        unsigned char wallpaperData[256 * 256 * 3];
        for(int y = 0; y < 256; y++) {
            for(int x = 0; x < 256; x++) {
                int idx = (y * 256 + x) * 3;
                // Base cream/beige color
                wallpaperData[idx + 0] = (unsigned char)(200 + (x % 16) * 2);     // Red
                wallpaperData[idx + 1] = (unsigned char)(190 + (y % 16));         // Green
                wallpaperData[idx + 2] = (unsigned char)(170 + ((x+y) % 16));     // Blue
            }
        }
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, wallpaperData);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        
        printf("Procedural wallpaper texture created (ID: %d)\n", textureWallpaper);
        return;
    }
    
    glBindTexture(GL_TEXTURE_2D, textureWallpaper);
    
    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    stbi_image_free(image);
    printf("Wallpaper texture loaded (ID: %d)\n", textureWallpaper);
}

void createCarpetTexture() {
    int width, height, channels;
    unsigned char *image = stbi_load("textures/carpet.jpg", &width, &height, &channels, 0);
    
    if (!image) {
        printf("Failed to load carpet.jpg texture\n");
        return;
    }
    
    glBindTexture(GL_TEXTURE_2D, textureCarpet);
    
    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    stbi_image_free(image);
    printf("Carpet texture loaded (ID: %d)\n", textureCarpet);
}

void createCurtainsTexture() {
    int width, height, channels;
    unsigned char *image = stbi_load("textures/curtains.jpg", &width, &height, &channels, 0);
    
    if (!image) {
        printf("Failed to load curtains.jpg texture\n");
        return;
    }
    
    glBindTexture(GL_TEXTURE_2D, textureCurtains);
    
    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    stbi_image_free(image);
    printf("Curtains texture loaded (ID: %d)\n", textureCurtains);
}

void createCouchTexture() {
    int width, height, channels;
    unsigned char *image = stbi_load("textures/couch.jpg", &width, &height, &channels, 0);
    
    if (!image) {
        printf("Failed to load couch.jpg texture\n");
        return;
    }
    
    glBindTexture(GL_TEXTURE_2D, textureCouch);
    
    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    stbi_image_free(image);
    printf("Couch texture loaded (ID: %d)\n", textureCouch);
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

    // walls - use wallpaper texture
    glBindTexture(GL_TEXTURE_2D, textureWallpaper);
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

    float deskThickness = 0.08f; // Added thickness to tabletop
    
    // desk top surface (top face)
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.0f, 1.0f, -1.0f);
    glTexCoord2f(2.0f, 0.0f); glVertex3f(2.0f, 1.0f, -1.0f);
    glTexCoord2f(2.0f, 1.0f); glVertex3f(2.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.0f, 1.0f, 1.0f);
    glEnd();
    
    // desk top bottom face
    glBegin(GL_QUADS);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.0f, 1.0f - deskThickness, -1.0f);
    glTexCoord2f(2.0f, 0.0f); glVertex3f(2.0f, 1.0f - deskThickness, -1.0f);
    glTexCoord2f(2.0f, 1.0f); glVertex3f(2.0f, 1.0f - deskThickness, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.0f, 1.0f - deskThickness, 1.0f);
    glEnd();
    
    // desk top edges
    glBegin(GL_QUADS);
    // Front edge
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.0f, 1.0f - deskThickness, -1.0f);
    glTexCoord2f(4.0f, 0.0f); glVertex3f(2.0f, 1.0f - deskThickness, -1.0f);
    glTexCoord2f(4.0f, 0.08f); glVertex3f(2.0f, 1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.08f); glVertex3f(-2.0f, 1.0f, -1.0f);
    // Back edge
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-2.0f, 1.0f - deskThickness, 1.0f);
    glVertex3f(2.0f, 1.0f - deskThickness, 1.0f);
    glVertex3f(2.0f, 1.0f, 1.0f);
    glVertex3f(-2.0f, 1.0f, 1.0f);
    // Left edge
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-2.0f, 1.0f - deskThickness, -1.0f);
    glVertex3f(-2.0f, 1.0f - deskThickness, 1.0f);
    glVertex3f(-2.0f, 1.0f, 1.0f);
    glVertex3f(-2.0f, 1.0f, -1.0f);
    // Right edge
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(2.0f, 1.0f - deskThickness, -1.0f);
    glVertex3f(2.0f, 1.0f - deskThickness, 1.0f);
    glVertex3f(2.0f, 1.0f, 1.0f);
    glVertex3f(2.0f, 1.0f, -1.0f);
    glEnd();
    
    glColor3fv(colorDarkWood); // Back to color for legs
    // desk legs
    float legWidth = 0.2f;
    float legDepth = 0.15f;
    float legHeight = 1.0f - deskThickness;

    // front left leg (4 faces for 3D look)
    glBegin(GL_QUADS);
    // Front face
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.8f, 0.0f, -0.8f);
    glVertex3f(-1.8f + legWidth, 0.0f, -0.8f);
    glVertex3f(-1.8f + legWidth, legHeight, -0.8f);
    glVertex3f(-1.8f, legHeight, -0.8f);
    // Back face
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(-1.8f, 0.0f, -0.8f + legDepth);
    glVertex3f(-1.8f + legWidth, 0.0f, -0.8f + legDepth);
    glVertex3f(-1.8f + legWidth, legHeight, -0.8f + legDepth);
    glVertex3f(-1.8f, legHeight, -0.8f + legDepth);
    // Left face
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-1.8f, 0.0f, -0.8f);
    glVertex3f(-1.8f, 0.0f, -0.8f + legDepth);
    glVertex3f(-1.8f, legHeight, -0.8f + legDepth);
    glVertex3f(-1.8f, legHeight, -0.8f);
    // Right face
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-1.8f + legWidth, 0.0f, -0.8f);
    glVertex3f(-1.8f + legWidth, 0.0f, -0.8f + legDepth);
    glVertex3f(-1.8f + legWidth, legHeight, -0.8f + legDepth);
    glVertex3f(-1.8f + legWidth, legHeight, -0.8f);
    glEnd();

    // front right leg (4 faces)
    glBegin(GL_QUADS);
    // Front face
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(1.8f - legWidth, 0.0f, -0.8f);
    glVertex3f(1.8f, 0.0f, -0.8f);
    glVertex3f(1.8f, legHeight, -0.8f);
    glVertex3f(1.8f - legWidth, legHeight, -0.8f);
    // Back face
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(1.8f - legWidth, 0.0f, -0.8f + legDepth);
    glVertex3f(1.8f, 0.0f, -0.8f + legDepth);
    glVertex3f(1.8f, legHeight, -0.8f + legDepth);
    glVertex3f(1.8f - legWidth, legHeight, -0.8f + legDepth);
    // Left face
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(1.8f - legWidth, 0.0f, -0.8f);
    glVertex3f(1.8f - legWidth, 0.0f, -0.8f + legDepth);
    glVertex3f(1.8f - legWidth, legHeight, -0.8f + legDepth);
    glVertex3f(1.8f - legWidth, legHeight, -0.8f);
    // Right face
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(1.8f, 0.0f, -0.8f);
    glVertex3f(1.8f, 0.0f, -0.8f + legDepth);
    glVertex3f(1.8f, legHeight, -0.8f + legDepth);
    glVertex3f(1.8f, legHeight, -0.8f);
    glEnd();

    // back legs - now with 3D
    glBegin(GL_QUADS);
    // Back left leg
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(-1.8f, 0.0f, 0.8f);
    glVertex3f(-1.8f + legWidth, 0.0f, 0.8f);
    glVertex3f(-1.8f + legWidth, legHeight, 0.8f);
    glVertex3f(-1.8f, legHeight, 0.8f);
    // Back right leg
    glVertex3f(1.8f - legWidth, 0.0f, 0.8f);
    glVertex3f(1.8f, 0.0f, 0.8f);
    glVertex3f(1.8f, legHeight, 0.8f);
    glVertex3f(1.8f - legWidth, legHeight, 0.8f);
    glEnd();
}

void drawChair() {
    glColor3fv(colorLightWood);
    glBindTexture(GL_TEXTURE_2D, textureWood);
    glColor3f(1.0f, 1.0f, 1.0f);

    float seatThickness = 0.06f;
    float backThickness = 0.05f;

    // chair seat (top and bottom with thickness)
    glBegin(GL_QUADS);
    // Top face
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, 0.6f, 0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, 0.6f, 0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.6f, 1.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.6f, 1.5f);
    // Bottom face
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-0.5f, 0.6f - seatThickness, 0.5f);
    glVertex3f(0.5f, 0.6f - seatThickness, 0.5f);
    glVertex3f(0.5f, 0.6f - seatThickness, 1.5f);
    glVertex3f(-0.5f, 0.6f - seatThickness, 1.5f);
    // Front edge
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(-0.5f, 0.6f - seatThickness, 0.5f);
    glVertex3f(0.5f, 0.6f - seatThickness, 0.5f);
    glVertex3f(0.5f, 0.6f, 0.5f);
    glVertex3f(-0.5f, 0.6f, 0.5f);
    // Back edge
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-0.5f, 0.6f - seatThickness, 1.5f);
    glVertex3f(0.5f, 0.6f - seatThickness, 1.5f);
    glVertex3f(0.5f, 0.6f, 1.5f);
    glVertex3f(-0.5f, 0.6f, 1.5f);
    // Left edge
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-0.5f, 0.6f - seatThickness, 0.5f);
    glVertex3f(-0.5f, 0.6f - seatThickness, 1.5f);
    glVertex3f(-0.5f, 0.6f, 1.5f);
    glVertex3f(-0.5f, 0.6f, 0.5f);
    // Right edge
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.5f, 0.6f - seatThickness, 0.5f);
    glVertex3f(0.5f, 0.6f - seatThickness, 1.5f);
    glVertex3f(0.5f, 0.6f, 1.5f);
    glVertex3f(0.5f, 0.6f, 0.5f);
    glEnd();
    
    // chair back (with thickness)
    glBegin(GL_QUADS);
    // Front face
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(-0.5f, 0.6f, 1.5f);
    glVertex3f(0.5f, 0.6f, 1.5f);
    glVertex3f(0.5f, 1.5f, 1.5f);
    glVertex3f(-0.5f, 1.5f, 1.5f);
    // Back face
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-0.5f, 0.6f, 1.5f + backThickness);
    glVertex3f(0.5f, 0.6f, 1.5f + backThickness);
    glVertex3f(0.5f, 1.5f, 1.5f + backThickness);
    glVertex3f(-0.5f, 1.5f, 1.5f + backThickness);
    // Top edge
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-0.5f, 1.5f, 1.5f);
    glVertex3f(0.5f, 1.5f, 1.5f);
    glVertex3f(0.5f, 1.5f, 1.5f + backThickness);
    glVertex3f(-0.5f, 1.5f, 1.5f + backThickness);
    // Bottom edge
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-0.5f, 0.6f, 1.5f);
    glVertex3f(0.5f, 0.6f, 1.5f);
    glVertex3f(0.5f, 0.6f, 1.5f + backThickness);
    glVertex3f(-0.5f, 0.6f, 1.5f + backThickness);
    // Left edge
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-0.5f, 0.6f, 1.5f);
    glVertex3f(-0.5f, 1.5f, 1.5f);
    glVertex3f(-0.5f, 1.5f, 1.5f + backThickness);
    glVertex3f(-0.5f, 0.6f, 1.5f + backThickness);
    // Right edge
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.5f, 0.6f, 1.5f);
    glVertex3f(0.5f, 1.5f, 1.5f);
    glVertex3f(0.5f, 1.5f, 1.5f + backThickness);
    glVertex3f(0.5f, 0.6f, 1.5f + backThickness);
    glEnd();
    
    // Chair legs (now with thickness)
    glColor3fv(colorDarkWood);
    float legThickness = 0.06f;
    glBegin(GL_QUADS);
    // Front left leg
    glVertex3f(-0.4f, 0.0f, 0.6f);
    glVertex3f(-0.4f + legThickness, 0.0f, 0.6f);
    glVertex3f(-0.4f + legThickness, 0.6f, 0.6f);
    glVertex3f(-0.4f, 0.6f, 0.6f);
    // Front right leg
    glVertex3f(0.4f - legThickness, 0.0f, 0.6f);
    glVertex3f(0.4f, 0.0f, 0.6f);
    glVertex3f(0.4f, 0.6f, 0.6f);
    glVertex3f(0.4f - legThickness, 0.6f, 0.6f);
    // Back left leg
    glVertex3f(-0.4f, 0.0f, 1.4f);
    glVertex3f(-0.4f + legThickness, 0.0f, 1.4f);
    glVertex3f(-0.4f + legThickness, 0.6f, 1.4f);
    glVertex3f(-0.4f, 0.6f, 1.4f);
    // Back right leg
    glVertex3f(0.4f - legThickness, 0.0f, 1.4f);
    glVertex3f(0.4f, 0.0f, 1.4f);
    glVertex3f(0.4f, 0.6f, 1.4f);
    glVertex3f(0.4f - legThickness, 0.6f, 1.4f);
    glEnd();
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
void drawWindow() {
    // window on left wall/
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

void drawSunlight() {
    if (!isDaytime) return;

    // Draw a simple translucent sunlight shaft entering from the window
    glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE); // don't write to depth so it blends over scene

    // Warm golden beam color
    glColor4f(1.0f, 0.85f, 0.45f, 0.25f);

    glBegin(GL_QUADS);
    // Coordinates form a slanted trapezoid from the window into the room
    glVertex3f(-4.98f, 3.4f, -0.6f); // near window top-left
    glVertex3f(-4.98f, 2.0f, 0.6f);  // near window lower-right
    glVertex3f(-2.0f, 1.0f, 1.8f);   // inner room far corner bottom
    glVertex3f(-2.0f, 3.2f, -1.8f);  // inner room far corner top
    glEnd();

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glPopAttrib();
}

void drawShelves() {
    // Wooden shelves on the back wall
    glBindTexture(GL_TEXTURE_2D, textureWood);
    glColor3f(1.0f, 1.0f, 1.0f);
    
    float shelfDepth = 0.3f;
    float shelfThickness = 0.08f;  // Increased thickness
    float shelfWidth = 3.0f;
    
    // Three shelves at different heights
    for(int i = 0; i < 3; i++) {
        float shelfY = 1.5f + i * 1.0f;
        float shelfX = -1.5f;
        
        // Shelf surface (top and bottom with all edges)
        glBegin(GL_QUADS);
        // Top surface
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(shelfX, shelfY, -4.5f);
        glTexCoord2f(2.0f, 0.0f); glVertex3f(shelfX + shelfWidth, shelfY, -4.5f);
        glTexCoord2f(2.0f, 0.2f); glVertex3f(shelfX + shelfWidth, shelfY, -4.5f + shelfDepth);
        glTexCoord2f(0.0f, 0.2f); glVertex3f(shelfX, shelfY, -4.5f + shelfDepth);
        
        // Bottom surface
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(shelfX, shelfY - shelfThickness, -4.5f);
        glVertex3f(shelfX + shelfWidth, shelfY - shelfThickness, -4.5f);
        glVertex3f(shelfX + shelfWidth, shelfY - shelfThickness, -4.5f + shelfDepth);
        glVertex3f(shelfX, shelfY - shelfThickness, -4.5f + shelfDepth);
        
        // Front edge
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(shelfX, shelfY - shelfThickness, -4.5f);
        glVertex3f(shelfX + shelfWidth, shelfY - shelfThickness, -4.5f);
        glVertex3f(shelfX + shelfWidth, shelfY, -4.5f);
        glVertex3f(shelfX, shelfY, -4.5f);
        
        // Back edge
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(shelfX, shelfY - shelfThickness, -4.5f + shelfDepth);
        glVertex3f(shelfX + shelfWidth, shelfY - shelfThickness, -4.5f + shelfDepth);
        glVertex3f(shelfX + shelfWidth, shelfY, -4.5f + shelfDepth);
        glVertex3f(shelfX, shelfY, -4.5f + shelfDepth);
        
        // Left edge
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(shelfX, shelfY - shelfThickness, -4.5f);
        glVertex3f(shelfX, shelfY - shelfThickness, -4.5f + shelfDepth);
        glVertex3f(shelfX, shelfY, -4.5f + shelfDepth);
        glVertex3f(shelfX, shelfY, -4.5f);
        
        // Right edge
        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(shelfX + shelfWidth, shelfY - shelfThickness, -4.5f);
        glVertex3f(shelfX + shelfWidth, shelfY - shelfThickness, -4.5f + shelfDepth);
        glVertex3f(shelfX + shelfWidth, shelfY, -4.5f + shelfDepth);
        glVertex3f(shelfX + shelfWidth, shelfY, -4.5f);
        glEnd();
        
        // Shelf support brackets (now as 3D structures instead of lines)
        glColor3fv(colorGold);
        float bracketSize = 0.15f;
        glDisable(GL_TEXTURE_2D);
        glBegin(GL_QUADS);
        // Left bracket
        glVertex3f(shelfX - bracketSize, shelfY - shelfThickness, -4.5f);
        glVertex3f(shelfX, shelfY - shelfThickness, -4.5f);
        glVertex3f(shelfX, shelfY - shelfThickness - bracketSize, -4.5f);
        glVertex3f(shelfX - bracketSize, shelfY - shelfThickness - bracketSize, -4.5f);
        
        // Right bracket
        glVertex3f(shelfX + shelfWidth, shelfY - shelfThickness, -4.5f);
        glVertex3f(shelfX + shelfWidth + bracketSize, shelfY - shelfThickness, -4.5f);
        glVertex3f(shelfX + shelfWidth + bracketSize, shelfY - shelfThickness - bracketSize, -4.5f);
        glVertex3f(shelfX + shelfWidth, shelfY - shelfThickness - bracketSize, -4.5f);
        glEnd();
        glEnable(GL_TEXTURE_2D);
        
        glBindTexture(GL_TEXTURE_2D, textureWood);
        glColor3f(1.0f, 1.0f, 1.0f);
    }
    
    // Add some books/objects on shelves
    glColor3fv(colorRed);
    // Small book on first shelf - now with thickness
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.2f, 1.51f, -4.45f);
    glVertex3f(-0.8f, 1.51f, -4.45f);
    glVertex3f(-0.8f, 1.71f, -4.45f);
    glVertex3f(-1.2f, 1.71f, -4.45f);
    // Book side
    glVertex3f(-1.2f, 1.51f, -4.42f);
    glVertex3f(-0.8f, 1.51f, -4.42f);
    glVertex3f(-0.8f, 1.71f, -4.42f);
    glVertex3f(-1.2f, 1.71f, -4.42f);
    glEnd();
    
    // Another book on second shelf - with thickness
    glColor3fv(colorGold);
    glBegin(GL_QUADS);
    glVertex3f(0.5f, 2.51f, -4.45f);
    glVertex3f(1.0f, 2.51f, -4.45f);
    glVertex3f(1.0f, 2.66f, -4.45f);
    glVertex3f(0.5f, 2.66f, -4.45f);
    // Book side
    glVertex3f(0.5f, 2.51f, -4.42f);
    glVertex3f(1.0f, 2.51f, -4.42f);
    glVertex3f(1.0f, 2.66f, -4.42f);
    glVertex3f(0.5f, 2.66f, -4.42f);
    glEnd();
}

void drawCurtains() {
    // Red curtains on left side of window
    if (textureCurtains == 0) {
        // No curtain texture loaded, draw simple colored panels
        glDisable(GL_TEXTURE_2D);
        glColor3fv(colorRed);
        glBegin(GL_QUADS);
        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-4.95f, 1.4f, -2.0f);
        glVertex3f(-4.95f, 1.4f, -1.0f);
        glVertex3f(-4.95f, 3.6f, -1.0f);
        glVertex3f(-4.95f, 3.6f, -2.0f);
        glEnd();
        glBegin(GL_QUADS);
        glVertex3f(-4.95f, 1.4f, 1.0f);
        glVertex3f(-4.95f, 1.4f, 2.0f);
        glVertex3f(-4.95f, 3.6f, 2.0f);
        glVertex3f(-4.95f, 3.6f, 1.0f);
        glEnd();
        glEnable(GL_TEXTURE_2D);
        return;
    }

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureCurtains);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glColor3f(1.0f, 1.0f, 1.0f); // Ensure texture colors show correctly

    // Subdivide panels into vertical strips to fake folds
    const int strips = 8;
    float leftZ0 = -2.0f, leftZ1 = -1.0f;
    float rightZ0 = 1.0f, rightZ1 = 2.0f;
    float y0 = 1.4f, y1 = 3.6f;
    float sRepeat = 2.0f; // how many times texture repeats across width

    // Left panel
    for (int i = 0; i < strips; ++i) {
        float t0 = (float)i / strips;
        float t1 = (float)(i + 1) / strips;
        float zA = leftZ0 + (leftZ1 - leftZ0) * t0;
        float zB = leftZ0 + (leftZ1 - leftZ0) * t1;
        float sA = t0 * sRepeat;
        float sB = t1 * sRepeat;
        float offset = 0.02f * sinf(i * 0.7f); // slight x offset to simulate fold
        float nx = 0.9f + 0.1f * cosf(i * 0.6f);
        float nz = 0.1f * sinf(i * 0.8f);

        glBegin(GL_QUADS);
        glNormal3f(nx, 0.0f, nz);
        glTexCoord2f(sA, 0.0f); glVertex3f(-4.95f + offset, y0, zA);
        glTexCoord2f(sB, 0.0f); glVertex3f(-4.95f + offset, y0, zB);
        glTexCoord2f(sB, 1.0f); glVertex3f(-4.95f + offset, y1, zB);
        glTexCoord2f(sA, 1.0f); glVertex3f(-4.95f + offset, y1, zA);
        glEnd();
    }

    // Right panel
    for (int i = 0; i < strips; ++i) {
        float t0 = (float)i / strips;
        float t1 = (float)(i + 1) / strips;
        float zA = rightZ0 + (rightZ1 - rightZ0) * t0;
        float zB = rightZ0 + (rightZ1 - rightZ0) * t1;
        float sA = t0 * sRepeat;
        float sB = t1 * sRepeat;
        float offset = 0.02f * sinf(i * 0.9f);
        float nx = 0.9f + 0.1f * cosf(i * 0.5f);
        float nz = 0.1f * sinf(i * 0.4f);

        glBegin(GL_QUADS);
        glNormal3f(nx, 0.0f, nz);
        glTexCoord2f(sA, 0.0f); glVertex3f(-4.95f + offset, y0, zA);
        glTexCoord2f(sB, 0.0f); glVertex3f(-4.95f + offset, y0, zB);
        glTexCoord2f(sB, 1.0f); glVertex3f(-4.95f + offset, y1, zB);
        glTexCoord2f(sA, 1.0f); glVertex3f(-4.95f + offset, y1, zA);
        glEnd();
    }
    
    // Curtain rod
    glColor3fv(colorGold);
    glLineWidth(3.0f);
    glBegin(GL_LINES);
    glVertex3f(-4.94f, 3.7f, -2.0f);
    glVertex3f(-4.94f, 3.7f, 2.0f);
    glEnd();
    glLineWidth(1.0f);
}

void drawDeskLamp() {
    // Desk lamp on right side of desk
    glColor3fv(colorGray);
    
    // Lamp base (cylinder)
    glPushMatrix();
    glTranslatef(1.5f, 1.0f, 0.3f);
    glScalef(0.15f, 0.05f, 0.15f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    // Lamp post (vertical line)
    glColor3fv(colorDarkWood);
    glLineWidth(3.0f);
    glBegin(GL_LINES);
    glVertex3f(1.5f, 1.05f, 0.3f);
    glVertex3f(1.5f, 2.0f, 0.3f);
    glEnd();
    glLineWidth(1.0f);
    
    // Lamp head (cone shape)
    glColor3fv(colorRed);
    glPushMatrix();
    glTranslatef(1.5f, 2.0f, 0.3f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    glutSolidCone(0.2f, 0.3f, 20, 10);
    glPopMatrix();
    
    // Lamp light glow effect if desk lamp is on
    if (deskLampLightOn) {
        glColor4f(1.0f, 1.0f, 0.8f, 0.3f);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glPushMatrix();
        glTranslatef(1.5f, 2.2f, 0.3f);
        glutSolidSphere(0.3f, 10, 10);
        glPopMatrix();
        glDisable(GL_BLEND);
    }
}

void drawDocuments() {
    // Additional documents and papers on desk
    glBindTexture(GL_TEXTURE_2D, texturePaper);
    glColor3f(1.0f, 1.0f, 1.0f);
    
    // Document stack 1 - right side of desk
    for(int i = 0; i < 3; i++) {
        float offset = i * 0.02f;
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(0.2f + offset, 1.01f + offset, -0.7f + offset);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(1.2f + offset, 1.01f + offset, -0.7f + offset);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(1.2f + offset, 1.02f, -0.2f + offset);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(0.2f + offset, 1.02f, -0.2f + offset);
        glEnd();
    }
    
    // Document stack 2 - center of desk
    for(int i = 0; i < 2; i++) {
        float offset = i * 0.03f;
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.3f + offset, 1.01f + offset, 0.2f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f + offset, 1.01f + offset, 0.2f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f + offset, 1.02f, 0.7f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.3f + offset, 1.02f, 0.7f);
        glEnd();
    }
    
    // Inkwell/pen holder
    glColor3fv(colorGray);
    glPushMatrix();
    glTranslatef(-1.5f, 1.05f, -0.7f);
    glutSolidCube(0.15f);
    glPopMatrix();
    
    // Pen/pencil
    glColor3fv(colorGold);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex3f(-1.5f, 1.2f, -0.7f);
    glVertex3f(-1.5f, 1.5f, -0.65f);
    glEnd();
    glLineWidth(1.0f);
}

void drawCarpet() {
    // Decorative carpet in the center of the room
    glBindTexture(GL_TEXTURE_2D, textureCarpet);
    glColor3f(1.0f, 1.0f, 1.0f); // White to show texture colors
    
    // Carpet on the floor
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.5f, 0.01f, -2.0f);
    glTexCoord2f(2.0f, 0.0f); glVertex3f(2.5f, 0.01f, -2.0f);
    glTexCoord2f(2.0f, 2.0f); glVertex3f(2.5f, 0.01f, 2.5f);
    glTexCoord2f(0.0f, 2.0f); glVertex3f(-2.5f, 0.01f, 2.5f);
    glEnd();
    
    // Carpet border (gold trim)
    glColor3fv(colorGold);
    glLineWidth(3.0f);
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_LINE_LOOP);
    glVertex3f(-2.5f, 0.02f, -2.0f);
    glVertex3f(2.5f, 0.02f, -2.0f);
    glVertex3f(2.5f, 0.02f, 2.5f);
    glVertex3f(-2.5f, 0.02f, 2.5f);
    glEnd();
    glLineWidth(1.0f);
    glEnable(GL_TEXTURE_2D);
}

void drawCouch() {
    // Couch on the right wall - improved 3D design
    glBindTexture(GL_TEXTURE_2D, textureCouch);
    glColor3f(1.0f, 1.0f, 1.0f); // White to show texture colors
    
    float couchDepth = 0.6f;     // How far it sticks out from wall
    float seatHeight = 0.4f;
    float backHeight = 1.0f;
    float armWidth = 0.25f;
    float armHeight = 0.5f;
    
    // ===== SEAT CUSHION =====
    glBegin(GL_QUADS);
    // Top face of seat
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(4.9f - couchDepth, seatHeight, -1.5f);
    glTexCoord2f(3.0f, 0.0f); glVertex3f(4.9f, seatHeight, -1.5f);
    glTexCoord2f(3.0f, 1.0f); glVertex3f(4.9f, seatHeight, 1.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(4.9f - couchDepth, seatHeight, 1.5f);
    
    // Front face of seat
    glNormal3f(0.0f, -1.0f, 0.5f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(4.9f - couchDepth, 0.0f, -1.5f);
    glTexCoord2f(3.0f, 0.0f); glVertex3f(4.9f, 0.0f, -1.5f);
    glTexCoord2f(3.0f, 0.4f); glVertex3f(4.9f, seatHeight, -1.5f);
    glTexCoord2f(0.0f, 0.4f); glVertex3f(4.9f - couchDepth, seatHeight, -1.5f);
    
    // Back face of seat (where it meets backrest)
    glNormal3f(0.0f, -0.2f, -1.0f);
    glVertex3f(4.9f - couchDepth, 0.0f, 1.5f);
    glVertex3f(4.9f, 0.0f, 1.5f);
    glVertex3f(4.9f, seatHeight, 1.5f);
    glVertex3f(4.9f - couchDepth, seatHeight, 1.5f);
    
    // Left side of seat
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(4.9f - couchDepth, 0.0f, -1.5f);
    glTexCoord2f(0.6f, 0.0f); glVertex3f(4.9f - couchDepth, seatHeight, -1.5f);
    glTexCoord2f(0.6f, 0.4f); glVertex3f(4.9f - couchDepth, seatHeight, 1.5f);
    glTexCoord2f(0.0f, 0.4f); glVertex3f(4.9f - couchDepth, 0.0f, 1.5f);
    
    // Right side of seat
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(4.9f, 0.0f, -1.5f);
    glVertex3f(4.9f, seatHeight, -1.5f);
    glVertex3f(4.9f, seatHeight, 1.5f);
    glVertex3f(4.9f, 0.0f, 1.5f);
    glEnd();
    
    // ===== BACKREST (angled) =====
    glBegin(GL_QUADS);
    // Top of backrest
    glNormal3f(0.0f, 1.0f, 0.3f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(4.9f - couchDepth * 0.7f, seatHeight + backHeight, -1.5f);
    glTexCoord2f(3.0f, 0.0f); glVertex3f(4.9f - couchDepth * 0.3f, seatHeight + backHeight, -1.5f);
    glTexCoord2f(3.0f, 1.0f); glVertex3f(4.9f - couchDepth * 0.3f, seatHeight + backHeight, 1.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(4.9f - couchDepth * 0.7f, seatHeight + backHeight, 1.5f);
    
    // Front face of backrest (against wall side)
    glNormal3f(0.0f, 0.3f, -1.0f);
    glVertex3f(4.9f - couchDepth * 0.7f, seatHeight, -1.5f);
    glVertex3f(4.9f - couchDepth * 0.3f, seatHeight, -1.5f);
    glVertex3f(4.9f - couchDepth * 0.3f, seatHeight + backHeight, -1.5f);
    glVertex3f(4.9f - couchDepth * 0.7f, seatHeight + backHeight, -1.5f);
    
    // Back face of backrest
    glNormal3f(0.0f, 0.3f, 1.0f);
    glVertex3f(4.9f - couchDepth * 0.7f, seatHeight, 1.5f);
    glVertex3f(4.9f - couchDepth * 0.3f, seatHeight, 1.5f);
    glVertex3f(4.9f - couchDepth * 0.3f, seatHeight + backHeight, 1.5f);
    glVertex3f(4.9f - couchDepth * 0.7f, seatHeight + backHeight, 1.5f);
    
    // Left side of backrest
    glNormal3f(-1.0f, 0.3f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(4.9f - couchDepth * 0.7f, seatHeight, -1.5f);
    glTexCoord2f(0.7f, 0.0f); glVertex3f(4.9f - couchDepth * 0.7f, seatHeight + backHeight, -1.5f);
    glTexCoord2f(0.7f, 1.0f); glVertex3f(4.9f - couchDepth * 0.7f, seatHeight + backHeight, 1.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(4.9f - couchDepth * 0.7f, seatHeight, 1.5f);
    
    // Right side of backrest
    glNormal3f(1.0f, 0.3f, 0.0f);
    glVertex3f(4.9f - couchDepth * 0.3f, seatHeight, -1.5f);
    glVertex3f(4.9f - couchDepth * 0.3f, seatHeight + backHeight, -1.5f);
    glVertex3f(4.9f - couchDepth * 0.3f, seatHeight + backHeight, 1.5f);
    glVertex3f(4.9f - couchDepth * 0.3f, seatHeight, 1.5f);
    glEnd();
    
    // ===== ARMRESTS =====
    glColor3fv(colorDarkWood);
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    // Left armrest - outside face
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(4.9f - couchDepth, seatHeight, -1.5f);
    glVertex3f(4.9f - couchDepth, seatHeight, -1.5f - armWidth);
    glVertex3f(4.9f - couchDepth, seatHeight + armHeight, -1.5f - armWidth);
    glVertex3f(4.9f - couchDepth, seatHeight + armHeight, -1.5f);
    
    // Left armrest - top face
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(4.9f - couchDepth, seatHeight + armHeight, -1.5f - armWidth);
    glVertex3f(4.9f - couchDepth * 0.5f, seatHeight + armHeight, -1.5f - armWidth);
    glVertex3f(4.9f - couchDepth * 0.5f, seatHeight + armHeight, -1.5f);
    glVertex3f(4.9f - couchDepth, seatHeight + armHeight, -1.5f);
    
    // Right armrest - outside face
    glVertex3f(4.9f - couchDepth, seatHeight, 1.5f);
    glVertex3f(4.9f - couchDepth, seatHeight, 1.5f + armWidth);
    glVertex3f(4.9f - couchDepth, seatHeight + armHeight, 1.5f + armWidth);
    glVertex3f(4.9f - couchDepth, seatHeight + armHeight, 1.5f);
    
    // Right armrest - top face
    glVertex3f(4.9f - couchDepth, seatHeight + armHeight, 1.5f + armWidth);
    glVertex3f(4.9f - couchDepth * 0.5f, seatHeight + armHeight, 1.5f + armWidth);
    glVertex3f(4.9f - couchDepth * 0.5f, seatHeight + armHeight, 1.5f);
    glVertex3f(4.9f - couchDepth, seatHeight + armHeight, 1.5f);
    glEnd();
    
    // ===== COUCH LEGS =====
    glColor3fv(colorGold);
    glBegin(GL_QUADS);
    float legSize = 0.1f;
    // Front left leg
    glVertex3f(4.9f, 0.0f, -1.5f);
    glVertex3f(4.9f - legSize, 0.0f, -1.5f);
    glVertex3f(4.9f - legSize, legSize, -1.5f - legSize);
    glVertex3f(4.9f, legSize, -1.5f - legSize);
    
    // Front right leg
    glVertex3f(4.9f, 0.0f, 1.5f);
    glVertex3f(4.9f - legSize, 0.0f, 1.5f);
    glVertex3f(4.9f - legSize, legSize, 1.5f + legSize);
    glVertex3f(4.9f, legSize, 1.5f + legSize);
    
    // Back left leg
    glVertex3f(4.9f - couchDepth, 0.0f, -1.5f);
    glVertex3f(4.9f - couchDepth - legSize, 0.0f, -1.5f);
    glVertex3f(4.9f - couchDepth - legSize, legSize, -1.5f - legSize);
    glVertex3f(4.9f - couchDepth, legSize, -1.5f - legSize);
    
    // Back right leg
    glVertex3f(4.9f - couchDepth, 0.0f, 1.5f);
    glVertex3f(4.9f - couchDepth - legSize, 0.0f, 1.5f);
    glVertex3f(4.9f - couchDepth - legSize, legSize, 1.5f + legSize);
    glVertex3f(4.9f - couchDepth, legSize, 1.5f + legSize);
    glEnd();
    
    glEnable(GL_TEXTURE_2D);
    
    // ===== DECORATIVE PILLOWS =====
    glBindTexture(GL_TEXTURE_2D, textureCouch);
    glColor3f(0.9f, 0.9f, 0.9f); // Lighter shade for pillows
    
    // Three pillows arranged on backrest
    for(int i = 0; i < 3; i++) {
        float zPos = -0.8f + i * 0.8f;
        
        glPushMatrix();
        glTranslatef(4.9f - couchDepth * 0.5f, seatHeight + 0.4f, zPos);
        glRotatef(15.0f, 1.0f, 0.0f, 0.0f); // Slight tilt
        glScalef(0.35f, 0.35f, 0.35f);
        glutSolidCube(1.0f);
        glPopMatrix();
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