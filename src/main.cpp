#include "config.h"
#include <iostream>
#include <ctime>
#include <cmath>
#include <glm/glm.hpp>
#include <vector>

unsigned int make_module(const std::string& filepath, unsigned int module_type);
unsigned int make_shader(const std::string& vertex_filepath, const std::string& fragment_filepath);
void gravity(std::vector<float>& vertices, int numTriangles, int numCircles, std::vector<float>& speeds);
void isEdge(std::vector<float>& vertices, int numTriangles, int numCircles, std::vector<float>& speeds);
void buildCircle(float radius, int count);

std::vector<float> vertices;
std::vector<float> speeds;

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
    
    int numTriangles = 38;

    buildCircle(0.1, numTriangles);

    unsigned int VBO, VAO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*6*vertices.size(), &vertices[0], GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        
        int numTriangles = 36;
        int numCircles = 1;
        
        isEdge(vertices, numTriangles, numCircles, speeds);
        gravity(vertices, numTriangles, numCircles, speeds);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*6*vertices.size(), &vertices[0]);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shader);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        glfwSwapBuffers(window);
    }

    glDeleteProgram(shader);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

void buildCircle(float radius, int count){
    float angle = 360.0f/count;
    int triangleCount = count-2;

    std::vector<glm::vec3> temp;

    speeds.insert(vertices.end(), -0.001f);

    for(int i=0; i<count; i++){
        float currentAngle = angle*i;
        float x = radius * cos(glm::radians(currentAngle));
        float y = radius * sin(glm::radians(currentAngle));
        float z = 0.0f;

        temp.push_back(glm::vec3(x,y,z));

        std::cout<<"x:"<<x<<" y:"<<y<<std::endl;

    }
    for(int i=0; i < triangleCount; i++){
        glm::vec3 v0 = temp[0];
        glm::vec3 v1 = temp[i+1];
        glm::vec3 v2 = temp[i+2];

        vertices.insert(vertices.end(), {v0.x, v0.y, v0.z, 0.5f, 1.0f, 0.0f});
        vertices.insert(vertices.end(), {v1.x, v1.y, v1.z, 0.5f, 1.0f, 0.0f});
        vertices.insert(vertices.end(), {v2.x, v2.y, v2.z, 0.5f, 1.0f, 0.0f});
    }
}

void isEdge(std::vector<float>& vertices, int numTriangles, int numCircles, std::vector<float>& speeds){
    for(int i = 0; i< numCircles; i++){
        std::cout<<speeds[i]<<std::endl;

        for(int z=0; z<numTriangles*3; z++){
            int index = 1+ (i*18) + (z*6);

            if(vertices[index]<= -0.9999f){
                speeds[i] = 0.65*std::abs(speeds[i]);
                if(std::abs(speeds[i])<0.0001f){
                    speeds[i] = 0.0f;
                }
                break;
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

void gravity(std::vector<float>& vertices, int numTriangles, int numCircles, std::vector<float>& speeds){
    for(int i = 0; i< numCircles; i++){
        if(speeds[i] != 0.0f){
            float acc = 0.0001f;
            float calculo = speeds[i] -acc;
            std::cout<<calculo<<std::endl;

            speeds[i] = calculo;
            for(int z=0; z < numTriangles*3; z++){
                vertices[1+(i*18)+(z*6)] = vertices[1+(i*18)+(z*6)] + speeds[i];
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
