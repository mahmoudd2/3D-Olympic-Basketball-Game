#include <math.h>
#include <algorithm> // Include this for std::min and std::max
#include <Vector>
#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include <glut.h>

#define GLUT_KEY_ESCAPE 27
#define DEG2RAD(a) (a * 0.0174532925)
#define M_PI 3.14159265358979323846


//Variables
// 
// 

// Timer VARIABLES
int timerDuration = 100.0f; //in seconds
int timerRemaining = timerDuration;

int playerScore = 0;    // Player's score
float playerYaw = 0.0f;  // Left-Right rotation (yaw)

//Checks
bool isGameOver = false;
bool gameStarted = true;
static bool isCameraInitialized = false;
bool isAnimating = false;  // Flag to control animation
bool GameWon = false;

float animationTime = 0.0f;

//vars for trophy 
float trophyRotation = 0.0f;

//Vars for jersey
float jerseyAnimationTime = 0.0f; // Time variable for animation
float bounceSpeed = 1.0f;   // Speed of the bouncing animation
float bounceHeight = 0.8f;  // Maximum height of the bounce

//Variables for olympic rings
float ringsAnimationTime = 0.0f; // Time variable to control the animation
float scaleSpeed = 1.0f;    // Speed of scaling animation

// Variables to store rotation angles
float playerLookX = 0.0f;
float playerLookY = 0.0f;
float sensitivity = 0.1f;

// Global color variables for the walls
float wallColorR = 0.3f;
float wallColorG = 0.3f;
float wallColorB = 0.3f;
float colorChangeSpeed = 0.01f; // Adjust for faster/slower color transitions


// Global variables
bool isBackboardWhite = true; // Tracks the current color state of the backboard
float backBoardAnimationTime = 0.0f; // Tracks the last time the color changed

bool isWallGrey = true;
float lastWallColorChange = 0.0f;

// Define boundary limits (adjust based on your game world size)
float minX = -8.7f;  // Minimum X (left boundary)
float maxX = 8.7f;   // Maximum X (right boundary)
float minZ = -8.9f;  // Minimum Z (back boundary)
float maxZ = 8.9f;   // Maximum Z (front boundary)


float playerX = 0.0f, playerY = 0.25f, playerZ = 0.0f; // Initial position of the player
float playerMinX, playerMaxX, playerMinY, playerMaxY, playerMinZ, playerMaxZ;
float playerHeight = 1.1f;
float playerWidth = 0.4f;
float playerDepth = 0.2f;

struct Basketball {
	float posX, posY, posZ; // Position of the basketball
	float radius;           // Radius of the basketball
	bool isCollected;       // Whether the basketball has been collected
};

// Global or member variable for rotation angle
float ballRotationAngle = 0.0f;

//// Example array of basketballs
//std::vector<Basketball> basketballs;

int collectedBalls = 0;
const int numOfBalls = 30;

// Draw multiple basketballs at different positions
Basketball basketballs[numOfBalls] = {
	{3.2f, 0.3f, -4.1f, 0.19f, false},  // First basketball
	{-2.8f, 0.3f, 2.7f, 0.19f, false},  // Second basketball
	{0.0f, 0.3f, -3.5f, 0.19f, false},  // Third basketball
	{4.5f, 0.3f, 1.2f, 0.19f, false},   // Fourth basketball
	{-1.9f, 0.3f, -2.4f, 0.19f, false}, // Fifth basketball
	{2.3f, 0.3f, 4.0f, 0.19f, false},   // Sixth basketball
	{-4.6f, 0.3f, -0.7f, 0.19f, false}, // Seventh basketball
	{0.8f, 0.3f, 2.6f, 0.19f, false},   // Eighth basketball
	{-3.1f, 0.3f, -3.9f, 0.19f, false}, // Ninth basketball
	{1.7f, 0.3f, -1.8f, 0.19f, false},  // Tenth basketball
	// Additional 20 basketballs
	{5.1f, 0.3f, -5.0f, 0.19f, false},  // Eleventh basketball
	{-6.0f, 0.3f, 3.4f, 0.19f, false},  // Twelfth basketball
	{3.9f, 0.3f, -2.7f, 0.19f, false},  // Thirteenth basketball
	{-0.5f, 0.3f, 4.2f, 0.19f, false},  // Fourteenth basketball
	{1.4f, 0.3f, -6.3f, 0.19f, false},  // Fifteenth basketball
	{-3.8f, 0.3f, 1.5f, 0.19f, false},  // Sixteenth basketball
	{2.1f, 0.3f, -0.9f, 0.19f, false},  // Seventeenth basketball
	{4.8f, 0.3f, 3.0f, 0.19f, false},   // Eighteenth basketball
	{-5.2f, 0.3f, -1.6f, 0.19f, false}, // Nineteenth basketball
	{0.0f, 0.3f, 5.5f, 0.19f, false},   // Twentieth basketball
	{-2.2f, 0.3f, -5.8f, 0.19f, false}, // Twenty-first basketball
	{3.3f, 0.3f, 2.9f, 0.19f, false},   // Twenty-second basketball
	{1.9f, 0.3f, -4.5f, 0.19f, false},  // Twenty-third basketball
	{-4.0f, 0.3f, 0.0f, 0.19f, false},  // Twenty-fourth basketball
	{5.5f, 0.3f, 1.7f, 0.19f, false},   // Twenty-fifth basketball
	{-6.4f, 0.3f, -3.2f, 0.19f, false}, // Twenty-sixth basketball
	{2.8f, 0.3f, 4.6f, 0.19f, false},   // Twenty-seventh basketball
	{0.5f, 0.3f, -5.4f, 0.19f, false},  // Twenty-eighth basketball
	{-1.7f, 0.3f, 2.3f, 0.19f, false},  // Twenty-ninth basketball
	{4.4f, 0.3f, -2.0f, 0.19f, false},  // Thirtieth basketball
};





class Vector3f {
public:
	float x, y, z;

	Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) {
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3f operator+(Vector3f& v) {
		return Vector3f(x + v.x, y + v.y, z + v.z);
	}

	Vector3f operator-(Vector3f& v) {
		return Vector3f(x - v.x, y - v.y, z - v.z);
	}

	Vector3f operator*(float n) {
		return Vector3f(x * n, y * n, z * n);
	}

	Vector3f operator/(float n) {
		return Vector3f(x / n, y / n, z / n);
	}

	Vector3f unit() {
		return *this / sqrt(x * x + y * y + z * z);
	}

	Vector3f cross(Vector3f v) {
		return Vector3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
};

class Camera {
public:
	Vector3f eye, center, up;
	float playerX, playerY, playerZ;

	Camera(float eyeX = 1.0f, float eyeY = 1.0f, float eyeZ = 1.0f, float centerX = 0.0f, float centerY = 0.0f, float centerZ = 0.0f, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f) {
		eye = Vector3f(eyeX, eyeY, eyeZ);
		center = Vector3f(centerX, centerY, centerZ);
		up = Vector3f(upX, upY, upZ);
		playerX = 0.0f;
		playerY = 0.25f;
		playerZ = 0.0f;
	}

	void moveX(float d) {
		Vector3f right = up.cross(center - eye).unit();
		eye = eye + right * d;
		center = center + right * d;
	}

	void moveY(float d) {
		eye = eye + up.unit() * d;
		center = center + up.unit() * d;
	}

	void moveZ(float d) {
		Vector3f view = (center - eye).unit();
		eye = eye + view * d;
		center = center + view * d;
	}

	void rotateX(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + up * sin(DEG2RAD(a));
		up = view.cross(right);
		center = eye + view;
	}

	void rotateY(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + right * sin(DEG2RAD(a));
		right = view.cross(up);
		center = eye + view;
	}

	void look() {
		gluLookAt(
			eye.x, eye.y, eye.z,
			center.x, center.y, center.z,
			up.x, up.y, up.z
		);
	}

	void topView() {
		eye = Vector3f(0.0f, 15.0f, 0.0f);     // Camera directly above the player and walls
		center = Vector3f(playerX, playerY, playerZ);  // Looking at the player.
		up = Vector3f(0.0f, 0.0f, -1.0f);     // 'Up' direction for top view is the negative Z-axis
	}

	void sideView() {
		eye = Vector3f(playerX - 3.0f, playerY + 0.75, playerZ);  // Camera positioned to the left or right
		center = Vector3f(playerX, playerY + 0.75, playerZ);  // Looking at the player.
		up = Vector3f(0.0f, 1.0f, 0.0f);      // Standard 'up' direction
	}

	void frontView() {
		eye = Vector3f(playerX, playerY + 0.75, playerZ + 3.0f);  // Camera placed in front of the player
		center = Vector3f(playerX, playerY + 0.75, playerZ);  // Looking directly at the player
		up = Vector3f(0.0f, 1.0f, 0.0f);      // Standard 'up' direction
	}

	void backView() {
		eye = Vector3f(playerX, playerY + 0.75, playerZ - 3.0f);  // Camera placed behind the player
		center = Vector3f(playerX, playerY + 0.75, playerZ);      // Looking directly at the player
		up = Vector3f(0.0f, 1.0f, 0.0f);                         // Standard 'up' direction
	}


};

Camera camera;

void mouseMotion(int x, int y) {
	// Map x and y to angles (adjust sensitivity as needed)
	playerLookX = (320 - x) * sensitivity; // assuming window width of 640
	playerLookY = (y - 240) * sensitivity; // assuming window height of 480

	playerLookY = std::min(std::max(playerLookY, -45.0f), 45.0f);  // Limits to -45 to 45 degrees


	// Ensure the display is updated
	glutPostRedisplay();
}
void drawMedal() {
	glColor3f(1.0f, 0.84f, 0.0f); // Gold color for the medal

	// Medal (Circle)
	glPushMatrix();
	glTranslatef(0.0f, -0.5f, 0.0f); // Position the medal below the ribbon
	GLUquadric* quad = gluNewQuadric();
	gluDisk(quad, 0.0, 1.0, 50, 1); // Radius 1.0
	glPopMatrix();

	// Outer border of the medal (slightly larger circle)
	glPushMatrix();
	glTranslatef(0.0f, -0.5f, 0.0f);
	glColor3f(0.8f, 0.8f, 0.8f); // Silver color for border
	gluDisk(quad, 1.0, 1.1, 50, 1); // Thin outer border
	glPopMatrix();

	// Ribbon (Three stripes: Red, White, and Blue)
	glPushMatrix();
	// Red Stripe
	glColor3f(1.0f, 0.0f, 0.0f); // Red
	glBegin(GL_QUADS);
	glVertex3f(-0.4f, 1.0f, 0.0f); // Wider top
	glVertex3f(-0.2f, -0.5f, 0.0f); // Bottom left
	glVertex3f(0.2f, -0.5f, 0.0f);  // Bottom right
	glVertex3f(0.4f, 1.0f, 0.0f);  // Wider top
	glEnd();

	// White Stripe
	glColor3f(1.0f, 1.0f, 1.0f); // White
	glBegin(GL_QUADS);
	glVertex3f(-0.3f, 1.0f, 0.01f); // Wider top
	glVertex3f(-0.1f, -0.5f, 0.01f); // Bottom left
	glVertex3f(0.1f, -0.5f, 0.01f);  // Bottom right
	glVertex3f(0.3f, 1.0f, 0.01f);  // Wider top
	glEnd();

	// Blue Stripe
	glColor3f(0.0f, 0.0f, 1.0f); // Blue
	glBegin(GL_QUADS);
	glVertex3f(-0.2f, 1.0f, 0.02f); // Wider top
	glVertex3f(-0.05f, -0.5f, 0.02f); // Bottom left
	glVertex3f(0.05f, -0.5f, 0.02f);  // Bottom right
	glVertex3f(0.2f, 1.0f, 0.02f);  // Wider top
	glEnd();
	glPopMatrix();
}



void drawWhistle() {
	glColor3f(0.8f, 0.8f, 0.8f); // Gray color

	// Main body of the whistle (cylinder)
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -0.3f); // Position the body
	glRotatef(90, 1.0f, 0.0f, 0.0f); // Rotate cylinder to align along x-axis
	GLUquadric* quad = gluNewQuadric();
	gluCylinder(quad, 0.5f, 0.5f, 1.0f, 32, 32); // Radius 0.5, height 1.0
	gluDisk(quad, 0.0, 0.5, 32, 32); // Cover the back of the cylinder
	glPopMatrix();

	// Add a rectangular mouthpiece (cube)
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.6f); // Position the mouthpiece
	glScalef(0.4f, 0.2f, 0.6f); // Scale it to form a rectangle
	glutSolidCube(1.0f); // Draw the mouthpiece
	glPopMatrix();

	// Add rounded cap (sphere for the back curve)
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -0.3f); // Position at the back of the whistle
	glutSolidSphere(0.5f, 32, 32); // Create a sphere for rounded edges
	glPopMatrix();

	// Add a hole (rectangle cutout)
	glPushMatrix();
	glTranslatef(0.0f, 0.1f, 0.2f); // Position the cutout
	glScalef(0.25f, 0.01f, 0.4f); // Thin rectangle to simulate a hole
	glutSolidCube(1.0f);
	glPopMatrix();

	// Add a loop (for the string attachment, small torus)
	glPushMatrix();
	glTranslatef(-0.4f, 0.0f, -0.3f); // Position the loop
	glRotatef(90, 1.0f, 0.0f, 0.0f); // Rotate to align
	glutSolidTorus(0.05, 0.15, 32, 32); // Inner radius 0.05, outer radius 0.15
	glPopMatrix();
}


// Function to draw a single shelf
void drawShelf(float width, float height, float depth) {
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f); // White color for the shelf
	glScalef(width, height, depth); // Scale the cube to form a shelf
	glutSolidCube(1.0f); // Draw the scaled cube
	glPopMatrix();
}


// Function to draw a cylinder
void drawCylinder(float radius, float height) {
	GLUquadric* quadric = gluNewQuadric();
	gluCylinder(quadric, radius, radius, height, 32, 32);
	gluDisk(quadric, 0, radius, 32, 1); // Base cap
	glPushMatrix();
	glTranslatef(0, 0, height);
	gluDisk(quadric, 0, radius, 32, 1); // Top cap
	glPopMatrix();
	gluDeleteQuadric(quadric);
}

// Function to draw a trophy
void drawTrophy() {
	glColor3f(1.0f, 0.84f, 0.0f); // Gold color

	// Draw the base
	glPushMatrix();
	glScalef(1.0f, 0.5f, 1.0f);
	glutSolidCube(1.0);
	glPopMatrix();

	// Draw the stem
	glPushMatrix();
	glTranslatef(0.0f, 1.0f, 0.0f);
	glRotatef(90, 1, 0, 0);
	drawCylinder(0.1f, 1.0f);
	glPopMatrix();

	// Draw the cup
	glPushMatrix();
	glTranslatef(0.0f, 1.5f, 0.0f);
	glutSolidSphere(0.5f, 32, 32); // Top rounded part
	glPopMatrix();

	// Draw the handles
	glPushMatrix();
	glTranslatef(0.4f, 1.5f, 0.0f);
	glRotatef(90, 0, 0, 1);
	glutSolidTorus(0.05, 0.3, 32, 32); // Handle
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.4f, 1.5f, 0.0f);
	glRotatef(90, 0, 0, 1);
	glutSolidTorus(0.05, 0.3, 32, 32); // Handle
	glPopMatrix();
}

void updateTrophyRotation() {
	// Increment the rotation angle
	trophyRotation += 2.0f; // Adjust speed of rotation here
	if (trophyRotation >= 360.0f) {
		trophyRotation -= 360.0f; // Keep it within 0-360 range
	}
}

// Function to draw a single ring
void drawRing(float centerX, float centerY, float radius, float thickness, float r, float g, float b) {
	glColor3f(r, g, b); // Set the ring color

	// Draw outer circle
	GLUquadric* quadric = gluNewQuadric();
	glPushMatrix();
	glTranslatef(centerX, centerY, 0.0f);
	gluDisk(quadric, radius - thickness, radius, 50, 1);
	glPopMatrix();

	gluDeleteQuadric(quadric);
}

// Function to draw the Olympic rings
void drawOlympicRings() {
	float radius = 1.0f;          // Radius of each ring
	float thickness = 0.1f;       // Thickness of the rings
	float spacing = 1.2f;         // Spacing between ring centers

	// Top row of rings
	drawRing(-spacing, 0.5f, radius, thickness, 0.0f, 0.0f, 1.0f); // Blue
	drawRing(0.0f, 0.5f, radius, thickness, 0.0f, 0.0f, 0.0f);     // Black
	drawRing(spacing, 0.5f, radius, thickness, 1.0f, 0.0f, 0.0f);  // Red

	// Bottom row of rings
	drawRing(-spacing / 2.0f, -0.5f, radius, thickness, 1.0f, 1.0f, 0.0f); // Yellow
	drawRing(spacing / 2.0f, -0.5f, radius, thickness, 0.0f, 1.0f, 0.0f);  // Green
}

// **M.A** on the Jersey (Blocky "M" and "A" using cubes)
void drawMA() {
	glColor3f(1.0f, 1.0f, 1.0f); // White color for the letters

	// **Letter M**
	// Left vertical bar of M
	// First digit
	glPushMatrix();
	glTranslatef(-0.32f, 0.67f, 0.02f); // Slightly to the left
	glScalef(0.03f, 0.3f, 0.1f);      // Scale for a vertical rectangle
	glutSolidCube(1.0);               // Vertical block for "1"
	glPopMatrix();

	glPushMatrix();
	glRotatef(45, 0, 0, 1);
	glTranslatef(0.35f, 0.7f, 0.02f);
	glScalef(0.03f, 0.2f, 0.1f);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glRotatef(-45, 0, 0, 1);
	glTranslatef(-0.59f, 0.47f, 0.02f);
	glScalef(0.03f, 0.2f, 0.1f);
	glutSolidCube(1);
	glPopMatrix();


	// Second digit
	glPushMatrix();
	glTranslatef(-0.01f, 0.68f, 0.02f); // Slightly to the right
	glScalef(0.03f, 0.3f, 0.1f);     // Scale for a vertical rectangle
	glutSolidCube(1.0);              // Vertical block for "1"
	glPopMatrix();


	//// **Letter A**
	//// Left vertical bar of A
	glPushMatrix();
	glTranslatef(0.125f, 0.7f, 0.02f);  // Position for the left vertical part of A
	glRotatef(-15, 0, 0, 1);
	glScalef(0.03f, 0.3f, 0.1f);      // Scale to make it a vertical block
	glutSolidCube(1.0f);              // Create the vertical block for "A"
	glPopMatrix();

	//// Right vertical bar of A
	glPushMatrix();
	glTranslatef(0.2f, 0.7f, 0.02f);  // Position for the right vertical part of A
	glRotatef(15, 0, 0, 1);
	glScalef(0.03f, 0.3f, 0.1f);      // Scale to make it a vertical block
	glutSolidCube(1.0f);              // Create the vertical block for "A"
	glPopMatrix();

	//// Horizontal bar of A (middle horizontal line)
	glPushMatrix();
	glTranslatef(0.15f, 0.65f, 0.02f); // Position for the horizontal part of A
	glRotatef(90, 0, 0, 1);
	glScalef(0.03f, 0.1f, 0.1f);       // Scale to make it a horizontal block
	glutSolidCube(1.0f);               // Create the horizontal block for "A"
	glPopMatrix();
}

void drawRealisticBasketballJersey() {
	// **1. Main Body (Smooth Body Shape Using Cuboids)**
	glColor3f(0.8f, 0.0f, 0.0f); // Red (Jersey Color)

	// Bottom Rectangular Section (Wider)
	glPushMatrix();
	glTranslatef(0.0f, -0.4f, 0.0f); // Position lower half
	glScalef(1.0f, 0.8f, 0.05f);      // Scale for width, height, and depth
	glutSolidCube(1.0);              // Base cube
	glPopMatrix();

	// Top Tapered Section (Shoulders)
	glPushMatrix();
	glTranslatef(0.0f, 0.4f, 0.0f);  // Position upper half
	glScalef(0.8f, 1.0f, 0.05f);      // Slightly narrower for shoulders
	glutSolidCube(1.0);              // Scaled cube for the top
	glPopMatrix();

	drawMA();
	// **4. Number "11" (Vertical Rectangles for the Digits)**
	glColor3f(1.0f, 1.0f, 1.0f); // White
	// First digit
	glPushMatrix();
	glTranslatef(-0.1f, 0.0f, 0.02f); // Slightly to the left
	glScalef(0.05f, 0.4f, 0.1f);      // Scale for a vertical rectangle
	glutSolidCube(1.0);               // Vertical block for "1"
	glPopMatrix();

	// Second digit
	glPushMatrix();
	glTranslatef(0.1f, 0.0f, 0.02f); // Slightly to the right
	glScalef(0.05f, 0.4f, 0.1f);     // Scale for a vertical rectangle
	glutSolidCube(1.0);              // Vertical block for "1"
	glPopMatrix();
}


void drawBasketballRing() {

	float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f; // Current time in seconds

	// Check if 5 seconds have passed since the last color change
	if (currentTime - backBoardAnimationTime >= 3.0f) {
		isBackboardWhite = !isBackboardWhite; // Toggle the color
		backBoardAnimationTime = currentTime;   // Update the last change time
	}

	// **1. Hoop (Ring)**
	glColor3f(1.0f, 0.5f, 0.0f); // Orange
	glPushMatrix();
	glRotatef(90, 1, 0, 0); // Align ring horizontally
	glTranslatef(0.0f, -0.05f, 0.0f);
	glutSolidTorus(0.02, 0.3, 20, 20); // Torus: inner radius 0.02, outer radius 0.3
	glPopMatrix();

	// **2. Backboard**
	if (isAnimating) {
		if (isBackboardWhite) {
			glColor3f(1.0f, 1.0f, 1.0f); // White
		}
		else {
			glColor3f(0.0f, 0.0f, 0.0f); // Black
		}
	}
	else {
		glColor3f(1.0f, 1.0f, 1.0f);
	}
	glPushMatrix();
	glTranslatef(0.0f, 0.15f, -0.4f); // Move slightly above and behind the hoop
	glScalef(1.0f, 1.0f, 0.05f); // Scale for backboard dimensions
	glutSolidCube(1.0); // Cube for backboard
	glPopMatrix();

	// **3. Vertical Support**
	glColor3f(1.0f, 1.0f, 1.0f); // White
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -0.35f); // Position below the hoop
	glRotatef(90, 1, 0, 0); // Align vertically
	gluCylinder(gluNewQuadric(), 0.02, 0.02, 0.5, 20, 20); // Cylinder: radius 0.02, height 0.5
	glPopMatrix();

	// **4. Horizontal Support**
	glColor3f(1.0f, 1.0f, 1.0f); // White
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.25f); // Position behind the hoop
	glRotatef(90, 1, 0, 0); // Align horizontally
	//glRotatef(45, 0, 0, 1); // Align horizontally
	gluCylinder(gluNewQuadric(), 0.02, 0.02, 0.5, 20, 20); // Cylinder: radius 0.02, length 0.4
	glPopMatrix();

	// **5. Diagonal Support**
	glColor3f(1.0f, 1.0f, 1.0f); // White
	glPushMatrix();
	glTranslatef(0.30f, 0.0f, -0.05f); // Position diagonally below the hoop
	glRotatef(90, 1, 0, 0); // Rotate to create a diagonal bar
	gluCylinder(gluNewQuadric(), 0.02, 0.02, 0.5, 20, 20); // Cylinder: radius 0.02, length 0.4
	glPopMatrix();

	// **5. Diagonal Support**
	glColor3f(1.0f, 1.0f, 1.0f); // White
	glPushMatrix();
	glTranslatef(-0.30f, 0.0f, -0.05f); // Position diagonally below the hoop
	glRotatef(90, 1, 0, 0); // Rotate to create a diagonal bar
	gluCylinder(gluNewQuadric(), 0.02, 0.02, 0.5, 20, 20); // Cylinder: radius 0.02, length 0.4
	glPopMatrix();
}

void drawWall(double width, double height, double depth) {
	glPushMatrix();
	glScaled(width, height, depth);
	glutSolidCube(1);
	glPopMatrix();
}

void drawSecondWall(double width, double height, double depth) {
	glPushMatrix();
	glScaled(width, height, depth);
	glutSolidCube(1);
	glPopMatrix();
}

void drawWalls() {
	glColor3f(0.75f, 0.60f, 0.35f);  // Slightly darker wood tone

	// Floor
	glPushMatrix();
	glTranslated(0.0, 0, 0.0);   // Position floor at the bottom
	drawWall(20.0, 0.0, 20.0);      // Large floor area
	glPopMatrix();

	// Back Wall

	float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f; // Current time in seconds

	// Check if 5 seconds have passed since the last color change
	if (currentTime - lastWallColorChange >= 5.0f) {
		isWallGrey = !isWallGrey; // Toggle the color
		lastWallColorChange = currentTime;   // Update the last change time
	}
	if (isWallGrey) {
		glColor3f(0.5f, 0.5f, 0.5f);
	}
	else {
		glColor3f(0.53f, 0.81f, 0.92f); // Sky Blue
	}
	glPushMatrix();
	glTranslated(0.0, 5, -10.0);   // Position back wall
	drawWall(20.0, 10.0, 0.1);       // Wide and tall back wall

	glPushMatrix();
	glColor3f(0.2f, 0.0f, 0.0f);
	glTranslated(0.0, -5.0, 0.5);
	drawSecondWall(20.0, 2.0, 1.0);
	glPopMatrix();

	//Back Wall Jersey
	glPushMatrix();
	if (isAnimating) {
		// Calculate the vertical offset for the bounce
		float verticalOffset = bounceHeight * sin(jerseyAnimationTime);
		glTranslatef(0.0f, verticalOffset, 0.0f); // Apply the bounce offset
	}
	else {
		glTranslatef(0.0f, 0.0f, 0.0f); // Apply the bounce offset
	}
	glTranslatef(-2.0f, -1.5f, 0.5f); // Adjust the position to display the jersey
	drawRealisticBasketballJersey();
	glPopMatrix();
	// Increment time for animation
	jerseyAnimationTime += bounceSpeed * 0.01f;

	// Back wall basketBall ring
	glPushMatrix();
	glTranslatef(0.0f, -1.5f, 0.5f); // Position for the back wall
	drawBasketballRing();
	glPopMatrix();

	// Add Olympic Rings
	// Calculate scale factor based on a sine wave
	glPushMatrix();
	if (isAnimating) {
		float scaleFactor = 0.8f + 0.2f * sin(ringsAnimationTime);
		glScalef(scaleFactor, scaleFactor, scaleFactor);
	}
	else {
		glScalef(1.0f, 1.0f, 1.0f);
	}
	// Apply scaling transformation
	glTranslatef(0.0f, 3.0f, 0.5f); // Position the rings close to the wall
	drawOlympicRings();
	glPopMatrix();
	// Increment time for animation
	ringsAnimationTime += scaleSpeed * 0.01f;

	glPopMatrix();

	if (isWallGrey) {
		glColor3f(0.5f, 0.5f, 0.5f);
	}
	else {
		glColor3f(0.53f, 0.81f, 0.92f); // Sky Blue
	}
	// Left Wall
	glPushMatrix();
	glTranslated(-10.0, 5, 0.0);   // Position left wall
	drawWall(0.1, 10.0, 20.0);       // Tall and long left wall

	// Draw a single shelf on the Left wall under the trophy
	glPushMatrix();
	glTranslatef(0.5f, 0.2f, -4.9f); // Position shelf on the right wall under the trophy
	glRotatef(90, 0.0f, 1.0f, 0.0f); // Rotate shelf to align with the right wall
	drawShelf(2.0f, 0.1f, 1.0f); // Dimensions: width = 2.0, height = 0.1, depth = 0.5
	glPopMatrix();

	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	glScaled(0.4, 0.4, 0.5);
	glTranslated(13, 2.5, 1.3);
	if (isAnimating) {
		updateTrophyRotation();
		glRotatef(trophyRotation, 0.0f, 1.0f, .0f);
	}
	drawMedal();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.2f, 0.0f, 0.0f);
	glTranslated(0.5, -5.0, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawSecondWall(20.0, 2.0, 1.0);
	glPopMatrix();

	// Left wall ring
	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	glTranslatef(0.0f, -1.5f, 0.5f); // Adjust height for the ring
	drawBasketballRing();
	glPopMatrix();

	glPopMatrix();

	if (isWallGrey) {
		glColor3f(0.5f, 0.5f, 0.5f);
	}
	else {
		glColor3f(0.53f, 0.81f, 0.92f); // Sky Blue
	}
	// Right Wall
	glPushMatrix();
	glTranslated(10.0, 5, 0.0);    // Position right wall
	drawWall(0.1, 10.0, 20.0);       // Tall and long right wall

	glPushMatrix();
	glColor3f(0.2f, 0.0f, 0.0f);
	glTranslated(-0.5, -5.0, 0.0);
	glRotatef(-90, 0.0, 1.0, 0.0);
	drawSecondWall(20.0, 2.0, 1.0);
	glPopMatrix();

	//Trophy
	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	glTranslatef(5.0f, 0.0f, -0.6f);
	glScalef(0.6f, 0.6f, 0.6f);
	if (isAnimating) {
		updateTrophyRotation();
		glRotatef(trophyRotation, 0.0f, 1.0f, .0f);
	}
	drawTrophy();
	glPopMatrix();

	// Draw a single shelf on the right wall under the trophy
	glPushMatrix();
	glTranslatef(-0.5f, -0.2f, -4.9f); // Position shelf on the right wall under the trophy
	glRotatef(-90, 0.0f, 1.0f, 0.0f); // Rotate shelf to align with the right wall
	drawShelf(2.0f, 0.1f, 1.0f); // Dimensions: width = 2.0, height = 0.1, depth = 0.5
	glPopMatrix();

	// Right wall ring
	glPushMatrix();
	glRotatef(-90, 0, 1, 0);
	glTranslatef(0.0f, -1.5f, 0.5f); // Adjust height for the ring
	drawBasketballRing();
	glPopMatrix();


	glPopMatrix();

	if (isWallGrey) {
		glColor3f(0.5f, 0.5f, 0.5f);
	}
	else {
		glColor3f(0.53f, 0.81f, 0.92f); // Sky Blue
	}

	// Front Wall
	glPushMatrix();
	glTranslated(0.0, 5, 10.0);    // Position front wall opposite to the back wall
	drawWall(20.0, 10.0, 0.1);       // Wide and tall front wall

	glPushMatrix();
	glColor3f(0.2f, 0.0f, 0.0f);
	glTranslated(0.0, -5.0, -0.5);
	drawSecondWall(20.0, 2.0, 1.0);
	glPopMatrix();

	// Front wall ring
	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	glTranslatef(0.0f, -1.5f, 0.5f); // Adjust height for the ring
	drawBasketballRing();
	glPopMatrix();

	glPopMatrix();
}

void drawCube(float width, float height, float depth) {
	glPushMatrix();
	glScalef(width, height, depth);
	glutSolidCube(1.0);
	glPopMatrix();
}

void drawFaceFeatures() {
	// Draw eyes
	glPushMatrix();
	glColor3f(0, 0, 0); // Black color for eyes

	// Left eye
	glPushMatrix();
	glTranslatef(-0.05f, 0.05f, 0.12f); // Adjust positioning for left eye relative to head
	glutSolidSphere(0.03, 10, 10); // Small sphere for the left eye
	glPopMatrix();

	// Right eye
	glPushMatrix();
	glTranslatef(0.05f, 0.05f, 0.12f); // Adjust positioning for right eye relative to head
	glutSolidSphere(0.03, 10, 10); // Small sphere for the right eye
	glPopMatrix();

	glPopMatrix();

	// Draw nose
	glPushMatrix();
	glColor3f(0.8f, 0.6f, 0.4f); // Skin tone color for the nose
	glTranslatef(0.0f, 0.0f, 0.12f); // Adjust position for nose in the middle of the head
	glutSolidSphere(0.02, 10, 10); // Small sphere for the nose
	glPopMatrix();

	// Draw mouth
	glPushMatrix();
	glColor3f(1.0f, 0.0f, 0.0f); // Red color for the mouth
	glTranslatef(0.0f, -0.05f, 0.102f); // Position slightly below the nose

	// Draw a small semi-circle or line for a smile
	glBegin(GL_LINE_STRIP);
	for (float angle = 0; angle <= M_PI; angle += 0.1) {
		float x = 0.05f * cos(angle);
		float y = 0.02f * sin(angle);
		glVertex3f(x, y, 0.0f);
	}
	glEnd();

	glPopMatrix();
	glFlush();
}

void updatePlayerBoundingBox(float playerX, float playerY, float playerZ,
	float playerWidth, float playerHeight, float playerDepth,
	float& minX, float& maxX, float& minY, float& maxY, float& minZ, float& maxZ) {
	// Calculate the bounds based on the player's position and size
	minX = playerX - playerWidth / 2;
	maxX = playerX + playerWidth / 2;
	minY = playerY - playerHeight / 2;
	maxY = playerY + playerHeight / 2;
	minZ = playerZ - playerDepth / 2;
	maxZ = playerZ + playerDepth / 2;
}

void drawPlayer() {

	glColor3f(0.5f, 0.5f, 0.5f);

	glPushMatrix();
	glRotatef(playerYaw, 0, 1, 0);  // Rotate left-right on Y-axis

	// Head
	glPushMatrix();
	glTranslatef(0.0f, 0.9f, 0.0f);


	// Apply the rotation to the head based on mouse movement
	glRotatef(playerLookX, 0, 1, 0);  // Rotate left-right on Y axis
	glRotatef(playerLookY, 1, 0, 0);  // Rotate up-down on X axis


	drawCube(0.2f, 0.2f, 0.2f);
	drawFaceFeatures();
	glPopMatrix();

	glColor3f(0.5f, 0.5f, 0.5f);

	// Body
	glPushMatrix();
	glTranslatef(0.0f, 0.5f, 0.0f);
	drawCube(0.3f, 0.5f, 0.15f);
	glPopMatrix();

	// Left Arm
	glPushMatrix();
	glTranslatef(-0.20f, 0.6f, 0.0f);
	glRotated(-45, 0, 0, 1);
	drawCube(0.1f, 0.3f, 0.1f);
	glPopMatrix();

	// Right Arm
	glPushMatrix();
	glTranslatef(0.20f, 0.6f, 0.0f);
	glRotated(45, 0, 0, 1);
	drawCube(0.1f, 0.3f, 0.1f);
	glPopMatrix();

	// Left Leg
	glPushMatrix();
	glTranslatef(-0.1f, 0.0f, 0.0f);
	drawCube(0.1f, 0.5f, 0.1f);
	glPopMatrix();

	// Right Leg
	glPushMatrix();
	glTranslatef(0.1f, 0.0f, 0.0f);
	drawCube(0.1f, 0.5f, 0.1f);
	glPopMatrix();

	glPopMatrix();
}



void drawBasketball(const Basketball& ball) {
	if (ball.isCollected) {
		// If the ball is collected, don't draw it
		return;
	}

	// Set color to orange for the basketball
	glColor3f(1.0f, 0.5f, 0.0f);

	// Push the current matrix for transformation
	glPushMatrix();

	// Translate to the specified position
	glTranslatef(ball.posX, ball.posY, ball.posZ);


	// Apply rotation around the Y-axis (or adjust axis as needed)
	glRotatef(ballRotationAngle, 0.0f, 1.0f, 0.0f);

	// Main body of the basketball (large sphere)
	glutSolidSphere(0.19, 20, 20); // Radius 0.2, 20 slices, 20 stacks for detail
	glColor3f(0.0f, 0.0f, 0.0f);

	// Draw the horizontal seam as a thin ring
	glColor3f(0, 0, 0); // Black color for seams
	glRotatef(90, 1, 0, 0); // Rotate for horizontal seam
	GLUquadric* quadric = gluNewQuadric();
	gluDisk(quadric, 0.18, 0.2, 20, 1); // Thin disk for the seam
	gluDeleteQuadric(quadric);

	// Vertical seam (front to back line)
	glPushMatrix();
	glRotatef(90, 0, 1, 0); // Rotate for vertical seam
	quadric = gluNewQuadric();
	gluDisk(quadric, 0.18, 0.2, 20, 1);
	gluDeleteQuadric(quadric);
	glPopMatrix();

	// Diagonal seam 1
	glPushMatrix();
	glRotatef(45, 0, 0, 1); // Diagonal rotation
	glRotatef(90, 0, 1, 0); // Front-facing
	quadric = gluNewQuadric();
	gluDisk(quadric, 0.18, 0.2, 20, 1);
	gluDeleteQuadric(quadric);
	glPopMatrix();

	// Diagonal seam 2
	glPushMatrix();
	glRotatef(-45, 0, 0, 1); // Opposite diagonal rotation
	glRotatef(90, 0, 1, 0); // Front-facing
	quadric = gluNewQuadric();
	gluDisk(quadric, 0.18, 0.2, 20, 1);
	gluDeleteQuadric(quadric);
	glPopMatrix();

	// Pop the transformation matrix
	glPopMatrix();
}

void updateBallRotation() {
	// Increment the rotation angle
	ballRotationAngle += 2.0f; // Adjust speed of rotation here
	if (ballRotationAngle >= 360.0f) {
		ballRotationAngle -= 360.0f; // Keep it within 0-360 range
	}
}


// Function to calculate the bounding box of the basketball
void calculateBasketballBoundingBox(const Basketball& ball, float& minX, float& maxX, float& minY, float& maxY, float& minZ, float& maxZ) {
	// Calculate the bounds based on the basketball's position and radius
	minX = ball.posX - ball.radius;
	maxX = ball.posX + ball.radius;

	minY = ball.posY - ball.radius;
	maxY = ball.posY + ball.radius;

	minZ = ball.posZ - ball.radius;
	maxZ = ball.posZ + ball.radius;
}


void setupLights() {
	// Set the light position
	GLfloat lightPos[] = { 5.0f, 5.0f, 5.0f, 1.0f }; // Position in the scene
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	// Set ambient light (soft background lighting)
	GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);

	// Set diffuse light (directional light for the object)
	GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);

	// Set specular light (highlights on shiny surfaces)
	GLfloat specularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
}

void setupMaterial() {
	// Set material properties
	GLfloat matAmbient[] = { 0.3f, 0.3f, 0.3f, 1.0f }; // Material ambient reflectivity
	GLfloat matDiffuse[] = { 0.7f, 0.0f, 0.0f, 1.0f }; // Material diffuse reflectivity
	GLfloat matSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // Material specular reflectivity
	GLfloat matShininess[] = { 50.0f };                 // Shininess coefficient

	glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
}


void setupCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, 640 / 480, 0.1, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (!isCameraInitialized) {
		camera.frontView(); // Set the initial view to the front view
		isCameraInitialized = true; // Prevent resetting the view in subsequent calls
	}

	// Update camera based on current player position
	camera.playerX = playerX;  // Player's current X position
	camera.playerY = playerY;  // Player's current Y position
	camera.playerZ = playerZ;  // Player's current Z position
	camera.look();
}

void Keyboard(unsigned char key, int x, int y) {
	float d = 0.1;

	switch (key) {
	case 'w':  // Move forward
		if (playerZ - d >= minZ) {
			playerZ -= d;  // Move along the Z axis (forward) if within bounds
			playerYaw = 0.0f;
		}
		break;
	case 's':  // Move backward
		if (playerZ + d <= maxZ) {
			playerZ += d;  // Move along the Z axis (backward) if within bounds
			playerYaw = 180.0f;  // Face backward (adjust yaw accordingly)

		}
		break;
	case 'a':  // Move left
		if (playerX - d >= minX) {
			playerX -= d;  // Move along the X axis (left) if within bounds
			playerYaw = 90.0f;  // Face left (adjust yaw accordingly)

		}
		break;
	case 'd':  // Move right
		if (playerX + d <= maxX) {
			playerX += d;  // Move along the X axis (right) if within bounds
			playerYaw = -90.0f;  // Face right (adjust yaw accordingly)

		}
		break;
	case 'm':
		if (isAnimating) {
			isAnimating = false;
			jerseyAnimationTime = 0.0f;
			ringsAnimationTime = 0.0f;
			trophyRotation = 0.0f;
		}
		else {
			isAnimating = true;
		}
		break;
	case 'z':
		camera.moveY(d);
		break;
	case 'x':
		camera.moveY(-d);
		break;
	case 'c':
		camera.moveX(d);
		break;
	case 'v':
		camera.moveX(-d);
		break;
	case 'b':
		camera.moveZ(d);
		break;
	case 'n':
		camera.moveZ(-d);
		break;
	case '1':  // Top View
		camera.topView();
		break;
	case '2':  // Side View
		camera.sideView();
		break;
	case '3':  // Front View
		camera.frontView();
		break;
	case '4':
		camera.backView();
		break;
	case GLUT_KEY_ESCAPE:
		exit(EXIT_SUCCESS);
	}

	glutPostRedisplay();
}

void Special(int key, int x, int y) {
	float a = 1.0;

	switch (key) {
	case GLUT_KEY_UP:
		camera.rotateX(a);
		break;
	case GLUT_KEY_DOWN:
		camera.rotateX(-a);
		break;
	case GLUT_KEY_LEFT:
		camera.rotateY(a);
		break;
	case GLUT_KEY_RIGHT:
		camera.rotateY(-a);
		break;
	}

	glutPostRedisplay();
}

bool checkBallCollision(float playerMinX, float playerMaxX, float playerMinY, float playerMaxY, float playerMinZ, float playerMaxZ,
	float ballMinX, float ballMaxX, float ballMinY, float ballMaxY, float ballMinZ, float ballMaxZ) {
	return (playerMaxX > ballMinX && playerMinX < ballMaxX &&
		playerMaxY > ballMinY && playerMinY < ballMaxY &&
		playerMaxZ > ballMinZ && playerMinZ < ballMaxZ);
}

void collectBall(Basketball& ball) {
	float ballMinX, ballMaxX, ballMinY, ballMaxY, ballMinZ, ballMaxZ;
	calculateBasketballBoundingBox(ball, ballMinX, ballMaxX, ballMinY, ballMaxY, ballMinZ, ballMaxZ);

	if (checkBallCollision(playerMinX, playerMaxX, playerMinY, playerMaxY, playerMinZ, playerMaxZ,
		ballMinX, ballMaxX, ballMinY, ballMaxY, ballMinZ, ballMaxZ)) {
		// Update the basketball's isCollected flag to true
		if (!ball.isCollected) {
			ball.isCollected = true;
			playerScore += 100; // Increment the score
			collectedBalls += 1;
		}
	}
}

void setupOrthoProjection() {
	// Save the current projection matrix
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	// Set up orthographic projection (2D plane)
	gluOrtho2D(0, 640, 0, 480); // Match your window size (width x height)

	// Switch to the modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

void restoreProjection() {
	// Restore the modelview matrix
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);

	// Restore the projection matrix
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void renderText(float x, float y, const char* text, void* font = GLUT_BITMAP_HELVETICA_18) {
	glRasterPos2f(x, y);
	for (const char* c = text; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c); // Render each character
	}
}


void renderScoreAndTime() {
	// Switch to orthographic projection for 2D text rendering
	setupOrthoProjection();

	// Render the score in the upper-left corner
	glColor3f(0.0f, 0.0f, 0.0f); // Set text color
	char scoreText[50];
	sprintf(scoreText, "Score: %d", playerScore);
	renderText(10, 460, scoreText); // Text near the upper-left corner (10px from left, 460px from top)

	// Render the timer in the upper-right corner
	char timerText[50];
	sprintf(timerText, "Time: %d", timerRemaining);
	renderText(550, 460, timerText); // Text near the upper-right corner (550px from left)

	// Restore the 3D perspective projection
	restoreProjection();
}




void Display() {

	if (isGameOver && !GameWon) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		setupOrthoProjection();

		glColor3f(1.0f, 0.0f, 0.0f); // Set text color to red
		char GameOver[50];
		sprintf(GameOver, "YOU LOSE!");
		renderText(280, 240, GameOver, GLUT_BITMAP_TIMES_ROMAN_24); // Text near the upper-left corner (10px from left, 460px from top)
		glFlush();
		restoreProjection();
		return;
	}
	else if (collectedBalls == numOfBalls) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		setupOrthoProjection();
		GameWon = true;
		glColor3f(1.0f, 0.0f, 0.0f); // Set text color to red
		char GameWin[50];
		sprintf(GameWin, "YOU Win!");
		renderText(280, 240, GameWin, GLUT_BITMAP_TIMES_ROMAN_24); // Text near the upper-left corner (10px from left, 460px from top)
		glFlush();
		restoreProjection();
		return;
	}



	setupCamera();
	setupLights();
	//setupMaterial();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Update the player bounding box
	updatePlayerBoundingBox(playerX, playerY, playerZ, playerWidth, playerHeight, playerDepth,
		playerMinX, playerMaxX, playerMinY, playerMaxY, playerMinZ, playerMaxZ);

	updateBallRotation();

	// Check for collisions with each basketball
	for (int i = 0; i < numOfBalls; ++i) {
		collectBall(basketballs[i]); // Check if player collects the basketball
	}

	drawWalls();

	// Draw multiple basketballs, only if they are not collected
	for (int i = 0; i < numOfBalls; ++i) {
		if (!basketballs[i].isCollected) {
			drawBasketball(basketballs[i]); // Draw the basketball if it's not collected
		}
	}

	// Set the player position and draw the player
	glPushMatrix();
	glTranslatef(playerX, playerY, playerZ); // Apply player movement
	glRotatef(180, 0, 1, 0);
	drawPlayer();  // Render player model
	glPopMatrix();

	renderScoreAndTime();

	glFlush();
}




void Animations(int value) {

	glutPostRedisplay();
	glutTimerFunc(16, Animations, 0);  // Adjust 30 for speed
}

void timer(int value) {
	// Update timer
	timerRemaining--; // Decrease timer by approximately 1/60 of a second
	if (timerRemaining <= 0) {
		timerRemaining = 0; // Prevent negative timer
		isGameOver = true;  // Set game over state
	}
	glutTimerFunc(1000, timer, 0);  // Adjust 30 for speed

}

void main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitWindowSize(640, 480);
	glutInitWindowPosition(50, 50);

	glutCreateWindow("Assignment 2");
	glutDisplayFunc(Display);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(Special);
	glutPassiveMotionFunc(mouseMotion); // Register mouse motion function

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	glutTimerFunc(0, Animations, 0);
	glutTimerFunc(0, timer, 0);

	glutMainLoop();
}
