#include "includes/SearchSpaceHeaders.h"
#include "includes/CorEngineHeaders.h"
#include "includes/AcoHeaders.h"

using namespace cor;

/** @class Subspace
*	Constructors
*/

Subspace::Subspace() {}

Subspace::~Subspace() {}



std::map<int, std::vector<std::string>>  Subspace::generateCandidateConsequentInterpretability(std::map<int, std::vector<std::vector<fl::scalar>>> Es,std::map<std::string, fl::Triangle > B,unsigned int Ns,unsigned int numberOfInputs) {

 bool encontrado,alMenosUno;
 fl::Triangle Bk,Bprima;
 unsigned int i,N;
 std::vector<std::vector<fl::scalar>> auxEs;
 std::vector<fl::scalar> auxE;
 std::vector<std::string> auxB;
 std::map<std::string, fl::Triangle >::iterator it,itprima;
 std::map<int, std::vector<std::string> > C;
 for (N = 0; N < Ns; N++){
 	
 	//std::cout << N << " : ";
 	for (it = B.begin(); it != B.end(); it++){
 		Bk=it->second;
 		alMenosUno=false;
 		//std::cout << it->first << " - ";
 		
 		for (i = 0; i < Es[N].size() && !alMenosUno; i++){
 			//std::cout << it->first << " - ";
 			auxEs=Es[N];
 			auxE=auxEs[i];
 			encontrado=true;
 			for (itprima = B.begin(); itprima != B.end(); itprima++){
 				Bprima=itprima->second;
 				//std::cout << itprima->first << " # ";
 				encontrado=encontrado && (Bk.membership(auxE[numberOfInputs]) >= Bprima.membership(auxE[numberOfInputs]));
 			}
			//std::cout << encontrado << " - ";
 			if (encontrado)
 			{
 				std::cout << it->first << " - ";
 				auxB.push_back(it->first);
 				alMenosUno=true;
 			}

 		}
 	}
 	//std::cout  << "  "<< std::endl;
 	if(!auxB.empty()){
 	C[N]=auxB;
 	auxB.clear();
    }
 }

  return C;
}

std::map<int, std::vector<std::string>>  Subspace::generateCandidateConsequentAccuracy(std::map<int, std::vector<std::vector<fl::scalar>>> Es,std::map<std::string, fl::Triangle > B,unsigned int Ns,unsigned int numberOfInputs) {

 bool encontrado,alMenosUno;
 fl::Triangle Bk,Bprima;
 unsigned int i,N;
 std::vector<std::vector<fl::scalar>> auxEs;
 std::vector<fl::scalar> auxE;
 std::vector<std::string> auxB;
 std::map<std::string, fl::Triangle >::iterator it,itprima;
 std::map<int, std::vector<std::string> > C;
 for (N = 0; N < Ns; N++){
 	
 	//std::cout << N << " : ";
 	for (it = B.begin(); it != B.end(); it++){
 		Bk=it->second;
 		alMenosUno=false;
 		//std::cout << it->first << " - ";
 		
 		for (i = 0; i < Es[N].size() && !alMenosUno; i++){
 			
 			auxEs=Es[N];
 			auxE=auxEs[i];
 			encontrado=true;
 			encontrado=encontrado && (Bk.membership(auxE[numberOfInputs]) != 0);
 			//std::cout << /*Bk.membership(*/auxE[numberOfInputs]/*)*/ << " - ";
 			if (encontrado)
 			{
 				//std::cout << it->first << " - ";
 				auxB.push_back(it->first);
 				alMenosUno=true;
 			}

 		}
 	}
 	//std::cout  << "  "<< std::endl;
 	if(!auxB.empty()){
 	C[N]=auxB;
 	auxB.clear();
 	}
 }

  return C;
}

std::map<int, std::vector<std::vector<fl::scalar>>> Subspace::generatePossitiveExampleInterpretability(std::map<int, std::vector<fl::scalar> > E,std::map<std::string, fl::Triangle > A,std::map<int, std::vector<std::string>> Ai,std::map<int, std::vector<std::string> > Ss) {

  std::map<int, std::vector<std::vector<fl::scalar>>> Es;
  bool cumple;
  unsigned int i,j,N,s;
  std::vector<std::string> auxSs;
  std::vector<std::string> auxAi;
  std::vector<fl::scalar> auxE;
  std::vector<std::vector<fl::scalar>> auxC;
  for (N = 0; N < Ss.size(); N++){
  	
	auxSs=Ss[N];
	//std::cout << N << " : ";
       for (i = 1; i <= E.size(); i++){
	     auxE=E[i];
		 cumple=true;

		 for (j = 0; j < Ai.size(); j++){		       
		     auxAi=Ai[j];

		        for (s=0;s<auxAi.size();s++){

		        		
		         cumple=cumple && (A[auxSs[j]].membership(auxE[j]) >= A[auxAi[s]].membership(auxE[j]));
		        	
		    			
		    	     }
	        }
	    	if (cumple)
	    	{
	    		
	    		//std::cout << auxE[0] << " - ";
	    		auxC.push_back(auxE);
	    	}
    	}
    	Es[N]=auxC;
    	auxC.clear();
    	//std::cout << " " << std::endl;
    }	

    return Es;

}

std::map<int, std::vector<std::vector<fl::scalar>>> Subspace::generatePossitiveExampleAccuracy(std::map<int, std::vector<fl::scalar> > E,std::map<std::string, fl::Triangle > A,std::map<int, std::vector<std::string> > Ss,unsigned int numberOfInputs) {

  std::map<int, std::vector<std::vector<fl::scalar>>> Es;
  unsigned int i,j,N;
  float Miu;
  std::vector<std::string> auxSs;
  std::vector<fl::scalar> auxE;
  std::vector<std::vector<fl::scalar>> auxC;

  for (N = 0; N < Ss.size(); N++){
  	
	auxSs=Ss[N];
	//std::cout << N << " : ";
       for (i = 1; i <= E.size(); i++){
	     auxE=E[i];
	     Miu=1.0;

	     for (j = 0; j < numberOfInputs; j++)
	     {
	     	Miu=Miu*A[auxSs[j]].membership(auxE[j]);
	     }
	     if (Miu!=0)
	     {
	     	//std::cout << auxE[0] ;
	     	auxC.push_back(auxE);
	     }
		 
    	}
    	Es[N]=auxC;
    	auxC.clear();
    	//std::cout << " " << std::endl;
    }	

    return Es;

}

std::map<int, std::vector<std::vector<fl::scalar>>> Subspace::generateSetWithPossitiveExample(std::map<int, std::vector<std::vector<fl::scalar>>> Es,unsigned int Ns) {
	std::map<int, std::vector<std::vector<fl::scalar>>> Smas;
	unsigned int N;
	for (N = 0; N < Ns; N++){
		
		if(!Es[N].empty()){

			Smas[N]=Es[N];
		}
	}

	//std::cout << "S+ :" <<Smas.size() << std::endl;

	return Smas;
}

