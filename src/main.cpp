//
//  Starter file--version 2--for CS 432, Assignment 4
//
//  Light and material properties are sent to the shader as uniform
//    variables.  Vertex positions and normals are attribute variables.
//  Shininess comes into play, so the side of a cube will not necessarily
//    appear as a uniform color.

#include "cs432.h"
#include "vec.h"
#include "mat.h"
#include "matStack.h"
#include "picking.h"
#include "characters.h"
#include <stdlib.h>
#include <time.h>

#define WIDTH 800
#define HEIGHT 800

// tick: every 50 milliseconds
#define TICK_INTERVAL 50

// typedefs to make code more readable
typedef vec4  color4;
typedef vec4  point4;

// data for the array coordinates of the vertices for our characters
static int charInfo[256][2];

// for spinning the characters
static float letterAngle = 0;

// vertex position and count for our cube
static int CubeStart;
static int CubeNumVertices;

// number of vertices--for now, be very generous so that we don't run out
static const int NumVertices = 100000;

// the properties of our objects
static point4 points[NumVertices];
static vec3 normals[NumVertices];
static color4 colorsDiffuse[NumVertices];
static color4 colorsSpecular[NumVertices];
static color4 colorsAmbient[NumVertices];
static GLfloat objShininess[NumVertices];

// ome color definitions
static color4 RED(1.0, 0.0, 0.0, 1.0);
static color4 GRAY(0.5, 0.5, 0.5, 1.0);
static color4 BLUE(0, 0, 1.0, 1.0);
static color4 GREEN(0.0, 1.0, 0.0, 1.0);
static color4 YELLOW(1.0, 1.0, 0.0, 1.0);
static color4 WHITE(1.0, 1.0, 1.0, 1.0);
static color4 CYAN(0.0, 1.0, 1.0, 1.0);
static color4 BLACK(0.0, 0.0, 0.0, 1.0);
static color4 LIGHTGRAY(0.95, 0.95, 0.95, 1.0);
static color4 VERYLIGHTGRAY(0.975, 0.975, 0.975, 1.0);

// Vertices of a unit cube centered at origin, faces aligned with axes
static point4 vertices[12] = {
    point4(-0.5, -0.5,  0.5, 1.0),
    point4(-0.5,  0.5,  0.5, 1.0),
    point4(0.5,  0.5,  0.5, 1.0),
    point4(0.5, -0.5,  0.5, 1.0),
    point4(-0.5, -0.5, -0.5, 1.0),
    point4(-0.5,  0.5, -0.5, 1.0),
    point4(0.5,  0.5, -0.5, 1.0),
    point4(0.5, -0.5, -0.5, 1.0),
};

// Array of rotation angles (in degrees) for each coordinate axis;
// These are used in rotating the cube.
static int Axis = 0; // 0 => x-rotation, 1=>y, 2=>z, 3=>none
static GLfloat Theta[] = { 0.0, 0.2, 0.723, 0.0 };
static float spinSpeed = 100;

// Variables used for dice rolling and scoring
static bool diceRoll = false;
static int initialHeight = 8;
static int heightTrack = 0;
static bool down = false;
//static bool myTurn = true;
static int myScore = 0;
static int AIScore = 0;
static int diceValue = 0;

static color4 colorToUse = RED;

// Model-view, model-view-start and projection matrices uniform location
static GLuint  ModelView, ModelViewStart, Projection;

// The matrix that defines where the camera is. This can change based on the
// user moving the camera with keyboard input
mat4 model_view_start = LookAt(0, 1, 2.5, 0, 1, -5, 0, 1, 0);

// our matrix-stack
static MatrixStack stack;

// Variables used to control the moving of the light
static bool lightSpin = true; // whether the light is moving
static GLfloat lightAngle = 0.0; // the current lighting angle

// variable used in generating the vetrices for our objects
static int Index[2] = { 0,NumVertices };

//----------------------------------------------------------------------------

// method for placing a charcter at the origin;
// requires that the initialize has been done to send the vertices to the GPU
void drawCharacter(char c) {
    glDrawArrays(GL_TRIANGLES, charInfo[c][0], charInfo[c][1]);
}
//----------------------------------------------------------------------------

// produces a quadralateral given its four corner-points, a, b, c and d.
// - col is the color
// - shininess is the shininess coefficient
//
// Vertex information is sent to the standard array, points, colorsDiffuse, etc.
static void
quad(point4 a, point4 b, point4 c, point4 d, color4 col, GLfloat shininess)
{
    // compute the normal vector for this face by taking a cross product
    vec4 u = b - a;
    vec4 v = c - b;
    vec3 normal = normalize(cross(u, v));

    // create the 6 faces, each with appropriate properties ...

    normals[*Index] = normal; points[*Index] = a; colorsDiffuse[*Index] = col;
    colorsSpecular[*Index] = col; colorsAmbient[*Index] = col; objShininess[*Index] = shininess;
    (*Index)++;

    normals[*Index] = normal; points[*Index] = b; colorsDiffuse[*Index] = col;
    colorsSpecular[*Index] = col; colorsAmbient[*Index] = col; objShininess[*Index] = shininess;
    (*Index)++;

    normals[*Index] = normal; points[*Index] = c; colorsDiffuse[*Index] = col;
    colorsSpecular[*Index] = col; colorsAmbient[*Index] = col; objShininess[*Index] = shininess;
    (*Index)++;

    normals[*Index] = normal; points[*Index] = a; colorsDiffuse[*Index] = col;
    colorsSpecular[*Index] = col; colorsAmbient[*Index] = col; objShininess[*Index] = shininess;
    (*Index)++;

    normals[*Index] = normal; points[*Index] = c; colorsDiffuse[*Index] = col;
    colorsSpecular[*Index] = col; colorsAmbient[*Index] = col; objShininess[*Index] = shininess;
    (*Index)++;

    normals[*Index] = normal; points[*Index] = d; colorsDiffuse[*Index] = col;
    colorsSpecular[*Index] = col; colorsAmbient[*Index] = col; objShininess[*Index] = shininess;
    (*Index)++;
}

// quad generates a square (of our cube) using two triangles
//
// parameters:
// - a, b, c and d: the vertex-numbers for this square
// - col: the color of the square
//   - note: the ambient, specular and diffuse colors are all set to
//     be the same color
// - shininess: the shininess of the square
//
// Vertex information is sent to the standard array, points, colorsDiffuse, etc.
static void
quad(int a, int b, int c, int d, color4 col, GLfloat shininess)
{
    quad(vertices[a], vertices[b], vertices[c], vertices[d], col, shininess);
}

//----------------------------------------------------------------------------
// creates a cube with each face having a different color
static void
colorCube()
{
    quad(1, 0, 3, 2, colorToUse, 30);
    quad(5, 4, 0, 1, colorToUse, 30);
    quad(6, 5, 1, 2, colorToUse, 30);
    quad(3, 0, 4, 7, colorToUse, 30);
    quad(2, 3, 7, 6, colorToUse, 30);
    quad(4, 5, 6, 7, colorToUse, 30);
}

//----------------------------------------------------------------------------

// the GPU light ID, this allows us to change the position of the light during execution
static int lightId;

// send updated light-position information to the GPU
static void updateLightPosition() {
    GLfloat lightX = sin(lightAngle * 0.023);
    GLfloat lightY = sin(lightAngle * 0.031);
    GLfloat lightZ = sin(lightAngle * 0.037);
    vec4 pos(lightX, lightY, lightZ, 0.0);
    glUniform4fv(lightId, 1, pos);

}
//----------------------------------------------------------------------------

// display the scene
static void
display(void)
{
    // set all to background color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // compute the initial model-view matrix based on camera position

    // set up the initial model-view, based on the current camera position/orientation
    mat4 model_view = model_view_start;
    model_view *= Scale(0.2, 0.2, 0.2);

    // update the light position based on the light-rotation information
    updateLightPosition();

    // send uniform matrix variables to the GPU
    glUniformMatrix4fv(ModelViewStart, 1, GL_TRUE, model_view_start);

    // draw the first dice
    stack.push(model_view);
    if (diceRoll) {
        // Whether dice is moving up or down
        if (!down) {
            heightTrack += 1;
        }
        else {
            heightTrack -= 1;
        }
        model_view *= Translate(0, heightTrack, 0);
        // If dice has reached height peak
        if (heightTrack == initialHeight) {
            down = !down;
        }
        // Dice bounce, lower height of next bounce
        else if (heightTrack < 0) {
            down = !down;
            initialHeight = initialHeight - 2;
        }
        // Dice has finished bouncing
        if (initialHeight == 0) {
            initialHeight = 8;
            Sleep(500);
            diceValue = rand() % (6) + 1;   // Since I couldn't figure out how to make dice roll
                                            // visual in time, this just generates a random value from 1 to 6
            // Rolled a 1, end turn
            if (diceValue == 1) {
                //myTurn = !myTurn;
                diceRoll = false;
            }
            // Add to my score if my turn
            //if (myTurn) {
                if (diceValue != 1) {
                    myScore += diceValue;
                }
            //}
            // Add to AI score if AI turn
            /*else {
                if (diceValue != 1) {
                    AIScore += diceValue;
                }
            }*/
            diceRoll = false;
        }
        spinSpeed -= 1;
        
    }
    // AI dice roll
    //if (!myTurn) {
        //diceRoll = true;
    //}
    //model_view *= RotateY(90);
    model_view *= Translate(0, 1, 0);
    model_view *= RotateX(Theta[0]) * RotateY(Theta[1]) * RotateZ(Theta[2]);
    glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);

    setPickId(1); // set pick-id, in case we're picking
    glDrawArrays(GL_TRIANGLES, CubeStart, CubeNumVertices);
    clearPickId(); // clear pick-id

    model_view = stack.pop();

    // draw the second dice
    stack.push(model_view);
    /*if (diceRoll) {
        // Whether dice is moving up or down
        if (!down) {
            heightTrack += 1;
        }
        else {
            heightTrack -= 1;
        }
        model_view *= Translate(0, heightTrack, 0);
        // If dice has reached height peak
        if (heightTrack == initialHeight) {
            down = !down;
        }
        // Dice bounce, lower height of next bounce
        else if (heightTrack < 0) {
            down = !down;
            initialHeight = initialHeight - 2;
        }
        // Dice has finished bouncing
        if (initialHeight == 0) {
            initialHeight = 8;
            Sleep(500);
            diceValue = rand() % (6) + 1;   // Since I couldn't figure out how to make dice roll
                                            // visual in time, this just generates a random value from 1 to 6
            // Rolled a 1, end turn
            if (diceValue == 1) {
                myTurn = !myTurn;
                diceRoll = false;
            }
            // Add to my score if my turn
            if (myTurn) {
                if (diceValue != 1) {
                    myScore += diceValue;
                }
            }
            // Add to AI score if AI turn
            else {
                if (diceValue != 1) {
                    AIScore += diceValue;
                }
            }
            diceRoll = false;
        }
        spinSpeed -= 1;

    }
    // AI dice roll
    if (!myTurn) {
        diceRoll = true;
    }*/
    //model_view *= RotateY(90);
    model_view *= Translate(1.5, 1, 0);
    model_view *= RotateX(Theta[0]) * RotateY(Theta[1]) * RotateZ(Theta[2]);
    //model_view *= Translate(1.5, 1, 0);
    glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);
    setPickId(2); // set pick-id, in case we're picking
    glDrawArrays(GL_TRIANGLES, CubeStart, CubeNumVertices);
    clearPickId(); // clear pick-id
    model_view = stack.pop();

    // draw the third dice
    stack.push(model_view);
    model_view *= Translate(-1.5, 1, 0);
    model_view *= RotateX(Theta[0]) * RotateY(Theta[1]) * RotateZ(Theta[2]);
    //model_view *= Translate(-1.5, 1, 0);
    glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);
    setPickId(3); // set pick-id, in case we're picking
    glDrawArrays(GL_TRIANGLES, CubeStart, CubeNumVertices);
    clearPickId(); // clear pick-id
    model_view = stack.pop();

    // draw the fourth dice
    stack.push(model_view);
    model_view *= Translate(-3, 1, 0);
    model_view *= RotateX(Theta[0]) * RotateY(Theta[1]) * RotateZ(Theta[2]);
    //model_view *= Translate(-3, 1, 0);
    glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);
    setPickId(4); // set pick-id, in case we're picking
    glDrawArrays(GL_TRIANGLES, CubeStart, CubeNumVertices);
    clearPickId(); // clear pick-id
    model_view = stack.pop();

    // draw the fifth dice
    stack.push(model_view);
    model_view *= Translate(3, 1, 0);
    model_view *= RotateX(Theta[0]) * RotateY(Theta[1]) * RotateZ(Theta[2]);
    //model_view *= Translate(3, 1, 0);
    glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);
    setPickId(5); // set pick-id, in case we're picking
    glDrawArrays(GL_TRIANGLES, CubeStart, CubeNumVertices);
    clearPickId(); // clear pick-id
    model_view = stack.pop();

    
    // Y
    stack.push(model_view);
    model_view *= Translate(-8, 12, -2);
    glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);
    drawCharacter('Y');
    model_view = stack.pop();
    // _ (Indicator for my turn)
    //if (myTurn) {
        stack.push(model_view);
        model_view *= Translate(-8, 11, -2);
        glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);
        drawCharacter('_');
        model_view = stack.pop();
    //}
    // o
    stack.push(model_view);
    model_view *= Translate(-7, 12, -2);
    glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);
    drawCharacter('o');
    model_view = stack.pop();
    // u
    stack.push(model_view);
    model_view *= Translate(-5.5, 12, -2);
    glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);
    drawCharacter('u');
    model_view = stack.pop();
    // :
    stack.push(model_view);
    model_view *= Translate(-4.5, 12, -2);
    glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);
    drawCharacter(':');
    model_view = stack.pop();
    // myScore
    if (myScore >= 100) {
        // Display win message
        stack.push(model_view);
        model_view *= Translate(-3.2, 12, -2);
        glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);
        drawCharacter('1');
        model_view = stack.pop();
        // Y
        stack.push(model_view);
        model_view *= Translate(-8, 0, -2);
        glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);
        setPickId(2);
        drawCharacter('Y');
        clearPickId();
        model_view = stack.pop();
        // o
        stack.push(model_view);
        model_view *= Translate(-7, 0, -2);
        glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);
        setPickId(2);
        drawCharacter('o');
        clearPickId();
        model_view = stack.pop();
        // u
        stack.push(model_view);
        model_view *= Translate(-5.5, 0, -2);
        glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);
        setPickId(2);
        drawCharacter('u');
        clearPickId();
        model_view = stack.pop();
        // W
        stack.push(model_view);
        model_view *= Translate(-3, 0, -2);
        glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);
        setPickId(2);
        drawCharacter('W');
        clearPickId();
        model_view = stack.pop();
        // i
        stack.push(model_view);
        model_view *= Translate(-2, 0, -2);
        glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);
        setPickId(2);
        drawCharacter('i');
        clearPickId();
        model_view = stack.pop();
        // n
        stack.push(model_view);
        model_view *= Translate(-1, 0, -2);
        glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);
        setPickId(2);
        drawCharacter('n');
        clearPickId();
        model_view = stack.pop();
        // !
        stack.push(model_view);
        model_view *= Translate(0, 0, -2);
        glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);
        setPickId(2);
        drawCharacter('!');
        clearPickId();
        model_view = stack.pop();
    }
    if (myScore > 9) {
        stack.push(model_view);
        model_view *= Translate(-2.8, 12, -2);
        glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);
        if (myScore >= 10 && myScore <= 19) {
            drawCharacter('1');
        }
        else if (myScore >= 20 && myScore <= 29) {
            drawCharacter('2');
        }
        else if (myScore >= 30 && myScore <= 39) {
            drawCharacter('3');
        }
        else if (myScore >= 40 && myScore <= 49) {
            drawCharacter('4');
        }
        else if (myScore >= 50 && myScore <= 59) {
            drawCharacter('5');
        }
        else if (myScore >= 60 && myScore <= 69) {
            drawCharacter('6');
        }
        else if (myScore >= 70 && myScore <= 79) {
            drawCharacter('7');
        }
        else if (myScore >= 80 && myScore <= 89) {
            drawCharacter('8');
        }
        else if (myScore >= 90 && myScore <= 99) {
            drawCharacter('9');
        }
        model_view = stack.pop();
    }
    stack.push(model_view);
    model_view *= Translate(-1.5, 12, -2);
    glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);
    drawCharacter('0' + (myScore % 10));
    model_view = stack.pop();
    // H
    stack.push(model_view);
    model_view *= Translate(-8, 8, -2);
    glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);
    setPickId(2);
    drawCharacter('H');
    clearPickId();
    model_view = stack.pop();
    // o
    stack.push(model_view);
    model_view *= Translate(-6, 8, -2);
    glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);
    setPickId(2);
    drawCharacter('o');
    clearPickId();
    model_view = stack.pop();
    // l
    stack.push(model_view);
    model_view *= Translate(-5, 8, -2);
    glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);
    setPickId(2);
    drawCharacter('l');
    clearPickId();
    model_view = stack.pop();
    // d
    stack.push(model_view);
    model_view *= Translate(-4, 8, -2);
    glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);
    setPickId(2);
    drawCharacter('d');
    clearPickId();
    model_view = stack.pop();
    // A
    stack.push(model_view);
    model_view *= Translate(1.5, 12, -2);
    glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);
    drawCharacter('A');
    model_view = stack.pop();
    // _ (Indicator for AI turn)
    //if (!myTurn) {
        stack.push(model_view);
        model_view *= Translate(1.5, 11, -2);
        glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);
        drawCharacter('_');
        model_view = stack.pop();
    //}
    // I
    stack.push(model_view);
    model_view *= Translate(3, 12, -2);
    glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);
    drawCharacter('I');
    model_view = stack.pop();
    // :
    stack.push(model_view);
    model_view *= Translate(4, 12, -2);
    glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);
    drawCharacter(':');
    model_view = stack.pop();
    // AIScore
    if (AIScore >= 100) {
        stack.push(model_view);
        model_view *= Translate(6.0, 12, -2);
        glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);
        drawCharacter('1');
        model_view = stack.pop();
    }
    if (AIScore > 9) {
        stack.push(model_view);
        model_view *= Translate(6.5, 12, -2);
        glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);
        if (AIScore >= 10 && AIScore <= 19) {
            drawCharacter('1');
        }
        else if (AIScore >= 20 && AIScore <= 29) {
            drawCharacter('2');
        }
        else if (AIScore >= 30 && AIScore <= 39) {
            drawCharacter('3');
        }
        else if (AIScore >= 40 && AIScore <= 49) {
            drawCharacter('4');
        }
        else if (AIScore >= 50 && AIScore <= 59) {
            drawCharacter('5');
        }
        else if (AIScore >= 60 && AIScore <= 69) {
            drawCharacter('6');
        }
        else if (AIScore >= 70 && AIScore <= 79) {
            drawCharacter('7');
        }
        else if (AIScore >= 80 && AIScore <= 89) {
            drawCharacter('8');
        }
        else if (AIScore >= 90 && AIScore <= 99) {
            drawCharacter('9');
        }
        model_view = stack.pop();
    }
    stack.push(model_view);
    model_view *= Translate(8.0, 12, -2);
    glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);
    drawCharacter('0' + (AIScore % 10));
    model_view = stack.pop();

    // swap buffers (so that just-drawn image is displayed) or perform picking,
    // depending on mode
    if (inPickingMode()) {
        endPicking();
    }
    else {
        glutSwapBuffers();
    }
}

// picking-finished callback: stop rotation if the cube has been selected
void scenePickingFcn(int code) {
    if (code == 1) { // the cube
        diceRoll = true;
    }
    else if (code == 2) {   // Hold
        //myTurn = false;
    }
}

//----------------------------------------------------------------------------

// for now, stop the spinning if the cube is clicked
static void
mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // perform a "pick", including any associated action
        startPicking(scenePickingFcn, x, y);
    }

}

//----------------------------------------------------------------------------

// timer function, called when the timer has ticked
static void
tick(int n)
{
    glutTimerFunc(n, tick, n); // schedule next tick

    // change the appropriate axis based on spin-speed
    //Theta[Axis] += spinSpeed;

    // Rotate dice by random speed
    if (diceRoll) {
        spinSpeed = rand() % (151) + 50;
        Theta[0] += spinSpeed;
        Theta[1] += spinSpeed;
        Theta[2] += spinSpeed;
    }

    // change the light angle
    if (lightSpin) {
        lightAngle += 5.0;
    }

    // tell GPU to display the frame
    glutPostRedisplay();

}
//----------------------------------------------------------------------------

// keyboard callback
static void
keyboard(unsigned char key, int x, int y)
{
    // Perform the appropriate action, based on the key that was pressed.
    // Default is to stop the cube-rotation
    switch (key) {

    case 'q': case 'Q': case 033: // upper/lower Q or escape
        // Q: quit the program
        exit(0);
        break;
    case 'x': case 'X':
        // X: set rotation on X-axis
        Axis = 0;
        break;
    case 'y': case 'Y':
        // Y: set rotation on y-axis
        Axis = 1;
        break;
    case 'z': case 'Z':
        // Z: set rotation on z-axis
        Axis = 2;
        break;
    default:
        // default: stop spinning of cube
        Axis = 3;
        break;
    case '+': case '=':
        // + or =: increase spin-speed
        spinSpeed += 0.05;
        break;
    case '-': case '_':
        // - or _: decrease spin-speed
        spinSpeed -= 0.05;
        break;
    case 'l': case 'L':
        // L: toggle whether the light is spinning around scene
        lightSpin = !lightSpin;
        break;
    case 'w':
        // move forward
        model_view_start = Translate(0, 0, 0.1) * model_view_start;
        break;
    case 's':
        // move backward
        model_view_start = Translate(0, 0, -0.1) * model_view_start;
        break;
    case 'a':
        // turn left
        model_view_start = RotateY(-1.5) * model_view_start;
        break;
    case 'd':
        // turn right
        model_view_start = RotateY(1.5) * model_view_start;
        break;
    case 'W':
        // turn up
        model_view_start = RotateX(-1.5) * model_view_start;
        break;
    case 'S':
        // turn down
        model_view_start = RotateX(1.5) * model_view_start;
        break;
    case 'A':
        // roll left
        model_view_start = RotateZ(-1.5) * model_view_start;
        break;
    case 'D':
        // roll right
        model_view_start = RotateZ(1.5) * model_view_start;
        break;
    case 'W' - 64: // control-w
        // move up
        model_view_start = Translate(0, -0.1, 0) * model_view_start;
        break;
    case 'S' - 64: // control-s
        // move down
        model_view_start = Translate(0, 0.1, 0) * model_view_start;
        break;
    case 'A' - 64: // control-a
        // move left
        model_view_start = Translate(0.1, 0, 0) * model_view_start;
        break;
    case 'D' - 64: // control-d
        // move right
        model_view_start = Translate(-0.1, 0, 0) * model_view_start;
        break;
    }
}

//----------------------------------------------------------------------------

// window-reshape callback
void reshape(int width, int height)
{
    glViewport(0, 0, width, height);

    GLfloat aspect = GLfloat(width) / height;
    mat4  projection = Perspective(65.0, aspect, 0.5, 100.0);

    glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);
}

// OpenGL initialization
static void init() {
    // initialize random number generator based on the time
    srand(time(NULL));

    // create the cube object
    CubeStart = Index[0];
    colorCube();
    CubeNumVertices = Index[0] - CubeStart;

    // create characters, generating them into our arrays
    for (int i = '!'; i <= '~'; i++) {
        // set start position
        charInfo[i][0] = Index[0];
        // generate the vertices for the character
        genCharacter(i, // character
            RED, // color
            0.3, // width of stroke
            0.2, // depth (z-direction)
            0.3, // shininess
            Index, // current index
            // the arrays to fill
            points, normals, colorsDiffuse,
            colorsSpecular, colorsAmbient, objShininess);
        // set number of vertices (end position minus start position)
        charInfo[i][1] = Index[0] - charInfo[i][0];
    }

    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER,
        sizeof(points) + sizeof(normals) + sizeof(colorsDiffuse) +
        sizeof(colorsSpecular) + sizeof(colorsAmbient),
        NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points),
        sizeof(normals), normals);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(normals),
        sizeof(colorsDiffuse), colorsDiffuse);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(normals) + sizeof(colorsDiffuse),
        sizeof(colorsSpecular), colorsSpecular);
    glBufferSubData(GL_ARRAY_BUFFER,
        sizeof(points) + sizeof(normals) + sizeof(colorsDiffuse) + sizeof(colorsSpecular),
        sizeof(colorsAmbient), colorsAmbient);
    glBufferSubData(GL_ARRAY_BUFFER,
        sizeof(points) + sizeof(normals) + sizeof(colorsDiffuse) + sizeof(colorsSpecular) + sizeof(colorsAmbient),
        sizeof(objShininess), objShininess);

    // Load shaders and use the resulting shader program
    const GLchar* vShaderCode =
        // all of our attributes from the arrays uploaded to the GPU
        "attribute  vec4 vPosition; "
        "attribute  vec3 vNormal; "
        "attribute  vec4 vDiffCol; "
        "attribute  vec4 vSpecCol; "
        "attribute  vec4 vAmbCol; "
        "attribute  float vObjShininess; "

        // uniform variables
        "uniform mat4 ModelViewStart; "
        "uniform mat4 ModelView; "
        "uniform mat4 Projection; "
        "uniform vec4 LightPosition; "
        "uniform vec4 LightDiffuse; "
        "uniform vec4 LightSpecular; "
        "uniform vec4 LightAmbient; "
        "uniform vec4 PickColor; "

        // variables to send on to the fragment shader
        "varying vec3 N,L, E, H; "
        "varying vec4 colorAmbient, colorDiffuse, colorSpecular; "
        "varying float shininess; "

        // main vertex shader
        "void main() "
        "{ "

        // Transform vertex  position into eye coordinates
        "vec3 pos = (ModelView * vPosition).xyz; "
        " "
        // compute the lighting-vectors
        "N = normalize( ModelView*vec4(vNormal, 0.0) ).xyz; "
        "L = normalize( (ModelViewStart*LightPosition).xyz - pos ); "
        "E = normalize( -pos ); "
        "H = normalize( L + E ); "

        // pass on the material-related variables
        "colorAmbient = vAmbCol; "
        "colorDiffuse = vDiffCol; "
        "colorSpecular = vSpecCol; "
        "shininess = vObjShininess; "

        // convert the vertex to camera coordinates
        "gl_Position = Projection * ModelView * vPosition; "
        "} "
        ;
    const GLchar* fShaderCode =
        // variables passed from the vertex shader
        "varying vec3 N,L, E, H; "
        "varying float shininess; "
        "varying vec4 colorAmbient, colorDiffuse, colorSpecular; "

        // uniform variables
        "uniform vec4 light_ambient, light_diffuse, light_specular; "
        "uniform float Shininess; "
        "uniform vec4 PickColor; "

        // main fragment shader
        "void main()  "
        "{  "

        // if we are picking, use the pick color, ignoring everything else
        "if (PickColor.a >= 0.0) { "
        "   gl_FragColor = PickColor; "
        "   return;"
        "} "

        // compute color intensities
        "vec4 AmbientProduct = light_ambient * colorAmbient; "
        "vec4 DiffuseProduct = light_diffuse * colorDiffuse; "
        "vec4 SpecularProduct = light_specular * colorSpecular; "

        // Compute fragment colors based on illumination equations
        "vec4 ambient = AmbientProduct; "
        "float Kd = max( dot(L, N), 0.0 ); "
        "vec4  diffuse = Kd*DiffuseProduct; "
        "float Ks = pow( max(dot(N, H), 0.0), shininess ); "
        "vec4  specular = Ks * SpecularProduct; "
        "if( dot(L, N) < 0.0 ) { "
        "  specular = vec4(0.0, 0.0, 0.0, 1.0); } "

        // add the color components
        "  gl_FragColor = ambient + specular + diffuse; "
        "}  "
        ;

    // set up the GLSL shaders
    GLuint program = InitShader2(vShaderCode, fShaderCode);

    glUseProgram(program);

    // set up vertex arrays
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
        BUFFER_OFFSET(0));

    GLuint vNormal = glGetAttribLocation(program, "vNormal");
    glEnableVertexAttribArray(vNormal);
    glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,
        BUFFER_OFFSET(sizeof(points)));

    GLuint vDiffCol = glGetAttribLocation(program, "vDiffCol");
    glEnableVertexAttribArray(vDiffCol);
    glVertexAttribPointer(vDiffCol, 4, GL_FLOAT, GL_FALSE, 0,
        BUFFER_OFFSET(sizeof(points) + sizeof(normals)));

    GLuint vSpecCol = glGetAttribLocation(program, "vSpecCol");
    glEnableVertexAttribArray(vSpecCol);
    glVertexAttribPointer(vSpecCol, 4, GL_FLOAT, GL_FALSE, 0,
        BUFFER_OFFSET(sizeof(points) + sizeof(normals) + sizeof(colorsDiffuse)));

    GLuint vAmbCol = glGetAttribLocation(program, "vAmbCol");
    glEnableVertexAttribArray(vAmbCol);
    glVertexAttribPointer(vAmbCol, 4, GL_FLOAT, GL_FALSE, 0,
        BUFFER_OFFSET(sizeof(points) + sizeof(normals) + sizeof(colorsDiffuse) + sizeof(colorsSpecular)));

    // Initialize lighting position and intensities
    point4 light_position(1, 1, 1, 0);
    color4 light_ambient(0, 0, 0, 1.0);
    color4 light_diffuse(1, 1, 1, 1.0);
    color4 light_specular(0.4, 0.4, 0.4, 1.0);

    glUniform4fv(glGetUniformLocation(program, "light_ambient"),
        1, light_ambient);
    glUniform4fv(glGetUniformLocation(program, "light_diffuse"),
        1, light_diffuse);
    glUniform4fv(glGetUniformLocation(program, "light_specular"),
        1, light_specular);

    lightId = glGetUniformLocation(program, "LightPosition");

    // initialize picking
    setGpuPickColorId(glGetUniformLocation(program, "PickColor"));

    // Retrieve transformation uniform variable locations
    ModelViewStart = glGetUniformLocation(program, "ModelViewStart");
    ModelView = glGetUniformLocation(program, "ModelView");
    Projection = glGetUniformLocation(program, "Projection");

    // enable z-buffer algorithm
    glEnable(GL_DEPTH_TEST);

    // set background color to be white
    glClearColor(1.0, 1.0, 1.0, 1.0);
}

//----------------------------------------------------------------------------

int main(int argc, char** argv)
{
    // perform OpenGL initialization
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Color Cube");
    glewInit();
    init();

    // set up callback functions
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutTimerFunc(TICK_INTERVAL, tick, TICK_INTERVAL); // timer callback

    // start executing the main loop, waiting for a callback to occur
    glutMainLoop();
    return 0;
}
