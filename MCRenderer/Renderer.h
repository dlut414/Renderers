/*
 * LICENCE
 * copyright 2014 ~ ****
 * Some rights reserved.
 * Author: HUFANGYUAN
 * Published under CC BY-NC
*/
#ifndef RENDERER_H_INCLUDED
#define RENDERER_H_INCLUDED

#include <cstdio>
#include "header.h"
#include "Camera.h"
#include "common.h"
#include "Point.h"
#include "PartToTri.h"

namespace Renderer
{

GLuint      programID1;
GLuint      programID2;
GLuint      matrixID[5];
GLuint      scalaID[4];

GLuint      vertexArrayID;

GLuint      vertexBufferPos;
GLuint      vertexBufferNorm;

GLuint      frameBuffer;
GLuint      depthBuffer;

GLfloat     r_screen_vertex_buffer_data[] =
{
        -1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f
};

Point*      r3_vertex_pos_data;
Vector*     r3_vertex_norm_data;
GLuint      dataSize;
GLuint      numEdge;

Camera      g_Camera;
glm::ivec2  g_MousePos;
glm::quat   g_Rotation   = glm::angleAxis<float>( -Common::PI * 0.f, glm::vec3(1, 0, 0) );
glm::vec3   g_Scale      = glm::vec3(1.0f);
glm::vec3   g_Pan        = glm::vec3(0.f, 0.f, 0.f);
glm::vec3   g_InitialCameraPosition = glm::vec3(0.f, 0.025f, 0.05f);
glm::quat   g_InitialCameraRotation = glm::angleAxis<float>( -Common::PI * 0.f, glm::vec3(1, 0, 0) );
glm::mat4   g_mvp;
glm::mat4   g_modelMat;
glm::mat4   g_viewMat;
glm::mat4   g_projectionMat;
glm::mat4   g_viewModelMat;
glm::mat4   g_projectionMatInv;
//GLfloat     f_pointRadius     = 2.0f;
//GLfloat     f_pointScale      = 1.0f;
GLfloat     f_near            = 0.001f;
GLfloat     f_far             = 10.0f;

bool        b_dirty           = true;
bool        b_stop            = true;
bool        b_leave           = false;
bool        b_point           = false;
float       f_width           = 1024;
float       f_height          = 1024;
float       f_scaleVel        = 0.005f;
float       f_panVel          = 0.0001f;
int         i_mouseButton     = GLUT_LEFT_BUTTON;
int         i_file;

PartToTri*  obj;
char        str[256];

///function
void Init(int argc, char** argv);
void MainLoop();
void Final();

void InitOBJ();
void InitVAO();
void InitVBO();
void InitFramebuffer();
void InitTexture();
void DrawBuffer_screen();
void Draw();
void Fps();
void myMouse(int, int, int, int);
void myMotion(int, int);
void myMouseWheel(int, int, int, int);
void myReshape(int, int);
void myKeyboard(unsigned char, int, int);
void myDisplay();
GLuint LoadShader(const char*, const char*);
void CreateVBO(GLuint*, unsigned);
void DeleteVBO(GLuint*);

void InitOBJ()
{
    sprintf(str, "../input/aoutput_%04d.prof", i_file);
    std::cout << (*obj << str) << std::endl;

    obj->build();

    std::cout << "# of vertex: " << obj->vert.size() << std::endl;
    std::cout << "# of triangles*3: " << obj->tri.size() << std::endl;
    std::cout << "# of isoValue: " << obj->isoValue << std::endl;
    std::cout << "boundary1: " << obj->box.pMin.x << " " << obj->box.pMin.y << " " << obj->box.pMin.z << " " << std::endl;
    std::cout << "boundary2: " << obj->box.pMax.x << " " << obj->box.pMax.y << " " << obj->box.pMax.z << " " << std::endl;

    g_InitialCameraPosition = glm::vec3((obj->box.pCenter()).x, (obj->box.pCenter()).z, 0.05f);

    r3_vertex_pos_data    = obj   ->  tri.data();
    r3_vertex_norm_data   = obj   ->  triNorm.data();

    numEdge   = obj->tri.size();
    dataSize  = obj->tri.size() * sizeof(Point);
}

void InitVAO()
{
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);
}

void InitVBO()
{
    glGenBuffers(1, &vertexBufferPos);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferPos);

    glGenBuffers(1, &vertexBufferNorm);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferNorm);
}

void InitFramebuffer()
{
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
}

void InitDepthbuffer()
{
    glGenRenderbuffers(1, &depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
}

void InitTexture()
{

}

void Init(int argc, char** argv)
{
    glutInit                (&argc, argv);
    glutInitDisplayMode     (GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition  (0, 0);
    glutInitWindowSize      (f_width, f_height);
    glutCreateWindow        ("MCRenderer");
    //glLightModeli           (GL_LIGHT_MODEL_TWO_SIDE, 1);
    glutMouseFunc           (myMouse);
    glutMotionFunc          (myMotion);
    //glutMouseWheelFunc      (myMouseWheel);
    glutReshapeFunc         (myReshape);
    glutKeyboardFunc        (myKeyboard);
    glutDisplayFunc         (myDisplay);

    glEnable                (GL_TEXTURE_2D);
    glEnable                (GL_CULL_FACE);
    //glDisable               (GL_CULL_FACE);
    glFrontFace             (GL_CW);
    //glEnable                (GL_POINT_SPRITE_ARB);
    //glEnable                (GL_VERTEX_PROGRAM_POINT_SIZE);
    glEnable                (GL_DEPTH_TEST);
    glDepthFunc             (GL_LESS);
    glEnable                (GL_ALPHA_TEST);
    glAlphaFunc             (GL_GREATER, 0.01f);
    //glEnable                (GL_BLEND);
    //glBlendFunc             (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor            (1.f, 1.f, 1.f, 0.f);
    //glClearAccum            (0.f, 0.f, 0.f, 0.f);


    glewInit              ();
    InitVAO               ();
    InitVBO               ();
    //InitFramebuffer       ();

    programID1 = LoadShader("./shader1/vertex.glsl", "./shader1/fragment.glsl");

    matrixID[0] = glGetUniformLocation (programID1, "projectionMat");
    matrixID[1] = glGetUniformLocation (programID1, "viewModelMat");
    scalaID[0]  = glGetUniformLocation (programID1, "pointRadius");
    scalaID[1]  = glGetUniformLocation (programID1, "pointScale");

    g_Camera.SetPosition        (g_InitialCameraPosition);
    g_Camera.SetProjectionRH    (45.0f, 1.0f, f_near, f_far);

    b_dirty           = true;
    b_stop            = true;
    b_leave           = false;
    b_point           = false;
}

void DrawBuffer_screen()
{
    //glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    ///use program
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(programID1);

    ///send data
    glUniformMatrix4fv(matrixID[0], 1, GL_FALSE, &g_projectionMat[0][0]);
    glUniformMatrix4fv(matrixID[1], 1, GL_FALSE, &g_viewModelMat[0][0]);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferPos);
    glBufferData(GL_ARRAY_BUFFER, dataSize, r3_vertex_pos_data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferNorm);
    glBufferData(GL_ARRAY_BUFFER, dataSize, r3_vertex_norm_data, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    ///draw
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLES, 0, numEdge);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void Draw()
{
    glMatrixMode    (GL_MODELVIEW);
    glLoadIdentity  ();

    glm::mat4 modelMatrix = glm::translate  (glm::mat4(1.0f), g_Pan )
                          * glm::toMat4     (g_Rotation)
                          * glm::scale      (glm::mat4(1.0f), g_Scale );

    g_viewModelMat      = g_Camera.GetViewMatrix() * modelMatrix;
    g_projectionMat     = g_Camera.GetProjectionMatrix();
    g_projectionMatInv  = glm::inverse(g_projectionMat);

    DrawBuffer_screen   ();

    glutSwapBuffers     ();

    glutReportErrors    ();
}

void Fps()
{

}

void MainLoop()
{
    glutMainLoop();
}

void Final()
{
    glDeleteBuffers(1, &vertexBufferPos);
    glDeleteBuffers(1, &vertexBufferNorm);
}

void myMouse(int button, int state, int x, int y)
{
    g_MousePos = glm::ivec2(x, y);

    switch(button)
    {
        case GLUT_LEFT_BUTTON:
        {
            i_mouseButton = GLUT_LEFT_BUTTON;
            break;
        }
        case GLUT_RIGHT_BUTTON:
        {
            i_mouseButton = GLUT_RIGHT_BUTTON;
            break;
        }
        case GLUT_MIDDLE_BUTTON:
        {
            i_mouseButton = GLUT_MIDDLE_BUTTON;
            break;
        }
    }
}

void myMotion(int x, int y)
{
    glm::ivec2 mousePos     = glm::ivec2( x, y );
    glm::vec2 delta         = glm::vec2( mousePos - g_MousePos );
    g_MousePos = mousePos;

    switch(i_mouseButton)
    {
        case GLUT_LEFT_BUTTON:
        {
            glm::quat rotX  = glm::angleAxis<float>( glm::radians(delta.y) * 0.5f, glm::vec3(1, 0, 0) );
            glm::quat rotY  = glm::angleAxis<float>( glm::radians(delta.x) * 0.5f, glm::vec3(0, 1, 0) );
            g_Rotation      = ( rotX * rotY ) * g_Rotation;
            break;
        }
        case GLUT_RIGHT_BUTTON:
        {
            g_Pan       += glm::vec3(f_panVel*delta.x, -f_panVel*delta.y, 0.0f);
            break;
        }
        case GLUT_MIDDLE_BUTTON:
        {
            g_Scale     += glm::vec3(delta.y * f_scaleVel);
            break;
        }
    }
    glutPostRedisplay();
    b_dirty = true;
}

void myMouseWheel(int button, int dir, int x, int y)
{
    g_Scale *= dir * f_scaleVel;
    b_dirty = true;
}

void myReshape(int width, int height)
{
    glViewport      (0, 0, width, width);
    glMatrixMode    (GL_MODELVIEW);
    glLoadIdentity  ();
    gluPerspective  (-90.0f, f_width/f_height, 1.0f, 100.0f);
    b_dirty = true;
}

void myKeyboard(unsigned char key, int a, int b)
{
    glutPostRedisplay();

    switch(key)
    {
        //esc
        case 0x1b:
        {
            b_leave = true; break;
        }

        //enter
        case 0x0d:
        {
            b_stop = !b_stop; break;
        }

        //p
        case 0x70:
        {
            b_point = !b_point; break;
        }

        //space
        case 0x20:
        {
            g_Rotation = g_InitialCameraRotation;
            g_Scale = glm::vec3(1.f);
            g_Pan = glm::vec3(0.0f, 0.0f, 0.0f);
            break;
        }

        //, 2c
        case 0x2c:
        {
            i_file--;
            InitOBJ();
            break;
        }

        //. 2e
        case 0x2e:
        {
            i_file++;
            InitOBJ();
            break;
        }
    }
    b_dirty = true;
}

void myDisplay()
{
    /*
    if(!b_stop)
    {
        b_dirty = true;
    }
    */
    Draw();
    Fps();

    if(b_dirty)
    {
        glutPostRedisplay();
        b_dirty = false;
    }

    if(b_leave)
    {
        glutLeaveMainLoop();
    }
}

GLuint LoadShader(const char* vertex_file_path, const char* fragment_file_path)
{
    GLuint VertexShaderID       = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID     = glCreateShader(GL_FRAGMENT_SHADER);

	std::string     VertexShaderCode;
	std::ifstream   VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open())
	{
		std::string Line = "";
		while(getline(VertexShaderStream, Line))
		{
			VertexShaderCode += "\n" + Line;
        }
		VertexShaderStream.close();
	}
	else
	{
		printf("Impossible to open %s. \n", vertex_file_path);
		getchar();
		return 0;
	}

	std::string     FragmentShaderCode;
	std::ifstream   FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open())
	{
		std::string Line = "";
		while(getline(FragmentShaderStream, Line))
		{
			FragmentShaderCode += "\n" + Line;
        }
		FragmentShaderStream.close();
	}
    else
	{
		printf("Impossible to open %s. \n", fragment_file_path);
		getchar();
		return 0;
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource  (VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader (VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv   (VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv   (VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 )
	{
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource  (FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader (FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv   (FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv   (FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 )
	{
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader  (ProgramID, VertexShaderID);
	glAttachShader  (ProgramID, FragmentShaderID);
	glLinkProgram   (ProgramID);

	// Check the program
	glGetProgramiv  (ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv  (ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 )
	{
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDeleteShader  (VertexShaderID);
	glDeleteShader  (FragmentShaderID);

	return ProgramID;
}

void CreateVBO(GLuint* vbo, unsigned size)
{
    glGenBuffers(1, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, *vbo);

    glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glutReportErrors();
}

void DeleteVBO(GLuint* vbo)
{
    glBindBuffer(1, *vbo);
    glDeleteBuffers(1, vbo);
    *vbo = 0;
}

} //namespace Renderer

#endif // RENDERER_H_INCLUDED
