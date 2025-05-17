#include "config.h"
#include <iostream>
#include <cmath>
#include <ctime>

using namespace std;

unsigned int make_module(const std::string& filepath, unsigned int module_type);
unsigned int make_shader(const std::string& vertex_filepath, const std::string& fragment_filepath);
void gravity(float* vertices, int numParameters, int numTriangles, float* speed);
void isEdge(float* vertices, int numParameters, int numTriangles, float* speed);

int main(){
    GLFWwindow* window;
    
    if(!glfwInit()){
        std::cout<<"GLFW could not start";
    }
    
    window = glfwCreateWindow(640, 640, "MyWindow", NULL, NULL);
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        glfwTerminate();
        return -1;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    unsigned int shader = make_shader("../src/shaders/vertex.txt", "../src/shaders/fragment.txt");
    
    int numTriangles = 3;
    int numParameters = 6;

    int verticesSpace = numTriangles * 18;

    float* vertices = new float[18*numTriangles] {
            -0.5f, 0.9f, 0.0f,  1.0f, 0.0f, 0.0f,
            -0.6f, 0.5f, 0.0f,   0.0f, 1.0f, 0.0f,
            -0.4f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,

            0.2f, 0.75f, 0.0f,   1.0f, 0.0f, 0.0f,
            0.2f, 0.2f, 0.0f,   0.0f, 1.0f, 0.0f,
            0.8f,  0.2f, 0.0f,   0.0f, 0.0f, 1.0f,
            
            0.2f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
            0.0f, 0.2f, 0.0f,   0.0f, 0.0f, 1.0f
        };

    float* speeds = new float[numTriangles]{
        0.0001f,
        0.0001f,
        0.0001f
    };

    unsigned int VBO, VAO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticesSpace * sizeof(float), vertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        
        //float time = glfwGetTime();   

        isEdge(vertices, numParameters, numTriangles, speeds);

        gravity(vertices, numParameters, numTriangles, speeds);
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, verticesSpace * sizeof(float), vertices);
        
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shader);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 9);
        glfwSwapBuffers(window);

    }
    
    glDeleteProgram(shader);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

void isEdge(float* vertices, int numParameters, int numTriangles, float* speed){

    for(int i = 0; i<numTriangles; i++){
        for(int z = 1 + (18*i); z < (i*18)+(numParameters*3) +1; z+=6){
            if(vertices[z]<= -1.0f){
                speed[i] = 0.0f;
            }
        }
    }
}

unsigned int make_shader(const std::string& vertex_filepath, const std::string& fragment_filepath){
    std::vector<unsigned int> modules;

        modules.push_back(make_module(vertex_filepath, GL_VERTEX_SHADER));
        modules.push_back(make_module(fragment_filepath, GL_FRAGMENT_SHADER));
        
        unsigned int shader = glCreateProgram();
        
        for (unsigned int shaderModule : modules){
            glAttachShader(shader, shaderModule);
        }

        glLinkProgram(shader);
        
        int success;
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if(!success){
            char error[1024];
            glGetProgramInfoLog(shader, 1024, NULL, error);
            std::cout<<error<<std::endl;
        }

        for (unsigned int shaderModule : modules){
            glDeleteShader(shaderModule);
        }
        return shader;
}

void gravity(float* vertices, int numParameters, int numTriangles, float* speed){
    for(int i = 0; i< numTriangles; i++){
        if(speed[i] != 0.0f){
            float acc = 0.00001f;
            float calculo = speed[i] -acc;
            
            speed[i] = calculo;

            for(int z=1 +(18*i); z < (18*i) + (numParameters*3) +1; z+=6){
                vertices[z] = vertices[z] + (speed[i]);
            }

        }
    }
}

unsigned int make_module(const std::string& filepath, unsigned int module_type){
    std::ifstream file;
    std::stringstream bufferedLines;
    std::string line;

    file.open(filepath);

    while(std::getline(file, line)){
        bufferedLines << line << "\n";
    }

    std::string shaderSource = bufferedLines.str();
    const char *shaderSrc = shaderSource.c_str();
    file.close();

    unsigned int shaderModule = glCreateShader(module_type);

    glShaderSource(shaderModule, 1, &shaderSrc, NULL);
    glCompileShader(shaderModule);

    int success;
    glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &success);

    if (!success) {
        char error[1024];
        glGetShaderInfoLog(shaderModule, 1024, NULL, error);
        std::cout << "Shader compilation error (" << filepath << "):\n" << error << std::endl;
    }

    return shaderModule;
}
