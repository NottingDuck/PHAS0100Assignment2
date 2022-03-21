#include "nbsimParticle.h"

namespace nbsim {

    Particle::Particle(Eigen::Vector3d position, Eigen::Vector3d velocity){
         position = position; 
         velocity = velocity;
    };
    
    // Deconstrutor:
    Particle::~Particle(){}

    Eigen::Vector3d Particle::getPosition() const{
        return position;
    };
    Eigen::Vector3d Particle::getVelocity() const{
        return velocity;
    };

    void Particle::integrateTimestep(Eigen::Vector3d acceleration, double timestep){        
        
        position = position + velocity * timestep;
        velocity = velocity + acceleration * timestep; 
    };


} // end namespace
