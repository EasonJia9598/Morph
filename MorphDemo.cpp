
/************************************************************************************
 
 File:             Morph.cpp
 
 Description:
 
 
 
 Author:            Zesheng Jia
 
 *************************************************************************************/



/* include the library header files */
#include <stdlib.h>
#include <GLUT/GLUT.h>
#include <math.h>
#include <iostream>
#include <time.h>
#include <string>


using namespace std;
// stars coordinates
// the ratio of the circumference to the diameter of a circle
#define PI  3.1415926
// conversion multiplier for converting from degrees to Radians
#define DEG_TO_RAD PI/180.0
#define R  0.3



int index2 = 0;

GLfloat theta = 1;
GLfloat thetaMinus = 0.01;
// cube vertices
GLfloat vertices[][3] =
{   {-1.0,-1.0,-1.0}, {1.0,-1.0,-1.0},
    {1.0,1.0,-1.0}, {-1.0,1.0,-1.0},
    {-1.0,-1.0, 1.0}, {1.0,-1.0, 1.0},
    {1.0,1.0, 1.0}, {-1.0,1.0, 1.0}};

// colors of the vertices
GLfloat colors[][3] =
        {{1.0,0.0,0.0}, {1.0,0.0,0.0},
        {1.0,1.0,0.0}, {0.0,1.0,0.0},
        {0.0,0.0,1.0}, {1.0,0.0,1.0},
        {1.0,0.78,0.23}, {0.0,1.0,1.0},
        {0.42,0.54,0.21}, {1.0,0.0,0.0},
        {1.0,1.0,0.0}, {0.0,1.0,0.0}
    };

// current camera position
GLfloat cameraPosition[] = {0, 0, 5};

// difference added at each frame
GLfloat interpDifference = 0.01;

// window dimensions
GLint windowWidth  = 500;
GLint windowHeight = 650;

// Define Star Coordiantes

#define L1  R * cos(36*DEG_TO_RAD)
#define L2  R * sin(36*DEG_TO_RAD)
#define L3  R * cos(72*DEG_TO_RAD)
#define L4  R * sin(72*DEG_TO_RAD)
#define L5  L2 * tan(72*DEG_TO_RAD)
#define L6  L2 / cos(72*DEG_TO_RAD)
#define L7  L6 * sin(54*DEG_TO_RAD)
#define L8  0.3 + L6 * cos(54*DEG_TO_RAD)

// Create star matrix
GLfloat starVertices[][2] = {
    {      0.0           , (GLfloat)(L5+L1) } , // v0
    { (GLfloat)(L2)      , (GLfloat)(L1)    } , // v1
    { (GLfloat)(L2+L6)   , (GLfloat)(L1)    } , // v2
    { (GLfloat)(L4)      , (GLfloat)(-L3)   } , // v3
    { (GLfloat)(L7)      , (GLfloat)(L8)*(-1)  } , // v4
    {      0.0           , (GLfloat)(-R)    } , // v5
    { (GLfloat)(-L7)     , (GLfloat)(L8)*(-1)    } , // v6
    { (GLfloat)(-L4)     , (GLfloat)(-L3)   } , // v7
    { (GLfloat)(-L2-L6)  , (GLfloat)(L1)    } , // v8
    { (GLfloat)(-L2)     , (GLfloat)(L1)    }   // v9
    
};

// Crete First Name letter matrix
GLfloat LetterVertices[][2] = {
    { 0.0  , 0.78 } , // 0
    { 0.68 , 0.78 } , // 1
    { 0.68 , 0.56 } , // 2
    { 0.125  , -0.4 } , // 3
    { 0.125  , -0.65} , // 4
    { -0.125 , -0.65} , // 5
    { -0.75, -0.65} , // 6
    { -0.125 , -0.4 } , // 7
    { -0.68, 0.56 } , // 8
    { -0.68, 0.78 } , // 9
    { -0.125 , 0.56 } , // 10 - a' -> 7
    { 0.125  , 0.56 } , // 11 - b' -> 3
    { -0.5 , -0.4 } , // 12 - c
    { -0.5 , -0.1 } , // 13 - d
    { -0.75, -0.1 } , // 14 - e
    { -0.125 , 0.56 } , // 15 - 7'
    { 0.125  , 0.56 }   // 16 - 3'
    
};

//Create container Matrix for moving the whole shape
GLfloat containerMatrix [17][2];


/* Button Matrix
 * Button 1 :
 * I (-0.8 , -1.15)  J (-0.4 , -1.15) K (-0.4 , -1.35) L (-0.8 , -1.35)
 * Button 2 :
 * P (-0.2 , -1.15) O
 *
 */
GLfloat buttonMatrix [][2] = {
    // Button1
    { -0.8  ,  -1.15 }   , // 0 - I
    { -0.4  ,  -1.15 }   , // 1 - J
    { -0.4  ,  -1.35 }   , // 2 - K
    { -0.8  ,  -1.35 }   , // 3 - L
    { -0.39 ,  -1.15 }   , // 4 - J' shadow
    { -0.39 ,  -1.36 }  , // 5 - K' shadow
    { -0.8  ,  -1.36 }  , // 6 - L' shadow
    // Button2
    { -0.2  ,  -1.15 }   , // 7 - P
    {  0.2  ,  -1.15 }   , // 8 - O
    {  0.2  ,  -1.35 }   , // 9 - N
    { -0.2  ,  -1.35 }   , // 10 - M
    {  0.21 ,  -1.15 }   , // 11 - O' shadow
    {  0.21 ,  -1.36 }  , // 12 - N' shadow
    { -0.2  ,  -1.36 }    // 13 - M' shadow
    
};

GLfloat ButtonPanelMatrix[][2]{
    { -1    , -1.5  } , // bottom - left corner
    { -1    , -1    } , // top - left corner
    {  1    , -1    } , // top - right corner
    {  1    , -1.5  }   // bottom - right corners
};

// MouseX
GLfloat mouseX = 0;
// MouseY
GLfloat mouseY = 0;
// GapOfY
GLfloat gapOfY = (GLfloat)windowHeight/5.0*2;
// GapofY
GLfloat gapOfX = (GLfloat)windowWidth/2.0;
// theta Stop Index
int thetaStop = 0;
int ButtonOn1 = 0;
int ButtonOn2 = 0;
int ButtonOn3 = 0;



/* Sparkle around
 *
 *
 */
GLfloat sparkleMarix[15][2] = {0};

//GLfloat sparkleVector[][2] = {
//    // x : y = 2 : 4 and x = 0.02 y = -0.04
//    { 0.02 , -0.04} , {-0.02 , 0.04 }     ,  // v1 and -v1
//    // x : y = 2 : 4 and x = 0.02 y =  0.04
//    { 0.02 , 0.04 } , {-0.02 , -0.04}     ,  // v2 and -v2
//    // x : y = 4 : 2 and x = -0.04 y = 0.02
//    {-0.04 , 0.02 } , { 0.04 , -0.02}     ,  // v3 and -v3
//    // x : y = 6 : 2 and x = - 0.06 y = -0.02
//    {-0.06 , -0.02} , { 0.06 , 0.02 }     ,  // v4 and -v4
//
//};

GLfloat sparkleRotatinAngle[] = {
    63.5      , // 1
    115.75    , // 2
    153.43    , // 3
    243.43    , // 5
    295.72    , // 6
    333.43    , // 7
    198.43    , // 4
    18.43
    
};

int sparkleMovingControlIndex = 0 ;
GLfloat sparkeCentralPiont[1][2];
GLfloat Alpha = 1;
GLfloat AlphaMinus = 0.01;
GLfloat Beta = 0;
GLfloat starRadius1 = 0.054721;
GLfloat  starRadius2 = 0.080332;
int SparkleStartTrigger = 0;




/*******************
 *******************
 *****Functions*****
 *******************
 *******************/


/************************************************************************
 
 Function:        WriteWords
 
 Description:    Write Words
 
 *************************************************************************/

void writeWords (string str , GLfloat x , GLfloat y , GLfloat Red , GLfloat Green , GLfloat Blue)
{
    // convert string to char array
    char* words = (char*)str.c_str();
    // initialize bit_map
    glColor3f(Red , Green, Blue);
    glRasterPos2f(x, y);

    for (int i = 0; i < str.size(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,words[i]);
    }
    glEnd();
    
    
}
/************************************************************************
 
 Function:        polygon
 
 Description:    Draw a no chaning star via list of vertices.
 
 *************************************************************************/

void polygon(GLfloat matrix[][2] ,int a, int b, int c , int d , GLfloat Red ,GLfloat Green ,GLfloat Blue )
{
    
    glBegin(GL_POLYGON);
    glColor3f(Red , Green , Blue);
    glVertex2fv(matrix[a]);
    glVertex2fv(matrix[b]);
    glVertex2fv(matrix[c]);
    glVertex2fv(matrix[d]);
    glEnd();
}

/************************************************************************
 
 Function:        polygonJ
 
 Description:    Draw a polygon via list of vertices.
                 And it changing between letter and star
 
 *************************************************************************/

void polygonJ(int a, int b, int c , int d)
{
    
    glBegin(GL_POLYGON);
    glColor3fv(colors[2]);
    glVertex2fv(containerMatrix[a]);
    glColor3fv(colors[3]);
    glVertex2fv(containerMatrix[b]);
    glColor3fv(colors[6]);
    glVertex2fv(containerMatrix[c]);
    glColor3fv(colors[1]);
    glVertex2fv(containerMatrix[d]);
    glEnd();
}

/************************************************************************
 
 Function:        Triangle
 
 Description:    Draw a no changing star triangle part via list of vertices.
 
 *************************************************************************/

void Triangle(int a, int b, int c )
{
    
    glBegin(GL_TRIANGLES);
    glColor3fv(colors[a%12]);
    glVertex2fv(starVertices[a]);
    
    glColor3fv(colors[b%12]);
    glVertex2fv(starVertices[b]);
    
    glColor3fv(colors[c%12]);
    glVertex2fv(starVertices[c]);
    
    glEnd();
}

/************************************************************************
 
 Function:        TriangleJ
 
 Description:    Draw the changing polygon triangle divided part via
                 list of vertices.
 
 *************************************************************************/

void TriangleJ(int a, int b, int c )
{
    
    glBegin(GL_TRIANGLES);
    glColor3fv(colors[a%12]);
    glVertex2fv(containerMatrix[a]);
    
    glColor3fv(colors[b%12]);
    glVertex2fv(containerMatrix[b]);
    
    glColor3fv(colors[c%12]);
    glVertex2fv(containerMatrix[c]);
    
    glEnd();
}

/************************************************************************
 
 Function:        Button
 
 Description:    Draw three buttons via list of vertices.
 
 *************************************************************************/

void drawButton()
{
    /*
     * Button 1 :
     * I (-0.8 , -1.15)  J (-0.4 , -1.15) K (-0.4 , -1.35) L (-0.8 , -1.35)
     * Button 2 :
     * P (-0.2 , -1.15) O
     *
     */
    /*******BUTTON 1********/
    // shadow Button
    polygon(buttonMatrix, 0, 4, 5, 6 ,0.3607843137 , 0.3607843137 , 0.3607843137);
    
    // main Buttion
    if (ButtonOn1) {
        polygon(buttonMatrix, 0, 1, 2, 3 , 0.058823529411765 , 0.533333333333333 , 0.92156862745098);
    }else{
        polygon(buttonMatrix, 0, 1, 2, 3 , 0.3607843137 , 0.3607843137 , 0.3607843137);
    }
    
    // Label
    writeWords("Morph", -0.7125, -1.27, 1.0, 1.0, 1.0);
    
    /*******BUTTON 2********/
    // shadow Button
    polygon(buttonMatrix, 7, 11, 12, 13 ,0.3607843137 , 0.3607843137 , 0.3607843137);
    
    // main Buttion
    if (ButtonOn2) {
        polygon(buttonMatrix, 7, 8, 9, 10 , 0.058823529411765 , 0.533333333333333 , 0.92156862745098);
    }else{
        polygon(buttonMatrix, 7, 8, 9, 10 , 0.3607843137 , 0.3607843137 , 0.3607843137);
    }
    // Label
    writeWords("Sparkle", -0.7125 + 0.6, -1.27, 1.0, 1.0, 1.0);
    
    
}


/************************************************************************
 
 Function:        PolygonSkeleton
 
 Description:    Draw the skeleton around the polygon
 
 *************************************************************************/

void PolygonSkeleton()
{
    // using line Loop
    int indexSpark = 0;
    
    // copy first three vertices
    for(int i = 0 ; i < 3 ; i++){
        sparkleMarix[indexSpark][0] = containerMatrix[i][0];
        sparkleMarix[indexSpark][1] = containerMatrix[i][1];
        indexSpark++;
    }
    
    sparkleMarix[indexSpark][0] = containerMatrix[16][0];
    sparkleMarix[indexSpark][1] = containerMatrix[16][1];
        indexSpark++;
    
    for(int i = 3 ; i < 7 ; i++){
        sparkleMarix[indexSpark][0] = containerMatrix[i][0];
        sparkleMarix[indexSpark][1] = containerMatrix[i][1];
        indexSpark++;
    }
    
    for(int i = 14 ; i > 11; i--){
        sparkleMarix[indexSpark][0] = containerMatrix[i][0];
        sparkleMarix[indexSpark][1] = containerMatrix[i][1];
        indexSpark++;
    }
    
    sparkleMarix[indexSpark][0] = containerMatrix[7][0];
    sparkleMarix[indexSpark][1] = containerMatrix[7][1];
        indexSpark++;
    
    sparkleMarix[indexSpark][0] = containerMatrix[15][0];
    sparkleMarix[indexSpark][1] = containerMatrix[15][1];
        indexSpark++;
    
    sparkleMarix[indexSpark][0] = containerMatrix[8][0];
    sparkleMarix[indexSpark][1] = containerMatrix[8][1];
    indexSpark++;

    sparkleMarix[indexSpark][0] = containerMatrix[9][0];
    sparkleMarix[indexSpark][1] = containerMatrix[9][1];
    indexSpark++;

}



/************************************************************************
 
 Function:        DrawSparkleStar
 
 Description:    Draw a hung around sparkling star
 
 *************************************************************************/

void drawSparkleStar(GLfloat Sx , GLfloat Sy){
    
    // draw four rays from a center point
    // initialize transparent mode
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(1.2);
        Beta += 2;
        if (Beta >= 360.0)
            Beta -= 360.0;
    
    for (int i = 0; i < 6; i++) {
        glBegin(GL_LINES);
      
        glColor4f(0.956862745098039 , 0.87843137254902,0.184313725490196,1.0);
        glVertex2f(Sx,Sy );
        glColor4f(0.956862745098039 , 0.87843137254902,0.184313725490196,0.0);
        glVertex2f(Sx + starRadius1 * cos(DEG_TO_RAD * (Beta + sparkleRotatinAngle[i])) , Sy  + starRadius1 * sin(DEG_TO_RAD * (Beta + sparkleRotatinAngle[i])));
        glEnd();
    }
    
    for (int i = 6; i < 8; i++) {
        glBegin(GL_LINES);
        glColor4f(0.956862745098039 , 0.87843137254902,0.184313725490196,1.0);
        glVertex2f(Sx,Sy );
        glColor4f(0.956862745098039 , 0.87843137254902,0.184313725490196,0.0);
        glVertex2f(Sx + starRadius2 * cos(DEG_TO_RAD * (Beta + sparkleRotatinAngle[i])) , Sy  + starRadius2 * sin(DEG_TO_RAD * (Beta + sparkleRotatinAngle[i])));
        glEnd();
    }
    

    
    
}



/************************************************************************
 
 Function:        sparkleStar
 
 Description:        // could rotating
 
                    // have transparence
 
                    // cycling by polygon's outside shape
 
 *************************************************************************/
void sparkleStar ()
{
    if (Alpha < 0) {
        Alpha = 0.99;
        sparkleMovingControlIndex++;
    }

    Alpha -= AlphaMinus;
    
    // draw draft polygon skeleton
    PolygonSkeleton();
    
    // find central point
    if (sparkleMovingControlIndex + 1 < 15)
    {
        
        
        sparkeCentralPiont[0][0] = Alpha * sparkleMarix[sparkleMovingControlIndex][0] + (1 - Alpha) * sparkleMarix[sparkleMovingControlIndex + 1][0];
        
        sparkeCentralPiont[0][1] = Alpha * sparkleMarix[sparkleMovingControlIndex][1] + (1 - Alpha) * sparkleMarix[sparkleMovingControlIndex + 1][1];
//        cout << "hi" << sparkeCentralPiont[0][0] << " " << sparkeCentralPiont[0][1] << endl;
//        cout << Alpha << endl;
//        cout << "x " << sparkeCentralPiont[0][0] << " y " << sparkeCentralPiont[0][1] << endl;
        
    }
    else if(sparkleMovingControlIndex == 14){
        sparkeCentralPiont[0][0] = Alpha * sparkleMarix[sparkleMovingControlIndex][0] + (1 - Alpha) * sparkleMarix[0][0];
        
        sparkeCentralPiont[0][1] = Alpha * sparkleMarix[sparkleMovingControlIndex][1] + (1 - Alpha) * sparkleMarix[0][1];
    }
    else
    {
        sparkleMovingControlIndex = 0;
    }
    

    
    drawSparkleStar(sparkeCentralPiont[0][0] , sparkeCentralPiont[0][1] );

    
}


/************************************************************************
 
 Function:        Morph
 
 Description:    Display callback, clears frame buffer and depth buffer,
 positions the camera and draws the cube.
 
 *************************************************************************/
void Morph() {
    
    // theta of the morphing minus the rate
    if (thetaStop) {
        theta -= thetaMinus;
    }
    
    // if reach the star point , then change
    if (theta < 0) {
        thetaMinus *= -1;
        unsigned int retTime = (unsigned int)time(0) + 0.7;
        while (time(0) < retTime);
    }
    
    // if reach the letter point , then change
    if (theta > 1) {
        thetaMinus *= -1;
        unsigned int retTime =  (unsigned int)time(0) + 0.7;
        while (time(0) < retTime);
    }
    
    //    cout << theta << endl;
    for (int i = 0; i < 10; i++) {
        containerMatrix[i][0] = theta * LetterVertices[i][0] + (1 - theta) * starVertices[i][0];
        containerMatrix[i][1] = theta * LetterVertices[i][1] + (1 - theta) * starVertices[i][1];
        
    }
    
    // 12
    containerMatrix[12][0] = theta * LetterVertices[12][0] + (1 - theta) * starVertices[7][0];
    containerMatrix[12][1] = theta * LetterVertices[12][1] + (1 - theta) * starVertices[7][1];
    // 13
    containerMatrix[13][0] = theta * LetterVertices[13][0] + (1 - theta) * starVertices[7][0];
    containerMatrix[13][1] = theta * LetterVertices[13][1] + (1 - theta) * starVertices[7][1];
    //14
    containerMatrix[14][0] = theta * LetterVertices[14][0] + (1 - theta) * starVertices[6][0];
    containerMatrix[14][1] = theta * LetterVertices[14][1] + (1 - theta) * starVertices[6][1];
    //15
    containerMatrix[15][0] = theta * LetterVertices[15][0] + (1 - theta) * starVertices[7][0];
    containerMatrix[15][1] = theta * LetterVertices[15][1] + (1 - theta) * starVertices[7][1];
    //16
    containerMatrix[16][0] = theta * LetterVertices[16][0] + (1 - theta) * starVertices[3][0];
    containerMatrix[16][1] = theta * LetterVertices[16][1] + (1 - theta) * starVertices[3][1];
    
}


/************************************************************************
 
 Function:        colorStar
 
 Description:    Map the vertices to colorStar
 
 *************************************************************************/
void colorStar()
{
    /* Star
    Triangle(0, 9, 1); // 1
    Triangle(1, 3, 2); // 2
    Triangle(7, 9, 8); // 5
    Triangle(7, 3, 5); // 6
    polygon(9, 1, 3, 7); //1
    Triangle(3, 5, 4);  // 3
    Triangle(7, 5, 6);  // 4
    */
//    for (int i = 0; i < 10; i++) {
//          cout << "v" << i << " x = " << starVertices[i][0] << " y = " << starVertices[i][1] << endl;
//    }
    
    /* Print J
     { -0.1 , 0.58 } , // 10 - a' -> 7
     { 0.1  , 0.58 } , // 11 - b' -> 3
     { -0.5 , -0.4 } , // 12 - c
     { -0.5 , -0.1 } , // 13 - d
     { -0.75, -0.1 } , // 14 - e
     { -0.1 , 0.58 } , // 15 - 9'
     { 0.1  , 0.58 }   // 16 - 1'
     */
    Morph();
    drawButton();
    PolygonSkeleton();
    TriangleJ(8, 9, 15);
    TriangleJ(9, 15, 0);
    TriangleJ(0, 15, 16);
    TriangleJ(0, 16, 1);
    TriangleJ(0, 16, 1);
    TriangleJ(1, 2, 16);
    TriangleJ(3, 4, 5);
    TriangleJ(3, 7, 5);
    TriangleJ(7, 5, 6);
    TriangleJ(12, 7, 6);
    TriangleJ(13, 12, 6);
    TriangleJ(14, 13, 6);
    polygonJ(15, 16, 3, 7);
    if (SparkleStartTrigger) {
        sparkleStar();
    }
    
    
}




/************************************************************************
 
 Function:        myDisplay
 
 Description:    Display callback, clears frame buffer and depth buffer,
 positions the camera and draws the cube.
 
 *************************************************************************/
void myDisplay(void)
{
    
    // clear the screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT);
    polygon(ButtonPanelMatrix, 0, 1, 2, 3, 1.0, 0.784313725490196, 0.333333333333333);
    
    // draw the color cube
    colorStar();
   
    // swap the drawing buffers
    glutSwapBuffers();
}



/************************************************************************
 
 Function:        myIdle
 
 Description:    Updates the animation when idle.
 
 *************************************************************************/
void myIdle()
{
    

    
    glutPostRedisplay();
}


/************************************************************************
 
 Function:        myMouseButton
 
 Description:    Check mouse position and invoke functions.
 
 ************************************************************************/

void myMouseButton(int button, int state, int x, int y)
{
    
    //Calculate for X
    mouseX = (GLfloat)x - gapOfX;
    mouseX = ((GLfloat)mouseX / gapOfX) ;

    
    // Calculate for Y
    mouseY = (GLfloat)y - gapOfY;
    mouseY = ((GLfloat)mouseY / gapOfY) * -1;

    printf("x : %d mouseX: %f " , x, mouseX);
    printf("y : %d mouseY: %f \n" , y,mouseY);
    
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if (mouseX <= -0.38 && mouseX >= -0.8 && mouseY <= -1.15 && mouseY>= -1.36) {
            if (thetaStop == 1) {
                ButtonOn1 = 0;
                thetaStop = 0;
            }else{
                ButtonOn1 = 1;
                thetaStop = 1;
            }
        }
        if (mouseX <= 0.22 && mouseX >= -0.2 && mouseY <= -1.15 && mouseY>= -1.36) {
            
            if (SparkleStartTrigger == 1) {
                ButtonOn2 = 0;
                SparkleStartTrigger = 0;
            }else{
                ButtonOn2 = 1;
                SparkleStartTrigger = 1;
            }
        }
        
    }
    

}







/************************************************************************
 
 Function:        initializeGL
 
 Description:    Initializes the OpenGL rendering context for display.
 
 *************************************************************************/
void initializeGL(void)
{
 
    // set background color to be black
    glClearColor(0,0,0,1.0);
    
    // set the drawing to be white
    glColor3f(1.0, 1.0, 1.0);
    
    // set window mode to 2D orthographic and set the window size
    gluOrtho2D(-1,1, -1.5, 1);
    
}




/************************************************************************
 
 Function:        main
 
 Description:    Sets up the openGL rendering context and the windowing
 system, then begins the display loop.
 
 *************************************************************************/
int main(int argc, char** argv)
{
    
    // initialize the toolkit
    glutInit(&argc, argv);
    // set display mode
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    // set window size
    glutInitWindowSize(windowWidth,windowHeight);
    // set window position on screen
    glutInitWindowPosition(100, 150);
    // open the screen window
    glutCreateWindow("colorcube");
    
    // register redraw function
    glutDisplayFunc(myDisplay);
    // register the idle function
    glutIdleFunc(myIdle);
    glutMouseFunc(myMouseButton);
    //initialize the rendering context
    initializeGL();
    // go into a perpetual loop
    glutMainLoop();
}


