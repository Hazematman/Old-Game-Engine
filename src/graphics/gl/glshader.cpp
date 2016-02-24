#include <fstream>
#include <streambuf>
#include "glshader.hpp"
#include "../../error.hpp"
using namespace std;

#define SHADER_ERROR "Shader Error"

ShaderProgram::ShaderProgram(const string &vertex, const string &fragment) {
  vertexFile = vertex;
  fragmentFile = fragment;
  vertexShader = compileShader(vertex, gl::VERTEX_SHADER);
  fragmentShader = compileShader(fragment, gl::FRAGMENT_SHADER);

  programID = gl::CreateProgram();
  gl::AttachShader(programID, vertexShader);
  gl::AttachShader(programID, fragmentShader);
  gl::LinkProgram(programID);

  GLint linkOK = false;
  gl::GetProgramiv(programID, gl::LINK_STATUS, &linkOK);
  if(linkOK == false) {
    throw Error(SHADER_ERROR, getLog(programID));
  }
}

ShaderProgram::~ShaderProgram() {
  gl::DetachShader(programID, vertexShader);
  gl::DetachShader(programID, fragmentShader);
  gl::DeleteShader(vertexShader);
  gl::DeleteShader(fragmentShader);
  gl::DeleteProgram(programID);
}

GLuint ShaderProgram::getID() {
  return programID;
}

GLint ShaderProgram::getUniform(const string &name) {
  GLint location = gl::GetUniformLocation(programID, name.c_str());
  if(location == -1) {
    throw Error(SHADER_ERROR, 
        vertexFile+" "+fragmentFile+":\nCan't find uniform: "+name);
  }

  return location;
}

GLuint ShaderProgram::compileShader(const string &file, GLenum type) {
  string data = loadFile(file);
  const char *sourcec = data.c_str();
  GLuint shader = gl::CreateShader(type);
  gl::ShaderSource(shader, 1, &sourcec, NULL);
  gl::CompileShader(shader);

  GLint compileOK = false;
  gl::GetShaderiv(shader, gl::COMPILE_STATUS, &compileOK);
  if(compileOK == false) {
    throw Error(SHADER_ERROR, file+":\n"+getLog(shader));
  }

  return shader;
}

string ShaderProgram::loadFile(const string &filename) {
  ifstream file(filename);
  if(file.is_open() == false) {
    throw Error(SHADER_ERROR, "Cannot open file "+filename);
  }
  return string(istreambuf_iterator<char>(file),
                istreambuf_iterator<char>());
}

string ShaderProgram::getLog(GLuint object) {
  GLint logLength = 0;
  if(gl::IsShader(object)) {
    gl::GetShaderiv(object, gl::INFO_LOG_LENGTH, &logLength);
  } else {
    gl::GetProgramiv(object, gl::INFO_LOG_LENGTH, &logLength);
  }

  char *log = new char[logLength];
  if(gl::IsShader(object)) {
    gl::GetShaderInfoLog(object, logLength, nullptr, log);
  } else {
    gl::GetProgramInfoLog(object, logLength, nullptr, log);
  }

  string out = log;
  delete[] log;
  return out;
}

