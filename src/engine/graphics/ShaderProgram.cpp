#include "ShaderProgram.h"

#include "engine/Exception.h"

#include <gl/glew.h>

#include <fstream>
#include <vector>
#include <string>

namespace MYENGINE
{

	std::vector<std::string> pGetCellsFromString(std::string _string, char _seperator, bool _addLeftovers = false)
	{
		std::vector<std::string> cells{};
		std::string cell = "";
		for (int i = 0; i < _string.length(); i++)
		{
			char ch = _string.at(i);
			if (ch == _seperator)
			{
				cells.push_back(cell);
				cell = "";
			}
			else if (ch != ' ')
			{
				cell.push_back(ch);
			}
		}
		if (_addLeftovers) { cells.push_back(cell); }
		return cells;
	}

	ShaderProgram::ShaderProgram(weak(ResourceManager) resourceManager, const char* _name)
		: Resource(resourceManager, _name)
	{
		std::string folderPath = "../../data/shaders/";
		folderPath.append(_name);
		folderPath.append("/");

		std::string vertPath = folderPath;
		vertPath.append("vert.txt");
		std::ifstream fileV;
		fileV.open(vertPath);
		if (!fileV.is_open()) { throw Exception("Error loading vertex shader file. The filepath was: " + vertPath); }
		std::string contentsV = "";
		std::string lineV = "";
		while (std::getline(fileV, lineV))
		{
			contentsV += lineV + "\n";
		}
		fileV.close();
		const char* vertexShaderSrc = contentsV.c_str();


		std::string fragPath = folderPath;
		fragPath.append("frag.txt");
		std::ifstream fileF;
		fileF.open(fragPath);
		if (!fileF.is_open()) { throw Exception("Error loading fragment shader file. The filepath was: " + fragPath); }
		std::string contentsF = "";
		std::string lineF = "";
		while (std::getline(fileF, lineF))
		{
			contentsF += lineF + "\n";
		}
		fileF.close();
		const char* fragmentShaderSrc = contentsF.c_str();

		// vertex shader
		GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShaderId, 1, &vertexShaderSrc, NULL);
		glCompileShader(vertexShaderId);
		GLint success = 0;
		glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);

		if (!success) {
			GLint maxLength = 0;
			glGetShaderiv(vertexShaderId, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(vertexShaderId, maxLength, &maxLength, &errorLog.at(0));
			std::cout << &errorLog.at(0) << std::endl;

			throw Exception("Vertex shader has failed to compile. The filepath was: " + vertPath);
		}

		// fragment shader
		GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShaderId, 1, &fragmentShaderSrc, NULL);
		glCompileShader(fragmentShaderId);
		glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);

		if (!success) {
			GLint maxLength = 0;
			glGetShaderiv(fragmentShaderId, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(fragmentShaderId, maxLength, &maxLength, &errorLog.at(0));
			std::cout << &errorLog.at(0) << std::endl;

			throw Exception("Fragment shader has failed to compile. The filepath was: " + fragPath);
		}

		// Create new shader program and attach our shader objects
		m_id = glCreateProgram();
		glAttachShader(m_id, vertexShaderId);
		glAttachShader(m_id, fragmentShaderId);


		std::string attrPath = folderPath;
		attrPath.append("attr.txt");
		std::ifstream fileA;
		fileA.open(attrPath);
		if (!fileA.is_open()) { throw Exception("Error loading shader attribute file. The filepath was: " + attrPath); }
		std::string lineA = "";
		std::getline(fileA, lineA);
		std::vector<std::string> attributes = pGetCellsFromString(lineA, '*', true);

		// Ensure the VAO "position" attribute stream gets set as the first position during the link.
		for (int i = 0; i < attributes.size(); i++)
		{
			glBindAttribLocation(m_id, i, attributes.at(i).c_str());
		}

		// Perform the link and check for failure
		glLinkProgram(m_id);
		glGetProgramiv(m_id, GL_LINK_STATUS, &success);

		if (!success) { throw Exception("Failed to link shader program. Shader name: " + std::string(GetName())); }

		// Detach and destroy the shader objects. These are no longer needed because we now have a complete shader program.
		glDetachShader(m_id, vertexShaderId);
		glDeleteShader(vertexShaderId);
		glDetachShader(m_id, fragmentShaderId);
		glDeleteShader(fragmentShaderId);
	}

	ShaderProgram::~ShaderProgram()
	{

	}

} // namespace MYENGINE
