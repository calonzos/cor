#include "includes/SearchSpaceHeaders.h"
#include "includes/CorEngineHeaders.h"
#include "includes/AcoHeaders.h"

using namespace cor;

/** @class RuleBlockGenerator
*	Constructors
*/

RuleBlockGenerator::RuleBlockGenerator() {}

RuleBlockGenerator::~RuleBlockGenerator() {}

/**
*	RuleBlockGenerator
*/

std::string RuleBlockGenerator::generateRule(std::vector<std::string> antecedents,std::string consequent,unsigned int numberOfInputs) {
	std::stringstream streamRule;

	unsigned int i;
	streamRule << "if ";
	for (i = 0; i < numberOfInputs; i++) {
		streamRule << "Input"+std::to_string(i) << " is " << antecedents[i] << " ";

		if (i != numberOfInputs - 1) {
			streamRule << "and ";
		}
	}
	
	streamRule << "then " << "Output" << " is " << consequent;
	return streamRule.str();
}


fl::RuleBlock * RuleBlockGenerator::generateRuleBlock(std::vector<std::string> solution,std::map<int, std::vector<std::vector<std::string>>> Cr, fl::Engine *engine, unsigned int numberOfInputs) {
	fl::RuleBlock * ruleBlock = new fl::RuleBlock();
	std::map<int, std::vector<std::vector<std::string>>>::iterator itCr;
	std::vector<std::vector<std::string>> auxCr;
	std::vector<std::string> auxR,auxA;
	ruleBlock->setEnabled(true);
	ruleBlock->setName("");
	ruleBlock->setConjunction(new fl::Minimum);
	ruleBlock->setDisjunction(new fl::Maximum);
	ruleBlock->setActivation(new fl::Minimum);

	unsigned int i,j;

	j=0;

	for (itCr = Cr.begin(); itCr != Cr.end(); itCr++){
      auxCr=itCr->second;
      auxR=auxCr[0];
      for (i = 0; i < numberOfInputs; i++){
      	auxA.push_back(auxR[i]);
      }
      if(solution[j] != "dc"){
      std::string rule = generateRule(auxA, solution[j],numberOfInputs);
      //std::cout << rule << std::endl;
	  ruleBlock->addRule(fl::Rule::parse(rule, engine));
	  }
	  auxA.clear();
	  j++;

    }

	return ruleBlock;
}


