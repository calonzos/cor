#ifndef BestWorstACO_h
#define BestWorstACO_h




#include "includes/SearchSpaceHeaders.h"
#include "includes/CorEngineHeaders.h"

#include "includes/aco/CorProblem.h"
#include "includes/aco/AntSolution.h"


namespace cor {
class BestWorstACO{
	private:
		fl::scalar last_best_length;
		unsigned int stuck_condition;
		CorProblem * corProblem;
		std::map<int, std::vector<std::vector<std::string>>> Cr;
		std::map<std::vector<std::string>, fl::scalar > H,Tao,P;
		unsigned int alfa,beta; 
		float ro;
		AntSolution *AntBest,*AntWorst;
	public:
	BestWorstACO(CorProblem * cP,unsigned int alfa, unsigned int beta, float ro);
	void Calculate_P();
	void AntSearchSolution(unsigned int numberOfAnts,unsigned int numberOfInputs);
	void EvaporatePheromone();
	void UpdatePheromone(unsigned int numberOfInputs);
	AntSolution * getAntBest();
	void setPheromone(std::map<std::vector<std::string>, fl::scalar > tao);
	void LocalSearch(AntSolution* Tbest, unsigned int Lsi, unsigned int Lsn, unsigned int numberOfInputs);
	std::vector<std::string> GenerateNeighbour(std::vector<std::string> Tcur,unsigned int numberOfSubspace, unsigned int numberOfInputs);
	void PheromoneMutation(AntSolution* Tbest, unsigned int numberOfInputs,float it, float Nit, float itr);
 };
}

#endif
