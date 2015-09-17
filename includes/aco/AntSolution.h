#ifndef AntSolution_h
#define AntSolution_h



#include "includes/SearchSpaceHeaders.h"
#include "includes/CorEngineHeaders.h"

#include "includes/aco/CorProblem.h"


namespace cor {
class AntSolution{
	private:
		unsigned int ant,numberOfRules;
		std::vector<std::string> path;
		fl::scalar MSE;
 
	public:
	AntSolution(unsigned int ant,unsigned int numberOfRules,std::vector<std::string> path,fl::scalar MSE);

	AntSolution();

	unsigned int getAnt();
	unsigned int getNumberOfRules();
	std::vector<std::string> getPath();
	fl::scalar getMSE();
	void setPath(std::vector<std::string> path);
	void setMSE(fl::scalar mse);
	void setNumberOfRules(unsigned int numberOfRules);
 };
}

#endif
