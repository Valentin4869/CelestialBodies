#pragma once
#include "stdafx.h"
#include "StrOps.h"


typedef double real; 

enum COLOR{BLACK, RED, GREEN, BLUE, WHITE, YELLOW, ORANGE, MAGENTA,
			G75, //75% white (GN: shades of grey)
			G50, //50% white
			G25, //25% white
		  };

enum  P3d_preset{p3d_random_all,//random values for all attributes
				p3d_id, //assign ids
				p3d_radius, // set radius according to mass
				p3d_random_m, //set random masses
				p3d_random_v,
				p3d_point_particles, //shapeless particles-- point objects
				p3d_01,
				p3d_02,
				p3d_03,
				p3d_04
				}; //set random velocities
class Vector3d;
class Particle3d;
struct Face3d;
class Object3d;
class Box3d;
struct Text3d;
struct State;
struct Derivative;



class Object3d{

public:

	int nVertex;
	int nFace;
	int nVertexNormal;

	Vector3d *vertex;
	Vector3d *vertexNormal;
	Face3d *face;
	
	void LoadOBJ(char*);
	Object3d():nVertex(0), nFace(0), vertex(nullptr), face(nullptr){}
	Object3d(char*);
	~Object3d(){}
};

class Vector3d{
public:
	real x,
		 y,
		 z;

	
	real Magnitude		(){return sqrt(x*x+ y*y+ z*z);}
	void Normalize		(){real magnitude=Magnitude(); (*this)*= (real)1.0/magnitude; }

	
	Vector3d operator*(real k){ return Vector3d(x*k, y*k, z*k);}
	friend Vector3d operator*(real k, Vector3d &v){ return v*k;}
	void operator*=(real k){ *this=(*this)*k;}

	Vector3d operator+(Vector3d &v2){ return Vector3d(x+ v2.x, y+ v2.y, z+ v2.z);}
	void operator+=(Vector3d &v2){ *this=(*this)+v2;}

	Vector3d operator-(Vector3d &v2){ return Vector3d(x- v2.x, y- v2.y, z- v2.z);}
	void operator-=(Vector3d &v2){ *this=(*this)-v2;}

	void  Set(real X, real Y, real Z){x=X; y=Y, z=Z;}
	void  Set(real V){x=y=z=V;}
	Vector3d			():x(0.0), y(0.0), z(0.0){}
	Vector3d			(real X, real Y, real Z): x(X), y(Y), z(Z){}
};

struct Face3d{
	int nFaceV;
	int *iVertex;
	int *iVertexNormal;

	Face3d():nFaceV(0), iVertex(nullptr), iVertexNormal(nullptr){}
	//~Face3d(){if (iVertex!=nullptr) delete[] iVertex; if (iVertexNormal!=nullptr) delete[] iVertexNormal;}
};



struct Text3d{
	real x, y;
	COLOR color;
	const char *text;
	Text3d():color(WHITE), x(0.0), y(2.0){x=0; y=2;}
	Text3d(char* txt){color=WHITE; x=0.0; y=0.0; text=new char[strlen(txt)]; text=txt;}
	
};

class Box: public Object3d
{







};

struct State
{
	Vector3d Position,
			Velocity,
			Acceleration;
	float mass;
	int i;

	//State(){mass=0;}
};

struct Derivative
{
	Vector3d dPosition,
			dVelocity;
};





class Particle3d{
public:

	int id;

	Vector3d position,
			velocity,
			acceleration;

	real damping;
	real inverseMass;
	real mass;
	real radius;
	Particle3d(){ radius=0.5; position.x=(rand()%500-250.0)/10.0; position.y=(rand()%500-250.0)/10.0; position.z=(rand()%500-250.0)/10.0;}
};

class PointParticle3d: public Particle3d{
public:
	PointParticle3d(){ mass=10; radius=0.5; position.x=(rand()%5000-2500.0)/10.0; position.y=(rand()%5000-2500.0)/10.0; position.z=(rand()%5000-2500.0)/10.0;}
};