#ifndef nbsimParticle_h
#define nbsimParticle_h

#include <Eigen/Dense>

//! Single namespace for all code in this package
namespace nbsim
{

// Particle Class
class Particle
{
private:
    Eigen::Vector3d position;
    Eigen::Vector3d velocity;
    
public:

    // Constructor:
    Particle(Eigen::Vector3d position, Eigen::Vector3d velocity);
    
    // Deconstrutor:
    ~Particle();

    Eigen::Vector3d getPosition() const;
    Eigen::Vector3d getVelocity() const;
    
    void integrateTimestep(Eigen::Vector3d acceleration, double timestep);

};


} // end namespace

#endif
