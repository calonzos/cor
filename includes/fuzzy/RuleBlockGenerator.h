#ifndef RuleBlockGenerator_h
#define RuleBlockGenerator_h

#include "fl/Headers.h"
#include "includes/SearchSpaceHeaders.h"



namespace cor {
	class RuleBlockGenerator {
	private:

	public:
		RuleBlockGenerator();
		~RuleBlockGenerator();

		std::string generateRule(std::vector<std::string> antecedents,std::string consequent,unsigned int numberOfInputs);

		fl::RuleBlock * generateRuleBlock(std::vector<std::string> solution,std::map<int, std::vector<std::vector<std::string>>> Cr, fl::Engine *engine, unsigned int numberOfInputs);

	};
}

#endif
