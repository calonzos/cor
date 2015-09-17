#ifndef Sample_h
#define Sample_h

#include <vector>
#include "fl/Headers.h"

namespace cor {
	class Sample{

	protected:
		std::vector<fl::scalar> inputValues;
		std::vector<fl::scalar> outputValues;
	public:
		Sample();
		Sample(const Sample &sample);
		Sample(std::vector<fl::scalar> inputValues, std::vector<fl::scalar> outputValues);
		~Sample();
		//Modifiers
		void addInputValue(const fl::scalar value);
		void addOutputValue(const fl::scalar value);
		//Observers
		unsigned int size() const;
		unsigned int inputSize() const;
		unsigned int outputSize() const;
		fl::scalar getInputValue(unsigned int inputIndex) const;
		fl::scalar getOutputValue(unsigned int outputIndex) const;
		std::vector<fl::scalar> getInputs() const;
		std::vector<fl::scalar> getOutputs() const;
		//Operators
		Sample &operator =(const Sample &sample);
		fl::scalar operator [](const unsigned int valueIndex) const;
		bool operator <(const Sample &sample) const;
		bool operator ==(const Sample &sample) const;
	};
}

#endif
