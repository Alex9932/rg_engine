/*
 * shader.h
 *
 *  Created on: Jul 28, 2020
 *      Author: alex9932
 */

#ifndef CL_SHADER_H_
#define CL_SHADER_H_


#include <GL/glew.h>
#include <rg_engine.h>

typedef GLuint rg_Shader;

rg_Shader shader_create(rg_string vs, rg_string fs, rg_string gs);

RG_INLINE void shader_delete(rg_Shader shader) { glDeleteProgram(shader); }
RG_INLINE GLint shader_uniform_get(rg_Shader shader, rg_string name)                            { return glGetUniformLocation(shader, name);               }
//RG_INLINE void shader_bind_frag_output(rg_Shader shader, int attachment, rg_string uniformName) { glBindFragDataLocation(shader, attachment, uniformName); }
//RG_INLINE void shader_bind_attrib_location(rg_Shader shader, int id, rg_string name)            { glBindAttribLocation(shader, id, name);                  }

// Start/Stop
RG_INLINE void shader_start(rg_Shader shader) { glUseProgram(shader); }
RG_INLINE void shader_stop ()                 { glUseProgram(0);      }

// Shader uniforms
RG_INLINE void shader_uniform_1i (GLuint loc, int i)                                 { glUniform1i(loc, i);                          }
RG_INLINE void shader_uniform_2i (GLuint loc, int i, int i2)                         { glUniform2i(loc, i, i2);                      }
RG_INLINE void shader_uniform_2ip(GLuint loc, int* i)                                { glUniform2i(loc, i[0], i[1]);                 }
RG_INLINE void shader_uniform_3i (GLuint loc, int i, int i2, int i3)                 { glUniform3i(loc, i, i2, i3);                  }
RG_INLINE void shader_uniform_3ip(GLuint loc, int* i)                                { glUniform3i(loc, i[0], i[1], i[2]);           }
RG_INLINE void shader_uniform_4i (GLuint loc, int i, int i2, int i3, int i4)         { glUniform4i(loc, i, i2, i3, i4);              }
RG_INLINE void shader_uniform_4ip(GLuint loc, int* i)                                { glUniform4i(loc, i[0], i[1], i[2], i[3]);     }
RG_INLINE void shader_uniform_1f (GLuint loc, float i)                               { glUniform1f(loc, i);                          }
RG_INLINE void shader_uniform_2f (GLuint loc, float i, float i2)                     { glUniform2f(loc, i, i2);                      }
RG_INLINE void shader_uniform_2fp(GLuint loc, float* i)                              { glUniform2f(loc, i[0], i[1]);                 }
RG_INLINE void shader_uniform_3f (GLuint loc, float i, float i2, float i3)           { glUniform3f(loc, i, i2, i3);                  }
RG_INLINE void shader_uniform_3fp(GLuint loc, float* i)                              { glUniform3f(loc, i[0], i[1], i[2]);           }
RG_INLINE void shader_uniform_4f (GLuint loc, float i, float i2, float i3, float i4) { glUniform4f(loc, i, i2, i3, i4);              }
RG_INLINE void shader_uniform_4fp(GLuint loc, float* i)                              { glUniform4f(loc, i[0], i[1], i[2], i[3]);     }
RG_INLINE void shader_uniform_mat4f(GLuint loc, float* matrix)                       { glUniformMatrix4fv(loc, 1, GL_FALSE, matrix); }


#endif /* CL_SHADER_H_ */
