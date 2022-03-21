#ifndef nbsimMassiveParticle_h
#define nbsimMassiveParticle_h

#include "nbsimParticle.h"
#include <iostream>
#include <memory>
#include <set>
#include <vector>

namespace nbsim
{

// Particle Class
class MassiveParticle : public Particle
{
private:

    double _mass;
    Eigen::Vector3d _acceletarion;
    std::set<std::shared_ptr<MassiveParticle>> Attractors;
    
public:

    // Constructor:
    MassiveParticle(Eigen::Vector3d position, Eigen::Vector3d velocity, double mass);

    // Deconstrutor:
    ~MassiveParticle();

    double getMu();
    void addAttractor(std::shared_ptr<MassiveParticle> attractor);
    void removeAttractor(std::shared_ptr<MassiveParticle> attractor);
    void calculateAcceleration();
    void integrateTimestep(double timestep);

};


} // end namespace

#endif
