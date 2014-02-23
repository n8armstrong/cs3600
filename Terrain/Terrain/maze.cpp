#include "maze.h"
#include <cstdlib>
#include <iostream>
#include <GLUT/GLUT.h>

int randomNumberInRange(int min, int max)
{
    return min + (rand() % (int)(max - min + 1));
}

void Maze::Cell::setVisited(bool visited)
{
    this->visited = visited;
}

void Maze::Cell::setLeft(bool left)
{
    this->left = left;
}

void Maze::Cell::setTop(bool top)
{
    this->top = top;
}

void Maze::Cell::setRight(bool right)
{
    this->right = right;
}

void Maze::Cell::setBottom(bool bottom)
{
    this->bottom = bottom;
}

bool Maze::Cell::getVisited()
{
    return visited;
}

bool Maze::Cell::getLeft()
{
    return left;
}

bool Maze::Cell::getTop()
{
    return top;
}

bool Maze::Cell::getRight()
{
    return right;
}

bool Maze::Cell::getBottom()
{
    return bottom;
}

// END GETTERS AND SETTERS //

void Maze::Cell::draw(int i, int j)
{
    glColor3ub(i*2342423%255, j*2234232342%255, j*i*128%255);
    if (bottom)
    {
        glBegin(GL_QUADS);
        glVertex3d(i, j, 0);
        glVertex3d(i+1, j, 0);
        glVertex3d(i+1, j, 1);
        glVertex3d(i, j, 1);
        glEnd();
    }
    if (right)
    {
        glBegin(GL_QUADS);
        glVertex3d(i+1, j, 0);
        glVertex3d(i+1, j+1, 0);
        glVertex3d(i+1, j+1, 1);
        glVertex3d(i+1, j, 1);
        glEnd();
    }
    if (top)
    {
        glBegin(GL_QUADS);
        glVertex3d(i+1, j+1, 0);
        glVertex3d(i, j+1, 0);
        glVertex3d(i, j+1, 1);
        glVertex3d(i+1, j+1, 1);
        glEnd();
    }
    if (left)
    {
        glBegin(GL_QUADS);
        glVertex3d(i, j+1, 0);
        glVertex3d(i, j, 0);
        glVertex3d(i, j, 1);
        glVertex3d(i, j+1, 1);
        glEnd();
    }
}

void Maze::draw()
{
	for(int i=0; i<M; i++)
		for(int j=0; j<N; j++)
		{
			cells[i][j].draw(i,j);
		}
}

Maze::Maze()
{
    srand(time(NULL));
	// Carve out the cell walls.
    visitCell(0, 0);
    cells[0][0].setBottom(false);
    cells[M-1][N-1].setTop(false);
}

void Maze::visitCell(int i, int j)
{
    cells[i][j].setVisited(true);
    while (true)
    {
        int nextI[M * N];
        int nextJ[M * N];
        size_t sizeI = 0;
        size_t sizeJ = 0;
        if (i > 0 && !cells[i-1][j].getVisited())
        {
            nextI[sizeI++] = i-1;
            nextJ[sizeJ++] = j;
        }
        if (i < M-1 && !cells[i+1][j].getVisited())
        {
            nextI[sizeI++] = i+1;
            nextJ[sizeJ++] = j;
        }
        if (j > 0 && !cells[i][j-1].getVisited())
        {
            nextI[sizeI++] = i;
            nextJ[sizeJ++] = j-1;
        }
        if (j < N-1 && !cells[i][j+1].getVisited())
        {
            nextI[sizeI++] = i;
            nextJ[sizeJ++] = j+1;
        }

        if (sizeI == 0) return;

        int index = rand() % (int)sizeJ;
        int ni = nextI[index];
        int nj = nextJ[index];

        if (ni == i+1)
        {
            cells[i][j].setRight(false);
            cells[i+1][j].setLeft(false);
        }
        if (ni == i-1)
        {
            cells[i][j].setLeft(false);
            cells[i-1][j].setRight(false);
        }
        if (nj == j-1)
        {
            cells[i][j].setBottom(false);
            cells[i][j-1].setTop(false);
        }
        if (nj == j+1)
        {
            cells[i][j].setTop(false);
            cells[i][j+1].setBottom(false);
        }

        this->visitCell(ni, nj);
    }

}

bool Maze::isLegal(double x, double y, double r)
{
    int cellx = (int)x;
    int celly = (int)y;
    double offsetx = x - cellx;
    double offsety = y - celly;
    Cell cell = cells[cellx][celly];

    // wall tests
    if ((cell.getLeft() && offsetx - r < 0) ||
        (cell.getRight() && offsetx + r > 1) ||
        (cell.getBottom() && offsety - r < 0) ||
        (cell.getTop() && offsety + r > 1))
        {
            return false;
        }


    // corner tests
    if ((offsetx + r > 1 && offsety - r < 0) || // bottom right
        (offsetx - r < 0 && offsety - r < 0) || // bottom left
        (offsetx - r < 0 && offsety + r > 1) || // top left
        (offsetx +r > 1 && offsety +r > 1)) // top right
        {
            return false;
        }

    return true;
}