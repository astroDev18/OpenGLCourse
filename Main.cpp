#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
// Declare our vertex shader
const char* vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"   FragColor = vec4(1.0f, 0.2f, 0.2f, 1.0f);\n"

	"}\n\0";


int main()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	// Error check if the window fails to create
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGLCourse", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, 800, 800);


	// Build and compile our Shader program
	// <----------------------------------->
	// Declare a shader object
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// Attatch the shader source code to the shader object and compile the shader
	// Shader to compile; Number of Strings; Source code of vertex shader (stores as variable); Length (Null here);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Verify successful compilation
	// Int to indicate success
	int success;
	// Container for error messages
	char infoLog[512];
	// use glGetShaderiv to see if compilation is successful
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	// Error message, retrieved with glGetShaderInfoLog
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Compile a fragment shader
	// Similar process, but using a GL_FRAGMENT_SHADER as shader type 
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Error check
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Shader Program
	// It is the final linked version of multiple shaders combined
	// Activate it when rendering objects at render calls
	unsigned int shaderProgram;
	// Creates a program and returns the ID reference to the newly created program object
	shaderProgram = glCreateProgram();
	// Attatch previously compiled shaders to the program object and then link with glLinkProgram
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	};

	// Delete shaders, no longer needed after linking
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);



	// Specify the color of the background
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	// Clean the back buffer and assign the new color to it
	glClear(GL_COLOR_BUFFER_BIT);
	// Swap the back buffer with the front buffer
	glfwSwapBuffers(window);

	// Storing vertex data within memory on the graphics card by setting up Vertex data Buffer
	// <------------------------------------------------------>
	// Input Vertex Data, handled in 3d x, y, and z.
	

	float vertices[] = {
		// first triangle
		-0.9f, -0.5f, 0.0f,  // left 
		-0.0f, -0.5f, 0.0f,  // right
		-0.45f, 0.5f, 0.0f,  // top 
		// second triangle
		 0.0f, 0.5f, 0.0f,  // left
		 0.9f, 0.5f, 0.0f,  // right
		 0.45f, -0.5f, 0.0f   // top 
	};

	unsigned int indices[] = {
		0, 1, 3, // First triangle
		1, 2, 3  // Second triangle
	};

	// VBO is used to store large amounts of data to be sent to the GPU's memory
	// without having to send each vertex one at a time
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VBO);
	// Bind our VAO, then bind and set vertex buffer, and then config vertex attributes
	glBindVertexArray(VAO);

	// Bind the buffer to GL_ARRAY_BUFFER with glBindBuffer
	// Copies our vertices array into a vertex buffer for OpenGL's use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Function copies previously defined vertex data into the currently bound buffer's memory
	// Type of buffer; size of data; data to send; graphics management type, here static as the data will not change;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// set vertix data interpretation before render
	// 1. Vertix attribute to configure (0); 2. size of attribute (vec3); 3. data type (float); normalization (false); 
	// 4. Stride, which is space between vertix attributes (3), 5. Data position offset (void);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Key Input
		processInput(window);

		// Render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT);

		// Draw our first triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);

		// Render elements from our index buffer
		// 1. Drawing mode; 2. Count of elements to draw; 3. Type of indices; 4. Offset in EBO;
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Take care of all GLFW events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}