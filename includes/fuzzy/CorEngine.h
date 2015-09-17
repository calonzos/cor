#ifndef CorEngine_h
#define CorEngine_h

#include "fl/Headers.h"
#include "includes/fuzzy/RuleBlockGenerator.h"

#include "includes/SearchSpaceHeaders.h"

namespace cor {
	class CorEngine {
	protected:
		fl::Engine *engine;
		fl::RuleBlock *ruleBlock;

		SearchSpace * searchSpace;
	public:
		CorEngine(SearchSpace * searchSpace_, fl::scalar resolutionFactor = 1.00000);
		~CorEngine();

		fl::scalar evaluateMSE();
		fl::scalar evaluateSample(Sample sample);
		void setRuleBlock(std::vector<std::string> solution,std::map<int, std::vector<std::vector<std::string>>> Cr,unsigned int numberOfInputs);

		void setInput(std::vector<fl::scalar>);
		
		
		fl::scalar getOutput() const;
		fl::Engine * getEngine() const;
	};
}

#endif
