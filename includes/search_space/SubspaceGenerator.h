#ifndef SubspaceGenerator_h
#define SubspaceGenerator_h

#include <vector>
#include "fl/Headers.h"
#include "includes/search_space/Sample.h"
#include "includes/search_space/SampleGenerator.h"
#include "includes/search_space/TermsGenerator.h"
#include "includes/search_space/Subspace.h"

namespace cor {

	class SubspaceGenerator {

	protected:
		
	private:

	public:
		SubspaceGenerator();
		~SubspaceGenerator();

		std::map<int, std::vector<std::string> > generateSubspacesOfTerms(std::map<int, std::vector<std::string>> A,unsigned int Ns,unsigned int numberOfInputs,unsigned int numberOfTerms);

void  NestedSubspaceTerm(std::map<int, std::vector<std::string>> A, unsigned int &input,std::vector<std::string> &aT ,unsigned int numberOfTerms,unsigned int numberOfInputs,std::map<int, std::vector<std::string> > &Gsubspace,unsigned int &HMindice);

		
	};
}

#endif
