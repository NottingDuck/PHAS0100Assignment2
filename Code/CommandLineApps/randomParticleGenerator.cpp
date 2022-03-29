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
#include <nbsimRandomParticle.h>
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
    << "\tOption: (Random Particle Simulator):\tUsers should input .1) step-size(unit: year) .2) length of time(unit: year)\n"
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
        int NParticles = 2000;

        std::vector<std::shared_ptr<nbsim::MassiveParticle>> ListParticles;

        nbsim::RandomParticle * p = new nbsim::RandomParticle;

        ListParticles = p->generateParticle();
        
        // Add the attractor:
        for(int i=0;i<NParticles;i++){
            for(int j=0;j<NParticles;j++){
                if(i != j){
                    ListParticles[i]->addAttractor(ListParticles[j]);
                }
            }
        }

        // At the begining: calculate the Energy
        ListParticles[0]->addAttractor(ListParticles[0]); // add itself
    
        ListParticles[0]->calculateEtotal();
        std::cout<<"Beginning:"<<"\n";
        std::cout<<"The kinetic energy of system is: "<<ListParticles[0]->getEkinetic()<<"\n";
        std::cout<<"The potential Energy of system is: "<<ListParticles[0]->getEpotential()<<"\n";
        std::cout<<"The Total Energy of system is: "<<ListParticles[0]->getEtotal()<<"\n";
        ListParticles[0]->removeAttractor(ListParticles[0]); // remove itself


        // Benchmark the time of the solar system: Begin
        std::clock_t c_start = std::clock();
        auto t_start = std::chrono::high_resolution_clock::now();

        // Outer time:
        for (double t = 0;t<totalTime;t+=step_size){
            // Loop 1: Acceleration:
            for (int i=0;i<NParticles;i++){
                ListParticles[i]->calculateAcceleration();
            }
            // Loop 2: intergateTimestep:
            for (int i=0;i<NParticles;i++){
                ListParticles[i]->integrateTimestep(step_size);

            }
        }

        // Benchmark the time of the solar system: End
        std::clock_t c_end = std::clock();
        auto t_end = std::chrono::high_resolution_clock::now();

        // Calculate the System Energy:
        ListParticles[0]->addAttractor(ListParticles[0]); // add itself
        
        ListParticles[0]->calculateEtotal();
        std::cout<<"End:"<<"\n";
        std::cout<<"The Kinetic Energy of system is: "<<ListParticles[0]->getEkinetic()<<"\n";
        std::cout<<"The Potential Energy of system is: "<<ListParticles[0]->getEpotential()<<"\n";
        std::cout<<"The Total Energy of system is: "<<ListParticles[0]->getEtotal()<<"\n";
        
        ListParticles[0]->removeAttractor(ListParticles[0]); // remove itself

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