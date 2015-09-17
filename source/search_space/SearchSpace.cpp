#include "includes/SearchSpaceHeaders.h"
#include "includes/CorEngineHeaders.h"
#include "includes/AcoHeaders.h"
#include <map>
#include <cassert>
#include <algorithm>
using namespace cor;

/**	@class SearchSpace
*	Constructors
*/
SearchSpace::SearchSpace(const char * filename, unsigned int numberOfInputs, unsigned int numberOfTerms, std::vector<std::pair<fl::scalar, fl::scalar> > domains, unsigned int option) {
	
    std::stringstream streamBaseName;
	std::stringstream sbn;
	std::string name,name2;
    std::vector<fl::Triangle> auxT;
    std::map<int, std::vector<fl::scalar> > E;
	std::vector<fl::scalar> auxI,auxO,auxE;
	std::vector<std::string> auxTI(numberOfTerms);
	unsigned int i, j,n,numberOfOutputs,Ns;
	n=1;
	Ns=1;
	for (i = 0; i < numberOfInputs; i++) {
		Ns=Ns*numberOfTerms;
	}

	//Generating samples
	SampleGenerator * sampleGenerator = new SampleGenerator(filename, numberOfInputs);
	samples = sampleGenerator->generateSamples();

	
	numberOfOutputs=samples[i].getOutputs().size();
	//std::cout << samples.size() << std::endl;
	for (i = 0; i < samples.size(); i++) {
		auxI=samples[i].getInputs();
		auxO=samples[i].getOutputs();
		for (j = 0; j < auxI.size(); j++) {

		  // std::cout << auxI[j] << " - ";

		}
		auxE=auxI;
		for (j = 0; j < auxO.size(); j++) {

		  // std::cout << auxO[j] << " ";
		   auxE.push_back(auxO[j]);
		}

		E[n] = auxE;
		n++;
	//	std::cout << " " << std::endl;
	}

	//Generating terms
	TermsGenerator * termsGenerator = new TermsGenerator(numberOfTerms);
	terms = termsGenerator->generateTermsFromDomains(domains);

	for (i = 0; i < numberOfInputs; i++) {
		auxT=terms[i];
		streamBaseName << "A" << i + 1;
		name=streamBaseName.str();
		for (j = 0; j < numberOfTerms; j++) {
			sbn << name << j + 1;
			name2=sbn.str();
			auxTI[j]=name2;
			//std::cout << name2 <<  ": " <<auxT[j].parameters()<< " - ";

			A[name2]=auxT[j];
			sbn.str("");

		}
		Ai[i]=auxTI;
		streamBaseName.str("");
		//std::cout << " " << std::endl;
 	}

 	for (i = numberOfInputs; i < numberOfOutputs+numberOfInputs; i++) {
		auxT=terms[i];
		name="B";
		for (j = 0; j < numberOfTerms; j++) {
			sbn << name << j + 1;
			name2=sbn.str();
		//	std::cout << name2 <<  ": " <<auxT[j].parameters()<< " - ";
			B[name2]=auxT[j];
			sbn.str("");

		}
		streamBaseName.str("");
		//std::cout << " " << std::endl;
 	}
   
	//Generating subspaces
	SubspaceGenerator * subspaceGenerator = new SubspaceGenerator();
	Ss=subspaceGenerator->generateSubspacesOfTerms(Ai,Ns,numberOfInputs,numberOfTerms);
	
    //Generating possitiveExamples
	Subspace *sp=new Subspace();
	//Es=sp->generatePossitiveExampleInterpretability(E,A,Ai,Ss);
	Es=sp->generatePossitiveExampleAccuracy(E,A,Ss,numberOfInputs);

	Smas=sp->generateSetWithPossitiveExample(Es,Ns);
	
	//Generating Candidate Consequent
	//C=sp->generateCandidateConsequentInterpretability(Es,B,Ns,numberOfInputs);
	C=sp->generateCandidateConsequentAccuracy(Es,B,Ns,numberOfInputs);

	Cr=this->generateCandidateSetRule(Ss,C);

	H=this->generateHeuristicInformation(numberOfInputs);

	Tao=this->PheromoneInitialization();

	delete sampleGenerator;
	delete termsGenerator;
	delete subspaceGenerator;
}

SearchSpace::~SearchSpace() {}

std::vector<Sample> SearchSpace::getSamples() const {
	return samples;
}

std::map<std::string, fl::Triangle > SearchSpace::getTermTriangleInput()  {
	return A;
}

std::map<int, std::vector<std::string>> SearchSpace::getTermInput()  {
	return Ai;
}

std::map<std::string, fl::Triangle > SearchSpace::getTermTriangleOutput()  {
	return B;
}

std::map<int, std::vector<std::string>> SearchSpace::getCandidateConsecuent()  {
	return C;
}

std::map<int, std::vector<std::vector<std::string>>> SearchSpace::getCandidateRules()  {
	return Cr;
}

std::map<int, std::vector<std::vector<fl::scalar>>> SearchSpace::getEjemploPositivos()  {
	return Es;
}

std::map<std::vector<std::string>, fl::scalar > SearchSpace::getHeuristicInformation()  {
	return H;
}

std::map<std::vector<std::string>, fl::scalar > SearchSpace::getPheromone()  {
	return Tao;
}

void SearchSpace::setPheromone(std::map<std::vector<std::string>, fl::scalar > tao)  {
	Tao=tao;
}

int SearchSpace::NumeberOfSubspace()  {
	return Ss.size();
}

int SearchSpace::NumeberOfPositiveSubspace()  {
	return Smas.size();
}

std::vector<std::vector<fl::Triangle> > SearchSpace::getTerms() const {
	return terms;
}



fl::scalar SearchSpace::getRange(unsigned int index) const {
	std::pair<fl::scalar, fl::scalar> domain = getDomain(index);
	return domain.second - domain.first;
}

std::pair<fl::scalar, fl::scalar> SearchSpace::getDomain(unsigned int index) const {
	fl::scalar minValue = 0, maxValue = 0;

	unsigned int i;
	std::vector<fl::scalar> values;
	for (i = 0; i < samples.size(); i++) {
		values.push_back(samples[i][index]);
	}

	minValue = *std::min_element(values.begin(), values.end());
	maxValue = *std::max_element(values.begin(), values.end());

	return std::make_pair(minValue, maxValue);
}


std::vector<std::pair<fl::scalar, fl::scalar> > SearchSpace::getAllDomains() const {

	unsigned int i, numberOfValues = samples[0].size();
	
	std::vector<std::pair<fl::scalar, fl::scalar> > domains;
     
	for (i = 0; i < numberOfValues; i++) {
		domains.push_back(getDomain(i));
	}

	return domains;
}

std::map<int, std::vector<std::vector<std::string>>> SearchSpace::generateCandidateSetRule(std::map<int, std::vector<std::string>> Ss,std::map<int, std::vector<std::string>> C){
	
	std::map<int, std::vector<std::vector<std::string>>> Cr;
	std::map<int, std::vector<std::string>>::iterator it;
	std::vector<std::string> auxSs,DC;
    std::vector<std::vector<std::string>> auxCr;
    unsigned int i;
    DC.push_back("dc");
	for (it = C.begin(); it != C.end(); it++){
		//std::cout << it->first << " : " << std::endl;
	
		for (i = 0; i < it->second.size(); i++) {
			auxSs.clear();
			auxSs=Ss[it->first];
			auxSs.push_back(it->second[i]);
		//	std::cout << auxSs[0] << " - "<< auxSs[1] << " - "<< auxSs[2] << std::endl;
			auxCr.push_back(auxSs);
	     }
	    // std::cout << DC[0] << std::endl;
	     auxCr.push_back(DC);
	     Cr[it->first]=auxCr;
	     auxCr.clear();
	}

	return Cr;
}

std::map<std::vector<std::string>, fl::scalar > SearchSpace::generateHeuristicInformation(unsigned int numberOfInputs){
	
	std::map<std::vector<std::string>, fl::scalar > H;
	std::map<int, std::vector<std::vector<std::string>>>::iterator it;
	std::vector<std::string> auxR;
    std::vector<std::vector<std::string>> auxCr;
    std::vector<std::vector<fl::scalar>> auxEs;
    std::vector<fl::scalar> auxMin,auxE,auxh;
    unsigned int i,j,k,l;
    fl::scalar miu,auxMiu,h;
	for (it = Cr.begin(); it != Cr.end(); it++){	  
	  auxCr=it->second;
	  auxEs=Es[it->first];
	  auxh.clear();
	  for (i = 0; i < auxCr.size(); i++) {
      	auxR=auxCr[i];
      	auxMin.clear();
      	miu=999999;
       if(auxR.size()!=1){
		for (j = 0; j < auxEs.size(); j++) {
			auxE=auxEs[j];

			for (k = 0; k < auxE.size(); k++){
				if (k<numberOfInputs){
				  auxMiu=A[auxR[k]].membership(auxE[k]);
			    }else{
			      auxMiu=B[auxR[k]].membership(auxE[k]);
			    }

			    if (miu>auxMiu){
			    	miu=auxMiu;
			    }
			    
			}
		//std::cout << auxR[2] << " - "  << j << " - " << miu << std::endl;
		auxMin.push_back(miu);

	    }
	    h=this->Heuristic_2(auxMin);
	    auxh.push_back(h);
	  //  std::cout <<"---- MAX: " << h << " ---------------" << std::endl;
	 //   std::cout << auxR[0] << " - " << auxR[1] << " - " << auxR[2] << " - " << h << std::endl;
	   }else{
	   	h=0;
	   	 for (l = 0; l < auxh.size(); l++)
	   	 {
	   		h=h+auxh[l];
	     }
	    h=h/auxh.size();
	    auxR.push_back(std::to_string(it->first)); 
	    //std::cout <<"---- DC: " << h << " ---------------" << std::endl;
	    //std::cout << auxR[0] << " - " << auxR[1] << " - " << h << std::endl;
	   }

	    H[auxR]=h;
	  }
	
	}

	return H;
}

std::map<std::vector<std::string>, fl::scalar > SearchSpace::PheromoneInitialization(){

  std::map<int, std::vector<std::vector<std::string>>>::iterator it;
  std::map<std::vector<std::string>, fl::scalar >::iterator itT;
  std::vector<std::vector<std::string>> auxCr;
  std::vector<std::string> auxR;
  std::vector<fl::scalar> auxT;
  fl::scalar hM,TaoValor;
  std::vector<fl::scalar>::iterator itMax;
  unsigned int i;
	
  TaoValor=0;
  for (it = Cr.begin(); it != Cr.end(); it++){
  auxCr=it->second;
    for (i = 0; i < auxCr.size(); i++) {
    	if (auxCr[i].size()==1){
    	auxR=auxCr[i];
    	auxR.push_back(std::to_string(it->first));
    	auxT.push_back(H[auxR]);  
    	Tao[auxR]=0;  	
    	}
    	else{
    	auxT.push_back(H[auxCr[i]]);
    	Tao[auxCr[i]]=0;
    	}
    
    }

    itMax=std::max_element(std::begin(auxT), std::end(auxT));
	hM=*itMax;
	auxT.clear();
    TaoValor=TaoValor+hM;
  }	

  TaoValor=TaoValor/Cr.size();
  for (itT = Tao.begin(); itT != Tao.end(); itT++){
  	Tao[itT->first]=TaoValor;
  }

  return Tao;

}

fl::scalar SearchSpace::Heuristic_1(std::vector<fl::scalar> auxMin)  {
	fl::scalar h;
	std::vector<fl::scalar>::iterator it;
	it=std::max_element(std::begin(auxMin), std::end(auxMin));
	h=*it;
	return h;
}

fl::scalar SearchSpace::Heuristic_2(std::vector<fl::scalar> auxMin)  {
	fl::scalar h=0;
	for (unsigned int i = 0; i < auxMin.size(); i++)
	{
		h=h+auxMin[i];
	}
	h=h/auxMin.size();
	return h;
}
