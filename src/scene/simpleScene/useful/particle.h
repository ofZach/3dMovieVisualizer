#ifndef PARTICLE_H
#define PARTICLE_H

#include "ofMain.h"

#include "ofxVectorMath.h"



class particle
{
    public:
        ofxVec3f pos;
        ofxVec3f vel;
        ofxVec3f frc;   // frc is also know as acceleration (newton says "f=ma")
			
	
        particle();
		virtual ~particle(){};

        void resetForce();
		void addForce(float x, float y);
		void addRepulsionForce(float x, float y, float radius, float scale);
	
		void addRepulsionForce(ofxVec3f pos, float radius, float scale);
		void addAttractionForce(ofxVec3f pos, float radius, float scale);
	
	
		void addAttractionForce(float x, float y, float radius, float scale);
		
		void addRepulsionForce(particle &p, float radius, float scale);
		void addRepulsionForce(particle *p, float radius, float scale);
	
		void addAttractionForce(particle &p, float radius, float scale);
		void addClockwiseForce(particle &p, float radius, float scale);
		void addCounterClockwiseForce(particle &p, float radius, float scale);

		void addDampingForce();
        
		void setInitialCondition(float px, float py, float vx, float vy);
        void update();
        void draw();
	
		float lastUpdateTime;


		void bounceOffWalls();

		bool  bFixed;

		float damping;
	
		ofPoint color;
	

    protected:
    private:
};

#endif // PARTICLE_H
