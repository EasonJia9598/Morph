
/************************************************************************************
 
 File:             Morph.cpp
 
 Description:
 
 
 
 Author:            Zesheng Jia
 
                // github upload
 *************************************************************************************/



/* include the library header files */
#include <stdlib.h>
#include <GLUT/GLUT.h>
#include <math.h>
#include <iostream>
#include <time.h>
#include <string>
#include <ctime>
#include <random>

using namespace std;
// stars coordinates
// the ratio of the circumference to the diameter of a circle
#define PI  3.1415926
// conversion multiplier for converting from degrees to Radians
#define DEG_TO_RAD PI/180.0
#define R  0.3



int index2 = 0;

GLfloat theta = 1;
// WINODWS
// GLfloat thetaMinus = 0.0001;
// MAC
GLfloat thetaMinus = 0.01;

// cube vertices
GLfloat vertices[][3] =
{   {-1.0,-1.0,-1.0}, {1.0,-1.0,-1.0},
    {1.0,1.0,-1.0}, {-1.0,1.0,-1.0},
    {-1.0,-1.0, 1.0}, {1.0,-1.0, 1.0},
    {1.0,1.0, 1.0}, {-1.0,1.0, 1.0}
    
};

// colors of the vertices
GLfloat colors[][3] =
{       {1.0,0.0,0.0}, {1.0,0.0,0.0},
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
//
GLint originalwindowWidth  = 500;
GLint originalwindowHeight = 650;

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
GLfloat buttonMatrix [][2] =
{
    // Button1
    { -0.8  ,  -1.15 }   , // 0 - I
    { -0.4  ,  -1.15 }   , // 1 - J
    { -0.4  ,  -1.35 }   , // 2 - K
    { -0.8  ,  -1.35 }   , // 3 - L
    
    { -0.79  ,  -1.16 }   , // 4 - I' shadow
    { -0.39  ,  -1.16 }   , // 5 - J' shadow
    { -0.39  ,  -1.36 }   , // 6 - K' shadow
    { -0.79  ,  -1.36 }   , // 7 - L' shadow
    
    // Button2
    { -0.2   ,  -1.15 }   , // 8 - P
    {  0.2   ,  -1.15 }   , // 9 - O
    {  0.2   ,  -1.35 }   , // 10 - N
    { -0.2   ,  -1.35 }   , // 11 - M
    
    { -0.19  ,  -1.16 }   , // 12 - P' shadow
    {  0.21  ,  -1.16 }   , // 13 - O' shadow
    {0.21  ,  -1.36 }   , // 14 - N' shadow
    { -0.19  ,  -1.36 }   , // 15 - M' shadow
    
    //Button3
	{  0.4   ,  -1.15  }   , // 16 - P
	{  0.8   ,  -1.15  }   , // 17 - O
	{  0.8   ,  -1.35  }   , // 18 - N
	{  0.4   ,  -1.35  }   , // 19 - M
    
    {  0.41  ,  -1.16  }   , // 20 - P
	{  0.81  ,  -1.16  }   , // 21 - O' shadow
	{  0.81  ,  -1.36  }   , // 22 - N' shadow
	{  0.41  ,  -1.36  }   ,  // 23 - M' shadow
    //Button J
    {  0.4   ,  -0.7  }   , // 24 - P
    {  0.6   ,  -0.7  }   , // 25 - O
    {  0.6   ,  -0.9    }   , // 26 - N
    {  0.4   ,  -0.9    }   , // 27 - M
    
    {  0.41  ,  -0.71  }   , // 28 - P
    {  0.61  ,  -0.71  }   , // 29 - O' shadow
    {  0.61  ,  -0.91   }   , // 30 - N' shadow
    {  0.41  ,  -0.91   }   , // 31 - M' shadow
    //Button K
    {  0.7   ,  -0.7  }   , // 32 - P
    {  0.9   ,  -0.7  }   , // 33 - O
    {  0.9   ,  -0.9    }   , // 34 - N
    {  0.7   ,  -0.9    }   , // 35 - M
    {  0.71  ,  -0.71  }   , // 36 - P
    {  0.91  ,  -0.71  }   , // 37 - O' shadow
    {  0.91  ,  -0.91   }   , // 38 - N' shadow
    {  0.71  ,  -0.91   }   , // 39 - M' shadow
    
    
    
    
};

GLfloat ButtonPanelMatrix[][2] =
{
    {  -1    , -1.5  } , // bottom - left corner
    {  -1    , -1    } , // top - left corner
    {   1    , -1    } , // top - right corner
    {   1    , -1.5  }   // bottom - right corners
};

// MouseX
GLfloat mouseX = 0;
// MouseY
GLfloat mouseY = 0;
// GapOfY
GLfloat gapOfY = (GLfloat)windowHeight/5.0*2;
// GapofY
GLfloat gapOfX = (GLfloat)windowWidth/2.0;
// GapValue
GLfloat gapValue = 0;
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

GLfloat sparkleRotatinAngle[] =
{
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
GLfloat sparkleCentralPiont[1][2];
GLfloat Alpha = 1;
// WINDOWS
//GLfloat AlphaMinus = 0.0001;
// MAC
GLfloat AlphaMinus = 0.005;

GLfloat Beta = 0;
GLfloat starRadius1 = 0.054721;
GLfloat  starRadius2 = 0.080332;
int SparkleStartTrigger = 0;
GLfloat starSizeAlterPara = 1;
GLfloat Gamma = 0.01;

/*
 * Sparks around
 */
int SparkingTrigger = 0;
int SparksColorIndex = 0;
/* All info about one sparks
 sparksMatrixs[][0] : Sx of central point
 sparksMatrixs[][1] : Sy of central point
 sparksMatrixs[][2] : Sx' of end piont with angle
 sparksMatrixs[][3] : Sy' of end piont with angle
 sparksMatrixs[][4] : speed of sepecific sparks
 sparksMatrixs[][5] : psai of rates of the lines, center position now
 sparksMatrixs[][6] : color
 sparksMatrixs[][7] : empty or not
 */
GLfloat sparksMatrixs[10][8] = {0};
//SetTime : time + timeGap
clock_t delayTime = 0;
clock_t startTime = 0;
// SparksIndex : the sparks show on the screen's number
int sparksIndex = 0;
GLfloat sparksRadius = 4;
int timeFlag = 1;

/*
 * Jump
 *
 */

GLfloat deltaJump = 0.01;
GLfloat deltaJumpCo = 0.05;
GLfloat coordinateX = 0.25;
GLfloat coordinateY = 0;
GLfloat movingX = 0;
GLfloat movingY = 0;
int JumpTriggerLeft = 0;
int JumpTriggerRight = 0;
int moveLeft = 0;
int moveRight = 0;
int moveUp = 0;
int moveDown = 0;

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
 
 Description:    Draw any polygons using certain matrix.
 
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
 
 Function:        PolygonSkeleton
 
 Description:    Draw the skeleton around the polygon
 
 *************************************************************************/

void drawCircle()
{
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glBegin(GL_LINE_STRIP);
    for (int i = 1; i <= 360; i++) {
        for (int j = 1 ; j < 10 ; j++)
            glVertex2f(0.2 * cos(DEG_TO_RAD * (i + 0.01*j)) + 0.3 , -0.5 +  0.2 * sin(DEG_TO_RAD * (i + 0.1*j)));
        
    }
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
    polygon(buttonMatrix, 4, 5, 6 , 7 , 0.3607843137 , 0.3607843137 , 0.3607843137);
    
    // main Buttion
    if (!ButtonOn1)
	{
        // Not on
        polygon(buttonMatrix, 0, 1, 2, 3 , 0.058823529411765 , 0.533333333333333 , 0.92156862745098);
        // Label
        writeWords("Sparkle", -0.7125 , -1.27, 1.0, 1.0, 1.0);
    }
    else
    {
        // On
        polygon(buttonMatrix, 4, 5, 6 , 7 , 0.0901961,0.627451,0.368627);
        // Label
        writeWords("Sparkle", -0.7125 , -1.28, 1.0, 1.0, 1.0);
    }
  
    
    
    
    
    /*******BUTTON 2********/

    // shadow Button
    polygon(buttonMatrix, 12, 13, 14, 15 ,0.3607843137 , 0.3607843137 , 0.3607843137);
    
    // main Buttion
    if (!ButtonOn2)
	{
        // Not on
        polygon(buttonMatrix, 8, 9, 10 ,11, 0.058823529411765 , 0.533333333333333 , 0.92156862745098);
        //label
        writeWords("Sparks", -0.7125 + 0.6 + 0.005, -1.27, 1.0, 1.0, 1.0);
    }
    else
    {
//        On
        polygon(buttonMatrix, 12, 13, 14 ,15, 0.0901961,0.627451,0.368627);
        // Label
        writeWords("Sparks", -0.7125 + 0.6 + 0.005, -1.28, 1.0, 1.0, 1.0);
    }
    
    /******BUTTON 3*******/

	// shadow Button
	polygon(buttonMatrix, 20, 21, 22, 23, 0.3607843137, 0.3607843137, 0.3607843137);

	// main Buttion
	if (!ButtonOn3)
	{
        // Not on
		polygon(buttonMatrix, 16, 17, 18, 19, 0.058823529411765, 0.533333333333333, 0.92156862745098);
        // Label
        writeWords("Morph", -0.7125 + 0.6 + 0.6 + 0.01, -1.27, 1.0, 1.0, 1.0);
	}
    else
    {
        // On
        polygon(buttonMatrix, 20, 21, 22, 23, 0.0901961,0.627451,0.368627);
        // Label
        writeWords("Morph", -0.7125 + 0.6 + 0.6 + 0.01, -1.28, 1.0, 1.0, 1.0);
    }
    
    
    
    /*******Button J*******/
    if (JumpTriggerLeft == 1)
    {
        // On
        polygon(buttonMatrix, 28, 29, 30, 31, 0.0901961,0.627451,0.368627);
        // Label
        writeWords("J ", -0.7125 + 0.6 + 0.6 , -0.83, 1.0, 1.0, 1.0);
    }
    else
    {
      
        // Not on
        polygon(buttonMatrix, 24, 25, 26, 27, 0.058823529411765, 0.533333333333333, 0.92156862745098);
        // Label
        writeWords("J ", -0.7125 + 0.6 + 0.6 , -0.83, 1.0, 1.0, 1.0);
    }
    
    /*******Button K*******/
    if (JumpTriggerRight == 1)
    {
        // On
        polygon(buttonMatrix, 36, 37, 38, 39, 0.0901961,0.627451,0.368627);
        // Label
        writeWords("K ", -0.7125 + 0.6 + 0.6 + 0.3, -0.83, 1.0, 1.0, 1.0);
    }
    else
    {
        
        // Not on
        polygon(buttonMatrix,  32, 33, 34, 35,0.058823529411765, 0.533333333333333, 0.92156862745098);
        // Label
        writeWords("K ", -0.7125 + 0.6 + 0.6 + 0.3, -0.83, 1.0, 1.0, 1.0);
    }
    
    
    
    
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
    for(int i = 0 ; i < 3 ; i++)
    {
        sparkleMarix[indexSpark][0] = containerMatrix[i][0];
        sparkleMarix[indexSpark][1] = containerMatrix[i][1];
        indexSpark++;
    }
    
    sparkleMarix[indexSpark][0] = containerMatrix[16][0];
    sparkleMarix[indexSpark][1] = containerMatrix[16][1];
        indexSpark++;
    
    for(int i = 3 ; i < 7 ; i++)
    {
        sparkleMarix[indexSpark][0] = containerMatrix[i][0];
        sparkleMarix[indexSpark][1] = containerMatrix[i][1];
        indexSpark++;
    }
    
    for(int i = 14 ; i > 11; i--)
    {
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

void drawSparkleStar(GLfloat Sx , GLfloat Sy)
{
    
    // draw four rays from a center point
    // initialize transparent mode
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(1.2);
    
    // WINDOWS SETTING
//        Beta += 0.1;
    // MAC SETTING
        Beta += 3;
    // WINDOWs SETTING
//    starSizeAlterPara += 0.1;
    // MAC SETTING
    starSizeAlterPara += Gamma;
    
    if (Beta >= 360.0)
    {
        Beta -= 360.0;
    }
    
    if (starSizeAlterPara >= 2 || starSizeAlterPara <= 0.5)
    {
        Gamma *= -1;
    }
    
    for (int i = 0; i < 6; i++)
    {
        glBegin(GL_LINES);
      
        glColor4f(0.956862745098039 , 0.87843137254902,0.184313725490196,1.0);
        glVertex2f(Sx,Sy );
        glColor4f(0.956862745098039 , 0.87843137254902,0.184313725490196,0.0);
        glVertex2f(Sx + starSizeAlterPara * starRadius1 * cos(DEG_TO_RAD * (Beta + sparkleRotatinAngle[i])) ,  Sy  + starSizeAlterPara * starRadius1 * sin(DEG_TO_RAD * (Beta + sparkleRotatinAngle[i])));
        glEnd();
    }
    
    for (int i = 6; i < 8; i++)
    {
        glBegin(GL_LINES);
        glColor4f(0.956862745098039 , 0.87843137254902,0.184313725490196,1.0);
        glVertex2f(Sx,Sy );
        glColor4f(0.956862745098039 , 0.87843137254902,0.184313725490196,0.0);
        glVertex2f(Sx + starSizeAlterPara * starRadius2 * cos(DEG_TO_RAD * (Beta + sparkleRotatinAngle[i])) ,  Sy  + starSizeAlterPara * starRadius2 * sin(DEG_TO_RAD * (Beta + sparkleRotatinAngle[i])));
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
    if (Alpha < 0)
    {
        Alpha = 0.99;
        sparkleMovingControlIndex++;
    }

    Alpha -= AlphaMinus;
    
    // draw draft polygon skeleton
    PolygonSkeleton();
    
    // find central point
    if (sparkleMovingControlIndex + 1 < 15)
    {
        
        
        sparkleCentralPiont[0][0] = Alpha * sparkleMarix[sparkleMovingControlIndex][0] + (1 - Alpha) * sparkleMarix[sparkleMovingControlIndex + 1][0];
        
        sparkleCentralPiont[0][1] = Alpha * sparkleMarix[sparkleMovingControlIndex][1] + (1 - Alpha) * sparkleMarix[sparkleMovingControlIndex + 1][1];
//        cout << "hi" << sparkleCentralPiont[0][0] << " " << sparkleCentralPiont[0][1] << endl;
//        cout << Alpha << endl;
//        cout << "x " << sparkleCentralPiont[0][0] << " y " << sparkleCentralPiont[0][1] << endl;
        
    }
    else if(sparkleMovingControlIndex == 14)
    {
        sparkleCentralPiont[0][0] = Alpha * sparkleMarix[sparkleMovingControlIndex][0] + (1 - Alpha) * sparkleMarix[0][0];
        
        sparkleCentralPiont[0][1] = Alpha * sparkleMarix[sparkleMovingControlIndex][1] + (1 - Alpha) * sparkleMarix[0][1];
    }
    else
    {
        sparkleMovingControlIndex = 0;
    }
    

    
    drawSparkleStar(sparkleCentralPiont[0][0] , sparkleCentralPiont[0][1] );

    
}


/************************************************************************
 
 Function:        Morph
 
 Description:    Display callback, clears frame buffer and depth buffer,
 positions the camera and draws the cube.
 
 *************************************************************************/
void Morph()
{
    
    // theta of the morphing minus the rate
    if (thetaStop)
    {
        theta -= thetaMinus;
    }
    
    // if reach the star point , then change
    if (theta < 0)
    {
        thetaMinus *= -1;
        unsigned int retTime = (unsigned int)time(0) + 0.7;
        while (time(0) < retTime);
    }
    
    // if reach the letter point , then change
    if (theta > 1)
    {
        thetaMinus *= -1;
        unsigned int retTime =  (unsigned int)time(0) + 0.7;
        while (time(0) < retTime);
    }
    
    // Jump function
    
    if (JumpTriggerLeft == 1) {
        if (coordinateX >= -0.25) {
            coordinateX -= deltaJump;
            coordinateY = -8 * coordinateX * coordinateX + 0.5;
            movingX -= deltaJump;
            
            cout << "dfsdfdsfsdf****************" << endl;
            cout << "COx = " << coordinateX << "  COy = " << coordinateY <<"  MovingX = " << movingX << endl;
        }else if(coordinateX + 0.25 <= 0 ){
             coordinateX = 0.25;
             JumpTriggerLeft = 0;
        }
        
    }
    
    if(JumpTriggerRight == 1){
        if (coordinateX <= 0.25) {
            coordinateX += deltaJump;
            coordinateY = -8 * coordinateX * coordinateX + 0.5;
            movingX += deltaJump;
            
            cout << "dfsdfdsfsdf****************" << endl;
            cout << "COx = " << coordinateX << "  COy = " << coordinateY <<"  MovingX = " << movingX << endl;
        }else if (coordinateX + 0.25 >= 0.5) {
       
            coordinateX = -0.25;
            JumpTriggerRight = 0;
        }
        

    }
    
    
    
    
    if (moveLeft) {
        movingX -= deltaJump * 2;
        moveLeft = 0;
    }
    
    if (moveRight) {
        movingX += deltaJump * 2;
        moveRight = 0;
    }

    if (moveDown) {
        movingY -= deltaJump * 2;
        moveDown = 0;
    }
    
    if (moveUp) {
        movingY += deltaJump * 2;
        moveUp = 0;
    }
    
    //    cout << theta << endl;
    for (int i = 0; i < 10; i++)
    {
        containerMatrix[i][0] = theta * LetterVertices[i][0] + (1 - theta) * starVertices[i][0] + movingX ;
        containerMatrix[i][1] = theta * LetterVertices[i][1] + (1 - theta) * starVertices[i][1] + movingY+ coordinateY;
        
    }
    
    // 12
    containerMatrix[12][0] = theta * LetterVertices[12][0] + (1 - theta) * starVertices[7][0] + movingX;
    containerMatrix[12][1] = theta * LetterVertices[12][1] + (1 - theta) * starVertices[7][1] + movingY+ coordinateY;
    // 13
    containerMatrix[13][0] = theta * LetterVertices[13][0] + (1 - theta) * starVertices[7][0] + movingX;
    containerMatrix[13][1] = theta * LetterVertices[13][1] + (1 - theta) * starVertices[7][1] + movingY+ coordinateY;
    //14
    containerMatrix[14][0] = theta * LetterVertices[14][0] + (1 - theta) * starVertices[6][0] + movingX;
    containerMatrix[14][1] = theta * LetterVertices[14][1] + (1 - theta) * starVertices[6][1] + movingY+ coordinateY;
    //15
    containerMatrix[15][0] = theta * LetterVertices[15][0] + (1 - theta) * starVertices[7][0] + movingX;
    containerMatrix[15][1] = theta * LetterVertices[15][1] + (1 - theta) * starVertices[7][1] + movingY+ coordinateY;
    //16
    containerMatrix[16][0] = theta * LetterVertices[16][0] + (1 - theta) * starVertices[3][0] + movingX;
    containerMatrix[16][1] = theta * LetterVertices[16][1] + (1 - theta) * starVertices[3][1] + movingY+ coordinateY;
    
}


/************************************************************************
 
 Function:        drawStarSparks
 
 Description:    Make the spar sparks multiple transparent lines, and
                 make it twinkle.
 
 *************************************************************************/

void drawStarSparks()
{
    // check 10 sparks container
    for (int i = 0; i < 10; i++)
    {
        // check empty or not
        if (sparksMatrixs[i][7] == 0)
        {
            continue;
        }
        // check is that done flying
        else if (sparksMatrixs[i][5] <= 0)
        {
            // then set empty
            for (int j = 0; j < 8; j++)
            {
                sparksMatrixs[i][j] = 0;
            }
            // Sparks number minus 1
            sparksIndex--;
        }
        // draw sparks
        else
        {
            // draw a ray from a center point
            // initialize transparent mode
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_LINE_SMOOTH);
            glLineWidth(2 - sparksMatrixs[i][5] * sparksMatrixs[i][5] );
            
                glBegin(GL_LINE_LOOP);
            
                // end point
                glColor4f(colors[(int)sparksMatrixs[i][6]][0] , colors[(int)sparksMatrixs[i][6]][1] ,colors[(int)sparksMatrixs[i][6]][2] , 0.0);
                glVertex2f(sparksMatrixs[i][0], sparksMatrixs[i][1]);
            
                // head point
                glColor4f(colors[(int)sparksMatrixs[i][6]][0] , colors[(int)sparksMatrixs[i][6]][1] ,colors[(int)sparksMatrixs[i][6]][2] , 1.0);
            
                GLfloat Sx2 = sparksMatrixs[i][5] * sparksMatrixs[i][0] + (1 - sparksMatrixs[i][5]) *  sparksMatrixs[i][2];
                GLfloat Sy2 = sparksMatrixs[i][5] * sparksMatrixs[i][1] + (1 - sparksMatrixs[i][5]) *  sparksMatrixs[i][3];
            
            // draw a circle at the lines' head
                for (int z = 0; z <18; z++) {
                    glVertex2f(Sx2 + 0.01 * cos(DEG_TO_RAD  + 90 + sparksMatrixs[i][2] - z * 10), Sy2 + 0.01 *  sin(DEG_TO_RAD  + 90 +  sparksMatrixs[i][2] - z * 10));
                }
                glEnd();
            /*
             Line without circle head
            glBegin(GL_LINES);
            // end point
            glColor4f(colors[(int)sparksMatrixs[i][6]][0] , colors[(int)sparksMatrixs[i][6]][1] ,colors[(int)sparksMatrixs[i][6]][2] , 0.0);
            glVertex2f(sparksMatrixs[i][0], sparksMatrixs[i][1]);
            // head point
            glColor4f(colors[(int)sparksMatrixs[i][6]][0] , colors[(int)sparksMatrixs[i][6]][1] ,colors[(int)sparksMatrixs[i][6]][2] , 1.0);

            GLfloat Sx2 = sparksMatrixs[i][5] * sparksMatrixs[i][0] + (1 - sparksMatrixs[i][5]) * sparksMatrixs[i][2];
            GLfloat Sy2 = sparksMatrixs[i][5] * sparksMatrixs[i][1] + (1 - sparksMatrixs[i][5]) * sparksMatrixs[i][3];

            glVertex2f( Sx2, Sy2 );
          
            glEnd();
            */
            
            // alter pasi
            
            sparksMatrixs[i][5] -= sparksMatrixs[i][4];
            
        }
    }
}

/************************************************************************
 
 Function:        getRandomNumber
 
 Description:    Get Random Number for min to max

 Eg :           getRandomNumber(1, 10, (unsigned int)time(NULL));
 
 *************************************************************************/
float getRandomNumber ( int min, int max, unsigned int seed )
{
    static std::default_random_engine e(seed);
    static std::uniform_real_distribution<double> u(min, max);
    return (float)u(e);
}

/************************************************************************
 
 Function:        setTimeGap
 
 Description:    setTime = time + random time gap
 
 *************************************************************************/

void setTimeGap ()
{
    int lay = (int)(getRandomNumber(5,30 , (unsigned int)time(NULL)) * 0.1);
    
    delayTime = (clock_t)lay * CLOCKS_PER_SEC * 0.01;
    if (timeFlag) {
        cout << "reset time" << endl;
        startTime = clock();
        timeFlag = 0;
    }
//    cout << "*************************SetTime**********************************" << endl;
//    cout << lay<< endl;
//    cout << delayTime << endl;
//    cout << "*************************SetTime**********************************" << endl;
}



/************************************************************************
 
 Function:        setSparks
 
 Description:    setSparks every information
 
 *************************************************************************/

void setSpark()
{
    // if in time gap , not draw
    if (clock() - startTime < delayTime)
    {
//        cout << "************************没有!!!!!**********************************" << endl;
        cout << clock() << endl;
        return;
    }
    timeFlag = 1;
    // set timeGap
    setTimeGap();
    
    // set sparks will draw this time
    int drawSparksNumber = (int)getRandomNumber(1, (10 - sparksIndex)%4, (unsigned int)time(NULL));
    
    // start set sparks info
    for (int i = 0; i < 10; i++)
    {
        // if index > 10 , not draw
        // if numbers down then stop
        if (sparksIndex >= 10 || drawSparksNumber <= 0)
        {
            return;
        }

        // check if not empty then continue;
        if (sparksMatrixs[i][7] != 0)
        {
            continue;
        }
       
        
        // set sparks info
        // Set Sx start
        sparksMatrixs[i][0] = sparkleCentralPiont[0][0];
        // Set Sy start
        sparksMatrixs[i][1] = sparkleCentralPiont[0][1];
        
        // get random direction
        GLfloat randomDirection = getRandomNumber(0, 8, (unsigned int)time(NULL)) * 40;
//        // Set Sx'
        sparksMatrixs[i][2] = sparksMatrixs[i][0] + sparksRadius * cos(DEG_TO_RAD + randomDirection);
//        // Set Sy'
        sparksMatrixs[i][3] = sparksMatrixs[i][1] + sparksRadius * sin(DEG_TO_RAD + randomDirection);
        
        // Set Delta of speed
        sparksMatrixs[i][4] = (GLfloat)getRandomNumber(5, 10, (unsigned int)time(NULL)) * 0.0003;
        // Set Initial psai of rates
        sparksMatrixs[i][5] = 1.0;
        // Set color
        sparksMatrixs[i][6] = (int)getRandomNumber(0, 12, (unsigned int)time(NULL));
        // Set not empty
        sparksMatrixs[i][7] = 1;
        
        cout << "set a new one" << i << "index = " << sparksIndex << endl;
        cout << "color" << sparksMatrixs[i][6] << endl;
        cout << "Delta = " << sparksMatrixs[i][4] << endl;
        cout << "random = " << getRandomNumber(10, 12, (unsigned int)time(NULL)) << endl;
        sparksIndex++;
        drawSparksNumber--;
    }
    
}

/************************************************************************
 
 Function:        clearSparksMatrix
 
 Description:   Clear every info in Sparks Matrix
 
 *************************************************************************/
void clearSparksMatrix()
{
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 8; j++) {
            sparksMatrixs[i][j] = 0;
        }
    }
    sparksIndex = 0;
    timeFlag = 0;
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
        if (SparkingTrigger) {
            setSpark();
            drawStarSparks();
        }
    }
//    drawCircle();
    
//    RoundRect(20, 0.04, 0.05, 0.39, 0.19, 0.4, 0.2 , -0.5 , -0.5);

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
    mouseY = (GLfloat)y - gapOfY + gapValue;
    mouseY = ((GLfloat)mouseY / gapOfY) * -1;

    printf("x : %d mouseX: %f " , x, mouseX);
    printf("y : %d mouseY: %f \n" , y,mouseY);
    
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if (mouseX <= -0.38 && mouseX >= -0.8 && mouseY <= -1.15 && mouseY>= -1.36) {
            
            if (SparkleStartTrigger == 1)
            {
                ButtonOn1 = 0;
                SparkleStartTrigger = 0;
                ButtonOn2 = 0;
                SparkingTrigger = 0;
                clearSparksMatrix();
            }
            else
            {
                ButtonOn1 = 1;
                SparkleStartTrigger = 1;
                
            }
            
            
        }
        if (mouseX <= 0.22 && mouseX >= -0.2 && mouseY <= -1.15 && mouseY>= -1.36) {
            if (SparkleStartTrigger == 1)
            {
                if (SparkingTrigger == 1) {
                    ButtonOn2 = 0;
                    SparkingTrigger = 0;
                    clearSparksMatrix();
                }
                else {
                    ButtonOn2 = 1;
                    SparkingTrigger = 1;
                }
                
            }
        }

		if (mouseX <= 0.82 && mouseX >= 0.4 && mouseY <= -1.15 && mouseY >= -1.36) {

            if (thetaStop == 1) {
                ButtonOn3 = 0;
                thetaStop = 0;
            }else{
                ButtonOn3 = 1;
                thetaStop = 1;
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
    glClearColor(0,0,0,0.5);
    
    // set the drawing to be white
    glColor3f(1.0, 1.0, 1.0);
    
    // set window mode to 2D orthographic and set the 2D coordinates
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    // set window mode to 2D orthographic and set the window size
    gluOrtho2D(-1,1, -1.5, 1);
    
}



/************************************************************************
 
 Function:        myReshape
 
 Description:    Changes the OpenGL aspect ratio to match a GLUT
 window resize.
 
 *************************************************************************/
void myReshape(int newWidth, int newHeight)
{
    // update the viewport to still be all of the window
    glViewport (-1, -1.5, newWidth, newHeight);
    
    // enter GL_PROJECTION mode so that we can change the 2D coordinates
    glMatrixMode (GL_PROJECTION);
    
    // clear the projection matrix
    glLoadIdentity ();

    GLfloat x2 = 1;
    GLfloat y2;
    // alter the 2D drawing coordinates so that it matches the shape of the window
    if (newWidth > ((GLfloat)originalwindowWidth/2.0))
    {
        // divide into two parts
        
        x2 = ((GLfloat)newWidth - (GLfloat)originalwindowWidth/2.0 ) / ((GLfloat)originalwindowWidth/2);
        cout << " 大于一半" << endl;
    }
    else
    {
        x2 = -1 + (GLfloat)newWidth  / ((GLfloat)originalwindowWidth/2.0);
        cout << " 小于一半" << endl;
    }
    
    if (newHeight <= originalwindowHeight) {
        
        gapValue = (GLfloat)originalwindowHeight - (GLfloat)newHeight;
        y2 = 1 - (gapValue / ((GLfloat)originalwindowHeight/5.0*2));
       
        cout << "上半部分" << endl;
    }
    else
    {
        gapValue = ((GLfloat)newHeight - (GLfloat)originalwindowHeight ) * -1 ;
        y2 = 1 - (gapValue / ((GLfloat)originalwindowHeight/5.0*2));
        cout << "下半部分" << endl;
    }
    
    
    
    gluOrtho2D(-1, x2, -1.5, y2);
    cout << "newWidth = " << newWidth << endl;
    cout << "newHeight = " << newHeight << endl;
    cout << "GLUT_WIDth = " << GLUT_WINDOW_WIDTH << endl;
    cout << "GLUT_height = " << GLUT_WINDOW_HEIGHT << endl;
}





void myKey(unsigned char key, int x, int y)
{
    if (key == 'J' || key == 'j' ) {
            JumpTriggerLeft = 1;
            cout << "Hello J*********" << endl;
    }
    if (key == 'K' || key == 'k' ) {
        JumpTriggerRight = 1;
        cout << "Hello J*********" << endl;
    }
    
    if (key == 'A'|| key == 'a') {
        moveLeft = 1;
        cout << "move left" << endl;
    }
    
    if (key == 'D'|| key == 'd') {
        moveRight = 1;
          cout << "move right" << endl;
    }
    
    if (key == 'S'|| key == 's') {
        moveDown = 1;
        cout << "move Down" << endl;
    }
    
    
    if (key == 'W'|| key == 'w') {
        moveUp = 1;
        cout << "move UP" << endl;
    }
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
    
    // register reshape function
    glutReshapeFunc(myReshape);
    // register keyboardFunc
    glutKeyboardFunc(myKey);
    //initialize the rendering context
    initializeGL();
    // go into a perpetual loop
    glutMainLoop();
}


