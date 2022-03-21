#include "nbsimMassiveParticle.h"

#define G 6.67408e-11;

namespace nbsim {

    MassiveParticle::MassiveParticle(Eigen::Vector3d position, Eigen::Vector3d velocity, double mass): Particle(position,velocity){
        _mass = mass; 
    };
    
    // Deconstrutor:
    MassiveParticle::~MassiveParticle(){}

    double MassiveParticle::getMu(){
        return _mass*G;
    };

    void MassiveParticle::addAttractor(std::shared_ptr<MassiveParticle> attractor){
        Attractors.insert(attractor);
    };
    void MassiveParticle::removeAttractor(std::shared_ptr<MassiveParticle> attractor){
        Attractors.erase(attractor);
    };
    void MassiveParticle::calculateAcceleration(){
        Eigen::Vector3d acceleration(0.0,0.0,0.0);

        for (auto attractor:Attractors){
            Eigen::Vector3d r_i = getPosition() - attractor->getPosition();
            acceleration+= -attractor->getMu()/r_i.dot(r_i)*r_i.normalized();
        }

        _acceletarion = acceleration;
    };

    void MassiveParticle::integrateTimestep(double timestep){
        getPosition() += getVelocity() * timestep;
        getVelocity() += _acceletarion * timestep; 
    };


} // end namespace
