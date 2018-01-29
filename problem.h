#ifndef _PROBLEM_H_
#define _PROBLEM_H_

#include <vector>
#include <string>
#include <ilcp/cp.h>

ILOSTLBEGIN

// Class for reading the problem
class Problem {

protected:
    IloEnv env;
    IloModel model;

public:
  Problem(IloEnv e):
    env(e)
    
  {} 
  virtual ~Problem() {}

  void solveProblem();
  void createModelCP();

};

#endif


