#ifndef TermsGenerator_h
#define TermsGenerator_h

#include <vector>
#include "fl/Headers.h"
#include "includes/search_space/Sample.h"
#include "includes/search_space/SampleGenerator.h"

namespace cor {
	class TermsGenerator {
	protected:
		unsigned int numberOfTerms;
	public:
		TermsGenerator(unsigned int numberOfTerms_); /**< Constructor */
		~TermsGenerator(); /**< Destructor*/
		unsigned int getNumberOfTerms() const; /**< Observer*/
		void setNumberOfTerms(unsigned int numberOfTerms_); /**< Modifier*/
		std::vector< std::vector<fl::Triangle> > generateTerms(const char * filename, unsigned int numberOfInputs); /**< Modifier*/
		std::vector<fl::Triangle> generateTermsFromDomain(std::string baseName, fl::scalar minValue, fl::scalar maxValue); /**< */	
		std::vector<std::vector<fl::Triangle> > generateTermsFromDomains(std::vector<std::pair<fl::scalar, fl::scalar> > domains);
	};
}


#endif
