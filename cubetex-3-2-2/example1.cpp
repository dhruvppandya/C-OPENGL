#if 1
// spherical camera + persp + lighting + texture
//
// Display a color cube
//
// Colors are assigned to each vertex and then the rasterizer interpolates
//   those colors across the triangles.  We us an orthographic projection
//   as the default projetion.

#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>
using namespace std;

#include <GLUT/glut.h>

#include "Angel.h"
#include "simplemodels.h"

GLuint loadBMP_custom(unsigned char** data, int* w, int* h, const char * imagepath);
GLfloat jx=27.0,jz=0.0;

// Array of rotation angles (in degrees) for each coordinate axis
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int      Axis = Zaxis;
GLfloat  Theta1[NumAxes] = {0.0, 0.0, 0.0 };
GLfloat  Theta2[NumAxes] = { 0.0, 0.0, 0.0 };
GLfloat  Theta3[NumAxes] = { 0.0, 0.0, 0.0 };
GLfloat  Theta4[NumAxes] = { 0.0, 0.0, 0.0 };
GLfloat  Theta5[NumAxes] = { 0.0, 0.0, 0.0 };
GLfloat  Theta6[NumAxes] = { 0.0, 0.0, 0.0 };
GLfloat  Theta7[NumAxes] = { 0.0, 0.0, 0.0 };
GLfloat  Theta8[NumAxes] = { 0.0, 0.0, 0.0 };
GLfloat  Theta9[NumAxes] = { 0.0, 0.0, 0.0 };
GLfloat  Theta10[NumAxes] = { 0.0, 0.0, 0.0 };
GLfloat  Theta11[NumAxes] = { 3.0, 0.0, 0.0 };
GLfloat  Theta12[NumAxes] = { 0.0, 0.0, 0.0 };
GLfloat  Theta13[NumAxes] = { 0.0, 0.0, 0.0 };
GLfloat  Theta14[NumAxes] = { 0.0, 0.0, 0.0 };
GLfloat  Theta15[NumAxes] = { 0.0, 0.0, 0.0 };
GLfloat  Theta16[NumAxes] = { 0.0, 0.0, 0.0 };
GLfloat  Theta17[NumAxes]  ={ 0.0,0.0,0.0};
GLfloat  Theta18[NumAxes]  ={90,0.0,0.0};



size_t CUBE_OFFSET;
size_t COLOR_OFFSET;
size_t CUBE_NORMALS_OFFSET;
size_t CUBE_TEXCOORDS_OFFSET;
size_t CYLINDER_OFFSET;
size_t CYLINDER_NORMALS_OFFSET;
size_t SPHERE_OFFSET;
size_t SPHERE_NORMALS_OFFSET;
size_t SPHERE_TEXCOORDS_OFFSET;

GLfloat mv,xv=-18.0,zv=-5.0;
mat4 earthorbit;
int viewe;
  int x;



//----------------------------------------------------------------------------

GLuint program;

GLuint vPosition;
GLuint vNormal;
GLuint vColor;
GLuint vTexCoord;

GLuint textures[17];

void
init()
{
    // Load shaders and use the resulting shader program
    program = InitShader( "vshader21.glsl", "fshader21.glsl" );
    glUseProgram( program );
    
    //---------------------------------------------------------------------
    colorcube();
    colortube();
    colorbube();
    //---------------------------------------------------------------------
    
    //----- generate a checkerboard pattern for texture mapping
    const int  TextureSize  = 256;
    GLubyte checker_tex[TextureSize][TextureSize][3];
    
    for ( int i = 0; i < TextureSize; i++ )
    {
        for ( int j = 0; j < TextureSize; j++ )
        {
            //GLubyte c = (((i & 0x4) == 0) ^ ((j & 0x1)  == 0)) * 255;
            GLubyte c = (((j & 0x2)  == 0)) * 255;
            checker_tex[i][j][0]  = c;
            checker_tex[i][j][1]  = c;
            checker_tex[i][j][2]  = c;
        }
    }
    
    //---- Initialize texture objects
    glGenTextures( 17, textures );
    
    unsigned char* pic1 = NULL;
    int w,h;
    
    loadBMP_custom(&pic1, &w, &h, "sun.bmp");
    
    glBindTexture( GL_TEXTURE_2D, textures[0] );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, pic1 );
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    
    
    unsigned char* pic2 = NULL;
    
    
    loadBMP_custom(&pic2, &w, &h, "mercury.bmp");
    
    glBindTexture( GL_TEXTURE_2D, textures[1] );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, pic2);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    
    
    unsigned char* pic3 = NULL;
    
    
    loadBMP_custom(&pic3, &w, &h, "venus.bmp");
    
    glBindTexture( GL_TEXTURE_2D, textures[2] );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, pic3);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    
    
    unsigned char* pic4 = NULL;
    
    
    loadBMP_custom(&pic4, &w, &h, "earth.bmp");
    
    glBindTexture( GL_TEXTURE_2D, textures[3] );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, pic4);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    
    
    unsigned char* pic5= NULL;
    
    
    loadBMP_custom(&pic5, &w, &h, "mars.bmp");
    
    glBindTexture( GL_TEXTURE_2D, textures[4] );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, pic5);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    
    unsigned char* pic6= NULL;
    
    
    loadBMP_custom(&pic6, &w, &h, "jupiter.bmp");
    
    glBindTexture( GL_TEXTURE_2D, textures[5] );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, pic6);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    
    
    
    unsigned char* pic7= NULL;
    
    
    loadBMP_custom(&pic7, &w, &h, "saturn.bmp");
    
    glBindTexture( GL_TEXTURE_2D, textures[6] );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, pic7);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    
    
    
    
    
    
    
    unsigned char* pic8= NULL;
    
    
    loadBMP_custom(&pic8, &w, &h, "uranus.bmp");
    
    glBindTexture( GL_TEXTURE_2D, textures[7] );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, pic8);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    
    
    unsigned char* pic9= NULL;
    
    
    loadBMP_custom(&pic9, &w, &h, "neptune.bmp");
    
    glBindTexture( GL_TEXTURE_2D, textures[8] );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, pic9);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    
    unsigned char* pic10= NULL;
    
    
    loadBMP_custom(&pic10, &w, &h, "pluto.bmp");
    
    glBindTexture( GL_TEXTURE_2D, textures[9] );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, pic10 );
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    
    unsigned char* pic11= NULL;
    
    
    loadBMP_custom(&pic11, &w, &h, "sky1.bmp");
    
    glBindTexture( GL_TEXTURE_2D, textures[10] );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, pic11 );
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    
    unsigned char* pic12= NULL;
    
    
    loadBMP_custom(&pic12, &w, &h, "moon.bmp");
    
    glBindTexture( GL_TEXTURE_2D, textures[11] );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, pic12 );
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    
    
    unsigned char* pic13= NULL;
    
    
    loadBMP_custom(&pic13, &w, &h, "ring.bmp");
    
    glBindTexture( GL_TEXTURE_2D, textures[12] );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, pic13 );
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    
    unsigned char* pic14= NULL;
    
    
    loadBMP_custom(&pic14, &w, &h, "ring2.bmp");
    
    glBindTexture( GL_TEXTURE_2D, textures[13] );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, pic14);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    
    unsigned char* pic15= NULL;
    
    
    loadBMP_custom(&pic15, &w, &h, "DeathStar.bmp");
    
    glBindTexture( GL_TEXTURE_2D, textures[14] );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, pic15);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    
    unsigned char* pic16= NULL;
    
    
    loadBMP_custom(&pic16, &w, &h, "asteroids.bmp");
    
    glBindTexture( GL_TEXTURE_2D, textures[15] );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, pic16);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    
    

    

    

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    //----set offset variables
    
    CUBE_OFFSET = 0;
    COLOR_OFFSET = sizeof(points_cube);
    CUBE_NORMALS_OFFSET = COLOR_OFFSET + sizeof(colors);
    CUBE_TEXCOORDS_OFFSET = CUBE_NORMALS_OFFSET + sizeof(normals_cube);
    CYLINDER_OFFSET = CUBE_TEXCOORDS_OFFSET + sizeof(tex_coords_cube);
    CYLINDER_NORMALS_OFFSET = CYLINDER_OFFSET + sizeof(points_cylinder);
    SPHERE_OFFSET = CYLINDER_NORMALS_OFFSET + sizeof(normals_cylinder);
    SPHERE_NORMALS_OFFSET = SPHERE_OFFSET + sizeof(points_sphere);
    SPHERE_TEXCOORDS_OFFSET = SPHERE_NORMALS_OFFSET + sizeof(normals_sphere);
    
    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );  // removed 'APPLE' suffix for 3.2
    glBindVertexArray( vao );
    
    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points_cube) + sizeof(normals_cube) + sizeof(tex_coords_cube) + sizeof(colors) + sizeof(points_cylinder) + sizeof(normals_cylinder) + sizeof(points_sphere) + sizeof(normals_sphere) + sizeof(tex_coords_sphere), NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, CUBE_OFFSET, sizeof(points_cube), points_cube );
    glBufferSubData( GL_ARRAY_BUFFER, COLOR_OFFSET, sizeof(colors), colors );
    glBufferSubData( GL_ARRAY_BUFFER, CUBE_NORMALS_OFFSET, sizeof(normals_cube), normals_cube );
    glBufferSubData( GL_ARRAY_BUFFER, CUBE_TEXCOORDS_OFFSET, sizeof(tex_coords_cube), tex_coords_cube );
    glBufferSubData( GL_ARRAY_BUFFER, CYLINDER_OFFSET, sizeof(points_cylinder), points_cylinder );
    glBufferSubData( GL_ARRAY_BUFFER, CYLINDER_NORMALS_OFFSET, sizeof(normals_cylinder), normals_cylinder );
    glBufferSubData( GL_ARRAY_BUFFER, SPHERE_OFFSET, sizeof(points_sphere), points_sphere );
    glBufferSubData( GL_ARRAY_BUFFER, SPHERE_NORMALS_OFFSET, sizeof(normals_sphere), normals_sphere );
    glBufferSubData( GL_ARRAY_BUFFER, SPHERE_TEXCOORDS_OFFSET, sizeof(tex_coords_sphere), tex_coords_sphere );
    
    
    //---------------------------------------------------------------------
    
    // set up vertex arrays
    vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_OFFSET) );
    
    vNormal = glGetAttribLocation( program, "vNormal" );
    glEnableVertexAttribArray( vNormal );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_NORMALS_OFFSET) );
    
    vColor = glGetAttribLocation( program, "vColor" );
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(COLOR_OFFSET) );
    
    vTexCoord = glGetAttribLocation( program, "vTexCoord" );
    glEnableVertexAttribArray( vTexCoord );
    glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_TEXCOORDS_OFFSET) );
    
    // Set the value of the fragment shader texture sampler variable (myTextureSampler) to GL_TEXTURE0
    glUniform1i( glGetUniformLocation(program, "myTextureSampler"), 0 );
    
    //---------------------------------------------------------------------
    
    glEnable( GL_DEPTH_TEST );
    glClearColor( 0.0, 0.0, 0.0, 1.0 );
}




//----------------------------------------------------------------------------

void
SetMaterial( vec4 ka, vec4 kd, vec4 ks, float s )
{
    glUniform4fv( glGetUniformLocation(program, "ka"), 1, ka );
    glUniform4fv( glGetUniformLocation(program, "kd"), 1, kd );
    glUniform4fv( glGetUniformLocation(program, "ks"), 1, ks );
    glUniform1f( glGetUniformLocation(program, "Shininess"), s );
}

//----------------------------------------------------------------------------

void
SetLight( vec4 lpos, vec4 la, vec4 ld, vec4 ls )
{
    glUniform4fv( glGetUniformLocation(program, "LightPosition"), 1, lpos);
    glUniform4fv( glGetUniformLocation(program, "AmbientLight"), 1, la);
    glUniform4fv( glGetUniformLocation(program, "DiffuseLight"), 1, ld);
    glUniform4fv( glGetUniformLocation(program, "SpecularLight"), 1, ls);
}


//----------------------------------------------------------------------------

mat4 proj_matrix;

mat4 view_matrix;

float r = 5.0;

void
display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    //---- lights
    
    SetLight( vec4( 2.0, 2.0, 2.0, 1.0 ), vec4(0.4, 0.4, 0.4, 1.0), vec4(0.7, 0.7, 0.7, 1.0), vec4(0.7, 0.7, 0.7, 1.0) );
    
    //---- camera intrinsic parameters
    
    float left = -1.0;
    float right = 1.0;
    float bottom = -1.0;
    float top = 1.0;
    float zNear = 1.0;
    float zFar = 600.0;
    
    //proj_matrix = Scale(2.0/(right-left), 2.0/(top-bottom), 2.0/(zNear-zFar));
    
    proj_matrix = Frustum( left, right, bottom, top, zNear, zFar );
    
    glUniformMatrix4fv( glGetUniformLocation( program, "projection" ), 1, GL_TRUE, proj_matrix );
    
    
    //---- camera extrinsic parameters
    
    float tr_y = Theta3[Yaxis]* M_PI/180.0;
    float tr_z = Theta3[Zaxis]* M_PI/180.0;
    float eye_z = r * (cos(tr_z)) * cos(tr_y);
    float eye_x = r * (cos(tr_z)) * sin(tr_y);
    float eye_y = r * sin(tr_z);
    
    vec4 up = vec4(0.0, cos(tr_z), 0.0, 0.0);
    cout << up << ' ' << normalize(up) << endl;
    
    if(viewe==0)
    {

    view_matrix = LookAt( vec4(eye_x,eye_y,eye_z, 1.0), vec4(0, 0.0, 0.0, 1.0), vec4(0.0, cos(tr_z), 0.0, 0.0));
    }
    else
    {
     view_matrix = LookAt( vec4(eye_x, eye_y, eye_z, 1.0), vec4(earthorbit[3][0], earthorbit[3][1], earthorbit[3][2], 1.0), vec4(0.0, cos(tr_z), 0.0, 0.0));
        cout<<earthorbit <<endl;
    }
    glUniformMatrix4fv( glGetUniformLocation( program, "view" ), 1, GL_TRUE, view_matrix );
    
    
    //---- action
    
    
    SetLight( vec4( 0.0, 0.0, 0.0, 1.0 ), vec4(0.7, 0.7, 0.7, 1.0), vec4(0.9, 0.9, 0.9, 1.0), vec4(0.5, 0.5, 0.5, 1.0) );
    
    //---- SUN
    glBindTexture( GL_TEXTURE_2D, textures[0] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial( vec4(0.9, 0.9, 0.5, 1.0), vec4(0.9, 0.9, 0.5, 1.0), vec4(0.9, 0.9, 0.0, 1.0), 0.5);
    glUniform1i( glGetUniformLocation(program, "light_out"), true );
    
    mat4 transform_bube1 = Translate( 0.0, 0.0, 0.0 ) * Scale(3.5, 3.5, 3.5);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform_bube1 );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_NORMALS_OFFSET) );
    glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_TEXCOORDS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );
    
    //---- spherical MOON 1
    glBindTexture( GL_TEXTURE_2D, textures[11] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial( vec4(0.5, 0.2, 0.7, 1.0), vec4(0.5, 0.2, 0.7, 1.0), vec4(0.7, 0.2, 0.8, 1.0), 5.5);
    glUniform1i( glGetUniformLocation(program, "light_out"), true);
    
    
    mat4 transform_bube22 = RotateY(Theta11[Xaxis])*Translate(1,0.5, 0.0)*RotateY(Theta11[Xaxis])*Scale(0.3, 0.3, 0.3);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform_bube22 );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_NORMALS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );
    
   //---- spherical MOON 1
    glBindTexture( GL_TEXTURE_2D, textures[11] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial( vec4(0.5, 0.2, 0.7, 1.0), vec4(0.5, 0.2, 0.7, 1.0), vec4(0.7, 0.2, 0.8, 1.0), 5.5);
    glUniform1i( glGetUniformLocation(program, "light_out"), true);
    
    
    transform_bube22 = RotateY(Theta16[Xaxis])*Translate(-1, 0, 0.0)*RotateY(Theta16[Xaxis])*Scale(0.3, 0.3, 0.3);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform_bube22 );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_NORMALS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );
    
    //---- spherical rocks
    glBindTexture( GL_TEXTURE_2D, textures[11] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial( vec4(0.5, 0.2, 0.7, 1.0), vec4(0.5, 0.2, 0.7, 1.0), vec4(0.7, 0.2, 0.8, 1.0), 5.5);
    glUniform1i( glGetUniformLocation(program, "light_out"), true);
    
    
     transform_bube22 = RotateY(Theta16[Xaxis])*Translate(1, -1, 0.0)*RotateY(Theta16[Xaxis])*Scale(0.3, 0.3, 0.3);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform_bube22 );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_NORMALS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );
    

    
    
    
    //---- Mercury
    glBindTexture( GL_TEXTURE_2D, textures[1] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial( vec4(0.5, 0.2, 0.7, 1.0), vec4(0.5, 0.2, 0.7, 1.0), vec4(0.7, 0.2, 0.8, 1.0), 5.5);
    glUniform1i( glGetUniformLocation(program, "light_out"), false );
    
    mat4 transform_bube2 = RotateY( Theta1[Xaxis] ) * Translate( 4.0, 0.0, 0 ) * Scale(1.2, -1.2, 1.2)*RotateY( Theta1[Xaxis] );
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform_bube2 );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_NORMALS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );
    
    //---- spherical MOON 1
    glBindTexture( GL_TEXTURE_2D, textures[11] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial( vec4(0.5, 0.2, 0.7, 1.0), vec4(0.5, 0.2, 0.7, 1.0), vec4(0.7, 0.2, 0.8, 1.0), 5.5);
    glUniform1i( glGetUniformLocation(program, "light_out"), true);
    
    
     transform_bube22 = (RotateY( Theta1[Xaxis] ) * Translate( 4.0, 0.0, 0.0 ))*RotateY(Theta11[Xaxis])*Translate(1, 0, 0.0)*RotateY(Theta11[Xaxis])*Scale(0.3, 0.3, 0.3);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform_bube22 );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_NORMALS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );
    
    
    //---- Venus
    glBindTexture( GL_TEXTURE_2D, textures[2] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial( vec4(0.5, 0.2, 0.7, 1.0), vec4(0.5, 0.2, 0.7, 1.0), vec4(0.7, 0.2, 0.8, 1.0), 5.5);
    glUniform1i( glGetUniformLocation(program, "light_out"), false );
    
    transform_bube2 = RotateY( Theta2[Xaxis] ) * Translate( 10 , 0.0, 0.0 ) * Scale(2.0, -2.0, 2.0)* RotateY( Theta2[Xaxis]);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform_bube2 );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_NORMALS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );
    
    //---- spherical MOON 1
    glBindTexture( GL_TEXTURE_2D, textures[11] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial( vec4(0.5, 0.2, 0.7, 1.0), vec4(0.5, 0.2, 0.7, 1.0), vec4(0.7, 0.2, 0.8, 1.0), 5.5);
    glUniform1i( glGetUniformLocation(program, "light_out"), true);
    
    
    transform_bube22 = (RotateY( Theta2[Xaxis] ) * Translate( 10.0, 0.0, 0.0 ))*RotateY(Theta11[Xaxis])*Translate(-2, 0, 0.0)*RotateY(Theta11[Xaxis])*Scale(0.3, 0.3, 0.3);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform_bube22 );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_NORMALS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );
    
    //---- spherical MOON 2
    glBindTexture( GL_TEXTURE_2D, textures[11] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial( vec4(0.5, 0.2, 0.7, 1.0), vec4(0.5, 0.2, 0.7, 1.0), vec4(0.7, 0.2, 0.8, 1.0), 5.5);
    glUniform1i( glGetUniformLocation(program, "light_out"), true);
    
    
    transform_bube22 = (RotateY( Theta2[Xaxis] ) * Translate( 10.0, 0.0, 0.0 ))*RotateY(Theta11[Xaxis])*Translate(2, 1, 0.0)*RotateY(Theta11[Xaxis])*Scale(0.3, 0.3, 0.3);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform_bube22 );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_NORMALS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );
    

    

    
    
    //---- earth
    glBindTexture( GL_TEXTURE_2D, textures[3] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial( vec4(0.5, 0.2, 0.7, 1.0), vec4(0.5, 0.2, 0.7, 1.0), vec4(0.7, 0.2, 0.8, 1.0), 5.5);
    glUniform1i( glGetUniformLocation(program, "light_out"), false );
    
    earthorbit =
    transform_bube2 = RotateY( Theta3[Xaxis] )  * Translate( 15, 0.0, 0) * RotateY(Theta3[Zaxis]) * Scale(2.5, -2.5, 2.52);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform_bube2 );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_NORMALS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );
    
    
  
    //---- spherical MOON
    glBindTexture( GL_TEXTURE_2D, textures[11] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial( vec4(0.5, 0.2, 0.7, 1.0), vec4(0.5, 0.2, 0.7, 1.0), vec4(0.7, 0.2, 0.8, 1.0), 5.5);
    glUniform1i( glGetUniformLocation(program, "light_out"), false );
    
   
    transform_bube22 = (RotateY( Theta3[Xaxis] )  * Translate( 15, 0.0, 0)) * RotateY(Theta13[Xaxis])*Translate(-2.25, 0, 0)*RotateY(Theta13[Xaxis])*Scale(0.5, 0.5, 0.5);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform_bube22 );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_NORMALS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );
    
    
    //---- Mars
    glBindTexture( GL_TEXTURE_2D, textures[4] );
    glUniform1i( glGetUniformLocation(program, "texture_on") ,true );
    
    SetMaterial( vec4(0.5, 0.2, 0.7, 1.0), vec4(0.5, 0.2, 0.7, 1.0), vec4(0.7, 0.2, 0.8, 1.0), 5.5);
    glUniform1i( glGetUniformLocation(program, "light_out"), false );
    
    transform_bube2 = RotateY( Theta4[Xaxis] ) * Translate( 20.0, 0.0, 0.0 ) *RotateY(Theta4[Xaxis])* Scale(2.7, -2.7, 2.7);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform_bube2 );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_NORMALS_OFFSET) );
   glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );
    
    //---- spherical MOON
    glBindTexture( GL_TEXTURE_2D, textures[11] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial( vec4(0.5, 0.2, 0.7, 1.0), vec4(0.5, 0.2, 0.7, 1.0), vec4(0.7, 0.2, 0.8, 1.0), 5.5);
    glUniform1i( glGetUniformLocation(program, "light_out"), false );
    
    
    transform_bube22 =(RotateY( Theta4[Xaxis] ) * Translate( 20, 0.0, 0.0 ))*RotateY(Theta11[Xaxis])*Translate(2.25, 0, 0.0)*RotateY(Theta11[Xaxis])*Scale(0.5, 0.5, 0.5);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform_bube22 );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_NORMALS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );
    
    
    //---- spherical MOON
    glBindTexture( GL_TEXTURE_2D, textures[11] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial( vec4(0.5, 0.2, 0.7, 1.0), vec4(0.5, 0.2, 0.7, 1.0), vec4(0.7, 0.2, 0.8, 1.0), 5.5);
    glUniform1i( glGetUniformLocation(program, "light_out"), false );
    
    
    transform_bube22 =(RotateY( Theta4[Xaxis] ) * Translate( 20, 0.0, 0.0 ))*RotateY(Theta11[Xaxis])*Translate(-2.25, 0, 0.0)*RotateY(Theta11[Xaxis])*Scale(0.5, 0.5, 0.5);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform_bube22 );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_NORMALS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );
    
    //----asteroid ring 1
    glBindTexture( GL_TEXTURE_2D, textures[15] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial( vec4(0.5, 0.2, 0.7, 1.0), vec4(0.5, 0.2, 0.7, 1.0), vec4(0.7, 0.2, 0.8, 1.0), 5.5);
    glUniform1i( glGetUniformLocation(program, "light_out"), false );
    
    
    for (x=0; x<500; x++) {
        
        x=x+10;
        
    
        transform_bube22 =RotateY(Theta7[Xaxis]) * RotateY(Theta17[Xaxis]+x)*Translate(25.0, 0.5, 0.2)*Scale(0.3, 0.3, 0.3);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform_bube22 );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_NORMALS_OFFSET) );
        glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );};
    
    //---- asteroid ring 2
    glBindTexture( GL_TEXTURE_2D, textures[15] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial( vec4(0.5, 0.2, 0.7, 1.0), vec4(0.5, 0.2, 0.7, 1.0), vec4(0.7, 0.2, 0.8, 1.0), 5.5);
    glUniform1i( glGetUniformLocation(program, "light_out"), false );
    
  
    for (x=0; x<900; x++) {
        
        x=x+4;
        
        
        transform_bube22 =RotateY(Theta10[Xaxis]) * RotateY(Theta17[Xaxis]+x)*Translate(25.0, 0.6, 0.1)*Scale(0.3, 0.3, 0.3);
        
        glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform_bube22 );
        
        glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
        glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_NORMALS_OFFSET) );
        glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );};
    
    //---- asteroid ring 3
    glBindTexture( GL_TEXTURE_2D, textures[15] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial( vec4(0.5, 0.2, 0.7, 1.0), vec4(0.5, 0.2, 0.7, 1.0), vec4(0.7, 0.2, 0.8, 1.0), 5.5);
    glUniform1i( glGetUniformLocation(program, "light_out"), false );
    
    
    for (x=0; x<700; x++) {
        
        x=x+6;
        
        
        transform_bube22 =RotateY(Theta9[Xaxis]+x)*Translate(25.6, 0, 0.0)*Scale(0.3, 0.3, 0.3);
        
        glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform_bube22 );
        
        glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
        glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_NORMALS_OFFSET) );
        glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );};
    
    //---- asteroid ring 4
    glBindTexture( GL_TEXTURE_2D, textures[15] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial( vec4(0.5, 0.2, 0.7, 1.0), vec4(0.5, 0.2, 0.7, 1.0), vec4(0.7, 0.2, 0.8, 1.0), 5.5);
    glUniform1i( glGetUniformLocation(program, "light_out"), false );
    
    
    for (x=0; x<1000; x++) {
        
        x=x+6;
        
        
        transform_bube22 =RotateY(Theta9[Xaxis]) * RotateY(Theta17[Xaxis]+x)* Translate(24.5, 0, 0.0)*Scale(0.3, 0.3, 0.3);
        
        glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform_bube22 );
        
        glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
        glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_NORMALS_OFFSET) );
        glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );};
    
    //---- spherical MOON
    glBindTexture( GL_TEXTURE_2D, textures[15] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial( vec4(0.5, 0.2, 0.7, 1.0), vec4(0.5, 0.2, 0.7, 1.0), vec4(0.7, 0.2, 0.8, 1.0), 5.5);
    glUniform1i( glGetUniformLocation(program, "light_out"), false );
    
    
    for (x=0; x<200; x++) {
        
        x=x+6;
        
        
        transform_bube22 =RotateY(Theta8[Xaxis]) * RotateY(Theta17[Xaxis]+x)*Translate(24.5, 0.5, 0.2)*Scale(0.3, 0.3, 0.3);
        
        glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform_bube22 );
        
        glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
        glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_NORMALS_OFFSET) );
        glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );};

    

    

    
    
    
    
        /*
        //---- Mars
    glBindTexture( GL_TEXTURE_2D, textures[4] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial( vec4(0.5, 0.2, 0.7, 1.0), vec4(0.5, 0.2, 0.7, 1.0), vec4(0.7, 0.2, 0.8, 1.0), 5.5);
    glUniform1i( glGetUniformLocation(program, "light_out"), false );
    
    transform_bube2 = RotateY( Theta4[Xaxis] ) * Translate( 14.0, 0.0, -10.0 ) *RotateY(Theta4[Xaxis])* Scale(2.7, -2.7, 2.7);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform_bube2 );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_NORMALS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );
    
    */
    
    //---- Jupiter
    glBindTexture( GL_TEXTURE_2D, textures[5] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial( vec4(0.5, 0.2, 0.7, 1.0), vec4(0.5, 0.2, 0.7, 1.0), vec4(0.7, 0.2, 0.8, 1.0), 5.5);
    glUniform1i( glGetUniformLocation(program, "light_out"), false );

    
    transform_bube2 = RotateY( Theta5[Xaxis] ) * Translate( 30.0, 0.0, 0.0 )* RotateY(Theta5 [Xaxis]) * Scale(3, -3, 3);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform_bube2 );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_NORMALS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );
    
    //---- spherical MOON
    glBindTexture( GL_TEXTURE_2D, textures[11] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial( vec4(0.5, 0.2, 0.7, 1.0), vec4(0.5, 0.2, 0.7, 1.0), vec4(0.7, 0.2, 0.8, 1.0), 5.5);
    glUniform1i( glGetUniformLocation(program, "light_out"), true);
    
    
    transform_bube22 = (RotateY( Theta5[Xaxis] ) * Translate( 30.0, 0.0, 0.0 ))*RotateY(Theta15[Xaxis])*Translate(2.0, 0, 0.0)*RotateY(Theta15[Xaxis])*Scale(0.5, 0.5, 0.5);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform_bube22 );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_NORMALS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );
    
    //---- spherical MOON 2
    glBindTexture( GL_TEXTURE_2D, textures[11] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial( vec4(0.5, 0.2, 0.7, 1.0), vec4(0.5, 0.2, 0.7, 1.0), vec4(0.7, 0.2, 0.8, 1.0), 5.5);
    glUniform1i( glGetUniformLocation(program, "light_out"), true);
    
    
    transform_bube22 = (RotateY( Theta5[Xaxis] ) * Translate( 30.0, 0.0, 0.0 ))*RotateY(Theta11[Xaxis])*Translate(-2.0, 1, 0.0)*RotateY(Theta11[Xaxis])*Scale(0.5, 0.5, 0.5);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform_bube22 );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_NORMALS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );
    
    //---- spherical MOON 3
    glBindTexture( GL_TEXTURE_2D, textures[11] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial( vec4(0.5, 0.2, 0.7, 1.0), vec4(0.5, 0.2, 0.7, 1.0), vec4(0.7, 0.2, 0.8, 1.0), 5.5);
    glUniform1i( glGetUniformLocation(program, "light_out"), true);
    
    
    transform_bube22 = (RotateY( Theta5[Xaxis] ) * Translate( 30.0, 0.0, 0.0 ))*RotateY(Theta11[Xaxis])*Translate(1.5, 2, 0.0)*RotateY(Theta11[Xaxis])*Scale(0.5, 0.5, 0.5);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform_bube22 );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_NORMALS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );
    
    //---- spherical MOON 4
    glBindTexture( GL_TEXTURE_2D, textures[11] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial( vec4(0.5, 0.2, 0.7, 1.0), vec4(0.5, 0.2, 0.7, 1.0), vec4(0.7, 0.2, 0.8, 1.0), 5.5);
    glUniform1i( glGetUniformLocation(program, "light_out"), true);
    
    
    transform_bube22 = (RotateY( Theta5[Xaxis] ) * Translate( 30.0, 0.0, 0.0 ))*RotateY(Theta11[Xaxis])*Translate(1.5, -2, 0.0)*RotateY(Theta11[Xaxis])*Scale(0.5, 0.5, 0.5);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform_bube22 );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_NORMALS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );
    
    
    
    
    
    //---- Saturn
    glBindTexture( GL_TEXTURE_2D, textures[6] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial( vec4(0.5, 0.2, 0.7, 1.0), vec4(0.5, 0.2, 0.7, 1.0), vec4(0.7, 0.2, 0.8, 1.0), 5.5);
    glUniform1i( glGetUniformLocation(program, "light_out"), false );
    
    transform_bube2 = RotateY( Theta6[Xaxis] ) * Translate( 35.0, 0.0, 0.0 ) * RotateY(Theta6[Xaxis]) * Scale(2.9, 2.9, 2.9);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform_bube2 );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_NORMALS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );
    
    //---- ring 1
    glBindTexture( GL_TEXTURE_2D, textures[12] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial( vec4(0.5, 0.2, 0.7, 1.0), vec4(0.5, 0.2, 0.7, 1.0), vec4(0.7, 0.2, 0.8, 1.0), 5.5);
    glUniform1i( glGetUniformLocation(program, "light_out"), false );
    
    transform_bube2 = RotateY( Theta7[Xaxis] ) * Translate( 35.0, 0.0,0.0 ) *  Scale(7.0, -0.0, 7.0);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform_bube2 );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_NORMALS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );
    
    
   
    //---- center part of ring
    glBindTexture( GL_TEXTURE_2D, textures[13] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial( vec4(0.5, 0.2, 0.7, 1.0), vec4(0.5, 0.2, 0.7, 1.0), vec4(0.7, 0.2, 0.8, 1.0), 5.5);
    glUniform1i( glGetUniformLocation(program, "light_out"), false );
    
    transform_bube2 = RotateY( Theta7[Xaxis] ) * Translate( 35.0, 0.0, 0.0 ) * Scale(5.5, 0.1, 5.5);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform_bube2 );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_NORMALS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );
    
    
   /*  //---- cylindrical object
     glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    glBindTexture( GL_TEXTURE_2D, textures[13] );
     
     SetMaterial( vec4(0.7, 0.0, 0.0, 1.0), vec4(1.0, 0.0, 0.0, 1.0), vec4(1.0, 0.5, 0.5, 1.0), 5.5);
     glUniform1i( glGetUniformLocation(program, "light_out"), false );
     
     mat4 transform_tube =(RotateY( Theta6[Xaxis] ) * Translate( 35.0, 0.0, 0.0 )) * RotateY( Theta4[Xaxis] ) *Translate( 0.0, 0.0 , 0.0 ) * Scale(4.5, 0, 4.5);
     glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform_tube );
     
     glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CYLINDER_OFFSET) );
     glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CYLINDER_NORMALS_OFFSET) );
     glDrawArrays( GL_TRIANGLES, 0, NumVerticesCylinder );*/
    
    
    
    
    
    //---- uranus
    glBindTexture( GL_TEXTURE_2D, textures[7] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial( vec4(0.5, 0.2, 0.7, 1.0), vec4(0.5, 0.2, 0.7, 1.0), vec4(0.7, 0.2, 0.8, 1.0), 5.5);
    glUniform1i( glGetUniformLocation(program, "light_out"), false );
    
    transform_bube2 = RotateY( Theta8[Xaxis] ) * Translate( 41.0, 0.0, 0.0) *RotateY(Theta8[Xaxis]) * Scale(2.0, -2.0, 2.0);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform_bube2 );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_NORMALS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );
    
    
    
    //---- ring 1
    glBindTexture( GL_TEXTURE_2D, textures[7] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial( vec4(0.5, 0.2, 0.7, 1.0), vec4(0.5, 0.2, 0.7, 1.0), vec4(0.7, 0.2, 0.8, 1.0), 5.5);
    glUniform1i( glGetUniformLocation(program, "light_out"), false );
    
    transform_bube2 = RotateY( Theta8[Xaxis] ) * Translate( 41.0, 0.0,0.0 ) * RotateZ(90)* Scale(7.0, -0.0, 7.0);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform_bube2 );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_NORMALS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );
    
    
    
    //---- center part of ring
    glBindTexture( GL_TEXTURE_2D, textures[13] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial( vec4(0.5, 0.2, 0.7, 1.0), vec4(0.5, 0.2, 0.7, 1.0), vec4(0.7, 0.2, 0.8, 1.0), 5.5);
    glUniform1i( glGetUniformLocation(program, "light_out"), false );
    
    transform_bube2 = RotateY( Theta8[Xaxis] ) * Translate( 41.0, 0.0, 0.0 )* RotateZ(90)* Scale(5.5, 0.1, 5.5);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform_bube2 );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_NORMALS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );
    
    
    
    
    //---- spherical MOON 1
    glBindTexture( GL_TEXTURE_2D, textures[11] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial( vec4(0.5, 0.2, 0.7, 1.0), vec4(0.5, 0.2, 0.7, 1.0), vec4(0.7, 0.2, 0.8, 1.0), 5.5);
    glUniform1i( glGetUniformLocation(program, "light_out"), true);
    
    
    transform_bube22 = (RotateY( Theta8[Xaxis] ) * Translate( 41.0, 0.0, 0.0 ))*RotateY(Theta11[Xaxis])*Translate(2, 0, 0.0)*RotateY(Theta11[Xaxis])*Scale(0.5, 0.5, 0.5);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform_bube22 );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_NORMALS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );
    
    //---- spherical MOON 2
    glBindTexture( GL_TEXTURE_2D, textures[11] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial( vec4(0.5, 0.2, 0.7, 1.0), vec4(0.5, 0.2, 0.7, 1.0), vec4(0.7, 0.2, 0.8, 1.0), 5.5);
    glUniform1i( glGetUniformLocation(program, "light_out"), true);
    
    
    transform_bube22 = (RotateY( Theta8[Xaxis] ) * Translate( 41.0, 0.0, 0.0 ))*RotateY(Theta11[Xaxis])*Translate(-1.5, 2, 0.0)*RotateY(Theta11[Xaxis])*Scale(0.5, 0.5, 0.5);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform_bube22 );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_NORMALS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );

    //---- spherical MOON 3
    glBindTexture( GL_TEXTURE_2D, textures[11] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial( vec4(0.5, 0.2, 0.7, 1.0), vec4(0.5, 0.2, 0.7, 1.0), vec4(0.7, 0.2, 0.8, 1.0), 5.5);
    glUniform1i( glGetUniformLocation(program, "light_out"), true);
    
    
    transform_bube22 = (RotateY( Theta8[Xaxis] ) * Translate( 41.0, 0.0, 0.0 ))*RotateY(Theta11[Xaxis])*Translate(1.5, 1, 0.0)*RotateY(Theta11[Xaxis])*Scale(0.5, 0.5, 0.5);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform_bube22 );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_NORMALS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );


    
    
    //---- Neptune
    glBindTexture( GL_TEXTURE_2D, textures[8] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial( vec4(0.5, 0.2, 0.7, 1.0), vec4(0.5, 0.2, 0.7, 1.0), vec4(0.7, 0.2, 0.8, 1.0), 5.5);
    glUniform1i( glGetUniformLocation(program, "light_out"), false );
    
    transform_bube2 = RotateY( Theta9[Xaxis] ) * Translate( 46.0, 0.0, 0 ) *RotateY(Theta9[Xaxis])* Scale(2.0, -2.0, 2.0);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform_bube2 );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_NORMALS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );
    //---- spherical MOON 1
    glBindTexture( GL_TEXTURE_2D, textures[11] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial( vec4(0.5, 0.2, 0.7, 1.0), vec4(0.5, 0.2, 0.7, 1.0), vec4(0.7, 0.2, 0.8, 1.0), 5.5);
    glUniform1i( glGetUniformLocation(program, "light_out"), true);
    
    
    transform_bube22 = (RotateY( Theta9[Xaxis] ) * Translate( 46.0, 0.0, 0.0 ))*RotateY(Theta11[Xaxis])*Translate(1.5, 1, 0.0)*RotateY(Theta11[Xaxis])*Scale(0.5, 0.5, 0.5);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform_bube22 );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_NORMALS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );
    
    
    
    //---- pluto
    glBindTexture( GL_TEXTURE_2D, textures[9] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial( vec4(0.5, 0.2, 0.7, 1.0), vec4(0.5, 0.2, 0.7, 1.0), vec4(0.7, 0.2, 0.8, 1.0), 5.5);
    glUniform1i( glGetUniformLocation(program, "light_out"), false );
    
    transform_bube2 = RotateY( Theta10[Xaxis] ) * Translate( 52.0, 0.0, 0 ) * RotateY(Theta10[Xaxis]) * Scale(2.0, -2.0, 2.0);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform_bube2 );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_NORMALS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );
    
    //---- spherical MOON 1
    glBindTexture( GL_TEXTURE_2D, textures[11] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial( vec4(0.5, 0.2, 0.7, 1.0), vec4(0.5, 0.2, 0.7, 1.0), vec4(0.7, 0.2, 0.8, 1.0), 5.5);
    glUniform1i( glGetUniformLocation(program, "light_out"), false);
    
    
    transform_bube22 = (RotateY( Theta10[Xaxis] ) * Translate( 52.0, 0.0, 0.0 ))*RotateY(Theta11[Xaxis])*Translate(-1.5, 0, 0.0)*RotateY(Theta11[Xaxis])*Scale(0.5, 0.5, 0.5);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform_bube22 );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_NORMALS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );
    
    
    //--- spherical planet
    glBindTexture( GL_TEXTURE_2D, textures[14] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial( vec4(0.5, 0.2, 0.7, 1.0), vec4(0.5, 0.2, 0.7, 1.0), vec4(0.7, 0.2, 0.8, 1.0), 5.5);
    glUniform1i( glGetUniformLocation(program, "light_out"), false );
    
    transform_bube2 = Translate( 57.0, 0.0, 1.9) * Scale(6.0, 6.0, 6.0);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform_bube2 );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_NORMALS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );
    
   //cylinder
    glUniform1i( glGetUniformLocation(program, "texture_on"), false );
    
    SetMaterial( vec4(0.7, 0.0, 0.0, 1.0), vec4(1.0, 0.0, 0.0, 1.0), vec4(1.0, 0.5, 0.5, 1.0), 5.5);
    glUniform1i( glGetUniformLocation(program, "light_out"), false );
   
        mat4 transform_tube = Translate( 47, -2, 1.5 )* RotateZ(110)* Scale(0.5, 13, 0.5);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform_tube );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CYLINDER_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CYLINDER_NORMALS_OFFSET) );
        glDrawArrays( GL_TRIANGLES, 0, NumVerticesCylinder )
    ;
    
  //---- spherical sun
    glBindTexture( GL_TEXTURE_2D, textures[10] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    SetMaterial( vec4(0.9, 0.9, 0.5, 1.0), vec4(0.9, 0.9, 0.5, 1.0), vec4(0.9, 0.9, 0.0, 1.0), 0.5);
    glUniform1i( glGetUniformLocation(program, "light_out"), true );
    
   transform_bube1 = Translate( 0.0, 0.0, 0.0 ) * Scale(300,300,300);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform_bube1 );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_NORMALS_OFFSET) );
    glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_TEXCOORDS_OFFSET) );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );
    
    
    
    

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    /* //---- cylindrical object
     glUniform1i( glGetUniformLocation(program, "texture_on"), false );
     
     SetMaterial( vec4(0.7, 0.0, 0.0, 1.0), vec4(1.0, 0.0, 0.0, 1.0), vec4(1.0, 0.5, 0.5, 1.0), 5.5);
     glUniform1i( glGetUniformLocation(program, "light_out"), false );
     
     mat4 transform_tube = Translate( 0.0, -2.5, 0.0 ) * Scale(1.5, 2.0, 1.5);
     glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform_tube );
     
     glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CYLINDER_OFFSET) );
     glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CYLINDER_NORMALS_OFFSET) );
     glDrawArrays( GL_TRIANGLES, 0, NumVerticesCylinder );*/
    
    
    /* //---- cube
     glBindTexture( GL_TEXTURE_2D, textures[10] );
     glUniform1i( glGetUniformLocation(program, "texture_on"), true );
     
     SetMaterial( vec4(0.5, 0.2, 0.7, 1.0), vec4(0.5, 0.2, 0.7, 1.0), vec4(0.7, 0.2, 0.8, 1.0), 5.5);
     glUniform1i( glGetUniformLocation(program, "light_out"), false );
     
     mat4 transform_cube = Translate( 0.0, 0.0, 0.0 ) * Scale(500, 500, 500);
     glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform_cube );
     
     glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_OFFSET) );
     glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_NORMALS_OFFSET) );
     glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_TEXCOORDS_OFFSET) );
     //glDrawArrays( GL_TRIANGLES, 0, NumVerticesCube );
     
     for(int i=0; i<6; i++)
     {
     glBindTexture( GL_TEXTURE_2D, textures[i%2] );
     glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_OFFSET+6*i*sizeof(vec4)));
     glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_NORMALS_OFFSET + 6*i*sizeof(vec3)));
     glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_TEXCOORDS_OFFSET + 6*i*sizeof(vec2)));
     glDrawArrays( GL_TRIANGLES, 0, 6 );
     }
    */
    glutSwapBuffers();
}


//----------------------------------------------------------------------------

void
keyboard( unsigned char key, int x, int y )
{
    switch( key ) {
        case 033:    // Escape key
        case 'q': case 'Q':
            exit( EXIT_SUCCESS );
            break;
            
        case 'y':    //---- theta
            Axis = Yaxis;
            
            
            Theta3[Axis] += 2.0;
            Theta3[Axis] = fmod(Theta3[Axis], 360.0);
            glutPostRedisplay();
            break;
        case 'z':    //---- phi
            Axis = Zaxis;
            Theta3[Axis] += 5.0;
            Theta3[Axis] = fmod(Theta3[Axis], 360.0);
            glutPostRedisplay();
            break;
            
        case 'r':    //---- increase radius
            r += 3.0;
            glutPostRedisplay();
            break;
        case 'R':    //---- decrease radius
            r -= 3.0;
            glutPostRedisplay();
            break;
            
        case 'u':
            //view_matrix = default_view_matrix;
            Theta3[Xaxis] = 0.0;
            Theta3[Yaxis] = 0.0;
            Theta3[Zaxis] = 0.0;
            glutPostRedisplay();
            break;
            
        case 'e':
            viewe++;
            viewe%=2;
            glutPostRedisplay();
            break;
            
    }
}




//----------------------------------------------------------------------------

void
mouse( int button, int state, int x, int y )
{
    if ( state == GLUT_DOWN ) {
        switch( button ) {
            case GLUT_LEFT_BUTTON:    Axis = Xaxis;  break;
            case GLUT_MIDDLE_BUTTON:  Axis = Yaxis;  break;
            case GLUT_RIGHT_BUTTON:   Axis = Zaxis;  break;
                
                
        }
    }
}


//----------------------------------------------------------------------------

void
idle( void )
{
    //Theta1[Axis] = fmod(Theta1[Axis]+1, 360.0);
    //Theta2[Axis] = fmod(Theta2[Axis]+2, 360.0);
   
    Theta1[Xaxis] = fmod(Theta1[Xaxis]+1.5, 360.0); //mercury
    Theta2[Xaxis] = fmod(Theta2[Xaxis]+1.2, 360.0);  //venus
    Theta3[Xaxis] = fmod(Theta3[Xaxis]+1.0, 360.0);    //Earth
    Theta4[Xaxis] = fmod(Theta4[Xaxis]+0.9,360.0);      //Mars
    Theta5[Xaxis] = fmod(Theta5[Xaxis]+0.7,360.0);      //Jupiter
    Theta6[Xaxis]=fmod(Theta6[Xaxis]+0.6,360.0);        //Staurn
    Theta7[Xaxis]=fmod(Theta7[Xaxis]+0.6,360.0 );       //Saturn Rings
    Theta8[Xaxis]=fmod(Theta8[Xaxis]+0.5,360.0 );          //Uranus
    Theta9[Xaxis] = fmod(Theta9[Xaxis]+0.4,360.0 );     //neptune
    Theta10[Xaxis]= fmod(Theta10[Xaxis]+0.3,360.0 );    //pluto
    
    Theta11[Xaxis]= fmod(Theta11[Xaxis]+2.0,360.0 );    //pluto
    Theta12[Xaxis]= fmod(Theta12[Xaxis]+1.5,360.0 );    //pluto
    Theta13[Xaxis]= fmod(Theta13[Xaxis]+1.0,360.0 );    //pluto
    Theta14[Xaxis]= fmod(Theta14[Xaxis]+1.2,360.0 );    //pluto
    Theta15[Xaxis]= fmod(Theta15[Xaxis]+1.6,360.0 );    //pluto
    Theta16[Xaxis]= fmod(Theta16[Xaxis]+9,360.0 );    //pluto
    Theta18[Xaxis]= fmod(Theta1[Xaxis]+2,360.0 );    //pluto

    jx=jx-1.0;
    jz=jz-0.003;
    glutPostRedisplay();
}

//----------------------------------------------------------------------------

void
reshape( int w, int h )
{
    glViewport(0,0,w,h);
}

//----------------------------------------------------------------------------

int
main( int argc, char **argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 800, 800 );
    glutCreateWindow( "Color Cube" );
    
    init();
    
    glutDisplayFunc( display );
    glutReshapeFunc( reshape );
    glutKeyboardFunc( keyboard );
    glutMouseFunc( mouse );
    glutIdleFunc( idle );
    
    glutMainLoop();
    return 0;
}

#endif
















