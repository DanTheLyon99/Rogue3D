
/* Derived from scene.c in the The OpenGL Programming Guide */
/* Keyboard and mouse rotation taken from Swiftless Tutorials #23 Part 2 */
/* http://www.swiftless.com/tutorials/opengl/camera2.html */

/* Frames per second code taken from : */
/* http://www.lighthouse3d.com/opengl/glut/index.php?fps */
/* Made by Daniel Lyon B.Sc. CS */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "graphics.h"
//double check
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>


//structs



typedef struct Chamber
{
   int northDoorX;
   int northDoorZ;
   int eastDoorX;
   int eastDoorZ;
   int southDoorX;
   int southDoorZ;
   int westDoorX;
   int westDoorZ;
   int centerX;
   int centerZ;
   int length;
   int width;
   int height;
   int section;
   //MOB INFO
   int inside;
   float mobPosX;
   float mobPosZ;
   int mobVisible;
   int mobVisibleChange;
   int mobAi;
   int mobModel;
   int mobState; // 0: searching or sitting (catcus) 1: attacking
   int mobAlive;
   //ITEM INFO
   int itemType;
   float itemPosX;
   float itemPosZ;
   float itemState;

   


}Chamber;
typedef struct arrow{
   float posX;
   float posZ;
}arrow;
/*
typedef struct Mob
{
   int mobPosX;
   int mobPosZ;
   int mobVisible;
   int mobVisibleChange;
   int mobAi;
   int mobModel;

}Mob;*/

struct uMesh {
	/* number corresponding to file number in ./models - what to draw */
   int meshNumber; 
	/* flag indicates if mesh should be drawn, 0 == no, 1 == yes */
   int drawMesh; 
	/* position of mesh */
   float xpos, ypos, zpos;
	/* rotation of mesh */
   float xrot, yrot, zrot;
	/* scale mesh */
   float scale;
};
struct uMesh userMesh[100];
struct timespec lastTime;
struct timespec newTime;

//GLOBAL
float spin = 0.0;
Chamber chambers[9];
int   level = 1;
//GLOBAL CLOUDS
int cloudLayer[30][30];
int cloudTime = 0;
//Moving Fish
int fishTime = 0;
//Mesh




extern int PointInFrustum( float x, float y, float z );

extern GLubyte world[WORLDX][WORLDY][WORLDZ];

/* mouse function called by GLUT when a button is pressed or released */
void mouse(int, int, int, int);

/* initialize graphics library */
extern void graphicsInit(int *, char **);

/* lighting control */
extern void setLightPosition(GLfloat, GLfloat, GLfloat);
extern GLfloat *getLightPosition();

/* viewpoint control */
extern void setViewPosition(float, float, float);
extern void getViewPosition(float *, float *, float *);
extern void getOldViewPosition(float *, float *, float *);
extern void setOldViewPosition(float, float, float);
extern void setViewOrientation(float, float, float);
extern void getViewOrientation(float *, float *, float *);

/* add cube to display list so it will be drawn */
extern void addDisplayList(int, int, int);

/* mob controls */
extern void createMob(int, float, float, float, float);
extern void setMobPosition(int, float, float, float, float);
extern void hideMob(int);
extern void showMob(int);

/* player controls */
extern void createPlayer(int, float, float, float, float);
extern void setPlayerPosition(int, float, float, float, float);
extern void hidePlayer(int);
extern void showPlayer(int);

/* tube controls */
extern void createTube(int, float, float, float, float, float, float, int);
extern void hideTube(int);
extern void showTube(int);

/* 2D drawing functions */
extern void draw2Dline(int, int, int, int, int);
extern void draw2Dbox(int, int, int, int);
extern void draw2Dcircle(float cx, float cy, float r, int num_segments);

extern void draw2Dtriangle(int, int, int, int, int, int);
extern void set2Dcolour(float[]);

/* texture functions */
extern int setAssignedTexture(int, int);
extern void unsetAssignedTexture(int);
extern int getAssignedTexture(int);
extern void setTextureOffset(int, float, float);

/* flag which is set to 1 when flying behaviour is desired */
extern int flycontrol;
/* flag used to indicate that the test world should be used */
extern int testWorld;
/* flag to print out frames per second */
extern int fps;
/* flag to indicate the space bar has been pressed */
extern int space;
/* flag indicates the program is a client when set = 1 */
extern int netClient;
/* flag indicates the program is a server when set = 1 */
extern int netServer;
/* size of the window in pixels */
extern int screenWidth, screenHeight;
/* flag indicates if map is to be printed */
extern int displayMap;
/* flag indicates use of a fixed viewpoint */
extern int fixedVP;

/* frustum corner coordinates, used for visibility determination  */
extern float corners[4][3];

/* determine which cubes are visible e.g. in view frustum */
extern void ExtractFrustum();
extern void tree(float, float, float, float, float, float, int);

/* allows users to define colours */
extern int setUserColour(int, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat,
                         GLfloat, GLfloat, GLfloat);
void unsetUserColour(int);
extern void getUserColour(int, GLfloat *, GLfloat *, GLfloat *, GLfloat *,
                          GLfloat *, GLfloat *, GLfloat *, GLfloat *);
/* mesh creation, translatio, rotation functions */
extern void setMeshID(int, int, float, float, float);
extern void unsetMeshID(int);
extern void setTranslateMesh(int, float, float, float);
extern void setRotateMesh(int, float, float, float);
extern void setScaleMesh(int, float);
extern void drawMesh(int);
extern void hideMesh(int);

                          

void gravity();
void avoidCubeEdge();
void gameWall();
int climbCube(int, int, int);
int MIN(int x, int y);
void drawDoor(int posX, int posZ, int direction);
int DoCollision(float x, float y, float z);
int inABlock(float x, float y, float z);
void drawChambers();
void drawHorizontalCorridor(int clear, int x1, int z1, int x2, int z2);
void drawVerticalCorridor(int clear, int x1, int z1, int x2, int z2);
int getRandomBetween(int minimum,int maximum);
void drawCorridorFull();
void oritentCheck();
float perlin2d(float x, float y, float freq, int depth);
void generateOverworld();
void generateCave();
void moveUpLevel();
void moveDownLevel();
void saveWorld(int worldNum);
void loadWorld(int worldNum);
void wipeWorld();
float newHeight(int posX , int posZ);
void loopclouds();
void drawMap();
void handleMesh();
//Mob stuff
void nextMove();
void spawnMobs();
void mobCollideCheck();
int nextTo(int id);
void visitCheck();
void moveTowardsPlayer(int id);
void loadDungeonStruct(int worldNum);
void saveDungeonStruct(int worldNum);
//Item stuff
void spawnItems();
void shoot();



/********* end of extern variable declarations **************/

/*** collisionResponse() ***/
/* -performs collision detection and response */
/*  sets new xyz  to position of the viewpoint after collision */
/* -can also be used to implement gravity by updating y position of vp*/
/* note that the world coordinates returned from getViewPosition()
	   will be the negative value of the array indices */
void shoot()
{
   printf("*BOW NOISE*\n");
   float roll,pitch,yaw;
   getViewOrientation(&pitch,&yaw,&roll);

   float x,y,z;
   getViewPosition(&x,&y,&z);
   //spawn the arrow
   setMeshID(45,18,-x,-y,-z);


}
void saveWorld(int worldNum)//DUN DUN DA DAAAA
{  
   char worldlevel[10];
   sprintf(worldlevel,"world%d",worldNum);
   //WRITE
   FILE *fptr = fopen(worldlevel, "wb");
   fwrite(world, sizeof(char), sizeof(world), fptr);
   fclose(fptr);


   
}
void saveDungeonStruct(int worldNum)
{
   char dungeon[10];
   sprintf(dungeon,"dungeon%d",worldNum);
   //WRITE
   FILE *fptr = fopen(dungeon, "wb");
   fwrite(chambers, sizeof(char), sizeof(Chamber), fptr);
   fclose(fptr);

}
void loadWorld(int worldNum)
{
   char worldlevel[10];
   sprintf(worldlevel,"world%d",worldNum);
   //READ
   FILE *fptr = fopen(worldlevel, "rb"); 
   fread(world, sizeof(char), sizeof(world), fptr);
   fclose(fptr);
}
void loadDungeonStruct(int worldNum)
{
   char dungeon[10];
   sprintf(dungeon,"dungeon%d",worldNum);
   //READ
   FILE *fptr = fopen(dungeon, "rb"); 
   fread(chambers, sizeof(char), sizeof(Chamber), fptr);
   fclose(fptr);

}
int inABlock(float x, float y, float z)
{
   //flipping coordinates for proper use in world array
   x = -x;
   y = -y;
   z = -z;
  //check if it is a grey down stair block
  if(world[(int)x][(int)y][(int)z] == 18){
    return  18;
  }

  else if(world[(int)(x - 0.2)][(int)(y)][(int)(z + 0.2)] == 18){
    return 18;
  }
 
  else if(world[(int)(x + 0.2)][(int)(y)][(int)(z - 0.2)] == 18){
    return 18;
  }

   else if(world[(int)(x + 0.2)][(int)(y)][(int)(z + 0.2)] == 18){
    return  18;
  }
  
  else if(world[(int)(x - 0.2)][(int)(y)][(int)(z - 0.2)] == 18){
    return 18;
  }

  //check if it is a white Up stair block
    if(world[(int)x][(int)y][(int)z] == 5){
    return  5;
  }

  else if(world[(int)(x - 0.2)][(int)(y)][(int)(z + 0.2)] == 5){
    return 5;
  }
 
  else if(world[(int)(x + 0.2)][(int)(y)][(int)(z - 0.2)] == 5){
    return 5;
  }

   else if(world[(int)(x + 0.2)][(int)(y)][(int)(z + 0.2)] == 5){
    return  5;
  }
  
  else if(world[(int)(x - 0.2)][(int)(y)][(int)(z - 0.2)] == 5){
    return 5;
  }
  //old

  if(world[(int)x][(int)y][(int)z] != 0){
    return  1;
  }

  else if(world[(int)(x - 0.2)][(int)(y)][(int)(z + 0.2)] != 0){
    return 1;
  }
 
  else if(world[(int)(x + 0.2)][(int)(y)][(int)(z - 0.2)] != 0){
    return 1;
  }

   else if(world[(int)(x + 0.2)][(int)(y)][(int)(z + 0.2)] != 0){
    return  1;
  }
  
  else if(world[(int)(x - 0.2)][(int)(y)][(int)(z - 0.2)] != 0){
    return 1;
  }
 

 return 0; 
}
void collisionResponse()
{

   // 	/* your code for collisions goes here */
   float oldX,oldY, oldZ;
   float x, y, z;
   getViewPosition(&x, &y, &z);
   getOldViewPosition(&oldX, &oldY, &oldZ);


   
   //takes away collision if fly control is on
   if(flycontrol == 0){
      // if we're on a white block doing UP stairs
      if(inABlock(x,y,z) == 5)
      {
         moveUpLevel();
      }
      // if we're on a grey block going DOWN stairs
      else if(inABlock(x,y,z) == 18)
      {
         moveDownLevel();
      }

    
     

    
      //Step detection
      else if(DoCollision(x, y, z) == 2)
      {
         getViewPosition(&x,&y,&z);
         setViewPosition(x,y-1,z);
      }
      //Wall collision
      else if (DoCollision(x, y, z) == 1)
      {
         //sticky walls
         
         if(DoCollision(x,y,z) != 1)
         {
            
            setViewPosition(x,y,z);
         }
         else if(DoCollision(x,y,oldZ) != 1)
         {
            setViewPosition(x,y,oldZ);
         }
         else if(DoCollision(oldX,y,z) != 1)
         {
            setViewPosition(oldX,y,z);
         }
         //normal wall collision
         else
         {
            getOldViewPosition(&oldX,&oldY,&oldZ);
            setViewPosition(oldX,oldY,oldZ);
         }
      }
      //World boundries
      if (x < -99 || x > 0)
      {
         getOldViewPosition(&x, &y, &z);
         setViewPosition(x, y, z);
      }
      if (z < -99 || z > 0)
      {
         getOldViewPosition(&x, &y, &z);
         setViewPosition(x, y, z);
      }
      if (y > 0 || y < -49)
      {
         getOldViewPosition(&x, &y, &z);
         setViewPosition(x, y, z);
      }
      //check to see if moved full block
      if((int)oldX != (int)x || (int)oldZ != (int)z)
      {    
         nextMove();

      }
   }

}
void nextMove()
{
   visitCheck();
   for(int i = 0; i < 9; i++){
      //if you're next to a cactus
      if(nextTo(i) == 1 && chambers[i].mobModel == 3 && chambers[i].mobAlive && level != 1) //if you're next to an alive cactus
      {
         //50% cactus 
         int attack = getRandomBetween(0,1);
         if(attack == 1)
            printf("Cactus Attacks And Hits!\n");
         else
            printf("Cactus Attacks And Misses!\n");

      }
      if(nextTo(i) == 1 && chambers[i].mobModel == 2 && chambers[i].mobAlive && level != 1) //if you're next to an alive bat
      {
         //50% cactus 
         int attack = getRandomBetween(0,1);
         if(attack == 1)
            printf("Bat Attacks And Hits!\n");
         else
            printf("Bat Attacks And Misses!\n");

      }
      if(nextTo(i) == 1 && chambers[i].mobModel == 1 && chambers[i].mobAlive && level != 1) //if you're next to an alive fish
      {
         //50% cactus 
         int attack = getRandomBetween(0,1);
         if(attack == 1)
            printf("Fish Attacks And Hits!\n");
         else
            printf("Fish Attacks And Misses!\n");

      }
      if(chambers[i].inside == 1)
      {
         moveTowardsPlayer(i);
      }
      else//do a little dance
      {
         if(chambers[i].mobModel != 3)//make a little love
         {
            if((chambers[i].mobPosX - chambers[i].centerX) < 4)//get down tonight
            {
               chambers[i].mobPosX++;
            }
            else
            {
               chambers[i].mobPosX--;

            }

         }
      }
     
     
   }
   mobCollideCheck();
   //LAST
   handleMesh();
   
}

int DoCollision(float x, float y, float z)
{

   
   //Step up if 1 block without anything ontop
   if(inABlock(x,y,z) == 1 && (inABlock(x,y-1,z) == 0))
   {
      return 2;
   }
   if(inABlock(x,y,z))
   {
      return 1;
   }
   else
   {
      return 0;
   }
}

/******* draw2D() *******/
/* draws 2D shapes on screen */
/* use the following functions: 			*/
/*	draw2Dline(int, int, int, int, int);		*/
/*	draw2Dbox(int, int, int, int);			*/
/*	draw2Dtriangle(int, int, int, int, int, int);	*/
/*	set2Dcolour(float []); 				*/
/* colour must be set before other functions are called	*/
void draw2D()
{

   if (testWorld)
   {
      /* draw some sample 2d shapes */
      if (displayMap == 1)
      {
         GLfloat green[] = {0.0, 0.5, 0.0, 0.5};
         set2Dcolour(green);
         draw2Dline(0, 0, 500, 500, 15);
         draw2Dtriangle(0, 0, 200, 200, 0, 200);

         GLfloat black[] = {0.0, 0.0, 0.0, 0.5};
         set2Dcolour(black);
         draw2Dbox(500, 380, 524, 388);
      }
   }
   else
   {

      /* your code goes here */
      if(displayMap > 0){
         drawMap();
      }
   }
}

/*** update() ***/
/* background process, it is called when there are no other events */
/* -used to control animations and perform calculations while the  */
/*  system is running */
/* -gravity must also implemented here, duplicate collisionResponse */
void update()
{
   int i, j, k;
   float *la;
   float x, y, z;

   /* sample animation for the testworld, don't remove this code */
   /* demo of animating mobs */
   if (testWorld)
   {

      /* update old position so it contains the correct value */
      /* -otherwise view position is only correct after a key is */
      /*  pressed and keyboard() executes. */
#if 0
// Fire a ray in the direction of forward motion
float xx, yy, zz;
getViewPosition(&x, &y, &z);
getOldViewPosition(&xx, &yy, &zz);
printf("%f %f %f %f %f %f\n", xx, yy, zz, x, y, z);
printf("%f %f %f\n",  -xx+((x-xx)*25.0), -yy+((y-yy)*25.0), -zz+((z-zz)*25.0));
createTube(2, -xx, -yy, -zz, -xx-((x-xx)*25.0), -yy-((y-yy)*25.0), -zz-((z-zz)*25.0), 5);
#endif

      getViewPosition(&x, &y, &z);
      setOldViewPosition(x, y, z);

      /* sample of rotation and positioning of mob */
      /* coordinates for mob 0 */
      static float mob0x = 50.0, mob0y = 25.0, mob0z = 52.0;
      static float mob0ry = 0.0;
      static int increasingmob0 = 1;
      /* coordinates for mob 1 */
      static float mob1x = 50.0, mob1y = 25.0, mob1z = 52.0;
      static float mob1ry = 0.0;
      static int increasingmob1 = 1;
      /* counter for user defined colour changes */
      static int colourCount = 0;
      static GLfloat offset = 0.0;

      /* move mob 0 and rotate */
      /* set mob 0 position */
      setMobPosition(0, mob0x, mob0y, mob0z, mob0ry);

      /* move mob 0 in the x axis */
      if (increasingmob0 == 1)
         mob0x += 0.2;
      else
         mob0x -= 0.2;
      if (mob0x > 50)
         increasingmob0 = 0;
      if (mob0x < 30)
         increasingmob0 = 1;

      /* rotate mob 0 around the y axis */
      mob0ry += 1.0;
      if (mob0ry > 360.0)
         mob0ry -= 360.0;

      /* move mob 1 and rotate */
      setMobPosition(1, mob1x, mob1y, mob1z, mob1ry);

      /* move mob 1 in the z axis */
      /* when mob is moving away it is visible, when moving back it */
      /* is hidden */
      if (increasingmob1 == 1)
      {
         mob1z += 0.2;
         showMob(1);
      }
      else
      {
         mob1z -= 0.2;
         hideMob(1);
      }
      if (mob1z > 72)
         increasingmob1 = 0;
      if (mob1z < 52)
         increasingmob1 = 1;

      /* rotate mob 1 around the y axis */
      mob1ry += 1.0;
      if (mob1ry > 360.0)
         mob1ry -= 360.0;

      /* change user defined colour over time */
      if (colourCount == 1)
         offset += 0.05;
      else
         offset -= 0.01;
      if (offset >= 0.5)
         colourCount = 0;
      if (offset <= 0.0)
         colourCount = 1;
      setUserColour(9, 0.7, 0.3 + offset, 0.7, 1.0, 0.3, 0.15 + offset, 0.3, 1.0);

      /* sample tube creation  */
      /* draws a purple tube above the other sample objects */
      createTube(1, 45.0, 30.0, 45.0, 50.0, 30.0, 50.0, 6);

      /* end testworld animation */
   }
   else
   {
      setRotateMesh(33,0,spin,0);
      if(spin >360)
      {
         spin = 0;

      }
      else
      {
         spin+= 4;
      }
      //gravity();
      float pitch;
      float yaw;
      float roll;

      /* your code goes here */
      loopclouds();
      clock_gettime(CLOCK_MONOTONIC_RAW, &newTime);
      uint64_t deltaTime = (newTime.tv_sec - lastTime.tv_sec) * 1000000 + (newTime.tv_nsec - lastTime.tv_nsec) / 1000;
      if( deltaTime > 20000){
         clock_gettime(CLOCK_MONOTONIC_RAW, &lastTime);
         //handleMesh();
      }
      getViewPosition(&x, &y, &z);
      //collisionResponse();
      //printf("HERE: X: %f, Y: %f , Z: %f\n", x, y, z); //SHOWS WHERE U R
      getViewOrientation(&pitch,&yaw,&roll);
      //stops neck from snapping
      oritentCheck();
      //printf("HERE: pitch: %f, yaw: %f , roll: %f\n", pitch, (float)((int)yaw %360), roll);

      //check where position -1

      //Checks if Flying is disabled before enabling gravity
      //if fly control is on, there won't be gravity or collision
      if (flycontrol == 0){
         gravity();
      }           

                
            
            
        
        
        
        
    }
      
      
}


/* called by GLUT when a mouse button is pressed or released */
/* -button indicates which button was pressed or released */
/* -state indicates a button down or button up event */
/* -x,y are the screen coordinates when the mouse is pressed or */
/*  released */
void mouse(int button, int state, int x, int y)
{

   if (button == GLUT_LEFT_BUTTON)
      printf("left button - ");
   else if (button == GLUT_MIDDLE_BUTTON)
      printf("middle button - ");
   else
      printf("right button - ");

   if (state == GLUT_UP)
      printf("up - ");
   else
      printf("down - ");

   printf("%d %d\n", x, y);
}

void gravity()
{
   float x,y,z;
   getViewPosition(&x,&y,&z);
   if(!inABlock(x,y+1,z) == 1)
   {
      setViewPosition(x,y + 0.5 ,z);
   }
   
}

/*Determine which way the user is jumping off the edge of a cube*/
void avoidCubeEdge()
{
   float x, y, z;             //New Viewpoint coordinate
   float oldX, oldY, oldZ;    //Old viewpoint coordinates
   float diffX, diffY, diffZ; //Stores the difference of the old and new coordinate

   int objX, objY, objZ;         //Object coordinate check #1
   int vpLocValueX, vpLocValueZ; //Decimal place of vp position

   /*Convert location to an integer*/
   getViewPosition(&x, &y, &z);
   getOldViewPosition(&oldX, &oldY, &oldZ);

   /*Determine if VP is at the very edge of the cube*/
   vpLocValueX = ((int)(floor(fabs(x) * 10))) % 10;
   vpLocValueZ = ((int)(floor(fabs(z) * 10))) % 10;

   /*Update the X axis based on the direction the user is going*/
   if (vpLocValueX < 3 || vpLocValueX > 6)
   {
      diffX = oldX - x;

      /*User is heading East*/
      if (diffX > 0)
      {
         x -= 0.2;
      }
      /*User is heading West*/
      else if (diffX < 0)
      {
         x += 0.2;
      }
   }

   /*Update the Y axis based on the direction the user is going*/
   if (vpLocValueZ < 3 || vpLocValueZ > 6)
   {
      diffZ = oldZ - z;

      /*User is heading North*/
      if (diffZ > 0)
      {
         z -= 0.2;
      }
      /*User is heading South*/
      else if (diffZ < 0)
      {
         z += 0.2;
      }
   }


  
}
//Randomly generates 9 rooms starting positions. Sections off room
int getRandomBetween(int minimum,int maximum){
   return rand()%((maximum+1)-minimum) + minimum;
}
void handleMesh()
{
  
   float x,y,z;
   getViewPosition(&x,&y,&z);

   for(int i = 0; i < 9; i++)
   {
      
      setTranslateMesh(i, chambers[i].mobPosX, 2, chambers[i].mobPosZ);
      setScaleMesh(i,0.5);
      if(chambers[i].mobModel == 3)
      {
         setTranslateMesh(i, chambers[i].mobPosX, 1, chambers[i].mobPosZ);// puts cacti on the ground

      }
      if(!chambers[i].mobAlive) //mob is dead
      {
         hideMesh(i);
      }

      //printf("mobPosX: %f mobPosZ: %f \n",chambers[i].mobPosX , chambers[i].mobPosZ );

      //visible
      // float distance = sqrt(pow((-x - chambers[i].centerX), 2) + pow((-z - chambers[i].centerZ), 2));
      // if(distance < 40 && PointInFrustum(chambers[i].centerX, 2.5, chambers[i].centerZ + meshOffset))
      // {
      //    drawMesh(i);
      //    chambers[i].mobVisible = 1;       
      // }
      // else
      // {
      //    hideMesh(i);
      //    chambers[i].mobVisible = 0;  
      // }
      //  if(level == 1)
      // {
      //    chambers[i].mobVisible = 0;  
      //    hideMesh(i);
      // }
      // if(chambers[i].mobVisible != chambers[i].mobVisibleChange)
      // {
      //    if(chambers[i].mobVisible)
      //    {
      //       //printf("Fish mesh #%d is visible\n ", i);
      //       chambers[i].mobVisibleChange = 1;
      //    }
      //    else
      //    {
      //       //printf("Fish mesh #%d is not visible\n ", i);
      //       chambers[i].mobVisibleChange = 0;
      //    }

      // }
      //visible
   }

}
void drawChambers()
{
   //0 will be swapped out for y position parameter later
   
   int i, j , k;
   int id = 0;
   
   for( i = 17; i <= 83; i+= 33)
   {
      for( j = 17; j <= 83; j+= 33)
      {
         Chamber curChamber;
         int width = getRandomBetween(5,10);
         int length = getRandomBetween(5,10);
         int height = 0 + getRandomBetween(5,8);
         int offSetCenterX = getRandomBetween(-4,4) + i;
         int offSetCenterZ = getRandomBetween(-4,4) + j;
         //printf("X: %d Z: %d \n ", offSetCenterX, offSetCenterZ);
         
         for(int l = (-width + offSetCenterX); l <= (width + offSetCenterX); l++)
         {
            for(int m = (-length + offSetCenterZ); m <= (length + offSetCenterZ); m++ )
            {
               //floor
               world[l][0][m] = getRandomBetween(19,21);
               //roof
               world[l][height][m] = 29;

            }
            
         }
         //MAKES HEIGHT
         for(int h = 0; h <= height; h++)
         {
            //EAST AND WEST WALLS
            for(k = (offSetCenterZ - length); k <= (offSetCenterZ + length); k++)
            {
               world[(offSetCenterX - width)][h][k] =29;
            }
            for(k = (offSetCenterZ - length); k <= (offSetCenterZ + length); k++)
            {
               world[(offSetCenterX + width)][h][k] =29;
            }
            //NORTH AND SOUTH WALLS
            for(k = (offSetCenterX - width); k <= (offSetCenterX + width); k++)
            {
               world[k][h][(offSetCenterZ - length)] =29;
            }
            for(k = (offSetCenterX - width); k <= (offSetCenterX + width); k++)
            {
               world[k][h][(offSetCenterZ + length)] =29;
            }
         }
         //NORTH DOOR
         if(offSetCenterZ > 33)
         {
            curChamber.northDoorZ = offSetCenterZ - length;
            curChamber.northDoorX = offSetCenterX + getRandomBetween(-width + 1, width - 1);
           
            world[curChamber.northDoorX][1][curChamber.northDoorZ] = 0;
            world[curChamber.northDoorX][2][curChamber.northDoorZ] = 0;
         }
         //EAST DOOR
         if(offSetCenterX <= 66)
         {
            curChamber.eastDoorX = offSetCenterX + width;
            curChamber.eastDoorZ = offSetCenterZ + getRandomBetween(-length + 1, length - 1);
      
            world[curChamber.eastDoorX][1][curChamber.eastDoorZ] = 0;
            world[curChamber.eastDoorX][2][curChamber.eastDoorZ] = 0;


         }
         //SOUTH DOOR
         if(offSetCenterZ <= 66)
         {
            curChamber.southDoorZ = offSetCenterZ + length;
            curChamber.southDoorX = offSetCenterX + getRandomBetween(-width + 1, width - 1);
            
            world[curChamber.southDoorX][1][curChamber.southDoorZ] = 0;
            world[curChamber.southDoorX][2][curChamber.southDoorZ] = 0;
         }
         //WEST DOOR
         if(offSetCenterX > 33)
         {
            curChamber.westDoorX = offSetCenterX - width;
            curChamber.westDoorZ = offSetCenterZ + getRandomBetween(-length + 1, length - 1);
          
            world[curChamber.westDoorX][1][curChamber.westDoorZ] = 0;
            world[curChamber.westDoorX][2][curChamber.westDoorZ] = 0;
         }
         
         //initialize the chamber variables for the dungeon
         curChamber.centerX = offSetCenterX;
         curChamber.centerZ = offSetCenterZ;
         curChamber.length = length;
         curChamber.width = width;
         curChamber.height = height;
         curChamber.section = id;
         curChamber.inside = 0;
         chambers[id] = curChamber;
         
         //Making Fish
         
         // userMesh[id].meshNumber = 1;
         // userMesh[id].drawMesh = 1;
         // userMesh[id].xpos = curChamber.centerX;
         // userMesh[id].ypos = 2;
         // userMesh[id].zpos = curChamber.centerZ;
         // userMesh[id].xrot = 0;
         // userMesh[id].yrot = 0;
         // userMesh[id].zrot = 0;
         // userMesh[id].scale = 1;

         world[chambers[4].centerX + 1][1][chambers[4].centerZ + 2] = 5; //spawn up stair block

         world[chambers[4].centerX + 3][1][chambers[4].centerZ + 4] = 18; //spawn down stair block

         
         id++;
      }
   }
   drawCorridorFull();
   


}
void spawnMobs()
{
   
   for(int i = 0; i < 9; i++){
      int mobType = getRandomBetween(1,3);
      setMeshID(i, mobType, chambers[i].centerX + 0.5 , 2, chambers[i].centerZ + 0.5);// places a random mob in the middle of each chamber
      chambers[i].mobPosX  = chambers[i].centerX + 0.5;
      chambers[i].mobPosZ  = chambers[i].centerZ + 0.5;
      chambers[i].mobVisible = 1;
      chambers[i].mobVisibleChange = 0;
      chambers[i].mobAi = mobType;
      chambers[i].mobModel = mobType;
      chambers[i].mobAlive = 1;
      

      if(mobType == 3)
      {

         chambers[i].mobPosX = chambers[i].centerX - 1.5;
      }

      


   }


}
void spawnItems()
{
      
   for(int i = 0; i < 9; i++){
      int itemType = getRandomBetween(4,18);
      int posZ =  getRandomBetween(chambers[i].centerZ - chambers[i].width, chambers[i].centerZ + chambers[i].width);
      setMeshID(i + 9, itemType, chambers[i].centerX + 0.5 , 1.5, posZ);// places a random mob in the middle of each chamber
      chambers[i].itemPosX  = chambers[i].centerX + 0.5;
      chambers[i].itemPosZ  = posZ;
      chambers[i].itemState = 1;
      chambers[i].itemType = itemType;
   

   }
}
void mobCollideCheck()
{
   float x,y,z;
   float oldX,oldY,oldZ;
   getViewPosition(&x,&y,&z);
   getOldViewPosition(&oldX,&oldY,&oldZ);



   for(int i = 0; i < 9; i++){
      
      

      if((int)-x == (int)chambers[i].mobPosX && (int)-z == (int)chambers[i].mobPosZ && chambers[i].mobAlive && level != 1) //if you bump into the mob AND they're alive
      {

         int randHit = getRandomBetween(0,1);
         printf("Attack!\n");
         setViewPosition(oldX,oldY,oldZ);
         if(randHit == 1)
         {
            printf("Player attacks and kills mob\n");
            chambers[i].mobAlive = 0;
         }
         else
         {
            printf("Player attacks and misses mob\n");
         }

      }

   }


}
void moveTowardsPlayer(int id)
{
   float x,y,z;
   getViewPosition(&x,&y,&z);
   if(chambers[id].mobModel != 3)
   {
      if(-x > chambers[id].mobPosX)
      {
         chambers[id].mobPosX++;
      }
      if(-x < chambers[id].mobPosX)
      {
         chambers[id].mobPosX--;
      }
      if(-z > chambers[id].mobPosZ)
      {
         chambers[id].mobPosZ++;
      }
      if(-z < chambers[id].mobPosZ)
      {
         chambers[id].mobPosZ--;
      }

   }

}
int nextTo(int id)
{
   float x,y,z;
   getViewPosition(&x,&y,&z);
   //mob is on your WEST
   if((int)-x -1 == (int)chambers[id].mobPosX && (int)-z == (int)chambers[id].mobPosZ)
   {
      return 1;
   }
   //mob is on your EAST
   else if((int)-x +1 == (int)chambers[id].mobPosX && (int)-z == (int)chambers[id].mobPosZ)
   {
      return 1;
   }

   //Mob is NORTH
   else if((int)-x == (int)chambers[id].mobPosX  && (int)-z -1 == (int)chambers[id].mobPosZ)
   {
      return 1;
   }
   //Mob is SOUTH
   else if((int)-x == (int)chambers[id].mobPosX && (int)-z +1 == (int)chambers[id].mobPosZ)
   {
      return 1;
   }
   //DIAGONALS
   //Mob is NORTH WEST
   else if((int)-x -1 == (int)chambers[id].mobPosX  && (int)-z -1 == (int)chambers[id].mobPosZ)
   {
      return 1;
   }
   //Mob is SOUTH WEST
   else if((int)-x -1 == (int)chambers[id].mobPosX && (int)-z +1 == (int)chambers[id].mobPosZ)
   {
      return 1;
   }
   //Mob is NORTH EAST
   else if((int)-x +1 == (int)chambers[id].mobPosX  && (int)-z -1 == (int)chambers[id].mobPosZ)
   {
      return 1;
   }
   //Mob is SOUTH EAST
   else if((int)-x +1 == (int)chambers[id].mobPosX && (int)-z +1 == (int)chambers[id].mobPosZ)
   {
      return 1;
   }

   //Mob Isn't near you
   else
   {
      return 0;
   }



}
void visitCheck()
{
   float x,y,z;
   getViewPosition(&x,&y,&z);
   for(int i = 0; i < 9; i++)
   {
      if(-x > chambers[i].centerX - chambers[i].width && -x < chambers[i].centerX + chambers[i].width &&
      -z > chambers[i].centerZ - chambers[i].height && -z < chambers[i].centerZ + chambers[i].height)
      {
         chambers[i].inside = 1;
      }
      else
      {
         chambers[i].inside = 0;
      }

   }

}
void drawCorridorFull()
{
   /* 0-3
      |
      1*/
   drawHorizontalCorridor(0,chambers[0].eastDoorX,chambers[0].eastDoorZ,chambers[3].westDoorX,chambers[3].westDoorZ);
   drawHorizontalCorridor(1,chambers[0].eastDoorX,chambers[0].eastDoorZ,chambers[3].westDoorX,chambers[3].westDoorZ);
   drawVerticalCorridor(0,chambers[0].southDoorX,chambers[0].southDoorZ,chambers[1].northDoorX,chambers[1].northDoorZ);
   drawVerticalCorridor(1,chambers[0].southDoorX,chambers[0].southDoorZ,chambers[1].northDoorX,chambers[1].northDoorZ);
   /* 3-6
      |
      4 */
   drawHorizontalCorridor(0,chambers[3].eastDoorX,chambers[3].eastDoorZ,chambers[6].westDoorX,chambers[6].westDoorZ);
   drawHorizontalCorridor(1,chambers[3].eastDoorX,chambers[3].eastDoorZ,chambers[6].westDoorX,chambers[6].westDoorZ);
   drawVerticalCorridor(0,chambers[3].southDoorX,chambers[3].southDoorZ, chambers[4].northDoorX,chambers[4].northDoorZ);
   drawVerticalCorridor(1,chambers[3].southDoorX,chambers[3].southDoorZ, chambers[4].northDoorX,chambers[4].northDoorZ);
   
   /* 6
      |
      7*/
   drawVerticalCorridor(0,chambers[6].southDoorX,chambers[6].southDoorZ,chambers[7].northDoorX,chambers[7].northDoorZ);
   drawVerticalCorridor(1,chambers[6].southDoorX,chambers[6].southDoorZ,chambers[7].northDoorX,chambers[7].northDoorZ);
   /* 1-4
      |
      2*/
   drawHorizontalCorridor(0,chambers[1].eastDoorX,chambers[1].eastDoorZ,chambers[4].westDoorX,chambers[4].westDoorZ);
   drawHorizontalCorridor(1,chambers[1].eastDoorX,chambers[1].eastDoorZ,chambers[4].westDoorX,chambers[4].westDoorZ);
   drawVerticalCorridor(0, chambers[1].southDoorX,chambers[1].southDoorZ, chambers[2].northDoorX,chambers[2].northDoorZ);
   drawVerticalCorridor(1, chambers[1].southDoorX,chambers[1].southDoorZ, chambers[2].northDoorX,chambers[2].northDoorZ);

   /*4-7
     |
     5*/
   drawHorizontalCorridor(0,chambers[4].eastDoorX,chambers[4].eastDoorZ,chambers[7].westDoorX,chambers[7].westDoorZ);
   drawHorizontalCorridor(1,chambers[4].eastDoorX,chambers[4].eastDoorZ,chambers[7].westDoorX,chambers[7].westDoorZ);
   drawVerticalCorridor(0, chambers[4].southDoorX,chambers[4].southDoorZ,chambers[5].northDoorX,chambers[5].northDoorZ);
   drawVerticalCorridor(1, chambers[4].southDoorX,chambers[4].southDoorZ,chambers[5].northDoorX,chambers[5].northDoorZ);
   /*7
     |
     8*/
   drawVerticalCorridor(0, chambers[7].southDoorX,chambers[7].southDoorZ,chambers[8].northDoorX,chambers[8].northDoorZ);
   drawVerticalCorridor(1, chambers[7].southDoorX,chambers[7].southDoorZ,chambers[8].northDoorX,chambers[8].northDoorZ);
   /* 2-5*/
   drawHorizontalCorridor(0,chambers[2].eastDoorX,chambers[2].eastDoorZ, chambers[5].westDoorX,chambers[5].westDoorZ);
   drawHorizontalCorridor(1,chambers[2].eastDoorX,chambers[2].eastDoorZ, chambers[5].westDoorX,chambers[5].westDoorZ);
   /* 5-8 */
   drawHorizontalCorridor(0,chambers[5].eastDoorX,chambers[5].eastDoorZ, chambers[8].westDoorX,chambers[8].westDoorZ);
   drawHorizontalCorridor(1,chambers[5].eastDoorX,chambers[5].eastDoorZ, chambers[8].westDoorX,chambers[8].westDoorZ);
   

      

}
void drawHorizontalCorridor(int mine,int x1, int z1, int x2, int z2)
{
   if(mine){

   
      for(int i = x1; i < (x1 + x2)/2; i++)
      {
         world[i][0][z1] = 1;
         world[i][1][z1] = 0; 
         world[i][2][z1] = 0; 
         world[i][3][z1] = 1;
      }
      if(z1 < z2){
         for(int i = z1; i <= z2;  i++)
         {
            world[(x1 + x2)/2][0][i] = 1;
            world[(x1 + x2)/2][1][i] = 0; 
            world[(x1 + x2)/2][2][i] = 0; 
            world[(x1 + x2)/2][3][i] = 1;
         }
      }
      else{
         for(int i = z1; i >= z2;  i--){
            world[(x1 + x2)/2][0][i] = 1;
            world[(x1 + x2)/2][1][i] = 0; 
            world[(x1 + x2)/2][2][i] = 0; 
            world[(x1 + x2)/2][3][i] = 1;
         }
      }
      for(int i = (x1 + x2)/2; i < x2; i++)
      {
         world[i][0][z2] = 1;
         world[i][1][z2] = 0; 
         world[i][2][z2] = 0;
         world[i][3][z2] = 1;
      }
   }
   else 
   {
      for(int i = x1; i <= (x1 + x2)/2; i++)
      {
         world[i][1][z1-1] = 1;
         world[i][1][z1+1] = 1;
         world[i][2][z1-1] = 1;
         world[i][2][z1+1] = 1;
      }
      if(z1 < z2){
         for(int i = z1; i <=z2;  i++)
         {
            world[((x1 + x2)/2)-1][1][i] = 1;
            world[((x1 + x2)/2)+1][1][i] = 1;
            world[((x1 + x2)/2)-1][2][i] = 1;
            world[((x1 + x2)/2)+1][2][i] = 1;
         }
      }
      else{
         for(int i = z1; i >= z2;  i--){
            world[((x1 + x2)/2)-1][1][i-1] = 1;
            world[((x1 + x2)/2)+1][1][i+1] = 1;
            world[((x1 + x2)/2)-1][2][i-1] = 1;
            world[((x1 + x2)/2)+1][2][i+1] = 1;
         }
      }
      for(int i = (x1 + x2)/2; i < x2; i++)
      {
         world[i][1][z2-1] = 1;
         world[i][1][z2+1] = 1;
         world[i][2][z2-1] = 1;
         world[i][2][z2+1] = 1;
      }
      
   }
}
   
//draws the vertical corridors
void drawVerticalCorridor(int mine, int x1, int z1, int x2, int z2)
{
   if(mine){
      for(int i = z1; i < (z1+z2)/2; i++)
      {
         //Floor
         world[x1][0][i] = 2;
         world[x1][1][i] = 0;
         world[x1][2][i] = 0;
         //Roof
         world[x1][3][i] = 2;
      }
      if(x2 > x1)
      {
         for(int i = x1; i <= x2; i++)
         {
            //Floor
            world[i][0][(z1+z2)/2] = 2;
            //Wall
            world[i][1][(z1+z2)/2] = 0;
            world[i][2][(z1+z2)/2] = 0;

            //Roof
            world[i][3][(z1+z2)/2] = 2;
         }
      }
      else
      {
         for(int i = x1; i >= x2; i--)
         {
            //Floor
            world[i][0][(z1+z2)/2] = 2;
            //Wall
            world[i][1][(z1+z2)/2] = 0;
            world[i][2][(z1+z2)/2] = 0;
            //Roof
            world[i][3][(z1+z2)/2] = 2;
         }
      }
      for(int i = (z1+z2)/2; i < z2; i++)
      {
         //Floor
         world[x2][0][i] = 2;
         //Wall
         world[x2][1][i] = 0;
         world[x2][2][i] = 0;
         //Roof
         world[x2][3][i] = 2;
      }
   }
   else{
      for(int i = z1; i <= (z1+z2)/2; i++)
      {
         //Floor
         //world[x1][0][i] = 2;
         //Wall
         world[x1-1][1][i] = 2;
         world[x1+1][1][i] = 2;
         world[x1-1][2][i] = 2;
         world[x1+1][2][i] = 2;

         //Roof
         //world[x1][3][i] = 2;
      }
   if(x2 > x1)
   {
      for(int i = x1; i <= x2; i++)
      {
         //Floor
         //world[i][0][(z1+z2)/2] = 2;
         //Wall
         world[i][1][((z1+z2)/2)-1] = 2;
         world[i][1][((z1+z2)/2)+1] = 2;
         world[i][2][((z1+z2)/2)-1] = 2;
         world[i][2][((z1+z2)/2)+1] = 2;

         //Roof
         //world[i][3][(z1+z2)/2] = 2;
      }
   }
   else
   {
      for(int i = x1; i >= x2; i--)
      {
         //Floor
         //world[i][0][(z1+z2)/2] = 2;
         //Wall
         world[i-1][1][((z1+z2)/2)-1] = 2;
         world[i+1][1][((z1+z2)/2)+1] = 2;
         world[i-1][2][((z1+z2)/2)-1] = 2;
         world[i+1][2][((z1+z2)/2)+1] = 2;
         //Roof
         //world[i][3][(z1+z2)/2] = 2;
      }
   }
   for(int i = (z1+z2)/2; i < z2; i++)
   {
      //Floor
      //world[x2][0][i] = 2;
      //Wall
      world[x2-1][1][i] = 2;
      world[x2+1][1][i] = 2;
      world[x2-1][2][i] = 2;
      world[x2+1][2][i] = 2;

      //Roof
      //world[x2][3][i] = 2;
   }
   }
}
void oritentCheck()
{
   float pitch, yaw , roll;
   getViewOrientation(&pitch,&yaw,&roll);
   if(pitch < -35)
   {
      setViewOrientation(-35,yaw,roll);
   }
   else if(pitch > 35)
   {
      setViewOrientation(35,yaw,roll);
   }
}
void generateOverworld()
{
   int i, j;
   float x,y,z;

   for(i = 0; i < WORLDX; i++)
   {
      for(j = 0; j < WORLDZ; j++)
      {
         int perlinHeight = (int)(perlin2d((float)i,(float)j,0.04,1) * 16);
         if(perlinHeight < 4)
         {
            world[i][perlinHeight][j] = getRandomBetween(19,21); // mud
         }
         else if(perlinHeight >= 4 && perlinHeight < 14)
         {
            world[i][perlinHeight][j] = getRandomBetween(22,24); //mountain
         }
         else
         {
            world[i][perlinHeight][j] = getRandomBetween(25,27); //mountain top glacier
         }

      }
   }



}
void generateCave()
{
   int i, j;
   float x,z;
   x = x-50/50;
   z = z-50/50;

   
        

   for(i = 0; i < WORLDX; i++)
   {
      for(j = 0; j < WORLDZ; j++)
      {
         //CAVE ROOF
         float y = 1 - (pow((float)(i - 50)/(float) 50, 2) + pow((float)(j-50)/(float)50 ,2))/2;
         int perlin = 7*perlin2d(i, j, 0.06, 1);
         world[i][(int)(y*10)][j] =  getRandomBetween(31,33);
         world[i][0][j] =  getRandomBetween(31,33);

         
   

      }
   }
   world[0][0][1] = 18;
   //WALLS
   for(i = 0; i < WORLDX; i++)
   {
      for(int k = 0; k < 49 ; k++)
      {
         world[i][k][98] = getRandomBetween(31,33);
         world[i][k][1] = getRandomBetween(31,33);

         world[98][k][i] = getRandomBetween(31,33);
         world[1][k][i] = getRandomBetween(31,33);
      }
     
   }
   //Stairs
   world[50][1][52] = 5; //spawn up stair block
   world[53][1][50] = 18; //spawn up stair block

}
   


void loopclouds(){
    if(time(NULL)-cloudTime){
       //handleMesh();
        for(int i =0; i< 100; i++){
            for(int j =0; j< 100; j++){
                if(j != 99){
                    world[i][20][j] = world[i][20][j+1];
                }else{
                    world[i][20][j] = world[i][20][0];
                }
            }
        }
        cloudTime = time(NULL);
        
    }
}
void wipeWorld()
{
     //GETS RID OF EVERY BLOCK LOADED INTO THE WORLD
      for(int i = 0; i < WORLDX; i++)
      {
         for(int j = 0; j < WORLDZ; j++)
         {
            for(int k = 0; k < WORLDY; k++)
            {
               world[i][k][j] = 0;
            }
         }    
      }
      // for(int i = 0; i < 100; i++)
      // {
      //    unsetMeshID(i);

      // }
}
void moveUpLevel()
{
   level++;  
 
   //setViewPosition(-50,-45,-50);
   wipeWorld();
   loadWorld(level);
   float y = newHeight(-50,-50);
   //printf("NEW Y: %f\n", y);
   setViewPosition(-50,y -2.5 ,-50);
   
}
float newHeight(int posX , int posZ)
{
   
   //LOOP THROUGH THE WORLD AT YOUR POSITION UNTIL
   //YOU HIT THE NEW HEIGHT OF THE NEXT WORLD

   for(int i = 0; i < WORLDY; i++)
   {
      if(world[-posX][i][-posZ] != 0)
      {
         //GIVES THE Y VALUE OF HEIGHT OF NEW WORLD
         return ((float) (-i));
      }
   }
   return 0;

}
void moveDownLevel()
{
   if(level >= -1){
      level--;
      wipeWorld();
      loadWorld(level);
      float y = newHeight(-50,-50);
      //printf("NEW Y: %f\n", y);
      setViewPosition(-50,y -1.5 ,-50);

      //spawnMobs();
      loadDungeonStruct(level);
   }

}
int MIN(int x, int y)
{
   if (x < y)
      return x;
   else
      return y;
}
/* https://gist.github.com/nowl/828013*/
static int SEED = 0;

static int hash[] = {208,34,231,213,32,248,233,56,161,78,24,140,71,48,140,254,245,255,247,247,40,
                     185,248,251,245,28,124,204,204,76,36,1,107,28,234,163,202,224,245,128,167,204,
                     9,92,217,54,239,174,173,102,193,189,190,121,100,108,167,44,43,77,180,204,8,81,
                     70,223,11,38,24,254,210,210,177,32,81,195,243,125,8,169,112,32,97,53,195,13,
                     203,9,47,104,125,117,114,124,165,203,181,235,193,206,70,180,174,0,167,181,41,
                     164,30,116,127,198,245,146,87,224,149,206,57,4,192,210,65,210,129,240,178,105,
                     228,108,245,148,140,40,35,195,38,58,65,207,215,253,65,85,208,76,62,3,237,55,89,
                     232,50,217,64,244,157,199,121,252,90,17,212,203,149,152,140,187,234,177,73,174,
                     193,100,192,143,97,53,145,135,19,103,13,90,135,151,199,91,239,247,33,39,145,
                     101,120,99,3,186,86,99,41,237,203,111,79,220,135,158,42,30,154,120,67,87,167,
                     135,176,183,191,253,115,184,21,233,58,129,233,142,39,128,211,118,137,139,255,
                     114,20,218,113,154,27,127,246,250,1,8,198,250,209,92,222,173,21,88,102,219};

int noise2(int x, int y)
{
    int tmp = hash[(y + SEED) % 256];
    return hash[(tmp + x) % 256];
}

float lin_inter(float x, float y, float s)
{
    return x + s * (y-x);
}

float smooth_inter(float x, float y, float s)
{
    return lin_inter(x, y, s * s * (3-2*s));
}

float noise2d(float x, float y)
{
    int x_int = x;
    int y_int = y;
    float x_frac = x - x_int;
    float y_frac = y - y_int;
    int s = noise2(x_int, y_int);
    int t = noise2(x_int+1, y_int);
    int u = noise2(x_int, y_int+1);
    int v = noise2(x_int+1, y_int+1);
    float low = smooth_inter(s, t, x_frac);
    float high = smooth_inter(u, v, x_frac);
    return smooth_inter(low, high, y_frac);
}

float perlin2d(float x, float y, float freq, int depth)
{
    float xa = x*freq;
    float ya = y*freq;
    float amp = 1.0;
    float fin = 0;
    float div = 0.0;

    int i;
    for(i=0; i<depth; i++){
        div += 256 * amp;
        fin += noise2d(xa, ya) * amp;
        amp /= 2;
        xa *= 2;
        ya *= 2;
    }

    return fin/div;
}
/*https://gist.github.com/nowl/828013*/
void drawMap()
{
   if(level != 1){
      float x,y,z;
      getViewPosition(&x,&y,&z);
      int scaling = 3;
      GLfloat characterColour[] = {1.0,0.0,0.0,1};
      set2Dcolour(characterColour);
      draw2Dcircle(-x*scaling,-z*scaling,5,7);
      for(int i = 0; i < 9; i++)
      {
         
         //Mobs
         GLfloat cactusColour[] = {0.0,1,0.0,1};
         GLfloat batColour[] = {0,0,0,1};
         GLfloat fishColour[] = {0,0,1,1};
         if(chambers[i].mobModel == 1){
            set2Dcolour(fishColour);
         }
         if(chambers[i].mobModel == 2){
            set2Dcolour(batColour);
         }
         if(chambers[i].mobModel == 3){
            set2Dcolour(cactusColour);
         }
         if(chambers[i].mobAlive){//only draw on map if mob is alive
            if(chambers[i].mobModel == 2){//draw bats as triangles
               draw2Dtriangle((chambers[i].mobPosX - 1)*scaling , (chambers[i].mobPosZ)*scaling , 
               (chambers[i].mobPosX + 1)*scaling , (chambers[i].mobPosZ)*scaling , (chambers[i].mobPosX)*scaling , (chambers[i].mobPosZ + 1)*scaling);
            }
            else{
               draw2Dbox((chambers[i].mobPosX - 1)*scaling , (chambers[i].mobPosZ - 1)*scaling , (chambers[i].mobPosX + 1)*scaling , (chambers[i].mobPosZ + 1)*scaling );

            }
         }
         //Chambers
         GLfloat chamberColour[] = {0.5,0.3,0.6,0.4};
         set2Dcolour(chamberColour);
         draw2Dbox((chambers[i].centerX - chambers[i].width) * scaling, (chambers[i].centerZ - chambers[i].length) * scaling, (chambers[i].centerX + chambers[i].width) *scaling, (chambers[i].centerZ + chambers[i].length)*scaling);
      
         //Corridors
         GLfloat corridorColour[] = {1.0,1.0 ,1.0, 1.0};
         set2Dcolour(corridorColour);
         draw2Dline((chambers[0].eastDoorX) * scaling,(chambers[0].eastDoorZ) * scaling ,(chambers[3].westDoorX) * scaling ,(chambers[3].westDoorZ) * scaling ,5);
         draw2Dline((chambers[0].southDoorX)* scaling ,(chambers[0].southDoorZ)* scaling ,(chambers[1].northDoorX)* scaling,(chambers[1].northDoorZ)* scaling,5);

         draw2Dline((chambers[3].eastDoorX)* scaling , (chambers[3].eastDoorZ)* scaling ,(chambers[6].westDoorX)* scaling , (chambers[6].westDoorZ)* scaling,5);
         draw2Dline((chambers[3].southDoorX)* scaling , (chambers[3].southDoorZ)* scaling , (chambers[4].northDoorX)* scaling ,(chambers[4].northDoorZ)* scaling,5);

         draw2Dline((chambers[6].southDoorX)* scaling ,(chambers[6].southDoorZ)* scaling , (chambers[7].northDoorX)* scaling , (chambers[7].northDoorZ)* scaling,5);
         
         draw2Dline((chambers[1].eastDoorX)* scaling , (chambers[1].eastDoorZ)* scaling , (chambers[4].westDoorX)* scaling , (chambers[4].westDoorZ)* scaling,5);
         draw2Dline((chambers[1].southDoorX)* scaling, (chambers[1].southDoorZ)* scaling, (chambers[2].northDoorX)* scaling, (chambers[2].northDoorZ)* scaling,5);
         
         draw2Dline((chambers[4].eastDoorX)* scaling , (chambers[4].eastDoorZ)* scaling , (chambers[7].westDoorX)* scaling , (chambers[7].westDoorZ)* scaling,5);
         draw2Dline((chambers[4].southDoorX)* scaling , (chambers[4].southDoorZ)* scaling ,(chambers[5].northDoorX)* scaling ,(chambers[5].northDoorZ)* scaling,5);

         draw2Dline((chambers[7].southDoorX)* scaling ,(chambers[7].southDoorZ)* scaling , (chambers[8].northDoorX)* scaling , (chambers[8].northDoorZ)* scaling, 5);

         draw2Dline((chambers[2].eastDoorX)* scaling, (chambers[2].eastDoorZ)* scaling, (chambers[5].westDoorX)* scaling,(chambers[5].westDoorZ)* scaling,5);

         draw2Dline((chambers[5].eastDoorX)* scaling, (chambers[5].eastDoorZ)* scaling, (chambers[8].westDoorX)* scaling, (chambers[8].westDoorZ)* scaling, 5);
         
         
         

      }
   }

   

}
int main(int argc, char **argv)
{
   //SEED TIME
   srand(time(NULL));
   int i, j, k;
   /* initialize the graphics system */
   graphicsInit(&argc, argv);

   /* the first part of this if statement builds a sample */
   /* world which will be used for testing */
   /* DO NOT remove this code. */
   /* Put your code in the else statment below */
   /* The testworld is only guaranteed to work with a world of
		with dimensions of 100,50,100. */
   if (testWorld == 1)
   {
      /* initialize world to empty */
      for (i = 0; i < WORLDX; i++)
         for (j = 0; j < WORLDY; j++)
            for (k = 0; k < WORLDZ; k++)
               world[i][j][k] = 0;

      /* some sample objects */
      /* build a red platform */
      for (i = 0; i < WORLDX; i++)
      {
         for (j = 0; j < WORLDZ; j++)
         {
            world[i][24][j] = 3;
         }
      }
      /* create some green and blue cubes */
      world[50][25][50] = 1;
      world[49][25][50] = 1;
      world[49][26][50] = 1;
      world[52][25][52] = 2;
      world[52][26][52] = 2;

      /* create user defined colour and draw cube */
      setUserColour(9, 0.7, 0.3, 0.7, 1.0, 0.3, 0.15, 0.3, 1.0);
      world[54][25][50] = 9;

      /* blue box shows xy bounds of the world */
      for (i = 0; i < WORLDX - 1; i++)
      {
         world[i][25][0] = 2;
         world[i][25][WORLDZ - 1] = 2;
      }
      for (i = 0; i < WORLDZ - 1; i++)
      {
         world[0][25][i] = 2;
         world[WORLDX - 1][25][i] = 2;
      }

      /* create two sample mobs */
      /* these are animated in the update() function */
      createMob(0, 50.0, 25.0, 52.0, 0.0);
      createMob(1, 50.0, 25.0, 52.0, 0.0);

      /* create sample player */
      createPlayer(0, 52.0, 27.0, 52.0, 0.0);
   }
   else
   {
        /* your code to build the world goes here */
      //RANDOMZIES PERLIN NOISE
      SEED = getRandomBetween(0,200);

      //Down Stair
      setAssignedTexture(18,54);
      //Up stair
      setAssignedTexture(5,53);
      //MUD
      setAssignedTexture(19, 23); 
      setAssignedTexture(20, 23);
      setAssignedTexture(21, 23);
      //MOUNTAINS
      setAssignedTexture(22, 41); 
      setAssignedTexture(23, 41);
      setAssignedTexture(24, 41);
      //MOUNTAIN GLACIER
      setAssignedTexture(25, 40); 
      setAssignedTexture(26, 40);
      setAssignedTexture(27, 40);
      //CLOUDS
      setUserColour(28, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0); //lightens the cube with color 28
      setAssignedTexture(28, 52); // assignes texture [second arg]52 in textures folder to block color [first arg] 28
      //DUNGEON WALL
      setAssignedTexture(29, 39); 
      //Corridor
      setAssignedTexture(1, 20); 
      setAssignedTexture(2, 20); 
      setAssignedTexture(3, 20); 
      //Cave
      setAssignedTexture(30,55);
      setAssignedTexture(31,55);
      setAssignedTexture(32,56);//iron
      setAssignedTexture(33,57);//coal
      
      
      

      
      

    
      //grey DOWN STAIR
      setUserColour(18,(GLfloat)254/500,(GLfloat)250/500,(GLfloat)253/500,(GLfloat)1,(GLfloat)254/500,(GLfloat)250/500,(GLfloat)253/500,(GLfloat)1);
      //MUD
      setUserColour(19,(GLfloat)64/500,(GLfloat)44/500,(GLfloat)27/500,(GLfloat)1,(GLfloat)64/500,(GLfloat)44/500,(GLfloat)27/500,(GLfloat)1);//brown
      setUserColour(20,(GLfloat)122/500,(GLfloat)79/500,(GLfloat)43/500,(GLfloat)1,(GLfloat)112/500,(GLfloat)79/500,(GLfloat)43/500,(GLfloat)1);//lighter brown
      setUserColour(21,(GLfloat)56/500,(GLfloat)36/500,(GLfloat)20/500,(GLfloat)1,(GLfloat)56/500,(GLfloat)36/500,(GLfloat)20/500,(GLfloat)1);//darker brown
      //MOUNTAINS
      setUserColour(22,(GLfloat)118/500,(GLfloat)198/500,(GLfloat)65/500,(GLfloat)1,(GLfloat)118/500,(GLfloat)198/500,(GLfloat)65/500,(GLfloat)1);//light green
      setUserColour(23,(GLfloat)94/500,(GLfloat)157/500,(GLfloat)52/500,(GLfloat)1,(GLfloat)94/500,(GLfloat)157/500,(GLfloat)52/500,(GLfloat)1);//moss green
      setUserColour(24,(GLfloat)49/500,(GLfloat)142/500,(GLfloat)56/500,(GLfloat)1,(GLfloat)49/500,(GLfloat)142/500,(GLfloat)56/500,(GLfloat)1);//darker green
      //MOUNTAIN TOP GLACIERS

      setUserColour(25,(GLfloat)173/500,(GLfloat)213/500,(GLfloat)219/500,(GLfloat)1,(GLfloat)173/500,(GLfloat)213/500,(GLfloat)219/500,(GLfloat)1);//mountain bluey white
      setUserColour(26,(GLfloat)158/500,(GLfloat)211/500,(GLfloat)219/500,(GLfloat)1,(GLfloat)158/500,(GLfloat)211/500,(GLfloat)219/500,(GLfloat)1);//mountain light bluey white
      setUserColour(27,(GLfloat)191/500,(GLfloat)213/500,(GLfloat)217/500,(GLfloat)1,(GLfloat)191/500,(GLfloat)213/500,(GLfloat)217/500,(GLfloat)1);//mountain darker bluey white
      //CLOUDS
      setUserColour(28,(GLfloat)250/500,(GLfloat)255/500,(GLfloat)255/500,(GLfloat)1,(GLfloat)250/500,(GLfloat)255/500,(GLfloat)255/500,(GLfloat)1);

      //DUNGEON WALLS
      setUserColour(29,(GLfloat)100/500,(GLfloat)100/500,(GLfloat)100/500,(GLfloat)100,(GLfloat)100/500,(GLfloat)100/500,(GLfloat)100/500,(GLfloat)100);
      setUserColour(29,(GLfloat)100/500,(GLfloat)100/500,(GLfloat)100/500,(GLfloat)100,(GLfloat)100/500,(GLfloat)100/500,(GLfloat)100/500,(GLfloat)100);
      setUserColour(29,(GLfloat)100/500,(GLfloat)100/500,(GLfloat)100/500,(GLfloat)100,(GLfloat)100/500,(GLfloat)100/500,(GLfloat)100/500,(GLfloat)100);
      //Cave
      setUserColour(30,(GLfloat)255/500,(GLfloat)102/500,(GLfloat)0/500,(GLfloat)255/500,(GLfloat)102/500,(GLfloat)0/500,(GLfloat)100/500,(GLfloat)100);
      setUserColour(31,(GLfloat)255/500,(GLfloat)120/500,(GLfloat)100/500,(GLfloat)31/500,(GLfloat)255/500,(GLfloat)120/500,(GLfloat)31/500,(GLfloat)100);
      setUserColour(32,(GLfloat)255/500,(GLfloat)139/500,(GLfloat)61/500,(GLfloat)255/500,(GLfloat)139/500,(GLfloat)61/500,(GLfloat)100/500,(GLfloat)100);//iron
      setUserColour(33,(GLfloat)255/500,(GLfloat)139/500,(GLfloat)61/500,(GLfloat)255/500,(GLfloat)139/500,(GLfloat)61/500,(GLfloat)100/500,(GLfloat)100);//coal

      





      
      
      //x depth y height 2 z
      // for (int i = 0; i < WORLDX; i++)
      // {

      //    for (int j = 0; j < WORLDZ; j++)
      //    {
      //       //GROUND IS AT 25 Y
      //       //Make a checker board pattern for the ground
      //       if ((i + j) & 1)
      //          world[i][25][j] = 4; //Black
      //       else
      //          world[i][25][j] = 5; //White

      //       //MAKE THE BOARD INTO 3X3, 9 ROOM SECTIONS
      //       if (i % 33 == 0)
      //       {
      //          world[i][25][j] = 1; //GREEN;
      //       }
      //       if (j % 33 == 0)
      //       {
      //          world[i][25][j] = 1; //GREEN;
      //       }
      //    }
      // }
      

      

      drawChambers();
      spawnMobs();
      spawnItems();
      saveDungeonStruct(-2);
      saveWorld(-2);
      wipeWorld();

      generateCave();
      saveWorld(-1);
      wipeWorld();

      drawChambers();
      spawnMobs();
      spawnItems();
      setMeshID(33,11,chambers[4].centerX,1.5,chambers[4].centerZ -2);

      saveDungeonStruct(0);
      saveWorld(0);
      wipeWorld();
      generateOverworld();

      
     
      
      
      

        //Cloud array 1
      for(int i = 0; i < 13; i++)
      {
         world[20+(i*5)][20][35+(i*5)]= 28;
         world[20+(i*5)][20][36+(i*5)]= 28;
         world[21+(i*5)][20][35+(i*5)]= 28;
         world[21+(i*5)][20][36+(i*5)]= 28;
         for(int j = 0; j < 15; j++)
         {
            world[12+ (j*3)][20][28+ (j*3)]= 28;
            world[32+ (j*3)][20][46+ (j*3)]= 28;
            world[23+ (j*3)][20][55+ (j*3)]= 28;
            world[23+ (j*3)][20][12+ (j*3)]= 28;
         }
      }
      //cloud array 2
      for(int i = 5; i < 23; i++)
      {
         world[16+(i*2)][20][16+(i*2)]= 28;
         world[19+(i*2)][20][12+(i*3)]= 28;
         world[2+(i*3)][20][33+(i*2)]= 28;
         world[21+(i*2)][20][31+(i*3)]= 28;
         for(int j = 0; j < 11; j++)
         {
            world[56+ (j*3)][20][45+ (j*3)]= 28;
            world[67+ (j*3)][20][24+ (j*3)]= 28;
            world[65+ (j*3)][20][21+ (j*3)]= 28;
            world[65 + (j*3)][20][32+ (j*3)]= 28;
         }
      }


      float y = newHeight(-50,-50);
      //DOWN STAIR
      world[50][(int)(-y) + 1][51] = 18;
      saveWorld(1);
      
      //printf("Y: %f\n", y);
      

      setViewPosition(-50,y - 2.5,-50);

      //world[51][27][51] = 18;
 
   

      

      setViewOrientation(0,0,0);


      
   

   
     
    

      


   
   }

   /* starts the graphics processing loop */
   /* code after this will not run until the program exits */
   glutMainLoop();
   return 0;
}
