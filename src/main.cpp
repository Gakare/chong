#include <GL/gl.h>
#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <glm/glm.hpp>

#define width 800u
#define height 600u
#define paddle_speed 0.01f
#define ball_speed 0.01f
#define title "cppong"

void framebuffer_size_callback(GLFWwindow *, int, int);
void rect_move1(GLFWwindow *, float *);
void rect_move2(GLFWwindow *, float *);
void check_paddle_bound(float *);
void close_game(GLFWwindow *);
unsigned int compile_shader(const char *, const char *);
void check_shader(const unsigned int *, const char *);
void ball_movement(float *, float *, float *, float *);
void initialize_ball(float *, float *);

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window = glfwCreateWindow(width, height, title, NULL, NULL);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if (!window) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glViewport(0, 0, width, height);

    // clang-format off
    const char *vss1 =
        "#version 330 core\n"
        "uniform float y_dir1;\n"
        "layout (location = 0) in vec3 pos1;\n"
        "void main() {\n"
        "gl_Position = vec4(pos1.x, pos1.y + y_dir1, pos1.z, 1.0f);\n"
        "}\0";

     const char *fss1 =
         "#version 330 core\n"
         "out vec4 fc1;\n"
         "void main() {\n"
         "fc1 = vec4(0.5f, 0.8f, 1.0f, 1.0f);\n"
         "}\0";
    // clang-format on

    unsigned int sp1 = compile_shader(vss1, fss1);

    // clang-format off
    float left_rect[12] = {
         -0.8f,   0.2f,   0.0f, // top right
         -0.85f,   0.2f,   0.0f, // top left
         -0.8f,  -0.15f,  0.0f, // bottom right
         -0.85f,  -0.15f,  0.0f  // bottom left
    };

    float right_rect[12] = {
         0.8f,   0.2f,  0.0f, // top right
         0.85f,   0.2f,  0.0f, // top left
         0.8f,  -0.15f, 0.0f, // bottom right
         0.85f,  -0.15f, 0.0f, // bottom left
    };

    unsigned int ind[6] = {
        0, 1, 3, // first triangle
        3, 2, 0  // second triangle
    };
    // clang-format on

    /* These are for the rectangles */
    unsigned int VAO[2], VBO[2], EBO[2];
    glGenVertexArrays(2, VAO);
    glGenBuffers(2, VBO);
    glGenBuffers(2, EBO);

    /* Left Rectangle Setup */
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, left_rect,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, ind,
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // clang-format off
    const char *vss2 =
        "#version 330 core\n"
        "uniform float y_dir2;\n"
        "layout (location = 0) in vec3 pos2;\n"
        "void main() {\n"
        "gl_Position = vec4(pos2.x, pos2.y + y_dir2, pos2.z, 1.0f);\n"
        "}\0";

    const char *fss2 =
        "#version 330 core\n"
        "out vec4 fc2;\n"
        "void main() {\n"
        "fc2 = vec4(1.0f, 0.7f, 0.7f, 1.0f);\n"
        "}\0";
    // clang-format on

    unsigned int sp2 = compile_shader(vss2, fss2);

    /* Right Rectangle Setup */
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, right_rect,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, ind,
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    /* Ball setup */
    // clang-format off
    float ball_vertices[12] = {
        -0.02f,  0.02f, 0.0f, // top left
         0.02f,  0.02f, 0.0f, // top right
        -0.02f, -0.02f, 0.0f, // bottom left
         0.02f, -0.02f, 0.0f, // bottom right
    }; 

     unsigned int ball_indices[6] = {
        0, 1, 2,
        1, 2, 3
    };

    const char *ball_vss =
        "#version 330 core\n"
        "uniform float ball_x;\n"
        "uniform float ball_y;\n"
        "layout (location = 0) in vec3 pos;\n"
        "void main() {\n"
        "gl_Position = vec4(pos.x + ball_x, pos.y + ball_y, pos.z, 1.0f);\n"
        "}\0";

     const char *ball_fss =
         "#version 330 core\n"
         "out vec4 fc1;\n"
         "void main() {\n"
         "fc1 = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
         "}\0";
    // clang-format on

    unsigned int ball_sp = compile_shader(ball_vss, ball_fss);

    unsigned int ball_vao, ball_vbo, ball_ebo;
    glGenVertexArrays(1, &ball_vao);
    glGenBuffers(1, &ball_vbo);
    glGenBuffers(1, &ball_ebo);

    glBindVertexArray(ball_vao);
    glBindBuffer(GL_ARRAY_BUFFER, ball_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, ball_vertices,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ball_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6,
                 ball_indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    float y_dir1 = 0.0f;
    float y_dir2 = 0.0f;

    float ball_x = 0.0f;
    float ball_y = 0.0f;
    float ball_vx = ball_speed;
    float ball_vy = ball_speed;

    initialize_ball(&ball_vx, &ball_vy);
    while (!glfwWindowShouldClose(window)) {
        close_game(window);
        glfwSwapBuffers(window);
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(sp1);
        rect_move1(window, &y_dir1);
        check_paddle_bound(&y_dir1);

        glUniform1f(glGetUniformLocation(sp1, "y_dir1"), y_dir1);
        glBindVertexArray(VAO[0]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glUseProgram(sp2);
        rect_move2(window, &y_dir2);
        check_paddle_bound(&y_dir2);

        glUniform1f(glGetUniformLocation(sp2, "y_dir2"), y_dir2);
        glBindVertexArray(VAO[1]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glUseProgram(ball_sp);
        ball_movement(&ball_x, &ball_y, &ball_vx, &ball_vy);

        glUniform1f(glGetUniformLocation(ball_sp, "ball_x"), ball_x);
        glUniform1f(glGetUniformLocation(ball_sp, "ball_y"), ball_y);
        glBindVertexArray(ball_vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glFlush();
    }

    glDeleteVertexArrays(2, VAO);
    glDeleteBuffers(2, VBO);
    glDeleteBuffers(2, EBO);
    glDeleteProgram(sp1);

    glfwDestroyWindow(window);
    glfwTerminate();

    exit(0);
}

void framebuffer_size_callback(GLFWwindow *w, int wid, int hei) {
    glViewport(0, 0, wid, hei);
}

void rect_move1(GLFWwindow *w, float *y_dir) {
    if (glfwGetKey(w, GLFW_KEY_K) == GLFW_PRESS) {
        *y_dir += paddle_speed;
    }
    if (glfwGetKey(w, GLFW_KEY_J) == GLFW_PRESS) {
        *y_dir -= paddle_speed;
    }
}

void rect_move2(GLFWwindow *w, float *y_dir) {
    if (glfwGetKey(w, GLFW_KEY_UP) == GLFW_PRESS) {
        *y_dir += paddle_speed;
    }
    if (glfwGetKey(w, GLFW_KEY_DOWN) == GLFW_PRESS) {
        *y_dir -= paddle_speed;
    }
}

void close_game(GLFWwindow *w) {
    if (glfwGetKey(w, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(w, 1);
    }
}

unsigned int compile_shader(const char *vss, const char *fss) {
    unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vss, NULL);
    glCompileShader(vs);
    check_shader(&vs, "VERTEX_SHADER");

    unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fss, NULL);
    glCompileShader(fs);
    check_shader(&fs, "FRAGMENT_SHADER");

    unsigned int sp;
    sp = glCreateProgram();
    glAttachShader(sp, vs);
    glAttachShader(sp, fs);
    glLinkProgram(sp);

    check_shader(&sp, "PROGRAM_SHADER");

    glUseProgram(sp);
    glDeleteShader(vs);
    glDeleteShader(fs);
    return sp;
}

void check_shader(const unsigned int *shader, const char *name) {
    int success;
    char log[512];

    glGetShaderiv(*shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(*shader, 512, NULL, log);
        printf("ERROR::SHADER::%s::COMPILATION_FAILED\n%s", name, log);
    }
}

void check_paddle_bound(float *y_dir) {
    if (*y_dir > 1)
        *y_dir = -1;
    if (*y_dir < -1)
        *y_dir = 1;
}

void ball_movement(float *x, float *y, float *vx, float *vy) {
    if (*y <= -1 || *y >= 1) {
        *vy = -(*vy);
    }
    if (*x <= -1 || *x >= 1) {
        *vx = -(*vx);
    }
    *y += *vy;
    *x += *vx;
}

void initialize_ball(float *vx, float *vy) {
    srand(static_cast<unsigned int>(time(0)));

    if (rand() % 2 == 0) {
        *vx = ball_speed;
        *vy = ball_speed;
    } else {
        *vx = -ball_speed;
        *vy = -ball_speed;
    }
}
