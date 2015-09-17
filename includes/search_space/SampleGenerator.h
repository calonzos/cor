#ifndef SampleGenerator_h
#define SampleGenerator_h

#include <iostream>
#include <utility>
#include <vector>
#include "includes/search_space/Sample.h"

namespace cor {
	class SampleGenerator {
	protected:
		const char * filename; /**< Filename */
		unsigned int numberOfInputs; /**< Number of inputs */
	public:
		SampleGenerator(const char * filename_, unsigned int numberOfInputs_); /**< Initialized constructor */
		~SampleGenerator(); /**< Destroyer*/
		const char * getFilename() const; /** < */
		unsigned int getNumberOfInputs() const; /**< */
		std::vector<std::pair<fl::scalar, fl::scalar>> getSamplesDomain(std::vector<Sample> samples) const; /**<*/
		void setNumberOfInputs(unsigned int numberOfInputs_); /**< */
		void setFilename(const char * filename); /**< */
		std::vector<Sample> generateSamples(); /**< */
	};
}


#endif
