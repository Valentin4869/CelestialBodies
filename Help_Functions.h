#pragma once

#include "stdafx.h"
#include "Primitives.h"


//---Vector Operations---
Vector3d compProduct(Vector3d& v1, Vector3d& v2);
Vector3d crossProduct(Vector3d& v1, Vector3d& v2);
real dotProduct(Vector3d& v1, Vector3d& v2);


//---Particle3d---
void InitP3d(Particle3d* P, int size, P3d_preset);
State P3dToState(Particle3d&p);
Particle3d StateToP3d(State&s);
void CopyParticles(State*, Particle3d*, int);
void CopyStates(State*, Particle3d*, int);


//---Drawing---
void DrawVector3d(Vector3d& v);
void DrawVector3d(Vector3d& v, real x, real y, real z);
void DrawObject3d(Object3d*);
void DrawParticle3d(Particle3d&);
void DrawPointParticle3d(PointParticle3d&);

real Distance3d(Vector3d p1, Vector3d p2);
void PrintVector3d(Vector3d v);
void SetColor(COLOR);
void RenderText(Text3d);
void GetProjection(double x, double y,double z, real &X,real &Y);
