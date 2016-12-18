// Physics_Engine.cpp : Defines the entry point for the console application.
//

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
	//real angle=asin((Position1.y-Position2.y)/d);
	//real angle2=asin((Position1.z-Position2.z)/d);
	//cout<<"angle "<<angle*180/3.141592<<endl;	
	//cout<<"angle2 "<<angle2*180/3.141592<<endl;	


	//Vector3d ACCELERATION(cos(angle)*f/m1, sin(angle)*f/m1, sin(angle2)*f/m1);
	
	tempAcceleration.Set((ip.Position.x-P[i].position.x)*f/d/ip.mass, (ip.Position.y-P[i].position.y)*f/d/ip.mass,
		(ip.Position.z-P[i].position.z)*f/d/ip.mass);
	ACCELERATION+=tempAcceleration;//accumulate accerleations

	
	/*if(print)
	{cout<<"aX: "<<(Position1.x-Position2.x)*f/d/m1<<endl;
	cout<<"aY: "<<(Position1.y-Position2.y)*f/d/m1<<endl;
	cout<<"aZ: "<<(Position1.z-Position2.z)*f/d/m1<<"\n";
	}*/
	//if(angle2*180/3.141592<0)
		//ACCELERATION.x=-cos(angle2)*f/m1;

//particles[0].acceleration=ACCELERATION;

	}
	//if(Position1.x<Position2.x)
		//ACCELERATION.x*=-1;
	//if(Position1.z<Position2.z)
	//ACCELERATION.z*=-1;
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
	//glHint				(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  //

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

float gconst=2.1544346900318837217592935665194;
	real deltat=0;
	int cut=0;
	float fac=0;
	int ifac=0;
int _tmain(int argc, char* argv[])
{
	   		

	
	
   Vector3d v1(4,4,4);
   Vector3d v2(2,2,2);

   
  // Particle3d t;
   //cout<<t.position.x<<endl;
   //system("pause");
  
   
  // p=(char*)"str564ing";
  // t=p;
   //p++;
   //delete[] p;
   //cout<<p<<endl;

   /*char arr[3], *p1, *p2;
   arr[0]='0';
   arr[1]='1';
   arr[2]='\0';

   p1=arr;
   p2=++p1;
   p2;

   cout<<"p1: "<<p1<<", P2: "<<p2<<endl;*/
 /*char *p=new char[9];
 p=NewStr("yhj765ll");

  
	p+=CleanNumeric(p);
 cout<<p<<endl;*/
   cout<<"x: "<<v1.x<<", y: "<<v1.y<<", z: "<<v1.z<<endl;
   cout<<"|v1|= "<<v1.Magnitude()<<endl;
   v1.Normalize();
   cout<<"x: "<<v1.x<<", y: "<<v1.y<<", z: "<<v1.z<<endl;   
   cout<<"|v1|= "<<v1.Magnitude()<<endl;
	InitGL(&argc,argv);
	
	

	/*obj.LoadOBJ("c:/e/cube.obj");
	
	obj.LoadOBJ("c:/e/cello2.obj");
	
	obj.LoadOBJ("c:/e/cube.obj");
	
	obj.LoadOBJ("c:/e/cello2.obj");
	
	obj.LoadOBJ("c:/e/cube.obj");
	obj.LoadOBJ("c:/e/cello2.obj");
	obj.LoadOBJ("c:/e/cube.obj");*/
	srand(time(0));
	//obj.LoadOBJ("c:/e/cello2.obj");
	//box.LoadOBJ("c:/e/box.obj");
	// for(int i=0; i<box.nVertex;i++)
	 //  box.vertex[i].x+=5;


	InitP3d(particles.data(),PARTICLE_C,p3d_id);
	InitP3d(point_particles.data(),POINT_PARTICLE_C,p3d_id);

	InitP3d(particles.data(),PARTICLE_C,p3d_random_m);
	//InitP3d(particles.data(),PARTICLE_C,p3d_random_v);
	//InitP3d(point_particles.data(),POINT_PARTICLE_C,p3d_random_v);
		//InitP3d(point_particles.data(),POINT_PARTICLE_C,p3d_random_m);

	for(int i=0; i<POINT_PARTICLE_C; i++)
		point_particles[i].velocity*=15;

	Vector3d vvv;
	vvv.Set(POINT_PARTICLE_C,POINT_PARTICLE_C,POINT_PARTICLE_C);
	for(int i=0; i<POINT_PARTICLE_C; i++)
		point_particles[i].mass/=200;
	
	
	

	point_particles[2].position.y=20;
	point_particles[2].position.x=-2000;
	point_particles[2].velocity.x=780;;
	point_particles[2].velocity.y=200;

	//factor: 10^20
	//velocities km/s

	

	//int ifac=-21;
	//float fac=pow(gconst,ifac);

	
	fac=pow(10.0, -6); //distance scale
	ifac=3.0*log10(fac);
	deltat=.01;
	cut=50;

	particles[0].mass= 0.07342*pow(10.0, 4+ifac );
	particles[0].position.Set(fac*(1.521*pow(10.0, 8)+ 405400), 0, 0);
	particles[0].velocity.Set(0,0,fac*30.5305076);

	particles[1].mass=5.976*pow(10.0, 4+ifac );
	particles[1].position.Set(fac*1.521*pow(10.0, 8), 0, 0);
	particles[1].velocity.Set(0,0,fac*29.990);
	
	particles[2].mass=1.98855*pow(10.0, 10+ifac);
	particles[2].position.Set(0,0,fac*0);
	particles[2].velocity.Set(0,0,0);

	particles[3].mass=1.98855*pow(10.0, 1+ifac);
	particles[3].position.Set(0,0,-200000);
	particles[3].velocity.Set(0,0,-10000);


	//30 Arietis
	//Aa
	//particles[0].mass= 2.60500*pow(10.0, 10+ifac );
	//particles[0].position.Set(fac*(1.521*pow(10.0, 8)+ 405400), 0, 0);
	//particles[0].velocity.Set(0,0,fac*254.312);
	////Bb
	//particles[1].mass=5.976*pow(10.0, 4+ifac );
	//particles[1].position.Set(fac*1.521*pow(10.0, 8), 0, 0);
	//particles[1].velocity.Set(0,0,fac*29.390);
	//
	//particles[2].mass=1.98855*pow(10.0, 1+ifac);
	//particles[2].position.Set(0,0,fac*0);
	//particles[2].velocity.Set(0,0,0);

	//particles[3].mass=1.98855*pow(10.0, 1+ifac);
	//particles[3].position.Set(0,0,-2000);
	particles[3].velocity.Set(0,0,0);





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
	particles[0].velocity.Set(0,0,0);
*/
	}
	InitP3d(particles.data(),PARTICLE_C,p3d_radius);
particles[0].radius=1.0;

	
	particles[2].radius=20.8;
particles[1].radius=.010;
particles[0].radius=0.010;
	//for(int i=0; i<PARTICLE_C; i++)
	//	particles[i].velocity.Set(0);

//	particles[0].velocity.Set(0,0,20);
//	particles[2].mass=100;
//
//particles[1].position.x=4;
//particles[2].position.x=-6;
//particles[2].position.y=-5;
//particles[2].position.z=-5;
//particles[1].position.z=4;
//	//particles[0].velocity.y=2.00;
//	particles[0].velocity.z=-1.8;
//	//particles[0].velocity.x=-1.4;
//	//particles[0].position.x=-8;
//		particles[0].position.x=particles[1].position.x+-8;
//		particles[0].position.y=particles[1].position.y;
//		particles[0].position.z=particles[1].position.z;
//	
		CopyParticles(p3d_states.data(),particles.data(),PARTICLE_C);
		CopyParticles(p_p3d_states.data(),point_particles.data(),POINT_PARTICLE_C);
	
		for(int i=0; i<PARTICLE_C; i++)
			cout<<i<<" m: "<<particles[i].mass<<":\n vx= "<<particles[i].velocity.x<<", vy= "<<particles[i].velocity.y<<", vz= "<<particles[i].velocity.z<<endl;
	//unsigned char *p;
	//
	//p=new unsigned char[10];
	//p[0]='O';
	//p[1]='K';
	//p[2]='\0';

	//disposal[0]=p;
	//cout<<p<<endl;
	////delete disposal[1];
	//disposal[1][4]='\0';
	//for(int i=0; i<10000; i++)
	//{cout<<disposal[i]<<endl; system("pause"); delete disposal[i];}
	
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

	 

	   // gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glColor3f(0.5,0,0);

	glPushMatrix();
	
	
  glTranslatef(-shor,sheight,-sdepth);
  glRotatef(-stheta, 1, 0,0);
	glRotatef(sphi, 0, 1, 0);

	//glPushMatrix();
	//glTranslatef(-2,0,0);
 // // glutSolidCube(2);
 //  glPopMatrix();

 //  glPushMatrix();
 // // DrawObject3d(&box);
 // glPopMatrix();

   

   /*real di=2;
   real d=sqrt((particles[0].position.x-particles[1].position.x)*(particles[0].position.x-particles[1].position.x) +
			  (particles[0].position.y-particles[1].position.y)*(particles[0].position.y-particles[1].position.y) +
	  (particles[0].position.z-particles[1].position.z)*(particles[0].position.z-particles[1].position.z));

	real f=6.67384*0.00000000010*10*15000000000/(d*d);*/

	
		
	//if(d<sd)
	//sd=d;
	//if(f>sf)
	//	sf=f;
	//
	////cout<<"-------------------------\nF : "<<f<<endl;
	////cout<<"sF: "<<sf<<endl;
	////cout<<"d: "<<d<<endl;
	////cout<<"sd: "<<sd<<endl;

	//real angle=asin((particles[0].position.y-particles[1].position.y)/d);
	//real angle2= atan((particles[0].position.x-particles[1].position.x)/(particles[0].position.z-particles[1].position.z));
	//cout<<"angle "<<angle*180/3.141592<<endl;	
	//cout<<"angle2 "<<angle2*180/3.141592<<endl;	
	//particles[0].acceleration.x=f*particles[0].position.x/d;
	//particles[0].acceleration.y=f*particles[0].position.y/d;
//particles[0].acceleration.x=cos(angle)*f;
	//particles[0].acceleration.y=sin(angle)*f;
	//particles[0].acceleration.z=cos(angle2)*particles[0].acceleration.x;

	

	

	
	/*particles[1].acceleration.x=cos(angle)*f;
	particles[1].acceleration.y=sin(angle)*f;
	particles[1].acceleration.z=cos(angle2)*particles[1].acceleration.x;*/
	//cout<<"ax: "<<particles[0].acceleration.x<<endl;
	//cout<<"ay: "<<particles[0].acceleration.y<<endl;
	//cout<<"az: "<<particles[0].acceleration.z<<endl;
//if(particles[0].position.x<particles[1].position.x)
//{particles[0].acceleration.x*=-1;}
//
//if(particles[1].position.x<particles[0].position.x)
//{particles[1].acceleration.x*=-1;}
//
////	if(particles[0].position.y<=particles[1].position.y)
//	//particles[0].acceleration.y*=-1;
//
//if(particles[0].position.z<particles[1].position.z)
//	particles[0].acceleration.z*=-1;
//
//	if(particles[1].position.z<particles[0].position.z)
//	particles[1].acceleration.z*=-1;




//particles[0].velocity.x+=particles[0].acceleration.x*fc;
//	particles[0].velocity.y+=particles[0].acceleration.y*fc;
//	particles[0].velocity.z+=particles[0].acceleration.z*fc;
//
//particles[0].position.x-=particles[0].velocity.x*fc;
//	particles[0].position.y-=particles[0].velocity.y*fc;
//	particles[0].position.z-=particles[0].velocity.z*fc;
	
	
	float fc=0.01;
	Vector3d vh(0,0,0);
	vh*=0;
	Vector3d vex;

	bool in=false;
	
	static real gt=0;
	
	
	for(int i=0; i<PARTICLE_C; i++)
	{
		//if(i==7 || i==8)
			//continue;
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
	//gt+=1.0/deltat;
	//if(int(gt)%60==0)
	//cout<<"t: "<<gt/60<<" minutes "<<endl;
	
	static int storefrd=0;
	if (keypress2)gt++; // frames drawn counter
	int seconds_passed=0;

	


	if(in){in=true; step=false; stepb=false;}
		
	
	
	
	
	static int t=clock();

		//for(int i=0; i<POINT_PARTICLE_C; i++)
		//{DirectionalGravity(i,point_particles.data(),POINT_PARTICLE_C,p_p3d_states[i],1.0/15.0);}
		
	//real G=6.67384*0.000000080;
	real G=6.67384;
	//real G=6.67384*pow(10.0,-11);
		//keypress2?G=6.67384*0.000000080: G=0.0;
	
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
		//CollisionResolution_Ground(PARTICLE_C, particles.data(), p3d_states.data());
		//CollisionResolution_Ground(PARTICLE_C, particles.data(),p3d_states.data());
		//Point collisions with ground
		//CollisionResolution_Ground(POINT_PARTICLE_C, point_particles.data(),p_p3d_states.data());
			
		//CopyStates(p_p3d_states.data(),point_particles.data(),POINT_PARTICLE_C);
		
		//glEnable(GL_LIGHTING);
		
	
	static bool justonce=true;
	

	if(gt/cut>=1.0) // if 60 frames were drawn
	{storefrd+=gt; gt=0; 
	seconds_passed=storefrd/deltat;
	cout<<"\nSimulation frames: "<<storefrd<<endl;
	/*cout<<"Time: "
		<<seconds_passed/3600.0/24.0<<" days, "
		<<seconds_passed/3600<<" hours, "
		<<seconds_passed/60<<" minutes, "
		<<seconds_passed<<" seconds\n";*/

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

	//for(int i=0; i<POINT_PARTICLE_C; i++)
	//{DrawParticle3d(point_particles[i]);}
	
 
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
	//glPointSize(1.80);
	glBegin(GL_POINTS);
	//SetColor(GREEN);
	glVertex3f(positions[i][0].x, positions[i][0].y, positions[i][0].z);
	glEnd();

	glPointSize(1);
	glBegin(GL_POINTS);
	//SetColor(G50);
	SetColor(WHITE);
	for(int j=1; j<positions[i].size(); j++)
	{//if(j>= 2*positions[i].size()/3)
	//SetColor(WHITE);
	//else if(j>= positions[i].size()/3)
		//SetColor(G75);
		/*if(i==3)
			glColor4f(float(j)/(float(positions[i].size())),0,0,float(j)/(float(positions[i].size())));
		else if(i==4)
			glColor4f(float(j)/(float(positions[i].size())),float(j)/(float(positions[i].size())),0,float(j)/(float(positions[i].size())));
		else
		glColor4f(float(j)/(float(positions[i].size())),float(j)/(float(positions[i].size())),float(j)/(float(positions[i].size())),float(j)/(float(positions[i].size())));
	*/

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

  // for(int i=0; i<POINT_PARTICLE_C; i++)
  //DrawVector3d(point_particles[i].velocity*0.1,point_particles[i].position.x,point_particles[i].position.y, point_particles[i].position.z);


   
  SetColor(RED);
 for(int i=0; i<PARTICLE_C; i++)
  {DrawVector3d(particles[i].acceleration*pow(3.1,ifac*-1),particles[i].position.x,particles[i].position.y, particles[i].position.z);}

 //for(int i=0; i<POINT_PARTICLE_C; i++)
	// DrawVector3d(point_particles[i].acceleration,point_particles[i].position.x,point_particles[i].position.y, point_particles[i].position.z);

 

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
	

//glTranslatef(1,0,0);
	//SetColor(BLUE);
							
							
	
	
	

//glBegin(GL_POINTS);

	float prevy=texty, prevz=textz;
	//	texty=prevy*cos(-stheta)-prevz*sin(-stheta);
		//textz=prevy*sin(-stheta) +prevz*cos(-stheta);
	//glVertex3f(textx,texty,textz);
//glEnd();



	

	Text3d txt1("Hello \nMisterrr\nwherepppp");
//	Text3d txts[PARTICLE_C];
	
	
	//for(int i=0; i<PARTICLE_C; i++)
	//{
	//	char*p=new char;
	//	*p=(i+48);
	//	//str.append("3");
	//	txts[i].text=p;
	//}

	//for(int i=0; i<PARTICLE_C; i++)
	//{
	//	
	//	GetProjection(particles[i].position.x, particles[i].position.y, particles[i].position.z, txts[i].x, txts[i].y);
	////txts[i].x-=particles[i].position.x, txts[i].y-=particles[i].position.y;
	//RenderText(txts[i]);
	//
	//}
	//	GetProjection(textx,texty,textz,txt1.x, txt1.y);
							
							
	
				//	glPopMatrix();


	//glVertex2f(textx+3,texty+4);

	
//Text3d txt("Rotaton Matrix \nMisterrr\nwherepppp");
//							glTranslatef(-shor,sheight,-sdepth);
//							txt.x=textx;
//							txt.y=txt.y*cos(-stheta)-textz*sin(-stheta);
//							textz=txt.y*sin(-stheta) - textz*cos(-stheta);
//					glPushMatrix();		
//					
//  //glRotatef(-stheta, 1, 0,0);
//	//glRotatef(sphi, 0, 1, 0);
//							RenderText(txt,textz-20);
//							glPopMatrix();
	
	//glLoadIdentity();
	
	
glPopMatrix();
							
	//Sleep(70);
//while((clock()-t1)/(CLOCKS_PER_SEC/1000)<1){}

tp+=0.01;

//cout<<tp<<" Seconds\n";
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