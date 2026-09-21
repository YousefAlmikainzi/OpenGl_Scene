#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define RESOLUTION_Y 800
#define RESOLUTION_X 1000

float obj_Vertices[] =
{
    //back -Z
    -.5f, -.5f, -.5,//0
    .5f, -.5f, -.5,//1
    -.5f, .5f, -.5,//2
    .5f, .5f, -.5,//3

    //front +Z
    -.5f, -.5f, .5,//4
    .5f, -.5f, .5,//5
    -.5f, .5f, .5,//6
    .5f, .5f, .5,//7

    //left -X
    -.5f, -.5f, -.5,//8
    -.5f, -.5f, .5,//9
    -.5f, .5f, -.5,//10
    -.5f, .5f, .5,//11

    //right +X
    .5f, -.5f, -.5,//12
    .5f, -.5f, .5,//13
    .5f, .5f, -.5,//14
    .5f, .5f, .5,//15

    //down -Y
    -.5f, -.5f, -.5,//16
    .5f, -.5f, -.5,//17
    -.5f, -.5f, .5,//18
    .5f, -.5f, .5,//19

    //up +Y
    -.5f, .5f, -.5,//20
    .5f, .5f, -.5,//21
    -.5f, .5f, .5,//22
    .5f, .5f, .5//23
};

unsigned int indices[] = {
    0, 1, 3,
    3, 2, 0,

    4, 5, 7,
    7, 6, 4,

    8, 9, 11,
    11, 10, 8,

    12, 13, 15,
    15, 14, 12,

    16, 17, 19,
    19, 18, 16,

    20, 21, 23,
    23, 22, 20
};

float normals[] = {
    //back -Z
    0.0f,0.0f,-1.0f,
    0.0f,0.0f,-1.0f,
    0.0f,0.0f,-1.0f,
    0.0f,0.0f,-1.0f,

    //front +Z
    0.0f,0.0f,1.0f,
    0.0f,0.0f,1.0f,
    0.0f,0.0f,1.0f,
    0.0f,0.0f,1.0f,

    //left -X
    -1.0f,0.0f,0.0f,
    -1.0f,0.0f,0.0f,
    -1.0f,0.0f,0.0f,
    -1.0f,0.0f,0.0f,

    //right +X
    1.0f,0.0f,0.0f,
    1.0f,0.0f,0.0f,
    1.0f,0.0f,0.0f,
    1.0f,0.0f,0.0f,

    //down -Y
    0.0f,-1.0f,0.0f,
    0.0f,-1.0f,0.0f,
    0.0f,-1.0f,0.0f,
    0.0f,-1.0f,0.0f,

    //up +Y
    0.0f,1.0f,0.0f,
    0.0f,1.0f,0.0f,
    0.0f,1.0f,0.0f,
    0.0f,1.0f,0.0f
};

float uv[]=
{
    0.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,

    0.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,

    0.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,

    0.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,

    0.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,

    0.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f
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
mat4 M_Rotate_Y(float angle);
mat4 M_Perspective(float fovyRadians, float aspect, float nearZ, float farZ);
mat4 M_LookAt(Vec3 position, Vec3 target, Vec3 worldUp);

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

    unsigned int VAO, VBO, NRS, EBO, CLR, UV;

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

    //ebo
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
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

    //bind uv
    glGenBuffers(1, &UV);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, UV);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uv), uv, GL_STATIC_DRAW);
    glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(4);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    int modelLoc = glGetUniformLocation(shaderProgram, "model");
    int viewLoc = glGetUniformLocation(shaderProgram, "view");
    int projLoc = glGetUniformLocation(shaderProgram, "projection");

    int colorLoc = glGetUniformLocation(shaderProgram, "uColor");


    Vec3 cameraPos = {-3.0f, -2.0f, 5.0f};
    Vec3 cameraTarget = {-2.1f, 2.1f, -2.1f};
    Vec3 worldUp = {0.0f, 1.0f, 0.0f};

    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.3f, 0.1f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);
        float angleTime = glfwGetTime();

        mat4 cube = M_Translate(0.25f, 0.6f, 0.0f);
        cube = M_MulMatrix(cube, M_Rotate_Z(angleTime / 2.0f));
        cube = M_MulMatrix(cube, M_Rotate_X(angleTime / 2.0f));
        cube = M_MulMatrix(cube, M_Scale(0.5f, 0.6f, 1.0f));

        mat4 view = M_LookAt(cameraPos, cameraTarget, worldUp);
        view = M_MulMatrix(view, M_Rotate_Y(45.0f));

        float fov = 45.0f * (3.14159265f / 180.0f);
        float aspect = (float)RESOLUTION_X/RESOLUTION_Y;
        mat4 projection = M_Perspective(fov, aspect, 0.1f, 100.0f);

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view.m[0]);

        glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection.m[0]);

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &cube.m[0]);

        glBindVertexArray(VAO);

        glUniform3f(colorLoc, 0.0f, 0.0f, 0.0f);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        //second cube
        mat4 cube2 = M_Translate(2.25f, 1.5f, -5.0f);
        cube2 = M_MulMatrix(cube2, M_Rotate_Z(angleTime / -2.0f));
        cube2 = M_MulMatrix(cube2, M_Rotate_X(angleTime / 2.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &cube2.m[0]);
        glUniform3f(colorLoc, 0.25f, .0f, 0.0f);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        //third cube
        mat4 cube3 = M_Translate(0.25f, 1.5f, 0.0f);
        cube3 = M_MulMatrix(cube3, M_Rotate_Z(angleTime / -2.0f));
        cube3 = M_MulMatrix(cube3, M_Rotate_X(angleTime / -2.0f));
        cube3 = M_MulMatrix(cube3, M_Scale(0.25f, 0.25f, 0.25f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &cube3.m[0]);
        glUniform3f(colorLoc, 1.f, 1.f, 1.0f);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

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

mat4 M_LookAt(Vec3 position, Vec3 target, Vec3 worldUp)
{
    mat4 c = M_Identity();

    //forward
    Vec3 forward;
    forward.x = position.x - target.x;
    forward.y = position.y - target.y;
    forward.z = position.z - target.z;
    float forwardRoot = sqrt(forward.x * forward.x + forward.y * forward.y + forward.z * forward.z);
    forward.x = forward.x /forwardRoot;
    forward.y = forward.y /forwardRoot;
    forward.z = forward.z /forwardRoot;

    c.m[2] = forward.x;
    c.m[6] = forward.y;
    c.m[10] = forward.z;

    //right
    Vec3 defualtUp = {0, 1, 0};
    Vec3 right = get_crossProduct(forward, defualtUp);
    float rightRoot = sqrt(right.x * right.x + right.y * right.y + right.z * right.z);

    right.x = right.x / rightRoot;
    right.y = right.y / rightRoot;
    right.z = right.z / rightRoot;

    c.m[0] = right.x;
    c.m[4] = right.y;
    c.m[8] = right.z;

    //up
    Vec3 up = get_crossProduct(right, forward);
    float upRoot = sqrt(up.x * up.x + up.y * up.y + up.z * up.z);
    up.x = up.x /upRoot;
    up.y = up.y /upRoot;
    up.z = up.z /upRoot;

    c.m[1] = up.x;
    c.m[5] = up.y;
    c.m[9] = up.z;

    mat4 translation = M_Translate(-position.x, -position.y, -position.z);
    return M_MulMatrix(c, translation);
}

float *Create_Sphere()
{
    float *sphere = malloc(sizeof(float) * 12);
    float b;
    float x;
    float z;
    float angle = 2.0f * 3.14159265f;
    int n = 4;
    for(int i = 0; i <= n - 1; i++)
    {
        b = (float)(i * (angle / n));
        x = sin(b);
        z = cos(b);
        sphere[i * 3 + 0] = x;
        sphere[i * 3 + 1] = 0;
        sphere[i * 3 + 2] = z;
    }
    return sphere;
}
