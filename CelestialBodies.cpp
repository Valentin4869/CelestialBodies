

#include "stdafx.h"
#include "Physics_Functions.h"
#include <stdio.h>
#include <math.h>
#include <string>
#include <time.h>

Box box;
bool leftButton = false, middleButton = false, rightButton=false;
float sphi=30.0, stheta=-30.0, sheight=0, shor=0;
int downX, downY;
float sdepth = 20;
bool keypress=false;
bool keypress2=false;
bool print=false;

float lightx=1, lighty=1, lightz=1;
bool stepb=false;
bool paused=true;
bool removep=false;
int removei=0;
Object3d obj;
void Render();
void Render3();
void Idle();
void mouse(int button, int state,int x, int y);
void motion(int x, int y);
void SpecialKey ( int k, int x, int y );
vector<vector<Vector3d>> positions(PARTICLE_C);
vector<vector<Vector3d>> p_positions(POINT_PARTICLE_C);

//Acceleration caused by particles in P on ip
Vector3d acceleration(State ip, vector<Particle3d> P, float t)
{
	/*
	const float k = 10;
	const float b = 1;
	return - k*state.x - b*state.v;*/
	Vector3d ACCELERATION;
	Vector3d tempAcceleration;
	for(int i=0; i<PARTICLE_C; i++)
	{

		
		if(ip.i==i) continue;
		
		
		real d=sqrt( (ip.Position.x-P[i].position.x)*(ip.Position.x-P[i].position.x) +
				 (ip.Position.y-P[i].position.y)*(ip.Position.y-P[i].position.y) +
				 (ip.Position.z-P[i].position.z)*(ip.Position.z-P[i].position.z)
			   );

		
	real wk=10.0;
	real f=-6.67384*0.000000180*wk*ip.mass*P[i].mass/(d*d);
		
	//cout<<"Distance: "<<d<<endl;
	//cout<<"F: "<<f<<"= "<<-6.67384*0.00000000010<<" * "<<ip.mass<<" * "<<P[i].mass<<"/("<<d*d<<")\n";
	/*if(print)
	{cout<<"DISTANCE: "<<d<<endl;
	cout<<"FORCE: m1: "<<m1<<", m2: "<<m2<<"----> "<<f<<endl;}*/
		


	
	
	tempAcceleration.Set((ip.Position.x-P[i].position.x)*f/d/ip.mass, (ip.Position.y-P[i].position.y)*f/d/ip.mass,
		(ip.Position.z-P[i].position.z)*f/d/ip.mass);
	ACCELERATION+=tempAcceleration;//accumulate accerleations

	
	/*if(print)
	{cout<<"aX: "<<(Position1.x-Position2.x)*f/d/m1<<endl;
	cout<<"aY: "<<(Position1.y-Position2.y)*f/d/m1<<endl;
	cout<<"aZ: "<<(Position1.z-Position2.z)*f/d/m1<<"\n";
	}*/
		
	

	}
	
	print=false;

	return ACCELERATION;

}

Derivative evaluate(int ip, vector<Particle3d> P, float t)
{
	Derivative output;
	output.dPosition = P[ip].velocity;
	
	output.dVelocity = acceleration(P3dToState(P[ip]), P, t);
	return output;
}

Derivative evaluate(int ip, vector<Particle3d> P, float t, float dt, Derivative &d)
{
	State state1, state2;

	Derivative output;
	state1.i=ip;
	state1.mass=P[ip].mass;
	state1.Position = P[ip].position + d.dPosition*dt;// dp/dt= velocity
	state1.Velocity = P[ip].velocity + d.dVelocity*dt;//p''=dv/dt= acceleration
	
	
	output.dPosition = state1.Velocity;
	
	output.dVelocity= acceleration(state1, P, t+dt);

	return output;
}

void integrate(int ip,vector<Particle3d> P, float t, float dt)
{
	
	Derivative k1 = evaluate(ip, P, t);
	Derivative k2 = evaluate(ip, P, t, dt*0.5f, k1);
	Derivative k3 = evaluate(ip, P, t, dt*0.5f, k2);
	Derivative k4 = evaluate(ip, P, t, dt, k3);

	Vector3d dPOSITION;
	Vector3d dVELOCITY;

	dPOSITION= 1.0f/6.0f*(k1.dPosition + 2.0f*(k2.dPosition + k3.dPosition) + k4.dPosition);
	dVELOCITY = 1.0f/6.0f*(k1.dVelocity + 2.0f*(k2.dVelocity + k3.dVelocity) + k4.dVelocity);
	
	
	/*P[ip].position += dPOSITION*dt;
	P[ip].velocity += dVELOCITY*dt;
	P[ip].acceleration = dVELOCITY;*/

	
	p3d_states[ip].Position += dPOSITION*dt;
	p3d_states[ip].Velocity += dVELOCITY*dt;
	p3d_states[ip].Acceleration = dVELOCITY*dt;

}




void reshape (int w, int h) {
    glViewport (0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glOrtho(0,800,0,600,0,0);
	gluPerspective (60, (GLfloat)w / (GLfloat)h, 1.0, 100000000000.0);
    glMatrixMode (GL_MODELVIEW);
	
}
void InitGL(int* argc, char** argv)
{
	 glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutInit            ( argc, argv ); 
	
	//glOrtho(-1,1,1,-1,-2,2);
  glutInitWindowSize	(800, 600); 
  glutInitWindowPosition(420,60);
  glutCreateWindow		("Physics Engine");  
	
  

  GLfloat mat_specular[] = { 0.20, 0.250, .250, .250 };
   GLfloat mat_shininess[] = { 40.0 };
  

   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);

   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
   

	glEnable			(GL_DEPTH_TEST);	
	glEnable(GL_BLEND);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   
   
	//glEnable(GL_COLOR_MATERIAL);
	
    			
	//glClearColor		(0.0f, 0.0f, 0.0f, 0.5f);				
									
	
    glShadeModel		(GL_SMOOTH);						
    glDepthFunc			(GL_LEQUAL);								
	//glEnable			(GL_BLEND);
	//glBlendFunc			(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glHint			(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

 

 // glutFullScreen      ( );  

//glutMotionFunc(motion);
//glutMouseFunc(mouse);
//glutPassiveMotionFunc(passive);
  glutMotionFunc(motion);
glutMouseFunc(mouse);
  glutDisplayFunc     (Render);
  glutReshapeFunc     ( reshape );
  glutSpecialFunc    ( SpecialKey );
 // glutSpecialFunc     ( arrow_keys );
  glutIdleFunc		  (Idle);
  


}

float gconst=2.15443469003;
	real deltat=1000;
	int cut=0;
	float fac=0;
	int ifac=0;
int _tmain(int argc, char* argv[])
{
	   		

	
	
   Vector3d v1(4,4,4);
   Vector3d v2(2,2,2);

  
   cout<<"x: "<<v1.x<<", y: "<<v1.y<<", z: "<<v1.z<<endl;
   cout<<"|v1|= "<<v1.Magnitude()<<endl;
   v1.Normalize();
   cout<<"x: "<<v1.x<<", y: "<<v1.y<<", z: "<<v1.z<<endl;   
   cout<<"|v1|= "<<v1.Magnitude()<<endl;
	InitGL(&argc,argv);
	
	

	
	srand(time(0));
	


	InitP3d(particles.data(),PARTICLE_C,p3d_id);
	InitP3d(point_particles.data(),POINT_PARTICLE_C,p3d_id);

	InitP3d(particles.data(),PARTICLE_C,p3d_random_m);
	


	//test:
	/*particles[1].mass=1;
	particles[1].position.Set(0, 0, 0);
	particles[1].velocity.Set(100.0,0,0);
	
	particles[2].mass=1;
	particles[2].position.Set(20,0,fac*0);
	particles[2].velocity.Set(0,0,0);*/



	//particles[3].mass=100;
	//particles[3].position.Set(10,-2,200);
	//particles[3].velocity.Set(0,0,-10);

	//particles[4].mass=450;
	//particles[4].position.Set(-40,-2,-200);
	//particles[4].velocity.Set(0,0,-10);

	
	//particles[3].mass=500;
	////particles[3].position.Set(0,-2,10+5*r);
	//particles[3].velocity.Set(0);
	//
	//particles[2].mass=500;
	//particles[2].position.Set(0,-2,5);
	//particles[2].velocity.Set(0,0,28);
	
	
	
	/*Triangular formation*/{	
		/*particles[0].mass=500;
	particles[0].position.Set(0,-2,10);
	particles[0].velocity.Set(0,0,0);
	particles[4].mass=500;
	particles[4].position.Set(-1.8,-2,7.1);
	particles[4].velocity.Set(0);
	particles[6].mass=500;
	particles[6].position.Set(0,-2,7.1);
	particles[6].velocity.Set(0);
	particles[5].mass=500;
	particles[5].position.Set(1.8,-2,7.1);
	particles[5].velocity.Set(0);
	particles[1].mass=500;
	particles[1].position.Set(-0.8,-2,8.6);
	particles[1].velocity.Set(0,0,0);
	particles[3].mass=500;
	particles[3].position.Set(0.8,-2,8.6);
	particles[3].velocity.Set(0,0,0);

	particles[2].mass=1950000000;
	particles[2].position.Set(0,-2,20);
	particles[2].velocity.Set(0,0,0);
	*/
	}

	/*Newton's Cradle*/{
		/*real r=particles[0].radius*2.0;
	particles[0].mass=500;
	particles[0].position.Set(0,-2,10);
	particles[0].velocity.Set(0,0,0);
	particles[4].mass=500;
	particles[4].position.Set(0,-2*r,r);
	particles[4].velocity.Set(0);
	particles[6].mass=500;
	particles[6].position.Set(0,-2,3*r);
	particles[6].velocity.Set(0);
	particles[7].mass=500;
	particles[7].position.Set(0,-2,10+12*r);
	particles[7].velocity.Set(0);
	particles[8].mass=500000;
	particles[8].position.Set(0,-2,-100);
	particles[8].velocity.Set(0,0,12);
	particles[5].mass=500000;
	particles[5].position.Set(.1,-2,6*r);
	particles[5].velocity.Set(0);
	particles[1].mass=500;
	particles[1].position.Set(-1000,-2,10+6*r);
	particles[1].velocity.Set(0,0,0);
	particles[3].mass=500;
	particles[3].position.Set(0,-1.8,10+8*r);
	particles[3].velocity.Set(0);
	particles[2].mass=500;
	particles[2].position.Set(0,-2,-30);
	particles[2].velocity.Set(0,0,20);*/

	}
	
	/*Asteroid*/{
	/*particles[1].mass=2500000000;
	particles[1].position.Set(0,-2,10);
	particles[1].velocity.Set(0,0,-15);
	
	particles[2].mass=5000;
	particles[2].position.Set(10,-2,15);
	particles[2].velocity.Set(12,0,-28);

	particles[3].mass=100;
	particles[3].position.Set(10,-2,200);
	particles[3].velocity.Set(0,0,-10);

	particles[4].mass=450;
	particles[4].position.Set(-40,-2,-200);
	particles[4].velocity.Set(0,0,-10);

	particles[0].mass=999999;
	particles[0].position.Set(550,-2,0);
	particles[0].velocity.Set(0,0,0);*/

	}

	

		CopyParticles(p3d_states.data(),particles.data(),PARTICLE_C);
		CopyParticles(p_p3d_states.data(),point_particles.data(),POINT_PARTICLE_C);
	
		for(int i=0; i<PARTICLE_C; i++)
			cout<<i<<" m: "<<particles[i].mass<<":\n vx= "<<particles[i].velocity.x<<", vy= "<<particles[i].velocity.y<<", vz= "<<particles[i].velocity.z<<endl;
	
	
	glutMainLoop(); 

	return 0;
}

float t=0;
float ts=0.1;
float r=0;
float vx=2, vy=2, vz=0;
float tp=0.0;
bool step=false;
float sd=99999999;
float sf=0;

void Render2()
{
	glDisable(GL_LIGHTING);
	 glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	 
	 glPushMatrix();
	//glLoadIdentity();
	
	float theta=30;
	
	
	glTranslatef(0,0,-20);glRotatef(theta,1,0,0);
	
	glPointSize(18);
SetColor(BLUE);
	
	glBegin(GL_POINTS);
	glVertex3f(0,0,0);
	glVertex3f(5,5,0);
	glEnd();


	glPopMatrix();
		 
	 glPushMatrix();
	// glLoadIdentity();
//glTranslatef(0,0,-20);
	SetColor(RED);
	glBegin(GL_POINTS);
	//glVertex3f(0,0,-20);
	//glVertex3f(5,5,0);
	//glVertex3f(5,5*cos(theta) ,-5*sin(theta)+ -20*cos(theta)); //r,t
	glVertex3f(5, 5*cos(theta), -5*sin(theta)+ -20); //t, r
	glEnd();

	//glPopMatrix();

	glFlush();
	glutSwapBuffers(); 
}





   void Render3()
{
	glEnable(GL_LIGHTING);
	 glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	 glColor3f(0.5,0,0);
	glPushMatrix();
	
	glTranslatef(-shor,sheight,-sdepth);
  glRotatef(-stheta, 1, 0,0);
	glRotatef(sphi, 0, 1, 0);
   glutSolidCube(2); 
   glPushMatrix();
   glTranslatef(-3,0,0);
    glutSolidCube(3);
  // glPopMatrix();
	

	glPopMatrix();
	glPopMatrix();
	glFlush();
	glutSwapBuffers(); 
}


void Render()
{
	clock_t t1, t2;
	t1=clock();

	glEnable(GL_LIGHTING);
	//glClear(GL_COLOR_BUFFER_BIT);	
	 //glClearColor (0.0,0.0,0.0,1.0);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	 

	  
	glColor3f(0.5,0,0);

	glPushMatrix();
	
	
  glTranslatef(-shor,sheight,-sdepth);
  glRotatef(-stheta, 1, 0,0);
	glRotatef(sphi, 0, 1, 0);
	
	float fc=0.01;
	Vector3d vh(0,0,0);
	vh*=0;
	Vector3d vex;

	bool in=false;
	
	static real gt=0;
	
	
	for(int i=0; i<PARTICLE_C; i++)
	{
		
		print=true;
		vh.Set(0.0,0.0,0.0);goto here;
		if(paused)
		{
			if(step)
			{integrate(i, particles,tp, 1.0/deltat);in =true;}
			else if(stepb)
			{integrate(i, particles,tp, -1.0/deltat);in =true;}
		}
		else
			integrate(i, particles,tp, 1.0/deltat);
	}

	here:
	
	
	static int storefrd=0;
	if (keypress2)gt++; // frames drawn counter
	int seconds_passed=0;

	


	if(in){in=true; step=false; stepb=false;}
		
	
	
	
	
	static int t=clock();

		
		
	real G=6.67384*0.00080;
	
	
	if(keypress2)
		for(int i=0; i<PARTICLE_C; i++)
		{
			UniversalGravitation(i,particles.data(),PARTICLE_C,p3d_states[i],1.0/deltat, G);
		}
		
	else
	{}
		if(keypress)
		for(int i=0; i<PARTICLE_C; i++)
		{DirectionalGravity(i,particles.data(),PARTICLE_C,p3d_states[i],1.0/deltat);}

		CollisionResolution(PARTICLE_C, particles.data(), p3d_states.data());
		
		
	
	static bool justonce=true;
	

	if(gt/cut>=1.0) // if 60 frames were drawn
	{storefrd+=gt; gt=0; 
	seconds_passed=storefrd/deltat;
	cout<<"\nSimulation frames: "<<storefrd<<endl;

	cout<<"Time: "
		<<seconds_passed/3600/24<<" days, "
		<<(seconds_passed%(3600*24))/3600<<" hours, "
		<<seconds_passed%3600/60<<" minutes, "
		<<seconds_passed%60/1.0<<" seconds\n\n";
	cout<<"P2: x= "<<particles[2].position.x
		<<", y= "<<particles[2].position.y
		<<", z= "<<particles[2].position.z<<endl;
	
	cout<<"P1: x= "<<particles[1].position.x
		<<", y= "<<particles[1].position.y
		<<", z= "<<particles[1].position.z<<endl;
	cout<<"vx= "<<particles[1].velocity.x/fac
		<<", vy= "<<particles[1].velocity.y/fac
		<<", vz= "<<particles[1].velocity.z/fac
		<<", speed= "<<particles[1].velocity.Magnitude()/fac<<endl;
	
	}
CopyStates(p3d_states.data(),particles.data(),PARTICLE_C);
	for(int i=0; i<PARTICLE_C; i++)
	{ DrawParticle3d(particles[i]);	}

	if(justonce)
		if(particles[2].position.x>=particles[1].position.x)
		{cout<<"Collision at "<<(storefrd+gt)/deltat<<" seconds\n";justonce=false;}
	goto skip;

	
	
 
   glPushMatrix();
   glTranslatef(2,0,0);
	glRotatef(r,0,1,0);
   DrawObject3d(&obj);
   glPopMatrix();
   r+=0.5;
  
   glPushMatrix();
   glRotatef(r*3,0.2,1,0);
   glTranslatef(4,2,0);
   glutSolidSphere(0.5,50,50);
  glPopMatrix();

skip:
  glDisable(GL_LIGHTING);
		GLfloat light_position[] = {lightx, lighty, lightz, 0.0};
		
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
 

	
	SetColor(GREEN);
	

	
 
	for(int i=0; i<POINT_PARTICLE_C; i++)
		{	
			if(p_positions[i].size()>100)
			p_positions[i].erase(p_positions[i].begin(),p_positions[i].begin()+1);}

	for(int i=0; i<POINT_PARTICLE_C; i++)
	p_positions[i].push_back(Vector3d(point_particles[i].position.x,point_particles[i].position.y,point_particles[i].position.z));
		for(int i=0; i<POINT_PARTICLE_C; i++)
{
	SetColor(BLACK);
	glBegin(GL_POINTS);
	
	glVertex3f(p_positions[i][0].x, p_positions[i][0].y, p_positions[i][0].z);
	glEnd();

	glPointSize(1);
	glBegin(GL_POINTS);
	
	
	for(int j=1; j<p_positions[i].size(); j++)
	{
		SetColor(G75);
		glColor4f(float(j)/(float(p_positions[i].size())),float(j)/(float(p_positions[i].size())),float(j)/(float(p_positions[i].size())),float(j)/(float(p_positions[i].size())));
	glVertex3f(p_positions[i][j].x, p_positions[i][j].y, p_positions[i][j].z);}
	glEnd();

	glEnd();
	}


  
 for(int i=0; i<PARTICLE_C; i++)
		{	
			if(positions[i].size()>2600000)
			positions[i].erase(positions[i].begin(),positions[i].begin()+1);}

	for(int i=0; i<PARTICLE_C; i++)
	positions[i].push_back(Vector3d(particles[i].position.x,particles[i].position.y,particles[i].position.z));

		for(int i=0; i<PARTICLE_C; i++)
{
	
	glBegin(GL_POINTS);
	
	glVertex3f(positions[i][0].x, positions[i][0].y, positions[i][0].z);
	glEnd();

	glPointSize(1);
	glBegin(GL_POINTS);
	
	SetColor(WHITE);
	for(int j=1; j<positions[i].size(); j++)
	{
	glVertex3f(positions[i][j].x, positions[i][j].y, positions[i][j].z);}
	glEnd();

	glEnd();
	}
	
 
  

glDisable(GL_LIGHTING);
glClear(GL_DEPTH_BUFFER_BIT);


 //Draw velocity and acceleration vectors
SetColor(GREEN); 
  for(int i=0; i<PARTICLE_C; i++)
  DrawVector3d(particles[i].velocity*pow(1.89,ifac*-1),particles[i].position.x,particles[i].position.y, particles[i].position.z);

   
  SetColor(RED);
 for(int i=0; i<PARTICLE_C; i++)
  {DrawVector3d(particles[i].acceleration*pow(3.1,ifac*-1),particles[i].position.x,particles[i].position.y, particles[i].position.z);}



 

   SetColor(BLUE);
	glBegin(GL_LINES);
	//glVertex3f(particles[0].position.x, particles[0].position.y, particles[0].position.z);
	//glVertex3f(particles[2].position.x, particles[2].position.y, particles[2].position.z);
	 SetColor(MAGENTA);
	//glVertex3f(particles[1].position.x, particles[1].position.y, particles[1].position.z);
	//glVertex3f(particles[0].position.x, particles[1].position.y, particles[0].position.z);
	 SetColor(GREEN);
	//glVertex3f(particles[0].position.x, particles[0].position.y, particles[0].position.z);
	//glVertex3f(particles[0].position.x, particles[1].position.y, particles[0].position.z);
	glEnd();

	
	
		


 glLineWidth(2.5);
SetColor(RED);
   //DrawVector3d(Vector3d(0.2*5,1*5,0));
				//Testcode
						{
						   /*Vector3d v1(2,2,2);
						   Vector3d v2(2,4,2);
							
						    SetColor(MAGENTA);
							DrawVector3d(v1);

							SetColor(YELLOW);
							DrawVector3d(v2);

							SetColor(WHITE);
							DrawVector3d(crossProduct(v1,v2));

							


							SetColor(G75);
							v1.Normalize();
							DrawVector3d(v1);

							SetColor(G50);
							DrawVector3d(v1+v2);*/
						}

						glPointSize(5);

					
				

	glBegin(GL_LINES);

	//glColor3f(1.0f,1,0);
	
	//glVertex3f(0,0,0);
	//glVertex3f(vx,vy,vz);

	
	
	glColor3f(1.0f,0,0);
	
 
	glVertex3f(0,0,0);
	//glVertex3f(.5*cos(temp*PI/180),.5*sin(temp*PI/180),0);
	glVertex3f(1,0,0);


	glColor3f(0,1.0f,0);
	glVertex3f(0,0,0);
	//glVertex3f(.5*cos(temp2*PI/180),.5*sin(temp2*PI/180),0);
glVertex3f(0,1,0);


	//glColor3f(0,0,1.0f);
	glVertex3f(0,0,0);
	//glVertex3f(0,.5*sin(temp3*PI/180),.5*cos(20*PI/180));
	glVertex3f(0,0,1);

	//glColor3f(1,0,1);
	//glVertex3f(lightx, lighty, lightz);
	//glVertex3f(lightx+0.5, lighty+0.5, lightz+0.5);
	glEnd();
   
	float textx=-shor, texty=sheight, textz=-sdepth;
	


	float prevy=texty, prevz=textz;


	Text3d txt1("Hello \nMisterrr\nwherepppp");


	
glPopMatrix();
							

tp+=0.01;

	glFlush();
	glutSwapBuffers(); 
}

void Idle()
{
	//t+=1;
	//if(step)
	{glutPostRedisplay();}
}


void motion(int x, int y)
{
	static int temp_x=x, temp_y=y;
	static float margin=0.05;


			
	if (leftButton)
	{
		sphi+=(float)(x-downX)/4.0;
	stheta+=(float)(downY-y)/4.0;
	}
	if (rightButton){ 
		if(sdepth<=2 && sdepth>=-2)
	sdepth += (float)(downY - y);

	else sdepth += (float)(downY - y)*(abs(sdepth))/50.0; 
	//cout<<sdepth<<": "<<downY<<" - "<<y<<endl;
	
	} // scale

  
  if(middleButton)
  {
	  sheight += (float)(downY - y)*(abs(sdepth)) / 120.0;
	  shor += (float)(downX - x)*(abs(sdepth))/ 120.0;
  }
			
		downX = x;   downY = y;

	
}

void mouse(int button, int state,int x, int y)
{
	downX=x; downY=y;
	 leftButton = ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN));

  middleButton = ((button == GLUT_MIDDLE_BUTTON) &&  (state == GLUT_DOWN));

  rightButton  = ((button == GLUT_RIGHT_BUTTON) &&  (state == GLUT_DOWN));

}

 void SpecialKey ( int k, int x, int y ) 
 {

	 switch ( k ) 
	{
  case GLUT_KEY_UP:     
	
	  //lighty+=0.1;
	  if(paused)
		  paused=false;
	  else
		  paused=true;
		break;
  
  case GLUT_KEY_DOWN: 
		lighty-=0.1;
	
		break;

  case GLUT_KEY_RIGHT:    
	  keypress2=!keypress2;
	 step=true;
		break;
	
	case GLUT_KEY_LEFT: 
		//lightz-=0.1;
		keypress=!keypress;
		break;
	}
 }
