/*=============================================================================

  PHAS0100ASSIGNMENT2: PHAS0100 Assignment 2 Gravitational N-body Simulation

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

=============================================================================*/

#include "catch.hpp"
#include "nbsimCatchMain.h"
#include "nbsimMyFunctions.h"
#include "nbsimParticle.h"
#include "nbsimMassiveParticle.h"
#include <iostream>
#include <vector>
#include <Eigen/Dense>


TEST_CASE( "My first test", "[some group identifier]" ) {
  int a = 5;
  REQUIRE( a < 6 );
}

TEST_CASE( "My second test", "[some group identifier]" ) {
  std::vector<int> a;
  REQUIRE( a.size() == 0 );
}

TEST_CASE( "Simple add", "[MyFirstAddFunction]") {
  REQUIRE( nbsim::MyFirstAddFunction(1, 2) == 3);
}

// ------ //
// Testing for Task 1:
TEST_CASE( "Test a: No acceleration", "[Particle]") {

  Eigen::Vector3d a(0,0,0);

  Eigen::Vector3d p_test(0,0,0);
  Eigen::Vector3d v_test(1,1,1);

  Eigen::Vector3d p_expect(0.01,0.01,0.01);
  Eigen::Vector3d v_expect(1,1,1);

  nbsim::Particle particle_a(p_test,v_test);
  double dt = 0.01;
  
  REQUIRE(particle_a.getPosition().isApprox(p_test));
  REQUIRE(particle_a.getVelocity().isApprox(v_test));

  particle_a.integrateTimestep(a,dt);
  REQUIRE(particle_a.getPosition().isApprox(p_expect));
  REQUIRE(particle_a.getVelocity().isApprox(v_expect));
}

TEST_CASE( "Test b: Constant acceleration", "[Particle]") {

  Eigen::Vector3d a(1,1,1);

  Eigen::Vector3d p_test(0,0,0);
  Eigen::Vector3d v_test(1,1,1);

  Eigen::Vector3d p_expect1(0.01,0.01,0.01);
  Eigen::Vector3d v_expect1(1.01,1.01,1.01);

  nbsim::Particle particle_b(p_test,v_test);
  double dt = 0.01;

  particle_b.integrateTimestep(a,dt);
  REQUIRE(particle_b.getPosition().isApprox(p_expect1));
  REQUIRE(particle_b.getVelocity().isApprox(v_expect1));

  Eigen::Vector3d p_expect2(0.0201,0.0201,0.0201);
  Eigen::Vector3d v_expect2(1.02,1.02,1.02);
 
  particle_b.integrateTimestep(a,dt);
  REQUIRE(particle_b.getPosition().isApprox(p_expect2));
  REQUIRE(particle_b.getVelocity().isApprox(v_expect2));
}

TEST_CASE( "Test c: Fictitious centripetal acceleration", "[Particle]") {

  Eigen::Vector3d p_test(0,0,0);
  Eigen::Vector3d v_test(1,1,1);

  nbsim::Particle particle_c(p_test,v_test);
  double dt = 0.1;
  double time = 0.3;

  for(double i=0;i<time;i+=dt){
    Eigen::Vector3d a = -particle_c.getPosition();
    particle_c.integrateTimestep(a,dt);
  }

  Eigen::Vector3d p_expect(0.299,0.299,0.299);
  Eigen::Vector3d v_expect(0.97,0.97,0.97);

  REQUIRE(particle_c.getPosition().isApprox(p_expect));
  REQUIRE(particle_c.getVelocity().isApprox(v_expect));

}

TEST_CASE( "Test a: Still models linear motion correctly with no attractors", "[MassiveParticle]") {

  Eigen::Vector3d p_test(0,0,0);
  Eigen::Vector3d v_test(1,1,1);

  Eigen::Vector3d p_expect(0.1,0.1,0.1);
  Eigen::Vector3d v_expect(1,1,1);

  double mass = 1;
  double dt = 0.1;

  nbsim::MassiveParticle MassiveParticle_a(p_test,v_test,mass);
  MassiveParticle_a.calculateAcceleration();
  MassiveParticle_a.integrateTimestep(dt);

  REQUIRE(MassiveParticle_a.getPosition().isApprox(p_expect));
  REQUIRE(MassiveParticle_a.getVelocity().isApprox(v_expect));

}

TEST_CASE( "Test b: With gravitationally attractors", "[MassiveParticle]") {

  Eigen::Vector3d p_1(1,0,0);
  Eigen::Vector3d v_1(0,0.5,0);

  Eigen::Vector3d p_2(-1,0,0);
  Eigen::Vector3d v_2(0,-0.5,0);

  double mu = 1;
  double mass = mu/G;

  std::shared_ptr<nbsim::MassiveParticle> attractor_ptr1(new nbsim::MassiveParticle(p_1,v_1,mass));
  std::shared_ptr<nbsim::MassiveParticle> attractor_ptr2(new nbsim::MassiveParticle(p_2,v_2,mass));

  attractor_ptr1 -> addAttractor(attractor_ptr2);
  attractor_ptr2 -> addAttractor(attractor_ptr1);

  double dt = 0.001;
  double t = 1;

  for (double i=0;i<=t;i+=dt){
    attractor_ptr1->calculateAcceleration();
    attractor_ptr1->integrateTimestep(dt);

    attractor_ptr2->calculateAcceleration();
    attractor_ptr2->integrateTimestep(dt);    
  }

  double distance = (attractor_ptr1->getPosition() - attractor_ptr2->getPosition()).norm();
  double error_expect = std::abs(distance-2);
  REQUIRE(error_expect <= 0.01);
}


