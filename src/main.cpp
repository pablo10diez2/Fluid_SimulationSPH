#include "Vector.h"
#include "config.h"

using namespace std;

unsigned int make_module(const std::string& filepath, unsigned int module_type);
unsigned int make_shader(const std::string& vertex_filepath, const std::string& fragment_filepath);
int main(){
    GLFWwindow *window;
    
    if(!glfwInit()){
        std::cout<<"GLFW could not start";
    }
    
    window = glfwCreateWindow(640, 480, "MyWindow", NULL, NULL);
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        glfwTerminate();
        return -1;
    }

    glClearColor(0.25f, 0.5f, 0.75f, 1.0f);

    unsigned int shader = make_shader("../src/shaders/vertex.txt", "../src/shaders/fragment.txt");

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shader);
        glfwSwapBuffers(window);
    }
    
    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
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

    if(!success){
        char error[1024];
        glGetShaderInfoLog(shaderModule, 1024, NULL, error);
        std::cout<<glGetShaderInfoLog<<std::endl;
    }
    return shaderModule;
}





