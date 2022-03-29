#include "nbsimRandomParticle.h"
#include <math.h>
#include <random>

#define Mu_max 0.0020517
#define Mu_min 6.6156e-06
#define r_max 50
#define r_min 1

namespace nbsim {

    RandomParticle::RandomParticle():MassiveParticle(_position,_velocity,_mu){};
    
    // Deconstrutor:
    RandomParticle::~RandomParticle(){}


    std::vector<std::shared_ptr<nbsim::MassiveParticle>> RandomParticle::generateParticle(){
        
        std::vector<std::shared_ptr<nbsim::MassiveParticle>> ListParticles;

        // Centre
        Eigen::Vector3d position_centre(0,0,0);
        Eigen::Vector3d velocity_centre(0,0,0);
        double Mu_centre = 100;

        std::shared_ptr<nbsim::MassiveParticle> particle_centre(new nbsim::MassiveParticle(position_centre,velocity_centre,Mu_centre));
        ListParticles.push_back(particle_centre);

        // generate random r, theta and mass
        std::default_random_engine t(time(0));
        std::uniform_real_distribution<double> distribution_theta(0, 2*M_PI); 
        std::uniform_real_distribution<double> distribution_r(r_min, r_max); 
        std::uniform_real_distribution<double> distribution_mu(Mu_min, Mu_max); 

        for(int i=1;i<2000;i++){
        
            double r = distribution_r(t); 
            double theta = distribution_theta(t); 
            double Mu = distribution_mu(t); 

            double rx = r*sin(theta);
            double ry = r*cos(theta);
            double rz = 0.0;
            
            double vx = -1/sqrt(r)*cos(theta);
            double vy = 1/sqrt(r)*sin(theta);
            double vz = 0.0;

            Eigen::Vector3d position(rx,ry,rz);
            Eigen::Vector3d velocity(vx,vy,vz);

            std::shared_ptr<nbsim::MassiveParticle> particle(new nbsim::MassiveParticle(position,velocity,Mu));

            ListParticles.push_back(particle);
        }

        return ListParticles;

    };

} // end namespace


