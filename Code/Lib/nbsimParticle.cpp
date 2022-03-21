#include "nbsimParticle.h"

namespace nbsim {

    Particle::Particle(Eigen::Vector3d position, Eigen::Vector3d velocity){
         _position = position; 
         _velocity = velocity;
    };
    
    // Deconstrutor:
    Particle::~Particle(){}

    Eigen::Vector3d Particle::getPosition() const{
        return _position;
    };
    Eigen::Vector3d Particle::getVelocity() const{
        return _velocity;
    };

    void Particle::integrateTimestep(Eigen::Vector3d acceleration, double timestep){        
        
        _position += _velocity * timestep;
        _velocity += acceleration * timestep; 
    };


} // end namespace
