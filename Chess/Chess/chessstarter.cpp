// (your name here)
// Chess animation starter kit.

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <GLUT/GLUT.h>
using namespace std;
#include "graphics.h"


// Global Variables
// Some colors you can use, or make your own and add them
// here and in graphics.h
GLdouble redMaterial[] = {0.7, 0.1, 0.2, 1.0};
GLdouble greenMaterial[] = {0.1, 0.7, 0.4, 1.0};
GLdouble brightGreenMaterial[] = {0.1, 0.9, 0.1, 1.0};
GLdouble blueMaterial[] = {0.1, 0.2, 0.7, 1.0};
GLdouble whiteMaterial[] = {1.0, 1.0, 1.0, 1.0};

double screen_x = 800;
double screen_y = 800;

enum piece_numbers {
    pawn = 100,
    knight,
    rook,
    bishop,
    queen,
    king
};


// Outputs a string of text at the specified location.
void text_output(double x, double y, char *string)
{
	void *font = GLUT_BITMAP_9_BY_15;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
	
	int len, i;
	glRasterPos2d(x, y);
	len = (int) strlen(string);
	for (i = 0; i < len; i++) 
	{
		glutBitmapCharacter(font, string[i]);
	}

    glDisable(GL_BLEND);
}

// Given the three triangle points x[0],y[0],z[0],
//		x[1],y[1],z[1], and x[2],y[2],z[2],
//		Finds the normal vector n[0], n[1], n[2].
void FindTriangleNormal(double x[], double y[], double z[], double n[])
{
	// Convert the 3 input points to 2 vectors, v1 and v2.
	double v1[3], v2[3];
	v1[0] = x[1] - x[0];
	v1[1] = y[1] - y[0];
	v1[2] = z[1] - z[0];
	v2[0] = x[2] - x[0];
	v2[1] = y[2] - y[0];
	v2[2] = z[2] - z[0];
	
	// Take the cross product of v1 and v2, to find the vector perpendicular to both.
	n[0] = v1[1]*v2[2] - v1[2]*v2[1];
	n[1] = -(v1[0]*v2[2] - v1[2]*v2[0]);
	n[2] = v1[0]*v2[1] - v1[1]*v2[0];

	double size = sqrt(n[0]*n[0] + n[1]*n[1] + n[2]*n[2]);
	n[0] /= -size;
	n[1] /= -size;
	n[2] /= -size;
}

// Loads the given data file and draws it at its default position.
// Call glTranslate before calling this to get it in the right place.
void DrawPiece(char filename[])
{
	// Try to open the given file.
	char buffer[200];
	ifstream in(filename);
	if(!in)
	{
		cerr << "Error. Could not open " << filename << endl;
		exit(1);
	}

	double x[100], y[100], z[100]; // stores a single polygon up to 100 vertices.
	int done = false;
	int verts = 0; // vertices in the current polygon
	int polygons = 0; // total polygons in this file.
	do
	{
		in.getline(buffer, 200); // get one line (point) from the file.
		int count = sscanf(buffer, "%lf, %lf, %lf", &(x[verts]), &(y[verts]), &(z[verts]));
		done = in.eof();
		if(!done)
		{
			if(count == 3) // if this line had an x,y,z point.
			{
				verts++;
			}
			else // the line was empty. Finish current polygon and start a new one.
			{
				if(verts>=3)
				{
					glBegin(GL_POLYGON);
					double n[3];
					FindTriangleNormal(x, y, z, n);
					glNormal3dv(n);
					for(int i=0; i<verts; i++)
					{
						glVertex3d(x[i], y[i], z[i]);
					}
					glEnd(); // end previous polygon
					polygons++;
					verts = 0;
				}
			}
		}
	}
	while(!done);

	if(verts>0)
	{
		cerr << "Error. Extra vertices in file " << filename << endl;
		exit(1);
	}

}

void DrawBoard()
{
//    glBegin(GL_QUADS);
//    glVertex3d(0, -1000, 0);
//    glVertex3d(0, -1000, 9000);
//    glVertex3d(9000, -1000, 9000);
//    glVertex3d(9000, -1000, 0);
//    glEnd();
    GLfloat checker_dark[] = {0.0, 0.0, 0.0, 1.0};
    GLfloat checker_light[] = {1.0, 1.0, 1.0, 1.0};
    bool oddCol = false;
    bool oddRow = false;
    for (int row=500; row<8000; row+=1000)
    {
        for (int col=500; col<8000; col+=1000)
        {
            if (!oddRow)
            {
                // even row
                if (!oddCol)
                {
                    // even col
                    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, checker_light);

                }
                else
                {
                    // odd col
                    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, checker_dark);
                }
            }
            else
            {
                // odd row
                if (!oddCol)
                {
                    // even col
                    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, checker_dark);

                }
                else
                {
                    // odd col
                    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, checker_light);
                }
            }
            glBegin(GL_POLYGON);
            glNormal3f(0, 1, 0);
            glVertex3d(col, 0, row);
            glVertex3d(col, 0, row + 1000);
            glVertex3d(col + 1000, 0, row + 1000);
            glVertex3d(col + 1000, 0, row);
            glEnd();

            glBegin(GL_POLYGON);
            glNormal3f(0, 0, -1);
            glVertex3d(col, 0, row);
            glVertex3d(col, -1000, row);
            glVertex3d(col + 1000, -1000, row);
            glVertex3d(col + 1000, 0, row);
            glEnd();

            oddCol = !oddCol;
        }
        oddRow = !oddRow;
    }
}

// NOTE: Y is the UP direction for the chess pieces.
double eye[3] = {4500, 12000, -8000}; // pick a nice vantage point.
double at[3]  = {4500, 0, 4000};

void Interpolate(double t, double t1, double t2, double &x, double &y, double &z, double x1, double y1, double z1, double x2, double y2, double z2)
{
    double r = (t-t1)/(t2-t1);
    if (r < 0)
        r = 0;
    if (r > 1)
        r = 1;
    x = x1 + (x2-x1) * r;
    y = y1 + (y2-y1) * r;
    z = z1 + (z2-z1) * r;
}

//
// GLUT callback functions
//

// This callback function gets called by the Glut
// system whenever it decides things need to be redrawn.
void display(void)
{
    static clock_t start_time = clock();
    clock_t current_time = clock();
    double t = double(current_time - start_time)/ CLOCKS_PER_SEC;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(eye[0], eye[1], eye[2],  at[0], at[1], at[2],  0,1,0); // Y is up!

	// Set the color for one side (white), and draw its 16 pieces.
	GLfloat mat_amb_diff1[] = {0.8, 0.9, 0.5, 1.0};
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff1);
    double x, y, z;

    DrawBoard();

    glPushMatrix();
	glTranslatef(1000, 0, 1000);
	glCallList(rook);
	glPopMatrix();

    glPushMatrix();
	glTranslatef(2000, 0, 1000);
	glCallList(knight);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3000, 0, 1000);
	glCallList(bishop);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(4000, 0, 1000);
	glCallList(king);
	glPopMatrix();

    // 6 white queen
    Interpolate(t, 2.7, 3.0,
                x, y, z,
                5000, 0, 1000,
                5000, 0, 2000);
    glPushMatrix();
    glTranslatef(x, y, z);
    glCallList(queen);
    glPopMatrix();

    // 4 green bishop
    Interpolate(t, 1.7, 2.0,
                x, y, z,
                6000, 0, 1000,
                3000, 0, 4000);
    if (z < 4000)
    {
        glPushMatrix();
        glTranslatef(x, y, z);
        glCallList(bishop);
        glPopMatrix();
    }
    else
    {
        Interpolate(t, 3.4, 3.7,
                    x, y, z,
                    3000, 0, 4000,
                    0, 0, 4000);
        glPushMatrix();
        glTranslatef(x, y, z);
        glCallList(bishop);
        glPopMatrix();
    }

    glPushMatrix();
	glTranslatef(7000, 0, 1000);
	glCallList(knight);
	glPopMatrix();

    glPushMatrix();
	glTranslatef(8000, 0, 1000);
	glCallList(rook);
	glPopMatrix();

	// Set the color for one side (black), and draw its 16 pieces.
	GLfloat mat_amb_diff2[] = {0.1, 0.5, 0.8, 1.0};
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff2);

    glPushMatrix();
	glTranslatef(1000, 0, 8000);
	glCallList(rook);
	glPopMatrix();

    glPushMatrix();
	glTranslatef(2000, 0, 8000);
	glCallList(knight);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3000, 0, 8000);
	glCallList(bishop);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(4000, 0, 8000);
	glCallList(king);
	glPopMatrix();

    // 5 blue queen
    Interpolate(t, 2.2, 2.5,
                x, y, z,
                5000, 0, 8000,
                5000, 0, 6000);
    if (z > 6000)
    {
        glPushMatrix();
        glTranslatef(x, y, z);
        glCallList(queen);
        glPopMatrix();
    }
    else
    {
        Interpolate(t, 3.2, 3.5,
                    x, y, z,
                    5000, 0, 6000,
                    3000, 0, 4000);
        glPushMatrix();
        glTranslatef(x, y, z);
        glCallList(queen);
        glPopMatrix();
    }

    // 3 blue bishop
    Interpolate(t, 1.2, 1.5,
                x, y, z,
                6000, 0, 8000,
                1000, 0, 3000);
	glPushMatrix();
	glTranslatef(x, y, z);
	glCallList(bishop);
	glPopMatrix();

    glPushMatrix();
	glTranslatef(7000, 0, 8000);
	glCallList(knight);
	glPopMatrix();

    glPushMatrix();
	glTranslatef(8000, 0, 8000);
	glCallList(rook);
	glPopMatrix();

	for(int x=1000; x<=8000; x+=1000)
	{
        if (!(x == 5000))
        {
            glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff2);
            glPushMatrix();
            glTranslatef(x, 0, 7000);
            glCallList(pawn);
            glPopMatrix();

            glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff1);
            glPushMatrix();
            glTranslatef(x, 0, 2000);
            glCallList(pawn);
            glPopMatrix();
        }
	}

    // 2 green pawn
    Interpolate(t, .7, 1.0,
                x, y, z,
                5000, 0, 2000,
                5000, 0, 4000);

    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff1);
    glPushMatrix();
    glTranslatef(x, y, z);
    glCallList(pawn);
    glPopMatrix();

    // 1 blue pawn
    Interpolate(t, .2, .5,
                x, y, z,
                5000, 0, 7000,
                5000, 0, 5000);

    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff2);
    glPushMatrix();
    glTranslatef(x, y, z);
    glCallList(pawn);
    glPopMatrix();

	glutSwapBuffers();
    glutPostRedisplay();
}


// This callback function gets called by the Glut
// system whenever a key is pressed.
void keyboard(unsigned char c, int x, int y)
{
	switch (c) 
	{
		case 27: // escape character means to quit the program
			exit(0);
			break;
		default:
			return; // if we don't care, return without glutPostRedisplay()
	}

	glutPostRedisplay();
}



void SetPerspectiveView(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double aspectRatio = (GLdouble) w/(GLdouble) h;
	gluPerspective( 
	/* field of view in degree */ 38.0,
	/* aspect ratio */ aspectRatio,
	/* Z near */ 100, /* Z far */ 30000.0);
	glMatrixMode(GL_MODELVIEW);
}

// This callback function gets called by the Glut
// system whenever the window is resized by the user.
void reshape(int w, int h)
{
	screen_x = w;
	screen_y = h;

	// Set the pixel resolution of the final picture (Screen coordinates).
	glViewport(0, 0, w, h);

	SetPerspectiveView(w,h);

}

// This callback function gets called by the Glut
// system whenever any mouse button goes up or down.
void mouse(int mouse_button, int state, int x, int y)
{
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
	{
	}
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_UP) 
	{
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) 
	{
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_UP) 
	{
	}
	glutPostRedisplay();
}

// Your initialization code goes here.
void InitializeMyStuff()
{
	// set material's specular properties
	GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat mat_shininess[] = {50.0};
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	// set light properties
	GLfloat light_position[] = {static_cast<GLfloat>(eye[0]), static_cast<GLfloat>(eye[1]), static_cast<GLfloat>(eye[2]),1};
	GLfloat white_light[] = {1,1,1,1};
	GLfloat low_light[] = {.3,.3,.3,1};
	glLightfv(GL_LIGHT0, GL_POSITION, light_position); // position first light
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light); // specify first light's color
	glLightfv(GL_LIGHT0, GL_SPECULAR, low_light);

	glEnable(GL_DEPTH_TEST); // turn on depth buffering
	glEnable(GL_LIGHTING);	// enable general lighting
	glEnable(GL_LIGHT0);	// enable the first light.

    glNewList(pawn, GL_COMPILE);
    DrawPiece("/Users/nate/school/3600/Chess/Chess/PAWN.POL");
    glEndList();

    glNewList(knight, GL_COMPILE);
    DrawPiece("/Users/nate/school/3600/Chess/Chess/KNIGHT.POL");
    glEndList();

    glNewList(rook, GL_COMPILE);
    DrawPiece("/Users/nate/school/3600/Chess/Chess/ROOK.POL");
    glEndList();

    glNewList(bishop, GL_COMPILE);
    DrawPiece("/Users/nate/school/3600/Chess/Chess/BISHOP.POL");
    glEndList();

    glNewList(queen, GL_COMPILE);
    DrawPiece("/Users/nate/school/3600/Chess/Chess/QUEEN.POL");
    glEndList();

    glNewList(king, GL_COMPILE);
    DrawPiece("/Users/nate/school/3600/Chess/Chess/KING.POL");
    glEndList();
}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screen_x, screen_y);
	glutInitWindowPosition(10, 10);

	int fullscreen = 0;
	if (fullscreen) 
	{
		glutGameModeString("800x600:32");
		glutEnterGameMode();
	} 
	else 
	{
		glutCreateWindow("Shapes");
	}

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);

	glClearColor(1,1,1,1);	
	InitializeMyStuff();

	glutMainLoop();

	return 0;
}
