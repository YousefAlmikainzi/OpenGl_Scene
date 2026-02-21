#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define RESOLUTION 1000

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
mat4 M_Rotate_Z(float angle);

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
    GLFWwindow *window = glfwCreateWindow(RESOLUTION, RESOLUTION, "OpenGlScene", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((void*)glfwGetProcAddress);
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

    unsigned int VAO, VBO;

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

    int modelLoc = glGetUniformLocation(shaderProgram, "model");
    int viewLoc = glGetUniformLocation(shaderProgram, "view");
    int projLoc = glGetUniformLocation(shaderProgram, "projection");

    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.3f, 0.1f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        float angleTime = glfwGetTime();

        mat4 model = M_Translate(0.25f, 0.6f, 0.0f);
        model = M_MulMatrix(model, M_Rotate_Z(angleTime * 2.0f));
        model = M_MulMatrix(model, M_Scale(0.5f, 0.6f, 1.0f));

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model.m[0]);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

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
    mat4 S = {0};

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
