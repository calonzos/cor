#ifndef CorProblem_h
#define CorProblem_h



#include "includes/SearchSpaceHeaders.h"
#include "includes/CorEngineHeaders.h"

using namespace cor;

class CorProblem {
protected:
    CorEngine * corEngine;
    SearchSpace * searchSpace;

public:
    CorProblem(const char * filename, unsigned int numberOfInputs, unsigned int numberOfTerms, std::vector<std::pair<fl::scalar, fl::scalar> > domains, fl::scalar resolutionFactor = 1.0000);
    ~CorProblem();

    SearchSpace * getSearchSpace() const;
    CorEngine * getCorEngine() const;
    

};


#endif
