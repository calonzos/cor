#include "includes/SearchSpaceHeaders.h"
#include "includes/CorEngineHeaders.h"
#include "includes/AcoHeaders.h"

using namespace cor;

/**	@class CorEngine
*	Constructors
*/
CorEngine::CorEngine (SearchSpace * searchSpace_, fl::scalar resolutionFactor) {
	searchSpace = searchSpace_;
	engine = new fl::Engine;

	//Setting terms
	std::map<std::string, fl::Triangle > A,B;
	std::map<int, std::vector<std::string>> Ai;
	std::vector<std::string> auxAi;
	std::map<std::string, fl::Triangle >::iterator it;
	A=searchSpace->getTermTriangleInput();
	Ai=searchSpace->getTermInput();
	B=searchSpace->getTermTriangleOutput();

	std::vector<std::vector<fl::Triangle> > terms = searchSpace->getTerms();
	std::vector<std::vector<fl::Triangle> > inputTerms = terms;
	std::vector<fl::Triangle> outputTerms = inputTerms.back();
	inputTerms.pop_back();

	unsigned int numberOfInputs = terms.size() - 1;

	//Setting domains
	std::vector<std::pair<fl::scalar, fl::scalar> > domains = searchSpace->getAllDomains();
	
	std::vector<std::pair<fl::scalar, fl::scalar> > inputBounds = domains;
	std::pair<fl::scalar, fl::scalar> outputBounds = inputBounds.back();
	
	//Setting inputs and output
	for (unsigned int i = 0; i < numberOfInputs; i++){
	fl::InputVariable* input = new fl::InputVariable;
    input->setName("Input"+std::to_string(i));
    input->setRange(inputBounds[i].first, inputBounds[i].second);
   // std::cout << inputBounds[i].first << " " << inputBounds[i].second << " " << std::endl;
    auxAi=Ai[i];
      for (unsigned int j=0; j < auxAi.size(); j++){
    	input->addTerm(new fl::Triangle (auxAi[j],A[auxAi[j]].getA(),A[auxAi[j]].getB(),A[auxAi[j]].getC()));
    	//std::cout << auxAi[j] << " " << A[auxAi[j]].getA() << " " << A[auxAi[j]].getB() << " " << A[auxAi[j]].getC() << std::endl;
      }  
    engine->addInputVariable(input);
	}

	fl::OutputVariable* output = new fl::OutputVariable;
    output->setName("Output");
    output->setRange(outputBounds.first, outputBounds.second);
    output->fuzzyOutput()->setAccumulation(new fl::Maximum);

	fl::scalar range = (ceil((outputBounds.second - outputBounds.first)*resolutionFactor));	
	//if (range < 200) {
	//	range = 7595;
	//}
	std::cout<< range << std::endl;
	std::cout<< (outputBounds.second - outputBounds.first)/range << std::endl;
	output->setDefuzzifier(new fl::Centroid(range));
	output->setDefaultValue(fl::nan);
	output->setLockValidOutput(true); 
	output->setLockOutputRange(true); 
    for (it = B.begin(); it != B.end(); it++){
    output->addTerm(new fl::Triangle(it->first, it->second.getA(),it->second.getB(),it->second.getC()));
	}
    engine->addOutputVariable(output);

	ruleBlock = new fl::RuleBlock;
	engine->addRuleBlock(ruleBlock);
	//engine->configure("", "", "AlgebraicProduct", "AlgebraicSum", "Centroid", outputVariable->range());
}

CorEngine::~CorEngine () {
	delete engine;
}

fl::scalar CorEngine::evaluateMSE() {

	std::vector<Sample> samples = searchSpace->getSamples();
	fl::scalar MSE = 0.0;
	
	unsigned int i;
	for (i = 0; i < samples.size(); i++) {
			MSE = evaluateSample(samples[i]) + MSE;
	}
	return MSE / (2*samples.size());
}

fl::scalar CorEngine::evaluateSample(Sample sample) {
	fl::scalar MSE;
	std::vector<fl::scalar> inputs = sample.getInputs();
	//std::cout << inputs[0] << " " << inputs[1] <<std::endl;
	setInput(inputs);
	fl::scalar output = getOutput();
	MSE = pow(output - sample.getOutputValue(0), 2);
	//std::cout << output << " - " << sample.getOutputValue(0) << " = " << output - sample.getOutputValue(0) <<std::endl;
	//std::cout << MSE << std::endl;
	return MSE;
}

void CorEngine::setRuleBlock(std::vector<std::string> solution,std::map<int, std::vector<std::vector<std::string>>> Cr,unsigned int numberOfInputs) {
	// use only latest rule block
	if (engine->numberOfRuleBlocks() > 0) {
		engine->removeRuleBlock(0);
	}

	RuleBlockGenerator ruleBlockGenerator;
	ruleBlock = ruleBlockGenerator.generateRuleBlock(solution,Cr, engine,numberOfInputs);
	engine->addRuleBlock(ruleBlock);
}



void CorEngine::setInput(std::vector<fl::scalar> inputs) {
	unsigned int i;
	for (i = 0; i < inputs.size(); i++) {
		std::string name = "Input"+std::to_string(i);
		engine->setInputValue(name, inputs[i]);
	}
}



fl::scalar CorEngine::getOutput() const {
	engine->process();
	return engine->getOutputValue("Output");
}


fl::Engine * CorEngine::getEngine() const {
	return engine;
}

