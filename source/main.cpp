#include "includes/CorHeaders.h"

#include <ctime>

using namespace cor;

/* ------   PROBLEMA  --------   */

bool Nuevo_Problema=false;
std::string id_problema="0";
std::string nombre_problema="Lineas electricas #1";
std::string tipo_problema="Modelado";
int num_instancias = 495;
int num_entradas   = 2;
int num_salidas    = 1;
int num_ejemplos   = 0;

/*---------------------------------------------------*/

/* ------    EXPERIMENTO --------  */

bool Nuevo_Experimento=false;
std::string id_experimento="0";
std::string fk_problema="0";
std::string nombre_algoritmo="BWAS";
unsigned int numberOfTerms = 7;
unsigned int numberOfInputs = 2;
unsigned int numberOfRuns = 1;//6;

/*----------------------------------------------------*/

/* -----------    CORRIDA -----------------------------*/

 std::string id_corrida="0";
 std::string fk_experimento="0";
 std::string enfoque_COR="Precision"; 
 std::string heuristica="Heuristica 2"; 

/*------------------------------------------------------*/

unsigned int numberOfFiles = 5;
unsigned int numberOfIterations = 50;
unsigned int stuckCondition = 5;
unsigned int maxNumberOfNeighbours = /*32;//*/20;
unsigned int lastEBSvalue = 0;
unsigned int iteration = 0;
unsigned int lastReset = 0;
unsigned int count = 0;
fl::scalar resolutionFactor = 0.26333;//0.025;//0.1;//1.000;
std::vector<unsigned int> tour;
fl::scalar currentLength;
fl::scalar lastLength, bestLength;

clock_t initialClock, currentClock, lastClock;

const char * filesPath = "../files/495/";//*/"../files/1056/";//
const char * partitionPath[] = {"first_partition/", "second_partition/", "third_partition/", "fourth_partition/", "fifth_partition/"};
const char * filename[] = /*{"ele2-4-1056-1.dat", "ele2-4-1056-2.dat", "ele2-4-1056-3.dat", "ele2-4-1056-4.dat", "ele2-4-1056-5.dat"};//*/{"ele1-2-495-1.dat", "ele1-2-495-2.dat", "ele1-2-495-3.dat", "ele1-2-495-4.dat", "ele1-2-495-5.dat"};

std::string generateFileName(const char * name = "", const char * prefix1 = "", const char * prefix2 = "", const char * prefix3 = "");



float getTime(clock_t t) {
	return (((float) t)/(CLOCKS_PER_SEC));
}

std::string generateFileName(const char * name, const char * prefix1, const char * prefix2, const char * prefix3) {
	std::stringstream filename;
	filename << prefix1;
	filename << prefix2;
	filename << prefix3;
	filename << name;
	return filename.str();
}



int main(int argv, char * argc[]) {

	fl::fuzzylite::setLogging(false);
	mongoDB *db = new mongoDB();
	//Set domains
	std::vector<std::pair<fl::scalar, fl::scalar>> domains;
	/*domains.push_back(std::make_pair(0.5, 11));// */domains.push_back(std::make_pair(1, 320));
	/*domains.push_back(std::make_pair(0.15, 8.55));// */domains.push_back(std::make_pair(60, 1673.329956));
	/*domains.push_back(std::make_pair(1.64, 142.5));// */domains.push_back(std::make_pair(80, 7675));
	//domains.push_back(std::make_pair(1, 165));//*/
	//domains.push_back(std::make_pair(64.470001, 8546.030273));//*/

	if(Nuevo_Problema){
  		db->InsertDocumentProblema(id_problema,nombre_problema,tipo_problema,num_instancias,num_entradas,num_salidas,num_ejemplos);
	}

	unsigned int i, j, k;
	std::vector<std::string> path;
	AntSolution *AntBest= new AntSolution(0,0,path,9999999999);
	for (i = 0; i < numberOfFiles; i++) {

	    
		id_corrida=std::to_string(i+atoi(id_corrida.c_str()));

		//Set files
		std::string trainingFilename = generateFileName(filename[i], filesPath, partitionPath[i]);
		std::string testFilename = generateFileName(filename[i], filesPath, partitionPath[i], "test-");
		

		std::cout << trainingFilename << std::endl;
		std::cout << trainingFilename.c_str() << std::endl;
	
		for (j = 0; j < numberOfRuns; j++) {

			srand (time(NULL));
			//Set CorProblem
			CorProblem * corProblem = new CorProblem(trainingFilename.c_str(), numberOfInputs, numberOfTerms, domains, resolutionFactor);
			if(Nuevo_Experimento){
				db->InsertDocumentExperimento(id_experimento,numberOfRuns,nombre_algoritmo,fk_problema,numberOfInputs,domains,"Triangular",numberOfTerms,corProblem);
			}

			
		
			std::cout << "Number of File: " << i << "\t\tNumber of Run: " << j << std::endl;
			std::cout << "Number of Input Subspaces: " << corProblem->getSearchSpace()->NumeberOfPositiveSubspace() << std::endl;

			BestWorstACO *bw = new BestWorstACO(corProblem,2,2,0.8);
			fl::scalar auxMSE;
			bool resetMSE=true;
			for (k = 0; k < numberOfIterations; k++) {
			 std::cout << "Iteracion#" << k << std::endl;
             bw->Calculate_P();
			 bw->AntSearchSolution(corProblem->getSearchSpace()->NumeberOfPositiveSubspace(),numberOfInputs);
			 bw->EvaporatePheromone();
			 bw->LocalSearch(bw->getAntBest(),20,corProblem->getSearchSpace()->NumeberOfPositiveSubspace(),numberOfInputs);
			 bw->UpdatePheromone(numberOfInputs);
			 bw->PheromoneMutation(bw->getAntBest(),numberOfInputs,k,numberOfIterations,lastReset);


			 /*if(k==0){
			 	AntBest->setMSE(bw->getAntBest()->getMSE());

			 }*/

			 if (AntBest->getMSE() > bw->getAntBest()->getMSE()){
			   		AntBest=bw->getAntBest();
			 }

			 if (resetMSE){
			 	auxMSE=bw->getAntBest()->getMSE();
			 	resetMSE=false;
			 }


			  if (auxMSE > bw->getAntBest()->getMSE()){
			   		auxMSE=bw->getAntBest()->getMSE();
			   		count=0;
			  }else{
			   	count++;
			  }

			  if (count==stuckCondition){
			 	std::cout << "...Re-initializing Pheromone..." << std::endl;
			  	count=0;
			  	lastReset=k;
			  	resetMSE=true;
			  	bw->setPheromone(corProblem->getSearchSpace()->getPheromone());
			  }


			 //break;
			 std::cout << AntBest->getMSE() << " <-BestGlobal - " << std::endl;
             std::cout << AntBest->getNumberOfRules() << " <-BestGlobal - " << std::endl;
			  
			}

			db->InsertDocumentCorrida(id_corrida,fk_experimento,enfoque_COR,corProblem,numberOfInputs,heuristica);
			
		}
		
		//Close files
		break;

	}
	std::cout << AntBest->getMSE() << " <-BestGlobal - " << std::endl;
    std::cout << AntBest->getNumberOfRules() << " <-BestGlobal - " << std::endl;
	return 0;
}

