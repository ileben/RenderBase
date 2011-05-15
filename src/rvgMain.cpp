#include "rvgMain.h"

//Ui

int resX = 640;
int resY = 480;

int mouseButton = 0;
Vec2 mouseDown;

//Matrices

MatrixStack matModelView;
MatrixStack matProjection;
MatrixStack matTexture;

//Shaders

Shader *shaderDefault;
VertexBuffer *buffer;

//Lorem Ipsum

const std::string loremIpsum =
"Lorem ipsum dolor sit amet, consectetur adipisicing elit,\n"
"sed do eiusmod tempor incididunt ut labore et dolore magna\n"
"aliqua. Ut enim ad minim veniam, quis nostrud exercitation\n"
"ullamco laboris nisi ut aliquip ex ea commodo consequat.\n"
"Duis aute irure dolor in reprehenderit in voluptate velit\n"
"esse cillum dolore eu fugiat nulla pariatur. Excepteur sint\n"
"occaecat cupidatat non proident, sunt in culpa qui officia\n"
"deserunt mollit anim id est laborum.";

const std::string loremIpsum3 =
  loremIpsum+"\n\n"+loremIpsum+"\n\n"+loremIpsum;


///////////////////////////////////////////////////////////////////
// Functions

void checkGlError (const std::string &text)
{
  static GLenum err = GL_NO_ERROR;
  GLenum newErr = glGetError();
  if (newErr != GL_NO_ERROR && newErr != err) {
    const GLubyte *errString = gluErrorString( newErr );
    std::cout << "GLERROR at '" << text << "': 0x" << std::hex  << int(newErr) << std::dec << " " << errString << std::endl;
  }
  err = newErr;
  glGetError();
}

void renderFullScreenQuad (Shader *shader)
{
  Float coords[] = {
    -1.0f, -1.0f,
    +1.0f, -1.0f,
    +1.0f, +1.0f,
    -1.0f, +1.0f
  };

  Int32 pos = shader->program->getAttribute( "in_pos" );
  glBindBuffer( GL_ARRAY_BUFFER, 0 );
  glVertexAttribPointer( pos, 2, GL_FLOAT, false, 2 * sizeof( Float ), coords );

  glEnableVertexAttribArray( pos );
  glDrawArrays( GL_QUADS, 0, 4 );
  glDisableVertexAttribArray( pos );
}

void renderQuad (Shader *shader, Vec2 min, Vec2 max)
{
  Float coords[] = {
    min.x, min.y,
    max.x, min.y,
    max.x, max.y,
    min.x, max.y
  };

  Int32 pos = shader->program->getAttribute( "in_pos" );
  glBindBuffer( GL_ARRAY_BUFFER, 0 );
  glVertexAttribPointer( pos, 2, GL_FLOAT, false, 2 * sizeof( Float ), coords );

  glEnableVertexAttribArray( pos );
  glDrawArrays( GL_QUADS, 0, 4 );
  glDisableVertexAttribArray( pos );
}

void display ()
{
	/////////////////////////////////////////////////////////
	//Color + Matrix setup

	glClearColor( 1,1,1,1 );
	glClear( GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	Shader *shader = shaderDefault;
	shader->use();

	Int32 uModelview = shader->program->getUniform( "modelview" );
	glUniformMatrix4fv( uModelview, 1, false, (GLfloat*) matModelView.top().m );

	Int32 uProjection = shader->program->getUniform( "projection" );
	glUniformMatrix4fv( uProjection, 1, false, (GLfloat*) matProjection.top().m );

	Int32 uColor = shaderDefault->program->getUniform( "color" );
	glUniform4f( uColor, 1.0f, 0.0f, 0.0f, 0.0f );

	//////////////////////////////////////////////////////////
	//Render

	renderQuad( shaderDefault, Vec2(100,100), Vec2(200,200) );

	//////////////////////////////////////////////////////////
	//FPS

	//Add to fps
	static int fps = 0;
	fps++;

	//Check if 1 second elapsed
	static int lastUpdate = glutGet( GLUT_ELAPSED_TIME );
	int now = glutGet( GLUT_ELAPSED_TIME );
	if (now - lastUpdate > 1000)
	{
		std::cout << "Fps: " << fps << std::endl;
		lastUpdate = now;
		fps = 0;
	}

	//Check for errors
	checkGlError( "display end" );
	glutSwapBuffers();
}

void reshape (int w, int h)
{
  std::cout << "reshape (" << w << "," << h << ")" << std::endl;

  resX = w;
  resY = h;

  Matrix4x4 m;
  m.setOrthoLH( 0, (Float)w, 0, (Float)h, -1.0f, 1.0f );
  matProjection.clear();
  matProjection.load( m );
  matModelView.clear();

  glViewport( 0,0, resX, resY );
}

void animate ()
{
  static int before = glutGet( GLUT_ELAPSED_TIME );
  int now = glutGet( GLUT_ELAPSED_TIME );
  float interval = (float) (now - before) / 1000.0f;
  before = now;
	
  glutPostRedisplay();
}

void keyDown (unsigned char key, int x, int y)
{
}

void specialKeyDown (int key, int x, int y)
{
}

void mouseClick (int button, int state, int x, int y)
{
  mouseButton = button;
  mouseDown.set( (Float)x, (Float)y );
}

void mouseMove (int x, int y)
{

}

void rvgGlutInit (int argc, char **argv)
{
  glutInit( &argc, argv );
  glutInitContextVersion( 3, 0 );
  glutInitDisplayMode( GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_STENCIL | GLUT_DEPTH );
  //glutInitDisplayString( "rgba alpha double stencil depth samples=32" );

  glutInitWindowPosition( 100,100 );
  glutInitWindowSize( resX,resY );
  int ret = glutCreateWindow( "Random Access Vector Graphics" );

  //glutGameModeString( "1280x1024:32@60" );
  //glutEnterGameMode();
  
  glutReshapeFunc( reshape );
  glutDisplayFunc( display );
  glutKeyboardFunc( keyDown );
  //glutKeyboardUpFunc( keyUp );
  glutSpecialFunc( specialKeyDown );
  glutMouseFunc( mouseClick );
  glutMotionFunc( mouseMove );
  //glutPassiveMotionFunc( mouseMove );
  glutIdleFunc( animate );

  int numSamples = 0;
  glGetIntegerv( GL_SAMPLES, &numSamples );
  std::cout << "Number of FSAA samples: " << numSamples << std::endl;
}

int main (int argc, char **argv)
{
  rvgGlutInit( argc, argv );
  rvgGLInit();
  wglSwapInterval( 0 );

  shaderDefault = new Shader();
  shaderDefault->addSource( ShaderType::Vertex, "shaders/default.vert.c" );
  shaderDefault->addSource( ShaderType::Fragment, "shaders/default.frag.c" );
  shaderDefault->load();
  
  glutMainLoop();
}
