#include "includes/SearchSpaceHeaders.h"
#include "includes/CorEngineHeaders.h"
#include "includes/AcoHeaders.h"


using namespace cor;

/**	@class CorProblem
*	Constructors
*/

CorProblem::CorProblem(const char * filename, unsigned int numberOfInputs, unsigned int numberOfTerms, std::vector<std::pair<fl::scalar, fl::scalar> > domains, fl::scalar resolutionFactor){
	
	searchSpace = new SearchSpace(filename, numberOfInputs, numberOfTerms, domains);	
	corEngine = new CorEngine(searchSpace, resolutionFactor);
	//corLocalSearch = new CorLocalSearch(searchSpace, corEngine);

}

CorProblem::~CorProblem() {
	delete corEngine;
	delete searchSpace;
	//delete corLocalSearch;
}


SearchSpace * CorProblem::getSearchSpace() const {
	return searchSpace;
}

CorEngine * CorProblem::getCorEngine() const {
	return corEngine;
}


