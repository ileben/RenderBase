#ifndef RVGMAIN_H
#define RVGMAIN_H 1

////////////////////////////////////////////////////////////////////
//Internal headers

#include "rvgDefs.h"
#include "rvgGLHeaders.h"
#include "rvgGpuBuffer.h"
#include "rvgShaders.h"
#include "rvgVectors.h"
#include "rvgMatrix.h"
#include "rvgMatrixStack.h"
#include "rvgVertexBuffer.h"

///////////////////////////////////////////////////////////////////
//Typedef for vector names to match GLSL (easier translation)

typedef Vec2 vec2;
typedef Vec3 vec3;
typedef Vec4 vec4;

typedef IVec2 ivec2;
typedef IVec3 ivec3;
typedef IVec4 ivec4;

///////////////////////////////////////////////////////////////////
//Kinda hax but this would be "global" anyway before OpenGL 3.0

extern MatrixStack matModelView;
extern MatrixStack matProjection;
extern MatrixStack matTexture;

/////////////////////////////////////////////////////////////////////
//GL utilities

void checkGlError (const std::string &text);
void renderFullScreenQuad (Shader *shader);
void renderQuad (Shader *shader, Vec2 min, Vec2 max);

#endif//RVGMAIN_H
