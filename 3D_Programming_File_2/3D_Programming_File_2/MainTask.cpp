//#include <iostream>
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include <glm.hpp>
//#include <fstream>
//#include <sstream>
//#include <vector>
//
//// Window dimensions
//const unsigned int SCR_WIDTH = 800;
//const unsigned int SCR_HEIGHT = 600;
//
//// Camera
//struct Camera {
//    glm::vec3 position;
//    glm::vec3 front;
//    glm::vec3 up;
//    glm::mat4 viewMatrix;
//    glm::mat4 projectionMatrix;
//};
//
//// Shader program sources
//const char* vertexShaderSource = R"(
//    #version 330 core
//    layout (location = 0) in vec3 aPos;
//    layout (location = 1) in vec3 aNormal;
//    layout (location = 2) in vec2 aTexCoord;
//
//    out vec3 FragPos;
//    out vec3 Normal;
//    out vec2 TexCoord;
//
//    uniform mat4 model;
//    uniform mat4 view;
//    uniform mat4 projection;
//
//    void main() {
//        FragPos = vec3(model * vec4(aPos, 1.0));
//        Normal = mat3(transpose(inverse(model))) * aNormal;
//        TexCoord = aTexCoord;
//        
//        gl_Position = projection * view * vec4(FragPos, 1.0);
//    }
//)";
//
//const char* fragmentShaderSource = R"(
//    #version 330 core
//    out vec4 FragColor;
//
//    struct Material {
//        sampler2D diffuse;
//        sampler2D specular;
//        float shininess;
//    }; 
//    
//    struct Light {
//        vec3 position;
//        vec3 ambient;
//        vec3 diffuse;
//        vec3 specular;
//    };
//
//    uniform vec3 viewPos;
//    uniform Material material;
//    uniform Light light;
//
//    in vec3 FragPos;
//    in vec3 Normal;
//    in vec2 TexCoord;
//
//    void main() {
//        // Ambient
//        vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;
//        
//        // Diffuse 
//        vec3 norm = normalize(Normal);
//        vec3 lightDir = normalize(light.position - FragPos);
//        float diff = max(dot(norm, lightDir), 0.0);
//        vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoord).rgb;
//        
//        // Specular
//        vec3 viewDir = normalize(viewPos - FragPos);
//        vec3 reflectDir = reflect(-lightDir, norm);  
//        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//        vec3 specular = light.specular * spec * texture(material.specular, TexCoord).rgb;  
//        
//        vec3 result = ambient + diffuse + specular;
//        FragColor = vec4(result, 1.0);
//    }
//)";
//
//// Global variables for shader programs
//GLuint shaderProgram;
//
//// Camera
//Camera camera;
//
//// Player
//glm::vec3 playerPosition(0.0f, 0.0f, 0.0f);
//float playerSpeed = 0.1f;
//
//// NPC
//glm::vec3 npcPosition(2.0f, 0.0f, 0.0f);
//float npcSpeed = 0.05f;
//bool useInterpolation = true;
//
//// Door
//bool doorOpen = false;
//
//// Function prototypes
//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void processInput(GLFWwindow* window);
//void loadNPCPath();
//void updateNPCPosition();
//void renderScene();
//
//// Function to read NPC path from a text file
//std::vector<glm::vec3> readPathFromFile(const std::string& filename) {
//    std::ifstream file(filename);
//    std::vector<glm::vec3> path;
//
//    if (file.is_open()) {
//        std::string line;
//        while (std::getline(file, line)) {
//            std::istringstream iss(line);
//            float x, y, z;
//            if (!(iss >> x >> y >> z)) {
//                std::cerr << "Error parsing line in file: " << filename << std::endl;
//                continue;
//            }
//            path.push_back(glm::vec3(x, y, z));
//        }
//        file.close();
//    }
//    else {
//        std::cerr << "Failed to open file: " << filename << std::endl;
//    }
//
//    return path;
//}
//
//// Global variable to hold NPC path
//std::vector<glm::vec3> npcPath;
//
//// Load NPC path from file
//void loadNPCPath() {
//    npcPath = readPathFromFile("interpolated_points.txt"); // Update file name with your file containing the NPC path
//}
//
//int main() {
//    // Initialize GLFW
//    glfwInit();
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//    // Create a windowed mode window and its OpenGL context
//    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Simple 3D Game", NULL, NULL);
//    if (window == NULL) {
//        std::cout << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//        return -1;
//    }
//    glfwMakeContextCurrent(window);
//    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//
//    // Initialize GLAD before calling any OpenGL function
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
//        std::cout << "Failed to initialize GLAD" << std::endl;
//        return -1;
//    }
//
//    // Enable depth testing
//    glEnable(GL_DEPTH_TEST);
//
//    // Set up camera
//    camera.position = glm::vec3(0.0f, 0.0f, 3.0f);
//    camera.front = glm::vec3(0.0f, 0.0f, -1.0f);
//    camera.up = glm::vec3(0.0f, 1.0f, 0.0f);
//    camera.viewMatrix = glm::lookAt(camera.position, camera.position + camera.front, camera.up);
//    camera.projectionMatrix = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//
//    // Load NPC path from file
//    loadNPCPath();
//
//    // Game loop
//    while (!glfwWindowShouldClose(window)) {
//        // Input
//        processInput(window);
//
//        // Update NPC position
//        updateNPCPosition();
//
//        // Rendering
//        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//        // Set up camera
//        glm::mat4 view = glm::lookAt(camera.position, camera.position + camera.front, camera.up);
//        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//
//        // Pass view and projection matrices to shaders
//        // shader.setMat4("view", view);
//        // shader.setMat4("projection", projection);
//
//        // Render scene
//        renderScene();
//
//        // Swap buffers and poll IO events
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//    // Terminate GLFW
//    glfwTerminate();
//    return 0;
//}
//
//// Framebuffer size callback
//void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
//    glViewport(0, 0, width, height);
//}
//
//// Process input
//void processInput(GLFWwindow* window) {
//    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, true);
//
//    // Player movement
//    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//        playerPosition.y += playerSpeed;
//    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//        playerPosition.y -= playerSpeed;
//    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//        playerPosition.x -= playerSpeed;
//    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//        playerPosition.x += playerSpeed;
//
//    // Door interaction
//    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
//        doorOpen = true;
//
//    // NPC movement switch
//    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//        useInterpolation = !useInterpolation;
//}
//
//// Update NPC position using loaded path
//void updateNPCPosition() {
//    // If the NPC path is loaded and not empty
//    if (!npcPath.empty()) {
//        // Implement logic to move NPC along the path
//    }
//    else {
//        std::cerr << "NPC path is not loaded!" << std::endl;
//    }
//}
//
//// Function to create shader program
//void createShaderProgram() {
//    // Vertex shader
//    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
//    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
//    glCompileShader(vertexShader);
//
//    // Fragment shader
//    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
//    glCompileShader(fragmentShader);
//
//    // Shader program
//    shaderProgram = glCreateProgram();
//    glAttachShader(shaderProgram, vertexShader);
//    glAttachShader(shaderProgram, fragmentShader);
//    glLinkProgram(shaderProgram);
//
//    // Delete shaders
//    glDeleteShader(vertexShader);
//    glDeleteShader(fragmentShader);
//}
//
//// Render function for substrate
//void renderSubstrate() {
//    // Define vertices for the substrate (a simple plane)
//    GLfloat substrateVertices[] = {
//        // Positions          // Normals         // Texture coords
//        1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,  1.0f, 1.0f,   // Top right
//        1.0f, 0.0f, -1.0f,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f,   // Bottom right
//        -1.0f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,   // Bottom left
//        -1.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,  0.0f, 1.0f    // Top left 
//    };
//    GLuint substrateIndices[] = {
//        0, 1, 3,   // First triangle
//        1, 2, 3    // Second triangle
//    };
//
//    // Bind VAO, VBO, EBO for substrate
//    GLuint VAO, VBO, EBO;
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//    glGenBuffers(1, &EBO);
//
//    glBindVertexArray(VAO);
//
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(substrateVertices), substrateVertices, GL_STATIC_DRAW);
//
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(substrateIndices), substrateIndices, GL_STATIC_DRAW);
//
//    // Set vertex position attribute
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
//    glEnableVertexAttribArray(0);
//    // Set vertex normal attribute
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
//    glEnableVertexAttribArray(1);
//    // Set vertex texture coordinate attribute
//    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
//    glEnableVertexAttribArray(2);
//
//    // Load and bind texture for the substrate
//    GLuint substrateTexture;
//    glGenTextures(1, &substrateTexture);
//    glBindTexture(GL_TEXTURE_2D, substrateTexture);
//    // Set texture wrapping parameters
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    // Set texture filtering parameters
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    // Load texture image
//    int width, height, nrChannels;
//    unsigned char* data = stbi_load("substrate_texture.jpg", &width, &height, &nrChannels, 0);
//    if (data) {
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//        glGenerateMipmap(GL_TEXTURE_2D);
//    }
//    else {
//        std::cerr << "Failed to load texture" << std::endl;
//    }
//    stbi_image_free(data);
//
//    // Use shader program
//    glUseProgram(shaderProgram);
//    // Set up textures, lighting, and other parameters
//    // Bind texture
//    glBindTexture(GL_TEXTURE_2D, substrateTexture);
//    // Draw substrate
//    glBindVertexArray(VAO);
//    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//
//    // Cleanup
//    glBindVertexArray(0);
//    glDeleteVertexArrays(1, &VAO);
//    glDeleteBuffers(1, &VBO);
//    glDeleteBuffers(1, &EBO);
//}
//
//
//// Render function for player
//void renderPlayer() {
//    // Render the player
//    // Use shader program
//    glUseProgram(shaderProgram);
//    // Set up textures, lighting, and other parameters
//    // Draw player
//}
//
//// Render function for trophies
//void renderTrophies() {
//    // Render the trophies
//    // Use shader program
//    glUseProgram(shaderProgram);
//    // Set up textures, lighting, and other parameters
//    // Draw trophies
//}
//
//// Render function for house
//void renderHouse() {
//    // Render the house
//    // Use shader program
//    glUseProgram(shaderProgram);
//    // Set up textures, lighting, and other parameters
//    // Draw house
//}
//
//// Render function for door
//void renderDoor() {
//    // Render the door
//    // Use shader program
//    glUseProgram(shaderProgram);
//    // Set up textures, lighting, and other parameters
//    // Draw door
//}
//
//// Render function for NPC
//void renderNPC() {
//    // Render the NPC
//    // Use shader program
//    glUseProgram(shaderProgram);
//    // Set up textures, lighting, and other parameters
//    // Draw NPC
//}
//
//// Render function for indoor scene if doorOpen
//void renderIndoorScene() {
//    // Render indoor scene if doorOpen
//    // Use shader program
//    glUseProgram(shaderProgram);
//    // Set up textures, lighting, and other parameters
//    // Draw indoor scene
//}
//
//// Render function
//void render(const string& title, const Camera& camera) {
//    // Clear the screen
//    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//    // Render each element
//    renderSubstrate();
//    renderPlayer();
//    renderTrophies();
//    renderHouse();
//    renderDoor();
//    renderNPC();
//    if (doorOpen) {
//        renderIndoorScene();
//    }
//
//    // Swap buffers
//    glfwSwapBuffers(window);
//}
//
//
//// Render scene
//void renderScene() {
//    // Render substrate or hill
//    // Render player
//    // Render trophies
//    // Render house
//    // Render door
//    // Render NPC
//    // Render indoor scene if doorOpen
//    // Add lighting, textures, and shaders
//    // 
//    // Draw all objects using OpenGL primitives or models
//}
