#include "includes/SearchSpaceHeaders.h"
#include "includes/CorEngineHeaders.h"
#include "includes/AcoHeaders.h"

using namespace cor;

/** @class TermsGenerator
*	Constructors
*/

TermsGenerator::TermsGenerator(unsigned int numberOfTerms_) : numberOfTerms(numberOfTerms_) {}

TermsGenerator::~TermsGenerator() {}

/**
*	Observers
*/

unsigned int TermsGenerator::getNumberOfTerms() const {
	return numberOfTerms;
}

/**
*	Modifiers
*/

std::vector<fl::Triangle> TermsGenerator::generateTermsFromDomain(std::string baseName, fl::scalar minValue, fl::scalar maxValue) {
	std::vector<fl::Triangle> terms;

	//Get midRange value
	fl::scalar midRange = (maxValue - minValue)/(numberOfTerms - 1);

	//Set A,B and C for every term [1...n] (n = numberOfTerms)
	unsigned int i;
	for (i = 0; i < numberOfTerms; i++) {
		//setting a, b, c
		fl::scalar a, b, c;

		b = i*midRange + minValue;
		a = b - midRange;
		c = b + midRange;

		/*if (i == 0) {
			a = b;
		}
		else if (i == numberOfTerms - 1) {
			c = b;
		}*/

		//Setting name
		std::string name;
		std::stringstream streamName;
		streamName << baseName << i + 1;
		streamName >> name;
		//Creating Term
		fl::Triangle * actualTerm = new fl::Triangle(name, a, b, c);
		terms.push_back(*actualTerm);
	}

	return terms;
}

std::vector<std::vector<fl::Triangle> > TermsGenerator::generateTermsFromDomains(std::vector<std::pair<fl::scalar, fl::scalar> > domains) {
	std::vector<std::vector<fl::Triangle> > terms;

	unsigned int i;
	for (i = 0; i < domains.size(); i++) {
		std::stringstream streamBaseName;
		if (i < domains.size() - 1) {
			streamBaseName << "A" << i + 1;
		}
		else {
			streamBaseName << "B";
		}
		terms.push_back(generateTermsFromDomain(streamBaseName.str(), domains[i].first, domains[i].second));
	}

	return terms;
}
