#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define RESOLUTION_Y 1200
#define RESOLUTION_X 1800

float obj_Vertices[] =
{
    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

    0.5f,  0.5f,  0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f,  0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
};

float normals[] = {
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,

    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,

    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,

    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,

    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,

    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f
};

typedef struct
{
    float x, y, z;
} Vec3;

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
mat4 M_Rotate_Z(float angle);
mat4 M_Rotate_X(float angle);
mat4 M_Perspective(float fovyRadians, float aspect, float nearZ, float farZ);

int main()
{
    FILE *vertexHLSL = fopen("VertexShader.vs", "r");
    FILE *fragmentHLSL = fopen("FragmentShader.fs", "r");
    char *vertexString = malloc(sizeof(char) * 2000);
    char *fragmentString = malloc(sizeof(char) * 2000);

    readEntireFile(vertexString, vertexHLSL);
    const char* vertexSource = vertexString;
    fclose(vertexHLSL);

    readEntireFile(fragmentString, fragmentHLSL);
    const char* fragmentSource = fragmentString;
    fclose(fragmentHLSL);

    glfwInit();
    GLFWwindow *window = glfwCreateWindow(RESOLUTION_X, RESOLUTION_Y, "OpenGlScene", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((void*)glfwGetProcAddress);
    glEnable(GL_DEPTH_TEST);
    glfwSetFramebufferSizeCallback(window, framebuffer);

    //vertex shader
    unsigned int vertexShaderID = 0;
    vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderID, 1, &vertexSource, NULL);
    glCompileShader(vertexShaderID);

    //fragment shader
    unsigned int fragmentShaderID = 0;
    fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderID, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShaderID);

    unsigned int shaderProgram = 0;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShaderID);
    glAttachShader(shaderProgram, fragmentShaderID);
    glLinkProgram(shaderProgram);

    free(vertexString);
    free(fragmentString);
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    unsigned int VAO, VBO, NRS;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(obj_Vertices), obj_Vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //bind normals
    glGenBuffers(1, &NRS);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, NRS);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    int modelLoc = glGetUniformLocation(shaderProgram, "model");
    int viewLoc = glGetUniformLocation(shaderProgram, "view");
    int projLoc = glGetUniformLocation(shaderProgram, "projection");

    int colorLoc = glGetUniformLocation(shaderProgram, "uColor");

    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.3f, 0.1f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);
        float angleTime = glfwGetTime();

        mat4 cube = M_Translate(0.25f, 0.6f, 0.0f);
        cube = M_MulMatrix(cube, M_Rotate_Z(angleTime * 2.0f));
        cube = M_MulMatrix(cube, M_Rotate_X(angleTime * 2.0f));
        cube = M_MulMatrix(cube, M_Scale(0.5f, 0.6f, 1.0f));

        mat4 view = M_Translate(0.0f, 0.0f, -5.0f);

        float fov = 45.0f * (3.14159265f / 180.0f);
        float aspect = (float)RESOLUTION_X/RESOLUTION_Y;
        mat4 projection = M_Perspective(fov, aspect, 0.1f, 100.0f);

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view.m[0]);

        glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection.m[0]);

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &cube.m[0]);

        glBindVertexArray(VAO);

        glUniform3f(colorLoc, 1.0f, 0.0f, 0.0f);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //second cube
        mat4 cube2 = M_Translate(2.25f, 1.5f, -5.0f);
        cube2 = M_MulMatrix(cube2, M_Rotate_Z(angleTime * -2.0f));
        cube2 = M_MulMatrix(cube2, M_Rotate_X(angleTime * 2.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &cube2.m[0]);
        glUniform3f(colorLoc, 0.0f, 1.0f, 0.0f);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //third cube
        mat4 cube3 = M_Translate(0.25f, 1.5f, 0.0f);
        cube3 = M_MulMatrix(cube3, M_Rotate_Z(angleTime * -2.0f));
        cube3 = M_MulMatrix(cube3, M_Rotate_X(angleTime * -2.0f));
        cube3 = M_MulMatrix(cube3, M_Scale(0.25f, 0.25f, 0.25f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &cube3.m[0]);
        glUniform3f(colorLoc, 0.0f, 0.0f, 1.0f);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
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
    mat4 S = M_Identity();

    S.m[0] = sx;
    S.m[5] = sy;
    S.m[10] = sz;

    return S;
}
mat4 M_Rotate_Z(float angle)
{
    mat4 R = M_Identity();
    float c = cos(angle);
    float s = sin(angle);

    R.m[0] = c;
    R.m[1] = s;
    R.m[4] = -s;
    R.m[5] = c;

    return R;
}

mat4 M_Rotate_X(float angle)
{
    mat4 R = M_Identity();
    float c = cos(angle);
    float s = sin(angle);

    R.m[5] = c;
    R.m[6] = s;
    R.m[9] = -s;
    R.m[10] = c;

    return R;
}

mat4 M_Rotate_Y(float angle)
{
    mat4 R = M_Identity();
    float c = cos(angle);
    float s = sin(angle);

    R.m[0] = c;
    R.m[2] = -s;
    R.m[8] = s;
    R.m[10] = c;

    return R;
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

mat4 M_Perspective(float fovyRadians, float aspect, float nearZ, float farZ)
{
    float f = 1.0f / tanf(fovyRadians * 0.5f);
    mat4 P = {0};

    P.m[0]  = f / aspect;
    P.m[5]  = f;
    P.m[10] = (farZ + nearZ) / (nearZ - farZ);
    P.m[11] = -1.0f;
    P.m[14] = (2.0f * farZ * nearZ) / (nearZ - farZ);

    return P;
}

Vec3 get_crossProduct(Vec3 A, Vec3 B)
{
    Vec3 r;

    r.x = (A.y * B.z) - (A.z * B.y);
    r.y = (A.z * B.x) - (A.x * B.z);
    r.z = (A.x * B.y) - (A.y * B.x);

    return r;
}
