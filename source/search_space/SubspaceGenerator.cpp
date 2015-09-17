#include "includes/SearchSpaceHeaders.h"
#include "includes/CorEngineHeaders.h"
#include "includes/AcoHeaders.h"

using namespace cor;

/** @class SubspaceGenerator
*	Constructors
*/

SubspaceGenerator::SubspaceGenerator() {}

SubspaceGenerator::~SubspaceGenerator() {}


std::map<int, std::vector<std::string> >  SubspaceGenerator::generateSubspacesOfTerms(std::map<int, std::vector<std::string>> A,unsigned int Ns,unsigned int numberOfInputs,unsigned int numberOfTerms){

  unsigned int input,HMindice;
  std::map<int, std::vector<std::string> > Gsubspace;
  std::vector<std::string> auxT(numberOfInputs);
  std::string term1;
  input=0;
  HMindice=0;

  NestedSubspaceTerm(A,input,auxT,numberOfTerms,numberOfInputs,Gsubspace,HMindice);
  return Gsubspace;
}

void  SubspaceGenerator::NestedSubspaceTerm(std::map<int, std::vector<std::string>> A, unsigned int &input,std::vector<std::string> &aT ,unsigned int numberOfTerms,unsigned int numberOfInputs,std::map<int, std::vector<std::string> > &Gsubspace,unsigned int &HMindice){ //must match with tableSize initialization
	std::map<int, std::vector<std::string>>::iterator itAux = A.find(input);
	unsigned int i;	
     for (i=0; i<numberOfTerms;i++){    
         aT[input]=itAux->second[i];
         input++;
         if(input == numberOfInputs){
      	 	Gsubspace[HMindice]=aT;
      	 	//std::cout << HMindice << " "<< aT[0] << " - " << aT[1] << " " << std::endl;
      	 	HMindice++;
      	 	input=input-1;
      	 }
      	 else{
      	 NestedSubspaceTerm(A,input,aT,numberOfTerms,numberOfInputs,Gsubspace,HMindice);
      	 input=input-1;
      	}
      	 
      }
      
    

}


