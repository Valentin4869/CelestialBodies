#include "stdafx.h"
#include "Physics_Functions.h"
#include "Global.h"

//Local variables
Vector3d (*acc_func)(State); //acceleration function pointer
Particle3d *P;	//array of particles to reference(p3d or pp3d)
State *PS;
short P_C;
Vector3d ContactNormal;
real b_g=6.67384*0.000080, s_g;
const short POINT_PARTICLE_C=1;
const short PARTICLE_C=5;
vector<Particle3d> particles(PARTICLE_C);
vector<PointParticle3d> point_particles(POINT_PARTICLE_C);
vector<State> p3d_states(PARTICLE_C);
vector<State> p_p3d_states(POINT_PARTICLE_C);


Vector3d accUniversalGravitation(State ip)
{
	
	Vector3d ACCELERATION;
	Vector3d tempAcceleration;
	
	for(int i=0; i<P_C; i++)
	{
		if(ip.i==i) continue;
		
		real d=sqrt( (ip.Position.x-P[i].position.x)*(ip.Position.x-P[i].position.x) +
				 (ip.Position.y-P[i].position.y)*(ip.Position.y-P[i].position.y) +
				 (ip.Position.z-P[i].position.z)*(ip.Position.z-P[i].position.z)
			   );

	real wk=1.0;
	real f=-b_g*wk*ip.mass*P[i].mass/(d*d);

	
	tempAcceleration.Set((ip.Position.x-P[i].position.x)*f/d/ip.mass, (ip.Position.y-P[i].position.y)*f/d/ip.mass,
		(ip.Position.z-P[i].position.z)*f/d/ip.mass);
	ACCELERATION+=tempAcceleration;
	
	}
	
	return ACCELERATION;

}
Vector3d accDirectionalGravity(State sip)
{	
	return Vector3d(0,-10,0);
}

Derivative evaluate(int ip)
{
	Derivative output;
	output.dPosition = P[ip].velocity ;
	output.dVelocity = acc_func(P3dToState(P[ip])); //arbitrary acceleration function

	return output;
}


Derivative evaluate(int ip, float dt, Derivative &d)
{
	State state1, state2;
	Derivative output;
	state1.i=ip;
	state1.mass=P[ip].mass;
	state1.Position = P[ip].position + d.dPosition*dt + d.dVelocity*0.5*dt*dt;// dp/dt= velocity
	state1.Velocity = P[ip].velocity + d.dVelocity*dt;//p''=dv/dt= acceleration
	
	output.dPosition = state1.Velocity;
	output.dVelocity= acc_func(state1); //arbitrary acceleration function

	return output;
}

void DirectionalGravity_simple(int ip, Particle3d* p , short p_c, State& fstate, float dt)
{
	P=p;
	P_C=p_c;
	acc_func=accDirectionalGravity;
	Derivative k1 = evaluate(ip);
	
	Vector3d dPOSITION;
	Vector3d dVELOCITY;

	dPOSITION= k1.dPosition;
	dVELOCITY =k1.dVelocity;
	
	
	fstate.Position += dPOSITION*dt; 
	fstate.Velocity += dVELOCITY*dt;
	fstate.Acceleration = dVELOCITY*dt;

}



void DirectionalGravity(int ip, Particle3d* p , short p_c, State& fstate, float dt)
{
	P=p;
	P_C=p_c;
	acc_func=accDirectionalGravity;
	Derivative k1 = evaluate(ip);
	Derivative k2 = evaluate(ip, dt*0.5f, k1);
	Derivative k3 = evaluate(ip, dt*0.5f, k2);
	Derivative k4 = evaluate(ip, dt, k3);

	Vector3d dPOSITION;
	Vector3d dVELOCITY;

	dPOSITION= 1.0f/6.0f*(k1.dPosition + 2.0f*(k2.dPosition + k3.dPosition) + k4.dPosition);
	dVELOCITY = 1.0f/6.0f*(k1.dVelocity + 2.0f*(k2.dVelocity + k3.dVelocity) + k4.dVelocity);
	
	
	fstate.Position += dPOSITION*dt; 
	fstate.Velocity += dVELOCITY*dt;
	fstate.Acceleration = dVELOCITY*dt;

}

void UniversalGravitation(int ip, Particle3d* p, short p_c, State& fstate , float dt, real G )
{
	P=p;
	P_C=p_c;
	
	if(G>=0)
	b_g=G;

	acc_func=accUniversalGravitation;
	Derivative k1 = evaluate(ip);
	Derivative k2 = evaluate(ip, dt*0.5f, k1);
	Derivative k3 = evaluate(ip, dt*0.5f, k2);
	Derivative k4 = evaluate(ip, dt, k3);

	Vector3d dPOSITION;
	Vector3d dVELOCITY;

	dPOSITION= 1.0f/6.0f*(k1.dPosition + 2.0f*(k2.dPosition + k3.dPosition) + k4.dPosition);
	dVELOCITY = 1.0f/6.0f*(k1.dVelocity + 2.0f*(k2.dVelocity + k3.dVelocity) + k4.dVelocity);
	
	
	/*P[ip].position += dPOSITION*dt;
	P[ip].velocity += dVELOCITY*dt;
	P[ip].acceleration = dVELOCITY;*/

	
	fstate.Position += dPOSITION*dt; 
	fstate.Velocity += dVELOCITY*dt;
	fstate.Acceleration = dVELOCITY*dt;
	

}

void UniversalGravitation_simple(int ip, Particle3d* p, short p_c, State& fstate , float dt, real G )
{
	P=p;
	P_C=p_c;
	
	if(G>=0)
	b_g=G;

	acc_func=accUniversalGravitation;
	Derivative k1 = evaluate(ip);
	
	Vector3d dPOSITION;
	Vector3d dVELOCITY;

	dPOSITION= k1.dPosition;
	dVELOCITY = k1.dVelocity;
	
	fstate.Position += dPOSITION*dt; 
	fstate.Velocity += dVELOCITY*dt;
	fstate.Acceleration = dVELOCITY*dt;

}
void CollisionResolution2(int p_c, Particle3d *p, State *ps)
{
	Vector3d Normal, Tangent, Tangent2, Tangent3;
	real ra, rb;
	real err_l;
	real C=.5; 
						
	for(int i=0; i<p_c; i++)
		{
			for(int j=i; j<p_c; j++)
			{
				if(i==j)
					continue;

				real d=(Distance3d(ps[i].Position, ps[j].Position)-(p[i].radius+p[j].radius));
				if( d<0)	
				{
					Normal.Set(ps[i].Position.x-ps[j].Position.x, ps[i].Position.y-ps[j].Position.y,ps[i].Position.z-ps[j].Position.z);
					Normal.Normalize();
					ContactNormal=Normal;
					dImpenetrationResolution(i, j, p, ps);
									
					Vector3d &s1=ps[i].Velocity, &s2=ps[j].Velocity;

					real v1n, v1t, v2n, v2t;
					real v1n2, v1t2, v2n2, v2t2;
					real v1n3, v1t3, v2n3, v2t3;
					real v1np, v2np;

										
					Tangent.Set(-Normal.y, Normal.x, 0);
					Tangent2.Set(-Normal.z, 0, Normal.x);
					Tangent3.Set(0, -Normal.z, Normal.y);
					v1n=Normal.x*s1.x + Normal.y*s1.y + Normal.z*s1.z;
					v2n=Normal.x*s2.x + Normal.y*s2.y + Normal.z*s2.z;

					v1t=Tangent.x*s1.x + Tangent.y*s1.y;
					v2t=Tangent.x*s2.x + Tangent.y*s2.y;
					v1t2=Tangent2.x*s1.x + Tangent2.z*s1.z;
					v2t2=Tangent2.x*s2.x + Tangent2.z*s2.z;
					v1t3=Tangent3.z*s1.z + Tangent3.y*s1.y;
					v2t3=Tangent3.z*s2.z + Tangent3.y*s2.y;

					v1np=(v1n*(ps[i].mass-C*ps[j].mass)+ v2n*ps[j].mass*(C+1))/(ps[i].mass+ps[j].mass);
					v2np=(v2n*(ps[j].mass-C*ps[i].mass)+ v1n*ps[i].mass*(C+1))/(ps[i].mass+ps[j].mass);
									
					ps[i].Velocity.x=Normal.x*v1np+ Tangent.x*v1t + Tangent2.x*v1t2 + Tangent3.x*v1t3;
					ps[i].Velocity.y=Normal.y*v1np+ Tangent.y*v1t + Tangent2.y*v1t2 + Tangent3.y*v1t3;
					ps[i].Velocity.z=Normal.z*v1np+ Tangent.z*v1t + Tangent2.z*v1t2 + Tangent3.z*v1t3;
					ps[j].Velocity.x=Normal.x*v2np+ Tangent.x*v2t + Tangent2.x*v2t2 + Tangent3.x*v2t3;
					ps[j].Velocity.y=Normal.y*v2np+ Tangent.y*v2t + Tangent2.y*v2t2 + Tangent3.y*v2t3;
					ps[j].Velocity.z=Normal.z*v2np+ Tangent.z*v2t + Tangent2.z*v2t2 + Tangent3.z*v2t3;
					}

				}

		}
	

}

void dImpenetrationResolution(int i, int j, Particle3d *p, State *ps)
{

				
	real di=Distance3d(p[i].position, ps[i].Position);//previous distances
	real dj=Distance3d(p[j].position, ps[j].Position);
	real err_l=abs((p[i].radius+p[j].radius)-Distance3d(p[i].position, p[j].position));

								real vm1=p[i].velocity.Magnitude(), vm2=p[j].velocity.Magnitude();
								real tv=1.0/(p[j].mass+vm2) + 1.0/(p[i].mass+vm1);
								//real tv=vm1 + vm2;
								
						Vector3d mv=ContactNormal;
						mv.Normalize();
						
						/*if(tv<=0)
							ps[i].Position+= mv*(err_l/2.0),
							ps[j].Position+= mv*(-err_l/2.0);	
						else
						{
							mv  *=(-err_l / tv);

							if(vm1<=0)	
							ps[j].Position+= mv*vm2;
							
							else if(vm2<=0)
							ps[i].Position+= mv*(-vm1);

							
						}*/
						
						mv  *=(-err_l / (tv));
							ps[i].Position+= mv*(1/(-p[i].mass+vm1)),
							ps[j].Position+= mv*(1/(p[j].mass+vm2));	
						
							


					
}

void CollisionResolution(int p_c, Particle3d *p, State *ps)
{
	real d1, d2;
	real ra, rb;
	real err_l;
	real C=0.60;
	Vector3d Normal, Tangent, Tangent2, Tangent3;

		for(int i=0; i<p_c; i++)
		{
			for(int j=i; j<p_c; j++)
			{
				if(i==j)
					continue;

				real d=(Distance3d(ps[i].Position, ps[j].Position)-(p[i].radius+p[j].radius));
				if( d<0)	
				{
						
					Normal.Set(ps[i].Position.x-ps[j].Position.x, ps[i].Position.y-ps[j].Position.y,ps[i].Position.z-ps[j].Position.z);
					Normal.Normalize();
					ContactNormal=Normal;
					dImpenetrationResolution(i,j,p,ps);
											
					real separatingVelocity = dotProduct(ps[i].Velocity-ps[j].Velocity,Normal);
					real newSepVelocity = -separatingVelocity * C;
					real deltaVelocity = newSepVelocity - separatingVelocity;
											
					const real totalInverseMass = 1.0/(ps[i].mass)+1.0/(ps[j].mass);
											
					// Calculate the impulse to apply.
					real impulse = deltaVelocity / totalInverseMass;
					// Find the amount of impulse per unit of inverse mass.
					Vector3d impulsePerIMass = Normal * impulse;
					// Apply impulses: they are applied in the direction of the contact,
					// and are proportional to the inverse mass.
					ps[i].Velocity=ps[i].Velocity + impulsePerIMass* (1/ps[i].mass);
					ps[j].Velocity=ps[j].Velocity + impulsePerIMass *(-1/ps[j].mass);
									

				}
				
			
			}
		}
				
		
}


void CollisionResolution_Ground(int p_c, Particle3d *p, State *ps){

	Vector3d Tangent, Tangent2, Tangent3;
	ContactNormal.Set(0,1,0);
	ContactNormal.Normalize();
	real v1n, v1t;
	real v1n2, v1t2;
	real v1n3, v1t3;
	real v1np;
	real C=0.45;
	real groundMass=9999999999;
								
	for(int i=0; i<p_c; i++)
		{
			
			if(ps[i].Position.y<-25)	
				{
					ps[i].Position.y=-25;
												
						
					Vector3d &s1=ps[i].Velocity;

							
					Tangent.Set(-ContactNormal.y, ContactNormal.x, 0);
					Tangent2.Set(-ContactNormal.z, 0, ContactNormal.x);
					Tangent3.Set(0, -ContactNormal.z, ContactNormal.y);
					v1n=ContactNormal.x*s1.x + ContactNormal.y*s1.y + ContactNormal.z*s1.z;

					v1t=Tangent.x*s1.x + Tangent.y*s1.y;
					v1t2=Tangent2.x*s1.x + Tangent2.z*s1.z;
					v1t3=Tangent3.z*s1.z + Tangent3.y*s1.y;

					v1np=(v1n*(ps[i].mass-C*groundMass)+ 0*groundMass*(C+1))/(ps[i].mass+groundMass);
					
									
					ps[i].Velocity.x=ContactNormal.x*v1np+ Tangent.x*v1t + Tangent2.x*v1t2 + Tangent3.x*v1t3;
					ps[i].Velocity.y=ContactNormal.y*v1np+ Tangent.y*v1t + Tangent2.y*v1t2 + Tangent3.y*v1t3;
					ps[i].Velocity.z=ContactNormal.z*v1np+ Tangent.z*v1t + Tangent2.z*v1t2 + Tangent3.z*v1t3;

				
			}
		}

}