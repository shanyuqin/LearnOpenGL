#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


#define __USE_GNU
#include <unistd.h>


using namespace std;

class Shader
{
public:
    // 程序ID
    unsigned int ID;

    // 构造器读取并构建着色器
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
        //1.从文件路径中获取顶点/片段着色器
      
        string vertexCode;
        string fragmentCode;
        ifstream vShaderFile;
        ifstream fShaderFile;
        
        vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
        fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
        try {
            //打开文件
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            //读取文件的缓冲内容到数据流中
            stringstream vShaderStream,fShaderStream;
            vShaderStream<<vShaderFile.rdbuf();
            fShaderStream<<fShaderFile.rdbuf();
            //关闭文件处理器
            vShaderFile.close();
            fShaderFile.close();
            //转换数据流到string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        } catch (ifstream::failure e) {
            cout<<"着色器文件没有成功读取"<<endl;
            return;
        }
        
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        
        //创建着色器
        unsigned int vertexShader,fragmentShader;
        vertexShader = glad_glCreateShader(GL_VERTEX_SHADER);
        glad_glShaderSource(vertexShader,1,&vShaderCode,NULL);
        glad_glCompileShader(vertexShader);
        checkCompileErrors(vertexShader,"VERTEX");
        
        fragmentShader = glad_glCreateShader(GL_FRAGMENT_SHADER);
        glad_glShaderSource(fragmentShader,1,&fShaderCode,NULL);
        glad_glCompileShader(fragmentShader);
        checkCompileErrors(fragmentShader,"FRAGMENT");
        //创建着色器程序
        ID = glad_glCreateProgram();
        glad_glAttachShader(ID,vertexShader);
        glad_glAttachShader(ID,fragmentShader);
        glad_glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");
         
        glad_glDeleteShader(vertexShader);
        glad_glDeleteShader(fragmentShader);
        
    }
    // 使用/激活程序
    void use() {
        glad_glUseProgram(ID);
    }
    // uniform工具函数
    void setBool(const string &name, bool value) const {
        glad_glUniform1i(glad_glGetUniformLocation(ID,name.c_str()),(int)value);
    }
    void setInt(const string &name, int value) const {
        glad_glUniform1i(glad_glGetUniformLocation(ID,name.c_str()),value);
    }
    void setFloat(const string &name, float value) const {
        glad_glUniform1f(glad_glGetUniformLocation(ID,name.c_str()),value);
    }
    void setVec2(const std::string &name, const glm::vec2 &value) const {
       glad_glUniform2fv(glad_glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
   }
   void setVec2(const std::string &name, float x, float y) const {
       glad_glUniform2f(glad_glGetUniformLocation(ID, name.c_str()), x, y);
   }
    void setVec3(const string &name,glm::vec3 &value) const {
        glad_glUniform3fv(glad_glGetUniformLocation(ID,name.c_str()),1,&value[0]);
    }
    void setVec3(const std::string &name, float x, float y, float z) const {
        glad_glUniform3f(glad_glGetUniformLocation(ID, name.c_str()), x, y ,z);
    }
    void setMat4(const string &name, const glm::mat4 mat) const {
        glad_glUniformMatrix4fv(glad_glGetUniformLocation(ID,name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    
    
private:
    void checkCompileErrors(unsigned int shader,string type) {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM") {
            glad_glGetShaderiv(shader,GL_COMPILE_STATUS,&success);
            if (!success) {
                glad_glGetShaderInfoLog(shader,1024,NULL,infoLog);
                cout<<type<<"类型的着色器编译错误\n"<<infoLog<<endl;
            }
        }else {
            glad_glGetProgramiv(shader,GL_LINK_STATUS,&success);
            if (!success) {
                glad_glGetProgramInfoLog(shader,1024,NULL,infoLog);
                cout<<type<<"类型的着色器编译错误\n"<<infoLog<<endl;
            }
        }
    }
};
#endif
