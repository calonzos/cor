#ifndef SearchSpace_h
#define SearchSpace_h

#include <vector>
#include "fl/Headers.h"
#include "includes/search_space/Sample.h"
#include "includes/search_space/SampleGenerator.h"
#include "includes/search_space/SubspaceGenerator.h"
#include "includes/search_space/TermsGenerator.h"
#include "includes/search_space/Subspace.h"
//#include "includes/datosBD/mongoDB.h"

namespace cor {
	class SearchSpace {
	protected:
		std::vector<Sample> samples;
		std::vector<std::vector<fl::Triangle>> terms;
                std::map<std::string, fl::Triangle > A,B;
		std::map<int, std::vector<std::string>> Ai;
		std::map<int, std::vector<std::string>> Ss;
		std::map<int, std::vector<std::vector<fl::scalar>>> Es,Smas;
		std::map<int, std::vector<std::string>> C;
		std::map<int, std::vector<std::vector<std::string>>> Cr;
		std::map<std::vector<std::string>, fl::scalar > H,Tao;

	public:

		SearchSpace(const char * trainingFilename, unsigned int numberOfInputs, unsigned int numberOfTerms, std::vector<std::pair<fl::scalar, fl::scalar> > domains, unsigned int option = 1);
		~SearchSpace();

		std::map<int, std::vector<std::vector<std::string>>> generateCandidateSetRule(std::map<int, std::vector<std::string>> Ss,std::map<int, std::vector<std::string>> C);

		std::map<std::vector<std::string>, fl::scalar > generateHeuristicInformation(unsigned int numberOfInputs);
		std::map<std::vector<std::string>, fl::scalar > PheromoneInitialization();


		fl::scalar Heuristic_1(std::vector<fl::scalar> auxMin);
		fl::scalar Heuristic_2(std::vector<fl::scalar> auxMin);


		std::vector<Sample> getSamples() const;

		std::map<std::string, fl::Triangle > getTermTriangleInput();
		std::map<int, std::vector<std::string>> getTermInput();
		std::map<std::string, fl::Triangle > getTermTriangleOutput();
                std::map<int, std::vector<std::vector<fl::scalar>>> getEjemploPositivos();
		std::map<std::vector<std::string>, fl::scalar > getHeuristicInformation();
		std::map<std::vector<std::string>, fl::scalar > getPheromone();
		int NumeberOfSubspace();
		int NumeberOfPositiveSubspace();
		std::map<int, std::vector<std::string>> getCandidateConsecuent();
		std::map<int, std::vector<std::vector<std::string>>> getCandidateRules();

		std::vector<std::vector<fl::Triangle> > getTerms() const;

		fl::scalar getRange(unsigned int index) const;
		std::pair<fl::scalar, fl::scalar> getDomain(unsigned int index) const;
		std::vector<std::pair<fl::scalar, fl::scalar> > getAllDomains() const;

		void setPheromone(std::map<std::vector<std::string>, fl::scalar > tao);
	};
}

#endif
