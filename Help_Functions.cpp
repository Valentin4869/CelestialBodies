#include "stdafx.h"
#include "Help_Functions.h"

Vector3d compProduct(Vector3d& v1, Vector3d& v2){
return Vector3d(v1.x+ v2.x, v1.y+ v2.y, v1.z+ v2.z);
}

real dotProduct(Vector3d& v1, Vector3d& v2){
	return (v1.x*v2.x+ v1.y*v2.y+ v1.z*v2.z);
}

Vector3d crossProduct(Vector3d& v1, Vector3d& v2){
	return Vector3d(v1.y*v2.z- v1.z*v2.y, v1.z*v2.x- v1.x*v2.z, v1.x*v2.y- v1.y*v2.x);
}

real Distance3d(Vector3d p1, Vector3d p2){
	real d=sqrt( (p1.x-p2.x)*(p1.x-p2.x) +
				 (p1.y-p2.y)*(p1.y-p2.y) +
				 (p1.z-p2.z)*(p1.z-p2.z)
			   );
	return d;
}

void DrawVector3d(Vector3d& v){

	DrawVector3d(v,0,0,0);
}

void DrawVector3d(Vector3d& v, real x, real y, real z)
{
	Vector3d tempv=v;
	tempv.Normalize();//normalize to get direction for drawing arrow head

	glBegin(GL_LINES);
	glVertex3f(x,y,z);
	glVertex3f(x+v.x, y+v.y, z+v.z);

	//glVertex3f(x+v.x, y+v.y, z+v.z);
	//glVertex3f(x+v.x-tempv.x*0.1*cos(3.14/6), y+v.y+0.06*sin(3.14/6), z+v.z);
	glEnd();
}

void DrawObject3d(Object3d* obj)
{
	
	//glBegin(GL_POLYGON);
	//glColor3f(1,1,1);
	//
	//for(int i=0; i<obj->nVertex; i++)
	//{glVertex3f(obj->vertex[i].x, obj->vertex[i].y, obj->vertex[i].z); 
	////glVertex3f(0,0,0);
	//
	//}

	//glEnd();
	
	for(int i=0; i<obj->nFace; i++)
	{
		
		glBegin(GL_POLYGON);
	glColor3f(1,1,1);

		for(int c=0; c< obj->face[i].nFaceV; c++)
			{
				//cout<<"Normal "<<obj->face[i].iVertexNormal[c]-1<<endl;
				if(obj->nVertexNormal>0)
				glNormal3f(obj->vertexNormal[obj->face[i].iVertexNormal[c]-1].x, 
					obj->vertexNormal[obj->face[i].iVertexNormal[c]-1].y,
					obj->vertexNormal[obj->face[i].iVertexNormal[c]-1].z);

				glVertex3f(obj->vertex[obj->face[i].iVertex[c]-1].x,
				obj->vertex[obj->face[i].iVertex[c]-1].y,
				obj->vertex[obj->face[i].iVertex[c]-1].z);

			}
glEnd();

	}
	
}


void SetColor(COLOR color){

	switch(color)
	{
		case BLACK:
		glColor3f(0.0, 0.0, 0.0);
		break;

		case G75:
		glColor3f(0.75, 0.75, 0.75);
		break;

		case G50:
		glColor3f(0.5, 0.5, 0.5);
		break;

		case G25:
		glColor3f(0.25, 0.25, 0.25);
		break;

		case WHITE:
		glColor3f(1.0, 1.0, 1.0);
		break;

		case RED:
		glColor3f(1.0, 0.0, 0.0);
		break;

		case GREEN:
		glColor3f(0.0, 1.0, 0.0);
		break;

		case BLUE:
		glColor3f(0.0, 0.0, 1.0);
		break;

		case YELLOW:
		glColor3f(1.0, 1.0, 0.0);
		break;

		case MAGENTA:
		glColor3f(1.0, 0.0, 1.0);
		break;
	default:
		break;
	}
}

void GetProjection(double x, double y,double z, real &X,real &Y)
{
	double mm[16], pm[16];
	int vw[4];

	glGetDoublev(GL_MODELVIEW_MATRIX,mm);
	glGetDoublev(GL_PROJECTION_MATRIX,pm);
	glGetIntegerv(GL_VIEWPORT,vw);

		double wx, wy, wz;
gluProject(x, y, z,mm,pm,vw,&wx, &wy, &wz);

X=wx;
Y=wy;
}

void RenderText(Text3d txt){
	goto t;
	//int length=strlen(txt.text);
	//float xAlignment=0, yAlignment=0;
	//short nNewlines=0;

	//SetColor(txt.color);

	//for(int i=0; i<length; i++)
	//{
	//	
	//	if(txt.text[i]=='\n')
	//	{xAlignment=0; yAlignment++; continue;}

	//		//glRasterPos3f(txt.position.x+ i*xAlignment, txt.position.y- nNewlines*yAlignment, txt.position.z);
	//	glRasterPos3f(txt.x+ xAlignment, txt.y- yAlignment, d);
	//	//glRasterPos2f(0+i, 5);
	//		glutBitmapCharacter(GLUT_BITMAP_8_BY_13,txt.text[i]);
	//	xAlignment++;
	//}
t:
	 glDisable(GL_DEPTH_TEST);
	 glDisable(GL_LIGHTING);
     //Temporarily set up the view in orthographic projection.
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, (double)800, 0.0, (double)600, -1.0, 1.0);

    // Move to modelview mode.
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
	
	int length=strlen(txt.text);
	float xAlignment=0, yAlignment=0;
	short nNewlines=0;

	SetColor(txt.color);

	for(int i=0; i<length; i++)
	{
		
		if(txt.text[i]=='\n')
		{xAlignment=0; yAlignment+=13; continue;}
		glRasterPos2f(txt.x+ xAlignment, txt.y- yAlignment);
			glutBitmapCharacter(GLUT_BITMAP_8_BY_13,txt.text[i]);
		xAlignment+=8;
	}


	glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
	glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
}

void DrawParticle3d(Particle3d &p)
{
	////p.acceleration.y=-5;
	

	
		
	//p.position.z-=p.velocity.z;


	//p.velocity.y+=-0.01;
	//p.position.y+=p.velocity.y;
	
	glPushMatrix();
	//glLoadIdentity();
	glTranslatef(p.position.x, p.position.y, p.position.z);
	
	glutSolidSphere(p.radius,40,40);
	//cout<<"drawing at x:"<<p.position.x<<", y: "<<p.position.y<<", z:"<< p.position.z<<endl;;
	glPopMatrix();

}

void DrawPointParticle3d(PointParticle3d &p)
{
		
	//glPushMatrix();
	//glTranslatef(p.position.x, p.position.y, p.position.z);
	glPointSize(3);
	glBegin(GL_POINTS);
	glVertex3f(p.position.x, p.position.y, p.position.z);
	glEnd();
	
	glPointSize(1);
	//glPopMatrix();

}

void PrintVector3d(Vector3d v){
	
	


}

void InitP3d(Particle3d* P, int size, P3d_preset p3d)
{
	switch(p3d)
	{
	
	case p3d_point_particles:
		
		for(int i=0; i<size; i++)
			P[i].radius=0.0; //could cause error
			
		break;
	
	case p3d_id:
		
		for(int i=0; i<size; i++)
			P[i].id=i;

		break;

	case p3d_random_m:
		
		for(int i=0; i<size; i++)
			P[i].mass=2000.0*(rand()%1000+0.001);
		break;

	case p3d_radius:
		
		for(int i=0; i<size; i++)
			P[i].radius=0.75+0*P[i].mass/(75*pow(10.0,10));
		break;

	case p3d_random_v:
		
		for(int i=0; i<size; i++)
			P[i].velocity=Vector3d((rand()%1000-500)/100.0, (rand()%1000-500)/100.0, (rand()%1000-500)/100.0);
		break;

	default:
		break;
	}
}

State P3dToState(Particle3d&p){

	State STATE;
	STATE.i=p.id;
	STATE.mass=p.mass;
	STATE.Position=p.position;
	STATE.Velocity=p.velocity;
	return STATE;
}

Particle3d StateToP3d(State&s){
	Particle3d PARTICLE;

	PARTICLE.position=s.Position;
	PARTICLE.velocity=s.Velocity;
	PARTICLE.acceleration=s.Acceleration;
	PARTICLE.mass=s.mass;
	
	return PARTICLE;
}

void CopyParticles(State* S, Particle3d* P, int size){
	for(int i=0; i<size; i++)
	S[i]=P3dToState(P[i]);

}

void CopyStates(State* S, Particle3d* P, int size){
	for(int i=0; i<size; i++)
	{
		P[i].position=S[i].Position;
		P[i].velocity=S[i].Velocity;
		P[i].acceleration=S[i].Acceleration;
		//P[i]=StateToP3d(S[i]);

	}
}




//Vector3d acceleration(State ip, vector<Particle3d> P)
//{
//	
//	Vector3d ACCELERATION;
//	Vector3d tempAcceleration;
//	for(int i=0; i<PARTICLE_C; i++)
//	{
//
//		if(ip.i==i) continue;
//		
//		real d=sqrt( (ip.Position.x-P[i].position.x)*(ip.Position.x-P[i].position.x) +
//				 (ip.Position.y-P[i].position.y)*(ip.Position.y-P[i].position.y) +
//				 (ip.Position.z-P[i].position.z)*(ip.Position.z-P[i].position.z)
//			   );
//		
//	real wk=1;
//	real f=-6.67384*0.00000180*wk*ip.mass*P[i].mass/(d*d);
//	
//	tempAcceleration.Set((ip.Position.x-P[i].position.x)*f/d/ip.mass, (ip.Position.y-P[i].position.y)*f/d/ip.mass,
//		(ip.Position.z-P[i].position.z)*f/d/ip.mass);
//	ACCELERATION+=tempAcceleration;//accumulate accerleations
//
//
//	}
//	
//	
//	return ACCELERATION;
//
//}


