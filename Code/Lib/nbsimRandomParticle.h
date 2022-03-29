#ifndef nbsimRandomParticle_h
#define nbsimRandomParticle_h

#include "nbsimMassiveParticle.h"
#include <Eigen/Dense>
#include <vector>
#include <memory>

#define G 6.67408e-11;

//! Single namespace for all code in this package
namespace nbsim
{

// Particle Class
class RandomParticle : public MassiveParticle
{
public:

    // Constructor:
    RandomParticle();
    
    // Deconstrutor:
    ~RandomParticle();

    std::vector<std::shared_ptr<nbsim::MassiveParticle>> generateParticle();
};


} // end namespace

#endif
