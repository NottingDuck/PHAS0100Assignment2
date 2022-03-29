#include "nbsimMassiveParticle.h"

namespace nbsim {

    MassiveParticle::MassiveParticle(Eigen::Vector3d position, Eigen::Vector3d velocity, double mu): Particle(position,velocity){
        _mu = mu;
    };
    
    // Deconstrutor:
    MassiveParticle::~MassiveParticle(){}

    double MassiveParticle::getMu(){
        return _mu;
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

        _acceleration = acceleration;
    };

    void MassiveParticle::integrateTimestep(double timestep){
        _position += _velocity * timestep;
        _velocity += _acceleration * timestep; 
    };

    // Energy:
    void MassiveParticle::calculateEkinetic(){
        
        double Ekinetic = 0.0;

        for (auto attractor:Attractors){
            Eigen::Vector3d v_i = attractor->getVelocity();
            double u_i = attractor->getMu();

            Ekinetic += 0.5*u_i*pow(v_i.norm(),2);
        }

        _Ekinetic = Ekinetic;
    };

    void MassiveParticle::calculateEpotential(){
        
        double Epotential = 0.0;

        for (auto attractor_i:Attractors){
            Eigen::Vector3d x_i = attractor_i->getPosition();
            double u_i = attractor_i->getMu();

            for (auto attractor_j:Attractors){
                Eigen::Vector3d x_j = attractor_j->getPosition();
                double u_j = attractor_j->getMu();
                
                if(attractor_j!=attractor_i){
                    // Energy:
                    Epotential += -0.5*(u_i*u_j)/(x_i-x_j).norm();
                }
            }
        }

        _Epotential = Epotential;

    };

    void MassiveParticle::calculateEtotal(){
        
        calculateEkinetic();
        calculateEpotential();
        _Etotal = _Ekinetic + _Epotential;
    };
    
    double MassiveParticle::getEkinetic() const{
        return _Ekinetic;
    }

    double MassiveParticle::getEpotential() const{
        return _Epotential;
    }

    double MassiveParticle::getEtotal() const{
        return _Etotal;
    }

} // end namespace
