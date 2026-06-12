//
// Created by kadx on 01/05/26.
//

#include "glad.h"
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <sstream>

struct ShaderProgramSource {
  std::string VertexSource;
  std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string &filepath) {
  std::ifstream stream(filepath);

  enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };

  std::string line;
  std::stringstream ss[2];
  ShaderType type = ShaderType::NONE;

  while (getline(stream, line)) {
    if (line.find("#shader") != std::string::npos) {
      if (line.find("vertex") != std::string::npos) {
        type = ShaderType::VERTEX;
      } else if (line.find("fragment") != std::string::npos) {
        type = ShaderType::FRAGMENT;
      }
    } else {
      ss[(int)type] << line << '\n';
    }
  }

  return {ss[0].str(), ss[1].str()};
}

unsigned int ShaderProgram(const unsigned int vertexShader,
                           const unsigned int fragmentShader) {
  const unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  glValidateProgram(shaderProgram);

  // check for linking errors
  int success;
  char infoLog[512];
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, 0, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
  }

  return shaderProgram;
}

unsigned int CompileShader(const unsigned int type, const std::string &source) {
  const unsigned int newShader = glCreateShader(type);
  const char *src = source.c_str();
  glShaderSource(newShader, 1, &src, nullptr);
  glCompileShader(newShader);

  // check for shader compile errors
  int success;
  glGetShaderiv(newShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    int length;
    glGetShaderiv(newShader, GL_INFO_LOG_LENGTH, &length);
    char infoLog[length];
    glGetShaderInfoLog(newShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  return newShader;
}

unsigned int CreateShader(const std::string &vertexShaderSource,
                          const std::string &fragmentShaderSource) {
  const unsigned int vertexShader =
      CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
  const unsigned int fragmentShader =
      CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
  const unsigned int shaderProgram = ShaderProgram(vertexShader, fragmentShader);

  // delete the shaders as they're linked into our program and no longer
  // necessary
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return shaderProgram;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

int main() {
  // glfw: initialize and configure
  // ------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // glfw window creation
  // --------------------
  GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // build and compile our shader program
  // ------------------------------------
  auto shaderSource = ParseShader("res/shaders/Basic.shader");
  unsigned int shaderProgram =
      CreateShader(shaderSource.VertexSource, shaderSource.FragmentSource);

  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  const float vertices[] = {
      0.5f,  0.5f,  0.0f, // top right
      0.5f,  -0.5f, 0.0f, // bottom right
      -0.5f, 0.5f,  0.0f, // top left
      -0.5f, -0.5f, 0.0f, // bottom left
  };
  const unsigned int indices[] = {0, 1, 2, 0, 3, 2};
  unsigned int VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  // bind the vertex array object first, then bind and set the vertex buffer(s),
  // and then configure vertex attributes(s).
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  // set up vertex attribute pointers
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // safely unbind since we've already configured the VAO
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // wireframe polygons
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // render loop
  // -----------
  while (!glfwWindowShouldClose(window)) {
    // input
    processInput(window);

    // rendering commands here
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // draw a triangle
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // check and call events and swap the buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteProgram(shaderProgram);

  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();

  return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

// process all input: query GLFW whether relevant keys are pressed/released this
// frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}
