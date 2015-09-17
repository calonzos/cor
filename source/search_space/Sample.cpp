#include "includes/SearchSpaceHeaders.h"
#include "includes/CorEngineHeaders.h"
#include "includes/AcoHeaders.h"

using namespace cor;

/** @class Sample
*	Constructors
*/
Sample::Sample() {}

Sample::Sample(const Sample &sample) : inputValues(sample.inputValues), outputValues(sample.outputValues) {}

Sample::Sample(std::vector<fl::scalar> inputValues_, std::vector<fl::scalar> outputValues_) : inputValues(inputValues_), outputValues(outputValues_) {} 

/** 
*	Destructor
*/
Sample::~Sample() {}

/** 
*	Modifiers
*/
void Sample::addInputValue(const fl::scalar inputValue) {
	inputValues.push_back(inputValue);
}

void Sample::addOutputValue(const fl::scalar outputValue) {
	outputValues.push_back(outputValue);
}

/** 
*	Observers
*/
unsigned int Sample::size() const {
	return inputValues.size() + outputValues.size();
}

unsigned int Sample::inputSize() const {
	return inputValues.size();
}

unsigned int Sample::outputSize() const {
	return outputValues.size();
}

fl::scalar Sample::getInputValue(unsigned int inputIndex) const {
	return inputValues[inputIndex];
}

fl::scalar Sample::getOutputValue(unsigned int outputIndex) const {
	return outputValues[outputIndex];
}

std::vector<fl::scalar> Sample::getInputs() const {
	return inputValues;
}

std::vector<fl::scalar> Sample::getOutputs() const {
	return outputValues;
}

/**
*	Operators
*/

Sample &Sample::operator =(const Sample &sample) {
	inputValues = sample.inputValues;
	outputValues = sample.outputValues;
	return *this;
}

fl::scalar Sample::operator [](const unsigned int valueIndex) const {
	if (valueIndex < inputSize()) {
		unsigned int inputIndex = valueIndex;
		return inputValues[inputIndex];
	}
	else {
		unsigned int outputIndex = valueIndex - inputSize();
		return outputValues[outputIndex];
	}
}

bool Sample::operator <(const Sample &sample) const {
	return outputValues < sample.outputValues;
}

bool Sample::operator ==(const Sample &sample) const {
	if (inputValues == sample.inputValues && outputValues == sample.outputValues) {
		return 1;
	}
	else {
		return 0;
	}
}
