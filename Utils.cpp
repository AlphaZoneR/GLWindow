#include "Utils.hpp"

int Utils::LoadShader(std::string VertexShader, std::string FragmentShader){
    int VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    int FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    int Result = GL_FALSE;
    int infolen;

    char const * VertexSourcePointer = VertexShader.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
    glCompileShader(VertexShaderID);

    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &infolen);
    if(infolen){
        std::vector<char> VertexShaderErrorMessage(infolen+1);
		glGetShaderInfoLog(VertexShaderID, infolen, NULL, &VertexShaderErrorMessage[0]);
		std::cout << &VertexShaderErrorMessage[0] << std::endl;
    }

    char const * FragmentSourcePointer = FragmentShader.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
    glCompileShader(FragmentShaderID);

    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &infolen);
    if(infolen){
        std::vector<char> FragmenShaderErrorMessage(infolen+1);
		glGetShaderInfoLog(VertexShaderID, infolen, NULL, &FragmenShaderErrorMessage[0]);
		std::cout << &FragmenShaderErrorMessage[0] << std::endl;
    }

    GLuint ProgramId = glCreateProgram();
    glAttachShader(ProgramId, VertexShaderID);
    glAttachShader(ProgramId, FragmentShaderID);
    glLinkProgram(ProgramId);

    glGetProgramiv(ProgramId, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramId, GL_INFO_LOG_LENGTH, &infolen);

    if(infolen){
        std::vector<char> ProgramErrorMessage(infolen+1);
        glGetProgramInfoLog(ProgramId, infolen, NULL, &ProgramErrorMessage[0]);
        std::cout << &ProgramErrorMessage[0] << std::endl;
    }

    glDetachShader(ProgramId, VertexShaderID);
    glDetachShader(ProgramId, FragmentShaderID);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);
    return ProgramId;

}



bool Utils::updateUniformFloat(float value, GLint programID, std::string uniformName){
    GLint loc = glGetUniformLocation(programID, uniformName.c_str());
    if(loc >= 0){
        glUniform1f(loc, value);
        return 1;
    }
    return 0;
}

bool Utils::updateUniformMat4(glm::mat4x4 lookAtMatrix, GLint programID, std::string uniformName){
    GLint loc = glGetUniformLocation(programID, uniformName.c_str());
    if(loc >= 0){
        glUniformMatrix4fv(loc, 1, 0, &lookAtMatrix[0][0]);
        return 1;
    }
    return 0;
}

bool Utils::updateUniformVec3(glm::vec3 vec, GLint programID, std::string uniformName){
    GLint loc = glGetUniformLocation(programID, uniformName.c_str());
    if(loc >= 0){
        glUniform3f(loc, vec[0], vec[1], vec[2]);
        return 1;
    }
    return 0;
}



