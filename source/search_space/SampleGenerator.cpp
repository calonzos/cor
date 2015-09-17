#include "includes/SearchSpaceHeaders.h"
#include "includes/CorEngineHeaders.h"
#include "includes/AcoHeaders.h"

using namespace cor;

/** @class SampleGenerator
*	Constructors
*/

SampleGenerator::SampleGenerator(const char * filename_, unsigned int numberOfInputs_) : filename(filename_), numberOfInputs(numberOfInputs_) {}

SampleGenerator::~SampleGenerator() {}

/**
*	Observers
*/

const char * SampleGenerator::getFilename() const {
	return filename;
}

unsigned int SampleGenerator::getNumberOfInputs() const {
	return numberOfInputs;
}

std::vector<std::pair<fl::scalar, fl::scalar>> SampleGenerator::getSamplesDomain(std::vector<Sample> samples) const {
	//Get domain for each input and output
	unsigned int i, j;
	std::vector<std::pair<fl::scalar, fl::scalar>> domain;
	for (i = 0; i < numberOfInputs + 1; i++) {
		std::vector<fl::scalar> values; /**< Auxiliar vector of values*/
		for (j = 0; j < samples.size(); j++)
		{
			values.push_back(samples[j][i]);
		}
		fl::scalar maxValue, minValue; /**< MaxValue and MinValue of vector values */
		maxValue = *std::max_element(values.begin(), values.end());
		minValue = *std::min_element(values.begin(), values.end());
		domain.push_back({minValue, maxValue});
	}
	return domain;
}

/**
*	Modifiers
*/

void SampleGenerator::setNumberOfInputs(unsigned int numberOfInputs_) {
	numberOfInputs = numberOfInputs_;
}

void SampleGenerator::setFilename(const char * filename_) {
	filename = filename_;
}

/**
*	Fill from casillas 
*/

std::vector<Sample> SampleGenerator::generateSamples() {

	// Opening file
	std::ifstream file(filename, std::ios::in | std::ios::binary);

	// Line of file
	std::string line;

	// vector of samples
	std::vector<Sample> samples;

	// Reading lines
	while (std::getline(file, line)) {

		boost::trim(line);
		boost::trim_if(line, boost::is_any_of("\t"));

		// char separator
		boost::char_separator<char> char_separator(" ");
		// boost token
		boost::tokenizer< boost::char_separator<char> > tok(line, char_separator);
		// token iterator
		boost::tokenizer< boost::char_separator<char> >::iterator tok_it;

		// auxiliar inputs and output
		std::vector<fl::scalar> inputs;
		std::vector<fl::scalar> output;

		// STATE MACHINE
		#define READING_INPUTS  1
		#define READING_OUTPUTS 2
		#define DONE_READING    3

		int state = READING_INPUTS;

		// iterating through tokens
		for (tok_it = tok.begin(); tok_it != tok.end(); tok_it++)
		{
			// format into T
			std::stringstream item;
			item << *tok_it;

			// state machine
			if (state == READING_INPUTS)
			{
				fl::scalar input;
				item >> input;
				inputs.push_back(input);
			}
			else if (state == READING_OUTPUTS)
			{
				fl::scalar output_;
				item >> output_;
				output.push_back(output_);
			}

			// state change function
			if (state == READING_INPUTS)
			{
				if (inputs.size() == numberOfInputs) // input
				{
					state = READING_OUTPUTS;
				}
			}
			else if (state == READING_OUTPUTS)
			{
				state = DONE_READING;
			}
		}

		// validate sample
		if (state == DONE_READING)
		{
			// create auxiliar sample
			Sample sample(inputs, output);
			// push new sample into container
			samples.push_back(sample);
		}
	}
	// close file
	file.close();

	//return
	return samples;
}
