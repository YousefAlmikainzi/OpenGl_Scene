#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

float obj_Vertices[] =
{
    0.5f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f
};

typedef struct
{
    float m[16];
} mat4;


void framebuffer(GLFWwindow* window, int width, int height);
void readEntireFile(char* strBuffer, FILE* shaderFile);
mat4 M_Identity();
mat4 M_Translate(float x, float y, float z);
mat4 M_Scale(float sx, float sy, float sz);
mat4 M_MulMatrix(mat4 a, mat4 b);

int main()
{
    return 0;
}

void framebuffer(GLFWwindow* window, int width, int height)
{
    glViewport(0,0,width, height);
}

void readEntireFile(char* strBuffer, FILE* shaderFile)
{
    fseek(shaderFile, 0, SEEK_END);
    long fileSize = ftell(shaderFile);
    rewind(shaderFile);
    fread(strBuffer, 1, fileSize, shaderFile);
    strBuffer[fileSize] = '\0';
}

mat4 M_Identity()
{
    mat4 I = {0};

    I.m[0] = 1;
    I.m[5] = 1;
    I.m[10] = 1;
    I.m[15] = 1;
    return I;
}

mat4 M_Translate(float x, float y, float z)
{
    mat4 T = M_Identity();
    T.m[12] = x;
    T.m[13] = y;
    T.m[14] = z;
    return T;
}

mat4 M_Scale(float sx, float sy, float sz)
{
    mat4 S = {0};

    S.m[0] = sx;
    S.m[5] = sy;
    S.m[10] = sz;

    return S;
}

mat4 M_MulMatrix(mat4 a, mat4 b)
{
    mat4 result = {0};
    int matrixSize = 4;

    for(int col = 0; col < matrixSize; col++)
    {
        for(int row = 0; row < matrixSize; row++)
        {
            float sum = 0.0f;
            for(int i = 0; i <matrixSize; i++)
            {
                sum += a.m[i*4 + row] * b.m[col*4 + i];
            }
            result.m[col * 4 + row] = sum;
        }
    }
    return result;
}
