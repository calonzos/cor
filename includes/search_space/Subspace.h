#ifndef Subspace_h
#define Subspace_h

#include <vector>

#include "fl/Headers.h"
#include "includes/search_space/Sample.h"
#include "includes/search_space/SampleGenerator.h"
#include "includes/search_space/TermsGenerator.h"

namespace cor {

	class Subspace {

	protected:

	private:
		
	public:

		Subspace();
		~Subspace();

std::map<int, std::vector<std::vector<fl::scalar>>> generatePossitiveExampleInterpretability(std::map<int, std::vector<fl::scalar> > E,std::map<std::string, fl::Triangle > A,std::map<int, std::vector<std::string>> Ai,std::map<int, std::vector<std::string> > Ss);

std::map<int, std::vector<std::vector<fl::scalar>>> generatePossitiveExampleAccuracy(std::map<int, std::vector<fl::scalar> > E,std::map<std::string, fl::Triangle > A,std::map<int, std::vector<std::string> > Ss,unsigned int numberOfInputs);


std::map<int, std::vector<std::vector<fl::scalar>>> generateSetWithPossitiveExample(std::map<int, std::vector<std::vector<fl::scalar>>> Es,unsigned int Ns);

std::map<int, std::vector<std::string>> generateCandidateConsequentInterpretability(std::map<int, std::vector<std::vector<fl::scalar>>> Es,std::map<std::string, fl::Triangle > B,unsigned int Ns,unsigned int numberOfInputs);

std::map<int, std::vector<std::string>> generateCandidateConsequentAccuracy(std::map<int, std::vector<std::vector<fl::scalar>>> Es,std::map<std::string, fl::Triangle > B,unsigned int Ns,unsigned int numberOfInputs);


		
	};
}

#endif

