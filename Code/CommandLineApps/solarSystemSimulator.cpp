/*=============================================================================
  PHAS0100ASSIGNMENT1: PHAS0100 Assignment 1 Game of Life Simulation.
  Copyright (c) University College London (UCL). All rights reserved.
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.
  See LICENSE.txt in the top level directory for details.
=============================================================================*/

#include <nbsimParticle.h>
#include <nbsimMassiveParticle.h>
#include <nbsimSolarSystemData.ipp>
#include <iostream>
#include <Eigen/Dense>
#include <chrono>

// Example, header-only library, included in project for simplicity's sake.


/**
 * \brief Demo file to check that includes and library linkage is correct.
 */

static void show_usage(std::string name)
{
    std::cerr << "Usage: " << name
    << "Options:\n"
    << "\tOption: (Solar System Simulator):\tUsers should input .1) step-size(unit: year) .2) length of time(unit: year)\n"
    << "Options:\n"
    << "\t-h,--help\t\t\tShow this help message.\n"
    << std::endl;
}

int main(int argc, char** argv)
{   
    // -h and --help:
    if (argc == 2){
        if((argv[1] == "-h") or (argv[1] == "--help")){
            show_usage(argv[0]);
            return 0;
        }
        else{
            show_usage(argv[0]);
            return 0;
        }
    }
    
    // Option: Solar System:
    if (argc == 3){

        double step_size = std::stod(argv[1]);
        double totalTime = std::stod(argv[2]);
        
        int NPLANETS = 9;

        std::string name[NPLANETS];
        Eigen::Vector3d position;
        Eigen::Vector3d velocity;
        double mu;

        std::shared_ptr<nbsim::MassiveParticle> planet[NPLANETS];

        // Split the planet:
        for (int i=0;i<NPLANETS;i++){
            name[i] =  nbsim::solarSystemData.at(i).name; 
            position = nbsim::solarSystemData.at(i).position;
            velocity = nbsim::solarSystemData.at(i).velocity;
            mu =  nbsim::solarSystemData.at(i).mu;

            std::shared_ptr<nbsim::MassiveParticle> particle(new nbsim::MassiveParticle(position,velocity,mu));
            planet[i] = particle;
        }

        // Add the attractor:
        for(int i=0;i<NPLANETS;i++){
            for(int j=0;j<NPLANETS;j++){
                if(i != j){
                    planet[i]->addAttractor(planet[j]);
                }
            }
        }

        // At the begining: calculate the Energy
        planet[0]->addAttractor(planet[0]); // add itself
    
        planet[0]->calculateEtotal();
        std::cout<<"Beginning:"<<"\n";
        std::cout<<"The kinetic energy of system is: "<<planet[0]->getEkinetic()<<"\n";
        std::cout<<"The potential Energy of system is: "<<planet[0]->getEpotential()<<"\n";
        std::cout<<"The Total Energy of system is: "<<planet[0]->getEtotal()<<"\n";
        planet[0]->removeAttractor(planet[0]); // remove itself


        // Benchmark the time of the solar system: Begin
        std::clock_t c_start = std::clock();
        auto t_start = std::chrono::high_resolution_clock::now();


        // Parallel:
        omp_set_num_threads (8);

        #pragma omp parallel
        // Outer time:
        for (double t = 0;t<totalTime;t+=step_size){
            
            // Loop 1: Acceleration:
            #pragma omp for
            for (int i=0;i<NPLANETS;i++){
                planet[i]->calculateAcceleration();
            }
            // Loop 2: intergateTimestep:
            #pragma omp for
            for (int i=0;i<NPLANETS;i++){
                planet[i]->integrateTimestep(step_size);

            }
        }

        // Benchmark the time of the solar system: End
        std::clock_t c_end = std::clock();
        auto t_end = std::chrono::high_resolution_clock::now();


        // Summarising the position:
        for(int i=0;i<NPLANETS;i++){
            std::cout<<name[i] <<":\n Original Position: \n"<<nbsim::solarSystemData.at(i).position<<"\n";
            std::cout<<" Current Position: \n"<<planet[i]->getPosition()<<"\n\n";
        }

        // Calculate the System Energy:
        planet[0]->addAttractor(planet[0]); // add itself
        
        planet[0]->calculateEtotal();
        std::cout<<"End:"<<"\n";
        std::cout<<"The Kinetic Energy of system is: "<<planet[0]->getEkinetic()<<"\n";
        std::cout<<"The Potential Energy of system is: "<<planet[0]->getEpotential()<<"\n";
        std::cout<<"The Total Energy of system is: "<<planet[0]->getEtotal()<<"\n";
        
        planet[0]->removeAttractor(planet[0]); // remove itself

        // Benchmark and output the time:
        std::cout<<"\nThe run time is: "
                 <<1000.0*(c_end - c_start)/CLOCKS_PER_SEC<<" ms\n"
                 <<"Wall clock time passed: "
                 <<std::chrono::duration<double, std::milli>(t_end-t_start).count()<<" ms\n";


        return 1;
    }

    // Default Option:
    else{
        show_usage(argv[0]);
        return 0;
    }
}