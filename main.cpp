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
float cameraDistance = 3.5f; // smaller orbit radius so camera stays inside the room
float cameraPosX = 0.0f;
float cameraPosY = 2.0f;
float cameraPosZ = 8.0f;
float cameraLookX = 0.0f;
float cameraLookY = 1.5f;
float cameraLookZ = 0.0f;
bool cameraMode = true; // false = orbital, true = FPS
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

// Texture loading state
bool texturesLoaded = false;

// Texture IDs
// Texture IDs
GLuint textureWood = 0;
GLuint texturePaper = 0;
GLuint texturePortrait = 0;
GLuint textureGlass = 0;
GLuint textureGround = 0;
GLuint textureWallpaper = 0;
GLuint textureCarpet = 0;
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
void drawDeskLamp();
void drawDocuments();
void drawCarpet();
void drawCouch();
void loadTextures();
void createWoodTexture();
void createPaperTexture();
void createWallpaperTexture();
void createCarpetTexture();
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

    // Note: Textures will be loaded on first display call to ensure OpenGL context is ready

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
    // Load textures on first display call (ensures OpenGL context is ready)
    if (!texturesLoaded) {
        loadTextures();
        texturesLoaded = true;
    }
    
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
        // Orbital Camera Mode (orbit inside the room)
        float angleRad = orbitalAngle * M_PI / 180.0f;
        // Keep orbit radius within room bounds so camera stays inside
        float maxRadius = 3.5f;
        float radius = fminf(cameraDistance, maxRadius);
        float camX = radius * sinf(angleRad);
        float camZ = radius * cosf(angleRad);
        float camY = 2.2f; // eye height inside room
        gluLookAt(camX, camY, camZ,
                  0.0f, 1.2f, 0.0f,
                  0.0f, 1.0f, 0.0f);
    }
}

// =========== Texture Loading =======
// ============= Texture Loading Function =============
void loadTextures() {
    // Ensure OpenGL state is properly set up
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Enable texture mapping
    glEnable(GL_TEXTURE_2D);
    // Ensure tightly packed rows for stb_image (avoid alignment issues)
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    // Generate texture IDs individually to avoid any accidental index mixups
    glGenTextures(1, &textureWood);
    printf("Generated textureWood: %u\n", textureWood);
    glGenTextures(1, &texturePaper);
    printf("Generated texturePaper: %u\n", texturePaper);
    glGenTextures(1, &textureWallpaper);
    printf("Generated textureWallpaper: %u\n", textureWallpaper);
    glGenTextures(1, &textureCarpet);
    printf("Generated textureCarpet: %u\n", textureCarpet);
    glGenTextures(1, &textureCouch);
    printf("Generated textureCouch: %u\n", textureCouch);
    glGenTextures(1, &textureGlass);
    printf("Generated textureGlass: %u\n", textureGlass);
    glGenTextures(1, &textureGround);
    printf("Generated textureGround: %u\n", textureGround);
    glGenTextures(1, &texturePortrait);
    printf("Generated texturePortrait: %u\n", texturePortrait);

    // Load textures from files (each function will bind the named texture ID)
    printf("Loading wood texture...\n");
    createWoodTexture();
    printf("Loading paper texture...\n");
    createPaperTexture();
    printf("Loading wallpaper texture...\n");
    createWallpaperTexture();
    printf("Loading carpet texture...\n");
    createCarpetTexture();
    printf("Loading couch texture...\n");
    createCouchTexture();
    printf("Loading glass texture...\n");
    createGlassTexture();
    printf("Loading ground texture...\n");
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


void createPaperTexture() {
    if (texturePaper == 0) {
        printf("ERROR: texturePaper is 0!\n");
        return;
    }
    
    int width, height, channels;
    unsigned char *image = stbi_load("textures/paper.jpg", &width, &height, &channels, 0);
    
    if (!image) {
        printf("Failed to load paper.jpg texture\n");
        return;
    }
    
    printf("Binding texturePaper (ID: %u), image size: %dx%d, channels: %d\n", texturePaper, width, height, channels);
    glBindTexture(GL_TEXTURE_2D, texturePaper);
    
    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
    // Use mipmap builder which also safely resizes to max texture size if needed
    if (gluBuild2DMipmaps(GL_TEXTURE_2D, format, width, height, format, GL_UNSIGNED_BYTE, image) != 0) {
        printf("Failed to build mipmaps for paper texture\n");
    }
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
    gluBuild2DMipmaps(GL_TEXTURE_2D, format, width, height, format, GL_UNSIGNED_BYTE, image);
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
    gluBuild2DMipmaps(GL_TEXTURE_2D, format, width, height, format, GL_UNSIGNED_BYTE, image);
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
    gluBuild2DMipmaps(GL_TEXTURE_2D, format, width, height, format, GL_UNSIGNED_BYTE, image);
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
    gluBuild2DMipmaps(GL_TEXTURE_2D, format, width, height, format, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    stbi_image_free(image);
    printf("Carpet texture loaded (ID: %d)\n", textureCarpet);
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
    gluBuild2DMipmaps(GL_TEXTURE_2D, format, width, height, format, GL_UNSIGNED_BYTE, image);
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
    
    // Desk legs (textured)
    glColor3f(1.0f, 1.0f, 1.0f);
    // desk legs
    float legWidth = 0.2f;
    float legDepth = 0.15f;
    float legHeight = 1.0f - deskThickness;

    // front left leg (4 faces for 3D look)
    glBegin(GL_QUADS);
    // Front face
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.8f, 0.0f, -0.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.8f + legWidth, 0.0f, -0.8f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.8f + legWidth, legHeight, -0.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.8f, legHeight, -0.8f);
    // Back face
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.8f, 0.0f, -0.8f + legDepth);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.8f + legWidth, 0.0f, -0.8f + legDepth);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.8f + legWidth, legHeight, -0.8f + legDepth);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.8f, legHeight, -0.8f + legDepth);
    // Left face
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.8f, 0.0f, -0.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.8f, 0.0f, -0.8f + legDepth);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.8f, legHeight, -0.8f + legDepth);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.8f, legHeight, -0.8f);
    // Right face
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.8f + legWidth, 0.0f, -0.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.8f + legWidth, 0.0f, -0.8f + legDepth);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.8f + legWidth, legHeight, -0.8f + legDepth);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.8f + legWidth, legHeight, -0.8f);
    glEnd();

    // front right leg (4 faces)
    glBegin(GL_QUADS);
    // Front face
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.8f - legWidth, 0.0f, -0.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.8f, 0.0f, -0.8f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.8f, legHeight, -0.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.8f - legWidth, legHeight, -0.8f);
    // Back face
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.8f - legWidth, 0.0f, -0.8f + legDepth);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.8f, 0.0f, -0.8f + legDepth);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.8f, legHeight, -0.8f + legDepth);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.8f - legWidth, legHeight, -0.8f + legDepth);
    // Left face
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.8f - legWidth, 0.0f, -0.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.8f - legWidth, 0.0f, -0.8f + legDepth);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.8f - legWidth, legHeight, -0.8f + legDepth);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.8f - legWidth, legHeight, -0.8f);
    // Right face
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.8f, 0.0f, -0.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.8f, 0.0f, -0.8f + legDepth);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.8f, legHeight, -0.8f + legDepth);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.8f, legHeight, -0.8f);
    glEnd();

    // back legs - now with 3D
    glBegin(GL_QUADS);
    // Back left leg
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.8f, 0.0f, 0.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.8f + legWidth, 0.0f, 0.8f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.8f + legWidth, legHeight, 0.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.8f, legHeight, 0.8f);
    // Back right leg
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.8f - legWidth, 0.0f, 0.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.8f, 0.0f, 0.8f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.8f, legHeight, 0.8f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.8f - legWidth, legHeight, 0.8f);
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
    // Front face (texture mapped)
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, 0.6f, 1.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, 0.6f, 1.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 1.5f, 1.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 1.5f, 1.5f);
    // Back face
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, 0.6f, 1.5f + backThickness);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, 0.6f, 1.5f + backThickness);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 1.5f, 1.5f + backThickness);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 1.5f, 1.5f + backThickness);
    // Top edge
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, 1.5f, 1.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, 1.5f, 1.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 1.5f, 1.5f + backThickness);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 1.5f, 1.5f + backThickness);
    // Bottom edge
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, 0.6f, 1.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, 0.6f, 1.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.6f, 1.5f + backThickness);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.6f, 1.5f + backThickness);
    // Left edge
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, 0.6f, 1.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, 1.5f, 1.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, 1.5f, 1.5f + backThickness);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.6f, 1.5f + backThickness);
    // Right edge
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, 0.6f, 1.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, 1.5f, 1.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 1.5f, 1.5f + backThickness);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, 0.6f, 1.5f + backThickness);
    glEnd();
    
    // Chair legs (now with thickness) and textured
    glColor3fv(colorDarkWood);
    float legThickness = 0.06f;
    glBegin(GL_QUADS);
    // Front left leg
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.4f, 0.0f, 0.6f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.4f + legThickness, 0.0f, 0.6f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.4f + legThickness, 0.6f, 0.6f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.4f, 0.6f, 0.6f);
    // Front right leg
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0.4f - legThickness, 0.0f, 0.6f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.4f, 0.0f, 0.6f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.4f, 0.6f, 0.6f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.4f - legThickness, 0.6f, 0.6f);
    // Back left leg
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.4f, 0.0f, 1.4f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.4f + legThickness, 0.0f, 1.4f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.4f + legThickness, 0.6f, 1.4f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.4f, 0.6f, 1.4f);
    // Back right leg
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0.4f - legThickness, 0.0f, 1.4f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.4f, 0.0f, 1.4f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.4f, 0.6f, 1.4f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.4f - legThickness, 0.6f, 1.4f);
    glEnd();
}

void drawRadio() {
    // Soft shadow under radio
    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, 0.25f);
    glBegin(GL_QUADS);
    glVertex3f(-0.65f, 1.005f, -0.9f);
    glVertex3f(0.65f, 1.005f, -0.9f);
    glVertex3f(0.65f, 1.005f, -0.7f);
    glVertex3f(-0.65f, 1.005f, -0.7f);
    glEnd();
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
    glPopAttrib();

    // Radio body as a small box with material shine
    GLfloat radioSpec[] = {0.6f, 0.6f, 0.6f, 1.0f};
    GLfloat radioShininess = 30.0f;
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, radioSpec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, radioShininess);
    glColor3fv(colorGray);

    float rx0 = -0.6f, rx1 = 0.6f;
    float ry0 = 1.1f, ry1 = 1.8f;
    float rzFront = -0.7f, rzBack = -0.95f;

    // Front face
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(rx0, ry0, rzFront);
    glVertex3f(rx1, ry0, rzFront);
    glVertex3f(rx1, ry1, rzFront);
    glVertex3f(rx0, ry1, rzFront);
    glEnd();

    // Back face
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(rx1, ry0, rzBack);
    glVertex3f(rx0, ry0, rzBack);
    glVertex3f(rx0, ry1, rzBack);
    glVertex3f(rx1, ry1, rzBack);
    glEnd();

    // Top
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(rx0, ry1, rzFront);
    glVertex3f(rx1, ry1, rzFront);
    glVertex3f(rx1, ry1, rzBack);
    glVertex3f(rx0, ry1, rzBack);
    glEnd();

    // Bottom
    glBegin(GL_QUADS);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(rx0, ry0, rzBack);
    glVertex3f(rx1, ry0, rzBack);
    glVertex3f(rx1, ry0, rzFront);
    glVertex3f(rx0, ry0, rzFront);
    glEnd();

    // Left
    glBegin(GL_QUADS);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(rx0, ry0, rzBack);
    glVertex3f(rx0, ry0, rzFront);
    glVertex3f(rx0, ry1, rzFront);
    glVertex3f(rx0, ry1, rzBack);
    glEnd();

    // Right
    glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(rx1, ry0, rzFront);
    glVertex3f(rx1, ry0, rzBack);
    glVertex3f(rx1, ry1, rzBack);
    glVertex3f(rx1, ry1, rzFront);
    glEnd();

    // Speaker grille: draw recessed dark rectangles on front
    glColor3f(0.15f, 0.15f, 0.15f);
    for(int i = 0; i < 4; i++){
        float x = -0.35f + i * 0.2f;
        float w = 0.14f;
        float h0 = 1.35f, h1 = 1.65f;
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(x, h0, rzFront + 0.001f);
        glVertex3f(x + w, h0, rzFront + 0.001f);
        glVertex3f(x + w, h1, rzFront + 0.001f);
        glVertex3f(x, h1, rzFront + 0.001f);
        glEnd();
    }

    // Knob
    glColor3fv(colorGold);
    glPushMatrix();
    glTranslatef(0.55f, 1.45f, rzFront + 0.01f);
    glutSolidSphere(0.04f, 12, 8);
    glPopMatrix();

    // Antenna
    glColor3f(0.8f, 0.8f, 0.8f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex3f(0.0f, ry1, rzBack + 0.0f);
    glVertex3f(0.0f, ry1 + 0.9f, rzBack - 0.05f);
    glEnd();
    glLineWidth(1.0f);

    // reset material specular to default low
    GLfloat defaultSpec[] = {0.2f, 0.2f, 0.2f, 1.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, defaultSpec);
}

void drawBooks() {
    // Stack of books as small boxes with wood texture covers
    glBindTexture(GL_TEXTURE_2D, textureWood);
    glColor3f(1.0f, 1.0f, 1.0f);

    // Helper lambda to draw a textured box
    auto drawBox = [](float x0, float x1, float y0, float y1, float z0, float z1) {
        glBegin(GL_QUADS);
        // Front
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x0, y0, z1);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x1, y0, z1);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x1, y1, z1);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x0, y1, z1);
        // Back
        glNormal3f(0.0f, 0.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x1, y0, z0);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x0, y0, z0);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x0, y1, z0);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x1, y1, z0);
        // Top
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x0, y1, z1);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x1, y1, z1);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x1, y1, z0);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x0, y1, z0);
        // Bottom
        glNormal3f(0.0f, -1.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x0, y0, z0);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x1, y0, z0);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x1, y0, z1);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x0, y0, z1);
        // Left
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x0, y0, z0);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x0, y0, z1);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x0, y1, z1);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x0, y1, z0);
        // Right
        glNormal3f(1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x1, y0, z1);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x1, y0, z0);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x1, y1, z0);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x1, y1, z1);
        glEnd();
    };

    // Book 1
    drawBox(0.5f, 1.0f, 1.01f, 1.2f, -0.35f, -0.25f);

    // Book 2
    drawBox(0.5f, 1.0f, 1.21f, 1.35f, -0.45f, -0.35f);

    // papers - use paper texture, make thin stack with slight offsets
    glBindTexture(GL_TEXTURE_2D, texturePaper);
    glColor3f(1.0f, 1.0f, 1.0f);
    for (int i = 0; i < 3; ++i) {
        float offset = i * 0.005f;
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f + offset, 1.01f + offset, 0.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f + offset, 1.01f + offset, 0.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f + offset, 1.05f + offset, 0.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f + offset, 1.05f + offset, 0.0f);
        glEnd();
    }
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
        float offset = i * 0.03f;
        // soft shadow under each paper
        glPushAttrib(GL_ENABLE_BIT);
        glDisable(GL_LIGHTING);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(0.0f, 0.0f, 0.0f, 0.18f);
        glBegin(GL_QUADS);
        glVertex3f(-1.52f + offset, 1.003f, -0.52f + offset);
        glVertex3f(-0.98f + offset, 1.003f, -0.52f + offset);
        glVertex3f(-0.98f + offset, 1.003f, -0.28f + offset);
        glVertex3f(-1.52f + offset, 1.003f, -0.28f + offset);
        glEnd();
        glDisable(GL_BLEND);
        glEnable(GL_LIGHTING);
        glPopAttrib();

        // slightly curved paper using center vertex raised
        glBegin(GL_TRIANGLES);
        glNormal3f(0.0f, 1.0f, 0.0f);
        // triangle 1
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.5f + offset, 1.01f, -0.5f + offset);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f + offset, 1.01f, -0.5f + offset);
        glTexCoord2f(0.5f, 1.0f); glVertex3f(-1.25f + offset, 1.03f, -0.4f + offset);
        // triangle 2
        glTexCoord2f(0.5f, 1.0f); glVertex3f(-1.25f + offset, 1.03f, -0.4f + offset);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f + offset, 1.01f, -0.3f + offset);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.5f + offset, 1.01f, -0.3f + offset);
        glEnd();

        // slight thickness underside (very thin quad)
        glBegin(GL_QUADS);
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(-1.5f + offset, 1.009f, -0.5f + offset);
        glVertex3f(-1.0f + offset, 1.009f, -0.5f + offset);
        glVertex3f(-1.0f + offset, 1.009f, -0.3f + offset);
        glVertex3f(-1.5f + offset, 1.009f, -0.3f + offset);
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
        glTexCoord2f(0.0f, 0.0f); glVertex3f(shelfX, shelfY - shelfThickness, -4.5f);
        glTexCoord2f(2.0f, 0.0f); glVertex3f(shelfX + shelfWidth, shelfY - shelfThickness, -4.5f);
        glTexCoord2f(2.0f, 0.2f); glVertex3f(shelfX + shelfWidth, shelfY - shelfThickness, -4.5f + shelfDepth);
        glTexCoord2f(0.0f, 0.2f); glVertex3f(shelfX, shelfY - shelfThickness, -4.5f + shelfDepth);
        
        // Front edge
        glNormal3f(0.0f, 0.0f, -1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(shelfX, shelfY - shelfThickness, -4.5f);
        glTexCoord2f(2.0f, 0.0f); glVertex3f(shelfX + shelfWidth, shelfY - shelfThickness, -4.5f);
        glTexCoord2f(2.0f, 0.4f); glVertex3f(shelfX + shelfWidth, shelfY, -4.5f);
        glTexCoord2f(0.0f, 0.4f); glVertex3f(shelfX, shelfY, -4.5f);
        
        // Back edge
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(shelfX, shelfY - shelfThickness, -4.5f + shelfDepth);
        glTexCoord2f(2.0f, 0.0f); glVertex3f(shelfX + shelfWidth, shelfY - shelfThickness, -4.5f + shelfDepth);
        glTexCoord2f(2.0f, 0.4f); glVertex3f(shelfX + shelfWidth, shelfY, -4.5f + shelfDepth);
        glTexCoord2f(0.0f, 0.4f); glVertex3f(shelfX, shelfY, -4.5f + shelfDepth);
        
        // Left edge
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(shelfX, shelfY - shelfThickness, -4.5f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(shelfX, shelfY - shelfThickness, -4.5f + shelfDepth);
        glTexCoord2f(1.0f, 0.4f); glVertex3f(shelfX, shelfY, -4.5f + shelfDepth);
        glTexCoord2f(0.0f, 0.4f); glVertex3f(shelfX, shelfY, -4.5f);
        
        // Right edge
        glNormal3f(1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(shelfX + shelfWidth, shelfY - shelfThickness, -4.5f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(shelfX + shelfWidth, shelfY - shelfThickness, -4.5f + shelfDepth);
        glTexCoord2f(1.0f, 0.4f); glVertex3f(shelfX + shelfWidth, shelfY, -4.5f + shelfDepth);
        glTexCoord2f(0.0f, 0.4f); glVertex3f(shelfX + shelfWidth, shelfY, -4.5f);
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
        // small soft shadow
        glPushAttrib(GL_ENABLE_BIT);
        glDisable(GL_LIGHTING);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(0.0f,0.0f,0.0f,0.15f);
        glBegin(GL_QUADS);
        glVertex3f(0.18f + offset, 1.003f + offset, -0.72f + offset);
        glVertex3f(1.22f + offset, 1.003f + offset, -0.72f + offset);
        glVertex3f(1.22f + offset, 1.003f + offset, -0.18f + offset);
        glVertex3f(0.18f + offset, 1.003f + offset, -0.18f + offset);
        glEnd();
        glDisable(GL_BLEND);
        glEnable(GL_LIGHTING);
        glPopAttrib();

        // paper with slight bend
        glBegin(GL_TRIANGLES);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(0.2f + offset, 1.01f + offset, -0.7f + offset);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(1.2f + offset, 1.01f + offset, -0.7f + offset);
        glTexCoord2f(0.5f, 1.0f); glVertex3f(0.7f + offset, 1.03f + offset, -0.45f + offset);
        glTexCoord2f(0.5f, 1.0f); glVertex3f(0.7f + offset, 1.03f + offset, -0.45f + offset);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(1.2f + offset, 1.01f + offset, -0.2f + offset);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(0.2f + offset, 1.01f + offset, -0.2f + offset);
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
    
    glBindTexture(GL_TEXTURE_2D, textureCouch);
    glColor3f(1, 1, 1);

    float couchDepth = 1.1f;
    float seatHeight = 0.55f;
    float seatThickness = 0.25f;

    float backHeight = 1.0f;
    float backTilt   = 0.15f;

    float armWidth = 0.35f;
    float armHeight = 0.75f;
    float couchLength = 2.6f;

    float xFront = 4.9f;
    float xBack  = xFront - couchDepth;
    float zLeft  = -couchLength * 0.5f;
    float zRight = couchLength * 0.5f;
    glPushMatrix();
    glTranslatef(xFront - couchDepth * 0.5f, 0, 0);  // pivot around seat center
    glRotatef(180.0f, 0, 1, 0);
    glTranslatef(-(xFront - couchDepth * 0.5f), 0, 0);
     
    // =======================
    // SEAT (rectangular block)
    // =======================
    glBegin(GL_QUADS);

    // Top
    glNormal3f(0,1,0);
    glTexCoord2f(0,0); glVertex3f(xBack,              seatHeight,        zLeft);
    glTexCoord2f(3,0); glVertex3f(xFront,             seatHeight,        zLeft);
    glTexCoord2f(3,1); glVertex3f(xFront,             seatHeight,        zRight);
    glTexCoord2f(0,1); glVertex3f(xBack,              seatHeight,        zRight);

    // Bottom
    glNormal3f(0,-1,0);
    glTexCoord2f(0,0); glVertex3f(xBack, seatHeight-seatThickness, zLeft);
    glTexCoord2f(3,0); glVertex3f(xFront,seatHeight-seatThickness, zLeft);
    glTexCoord2f(3,1); glVertex3f(xFront,seatHeight-seatThickness, zRight);
    glTexCoord2f(0,1); glVertex3f(xBack, seatHeight-seatThickness, zRight);

    // Front vertical face
    glNormal3f(0,0,-1);
    glTexCoord2f(0,0); glVertex3f(xBack, seatHeight-seatThickness, zLeft);
    glTexCoord2f(3,0); glVertex3f(xFront,seatHeight-seatThickness, zLeft);
    glTexCoord2f(3,1); glVertex3f(xFront,seatHeight,               zLeft);
    glTexCoord2f(0,1); glVertex3f(xBack, seatHeight,               zLeft);

    // Back vertical face
    glNormal3f(0,0,1);
    glTexCoord2f(0,0); glVertex3f(xBack, seatHeight-seatThickness, zRight);
    glTexCoord2f(3,0); glVertex3f(xFront,seatHeight-seatThickness, zRight);
    glTexCoord2f(3,1); glVertex3f(xFront,seatHeight,               zRight);
    glTexCoord2f(0,1); glVertex3f(xBack, seatHeight,               zRight);

    // Left side
    glNormal3f(-1,0,0);
    glTexCoord2f(0,0); glVertex3f(xBack,              seatHeight-seatThickness, zLeft);
    glTexCoord2f(1,0); glVertex3f(xBack,              seatHeight,               zLeft);
    glTexCoord2f(1,1); glVertex3f(xBack,              seatHeight,               zRight);
    glTexCoord2f(0,1); glVertex3f(xBack,              seatHeight-seatThickness, zRight);

    // Right side
    glNormal3f(1,0,0);
    glTexCoord2f(0,0); glVertex3f(xFront, seatHeight-seatThickness, zLeft);
    glTexCoord2f(1,0); glVertex3f(xFront, seatHeight,               zLeft);
    glTexCoord2f(1,1); glVertex3f(xFront, seatHeight,               zRight);
    glTexCoord2f(0,1); glVertex3f(xFront, seatHeight-seatThickness, zRight);

    glEnd();

    // =======================
    // BACKREST (tilted slab)
    // =======================
    glBegin(GL_QUADS);

    float xb1 = xBack + backTilt;   // top slightly forward for comfy angle
    float xb2 = xBack;              // bottom exactly at seat back

    // Backrest height
    float y1 = seatHeight;
    float y2 = seatHeight + backHeight;

    // Front face (touching wall)
    glNormal3f(0,0,-1);
    glTexCoord2f(0,0); glVertex3f(xb2, y1, zLeft);
    glTexCoord2f(1,0); glVertex3f(xb1, y2, zLeft);
    glTexCoord2f(1,1); glVertex3f(xb1, y2, zRight);
    glTexCoord2f(0,1); glVertex3f(xb2, y1, zRight);

    // Back face
    glNormal3f(0,0,1);
    glTexCoord2f(0,0); glVertex3f(xb2+0.2f, y1, zLeft);
    glTexCoord2f(1,0); glVertex3f(xb1+0.2f, y2, zLeft);
    glTexCoord2f(1,1); glVertex3f(xb1+0.2f, y2, zRight);
    glTexCoord2f(0,1); glVertex3f(xb2+0.2f, y1, zRight);

    // Left side
    glNormal3f(-1,0,0);
    glTexCoord2f(0,0); glVertex3f(xb2,      y1, zLeft);
    glTexCoord2f(0,1); glVertex3f(xb1,      y2, zLeft);
    glTexCoord2f(1,1); glVertex3f(xb1+0.2f, y2, zLeft);
    glTexCoord2f(1,0); glVertex3f(xb2+0.2f, y1, zLeft);

    // Right side
    glNormal3f(1,0,0);
    glTexCoord2f(0,0); glVertex3f(xb2,      y1, zRight);
    glTexCoord2f(0,1); glVertex3f(xb1,      y2, zRight);
    glTexCoord2f(1,1); glVertex3f(xb1+0.2f, y2, zRight);
    glTexCoord2f(1,0); glVertex3f(xb2+0.2f, y1, zRight);

    glEnd();

    // =======================
    // ARMRESTS (thicker)
    // =======================
    glBegin(GL_QUADS);

    float armX1 = xFront;
    float armX2 = xFront - armWidth;

    // Left armrest
    glNormal3f(0,1,0); // top
    glTexCoord2f(0,0); glVertex3f(armX2, seatHeight+armHeight, zLeft - armWidth);
    glTexCoord2f(1,0); glVertex3f(armX1, seatHeight+armHeight, zLeft - armWidth);
    glTexCoord2f(1,1); glVertex3f(armX1, seatHeight+armHeight, zLeft);
    glTexCoord2f(0,1); glVertex3f(armX2, seatHeight+armHeight, zLeft);

    // Front face
    glNormal3f(0,0,-1);
    glTexCoord2f(0,0); glVertex3f(armX2, seatHeight, zLeft - armWidth);
    glTexCoord2f(1,0); glVertex3f(armX1, seatHeight, zLeft - armWidth);
    glTexCoord2f(1,1); glVertex3f(armX1, seatHeight+armHeight, zLeft - armWidth);
    glTexCoord2f(0,1); glVertex3f(armX2, seatHeight+armHeight, zLeft - armWidth);

    // Right armrest (same mirrored)
    glNormal3f(0,1,0);
    glTexCoord2f(0,0); glVertex3f(armX2, seatHeight+armHeight, zRight + armWidth);
    glTexCoord2f(1,0); glVertex3f(armX1, seatHeight+armHeight, zRight + armWidth);
    glTexCoord2f(1,1); glVertex3f(armX1, seatHeight+armHeight, zRight);
    glTexCoord2f(0,1); glVertex3f(armX2, seatHeight+armHeight, zRight);

    glEnd();
    glPopMatrix();

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