#include "includes/SearchSpaceHeaders.h"
#include "includes/CorEngineHeaders.h"
#include "includes/AcoHeaders.h"
#include <list>

using namespace cor;

BestWorstACO::BestWorstACO(CorProblem * cP,unsigned int alfa, unsigned int beta, float ro) {
 
  corProblem=cP;
  SearchSpace *ss=corProblem->getSearchSpace();
  H=ss->getHeuristicInformation();
  Tao=ss->getPheromone();
  Cr=ss->getCandidateRules();
  this->alfa=alfa;
  this->beta=beta;
  this->ro=ro;
}


void BestWorstACO::Calculate_P() {

std::map<int, std::vector<std::vector<std::string>>>::iterator itCr;
std::vector<std::vector<std::string>> auxCr;
std::vector<std::string> auxR;
fl::scalar suma,auxP;
unsigned int i;
for (itCr = Cr.begin(); itCr != Cr.end(); itCr++){
  auxCr=itCr->second;
  suma=0;
    for (i = 0; i < auxCr.size(); i++) {
    auxR=auxCr[i];
    if (auxR.size()==1){
      auxR.push_back(std::to_string(itCr->first));
    }
    suma=suma + pow(Tao[auxR],alfa) + pow(H[auxR],beta);
    }
    for (i = 0; i < auxCr.size(); i++) {
    auxR=auxCr[i];
    if (auxR.size()==1){
      auxR.push_back(std::to_string(itCr->first));
    }
    auxP=(pow(Tao[auxR],alfa) + pow(H[auxR],beta))/suma;
    
    P[auxR]=auxP;
    }

}
  
}

void BestWorstACO::AntSearchSolution(unsigned int numberOfAnts,unsigned int numberOfInputs){

std::map<int, std::vector<std::vector<std::string>>>::iterator itCr;
std::map<int, std::vector<std::string>>::iterator itAs;
std::map<int, std::vector<std::string>> AntSolutions;
std::vector<std::vector<std::string>> auxCr;
std::vector<std::string> auxR,solution;
std::vector<AntSolution> AntPathMse;
typedef std::pair<fl::scalar,std::string> pB;
std::list<pB> test_list;
std::vector<fl::scalar> auxT;
std::string Bk;
float r,a,b;
unsigned int numberOfRules;


  for (unsigned int i = 0; i < numberOfAnts; i++){

    for (itCr = Cr.begin(); itCr != Cr.end(); itCr++){
      auxCr=itCr->second;
      test_list.clear();
      
      r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX); 
      //std::cout << r << std::endl;
      for (unsigned int j = 0; j < auxCr.size(); j++) {
       auxR=auxCr[j];
       if (auxR.size()==1){
       auxR.push_back(std::to_string(itCr->first));
       test_list.push_back(std::make_pair(P[auxR],auxR[0])); 
       //std::cout << auxR[0] << " = " << P[auxR] << " - ";     
       }
       else{
        test_list.push_back(std::make_pair(P[auxR],auxR[numberOfInputs]));
      //  std::cout << auxR[numberOfInputs] << " = " << P[auxR] << " - ";          
       }
      }
     // std::cout << " " << std::endl;
      test_list.sort([](const pB & a, const pB & b) { return a.first > b.first; });

      a=0;
      b=0;
      for (std::list<pB>::iterator it=test_list.begin(); it != test_list.end(); ++it){
        b=a+it->first;
        //std::cout << r << " " << a << " "<< b << std::endl;
        if(r>a && r<=b){
          //std::cout << r << " " << a << " "<< b << " "<< it->second << std::endl;
          Bk=it->second;
        }
        a=a+it->first;
      }
     // std::cout << Bk << std::endl;
      solution.push_back(Bk);
    }

    /*for (unsigned int j = 0; j < solution.size(); j++){
    std::cout << solution[j] << " - ";
    }
    std::cout << solution.size() << std::endl;*/
    AntSolutions[i]=solution;
    solution.clear();
    
  }


   CorEngine *corEngine = corProblem->getCorEngine();

   for (itAs = AntSolutions.begin(); itAs != AntSolutions.end(); itAs++){

    solution=itAs->second;
    numberOfRules=0;
    for (unsigned int l = 0; l < solution.size(); l++){
      if (solution[l]!="dc"){
        numberOfRules++;
      }
    }
   corEngine->setRuleBlock(solution,Cr,numberOfInputs);
   fl::scalar MSE = corEngine->evaluateMSE();
   //std::cout << itAs->first << " - " << numberOfRules << " - " << MSE << std::endl;
   AntSolution *as= new AntSolution(itAs->first,numberOfRules,solution,MSE);
   AntPathMse.push_back(*as);

   if (itAs->first==0){
    AntBest=as;
    AntWorst=as;
    }else{

      if (AntBest->getMSE() > as->getMSE()){
        AntBest=as;
      }

      if (AntWorst->getMSE() < as->getMSE()){
        AntWorst=as;
      }
    }

   }
	std::cout << AntBest->getMSE() << " <-Best - " << AntWorst->getMSE() << " <-Worst - " << std::endl;
  std::cout << AntBest->getNumberOfRules() << " <-Best - " << AntWorst->getNumberOfRules() << " <-Worst - " << std::endl;

}


void BestWorstACO::EvaporatePheromone() {

  std::map<std::vector<std::string>, fl::scalar >::iterator itT;

  for (itT = Tao.begin(); itT != Tao.end(); itT++){
    
    itT->second = itT->second * (1-this->ro);
  }

  /*for (itT = Tao.begin(); itT != Tao.end(); itT++){
    
    std::cout << itT->second << " - ";
    break;
  }
  std::cout << std::endl;*/

  //SearchSpace *ss=corProblem->getSearchSpace();
  //ss->setPheromone(Tao);

}


void BestWorstACO::UpdatePheromone(unsigned int numberOfInputs) {

  std::map<int, std::vector<std::vector<std::string>>>::iterator itCr;
  std::vector<std::vector<std::string>> auxCr;
  std::vector<std::string> auxR,solutionB,solutionW,auxA;
  std::string auxB;
  fl::scalar auxTao;
  solutionB=AntBest->getPath();
  solutionW=AntWorst->getPath();
  unsigned int s,i;
  s=0;
  for (itCr = Cr.begin(); itCr != Cr.end(); itCr++){
    auxCr=itCr->second;
    auxR=auxCr[0];
    auxA.clear();
  //  std::cout << solutionB[s] << " - " << solutionW[s] << std::endl;
    auxB=solutionB[s];
    if(solutionB[s] != "dc"){
      for (i = 0; i < numberOfInputs; i++){
        auxA.push_back(auxR[i]);
      }
      auxA.push_back(auxB);
    }else{
        auxA.push_back(auxB);
        auxA.push_back(std::to_string(itCr->first));
    }
    auxTao=Tao[auxA];
    
    auxTao=auxTao+(1/AntBest->getMSE());
    
    Tao[auxA]=auxTao;
    if (solutionB[s] != solutionW[s]){
      auxA.clear();
      auxB=solutionW[s];
      if(solutionW[s] != "dc"){
        for (i = 0; i < numberOfInputs; i++){
          auxA.push_back(auxR[i]);
        }
        auxA.push_back(auxB);
      }else{
          auxA.push_back(auxB);
          auxA.push_back(std::to_string(itCr->first));
      }
      auxTao=Tao[auxA];
     // std::cout << auxTao << " - ";
      auxTao=auxTao*(1-this->ro);
      //std::cout << auxTao << std::endl;
      Tao[auxA]=auxTao;
    }

   s++;
  }

  //SearchSpace *ss=corProblem->getSearchSpace();
 // ss->setPheromone(Tao);
 /* std::map<std::vector<std::string>, fl::scalar >::iterator itT;
  for (itT = Tao.begin(); itT != Tao.end(); itT++){
    
    std::cout << itT->second << " - ";
  }
  std::cout << std::endl;*/

}

void BestWorstACO::LocalSearch(AntSolution* Tbest, unsigned int Lsi, unsigned int Lsn, unsigned int numberOfInputs)  {
    
std::vector<std::string> Tcur,Tq,T;
bool seguir=true;
fl::scalar MSEt,MSEtq,MSEtc;
CorEngine *corEngine = corProblem->getCorEngine();


Tcur=Tbest->getPath();

corEngine->setRuleBlock(Tcur,Cr,numberOfInputs);
MSEtc = corEngine->evaluateMSE();

for (unsigned int h = 0; h < Lsi && seguir; h++){
  for (unsigned int q = 0; q < Lsn; q++){
    Tq=this->GenerateNeighbour(Tcur,Lsn,numberOfInputs);
    corEngine->setRuleBlock(Tq,Cr,numberOfInputs);
    MSEtq=corEngine->evaluateMSE();
    if (q==0){
      T=Tq;
      MSEt=MSEtq;
    }else{
      if (MSEtq < MSEt){
         T=Tq;
         MSEt=MSEtq;
      }
    }
  }

  if (MSEt < MSEtc){
    Tcur=T;
   // std::cout << h << " " << "MSEt " << MSEt << " MSEtc " << MSEtc << std::endl; 
    MSEtc=MSEt;
  }else{
    seguir=false;
  }
  
}

 unsigned int numberOfRules=0;
 for (unsigned int l = 0; l < Tcur.size(); l++){
      if (Tcur[l]!="dc"){
        numberOfRules++;
      }
  }

 AntBest->setPath(Tcur);
 AntBest->setMSE(MSEtc);
 AntBest->setNumberOfRules(numberOfRules);

}


std::vector<std::string> BestWorstACO::GenerateNeighbour(std::vector<std::string> Tcur,unsigned int numberOfSubspace, unsigned int numberOfInputs){
  
  unsigned int s,contS,Sprima;
  float j,pS,aux,a,b; 
  std::vector<std::string> Tprima,Cs,auxR;
  std::string bk,Bprima;
  typedef std::pair< float,unsigned int> pB;
  std::list<pB> listRandS;
  std::map<int, std::vector<std::vector<std::string>>>::iterator itCr;
  std::vector<std::vector<std::string>> auxCr;
  pS=0;
  Cs.clear();
  listRandS.clear();
  contS=0;
  aux=numberOfSubspace;
  j= static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
  for (unsigned int i = 0; i < numberOfSubspace; i++){
    pS=pS+1/aux;
    listRandS.push_back(std::make_pair(pS,i));
   // std::cout << pS << std::endl;
  }

  a=0;
  b=0;
  for (std::list<pB>::iterator it=listRandS.begin(); it != listRandS.end(); ++it){
    b=a+1/aux;
      //  std::cout << j << " " << a << " "<< b << std::endl;
    if(j>a && j<=b){
     //     std::cout << j << " " << a << " "<< b << " "<< it->second << std::endl;
       s=it->second;
    }
    a=a+1/aux;
  }
  //std::cout << s << std::endl;
  bk=Tcur[s];
  //std::cout << bk << std::endl;

  for (itCr = Cr.begin(); itCr != Cr.end(); itCr++){
    auxCr=itCr->second;
    if (contS == s){
      for (unsigned int k = 0; k < auxCr.size(); k++) {
        auxR=auxCr[k];
        if (auxR.size()==1){
          if(bk!=auxR[0]) 
           Cs.push_back(auxR[0]);
        }else{
          if(bk!=auxR[numberOfInputs])
           Cs.push_back(auxR[numberOfInputs]);
          }
      } 
    }
    contS++;
  }

  if (Cs.size()!=1){
    aux=Cs.size();
    listRandS.clear();
    pS=0;
    j= static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    for (unsigned int i = 0; i < Cs.size(); i++){
    pS=pS+1/aux;
    listRandS.push_back(std::make_pair(pS,i));
    //std::cout << pS << std::endl;
    }
    a=0;
    b=0;
    for (std::list<pB>::iterator it=listRandS.begin(); it != listRandS.end(); ++it){
     b=a+1/aux;
       //  std::cout << j << " " << a << " "<< b << std::endl;
       if(j>a && j<=b){
      //     std::cout << j << " " << a << " "<< b << " "<< it->second << std::endl;
        Sprima=it->second;
       }
     a=a+1/aux;
    }
    //std::cout << Sprima << std::endl;
    Bprima=Cs[Sprima];
    //std::cout << Bprima << std::endl;
  }else{
    Bprima=Cs[0];
  }

  Tprima=Tcur;
  Tprima[s]=Bprima;

  //for (unsigned int m = 0; m < numberOfSubspace; m++){
    //std::cout << m << " - " << Tcur[m] << " - " << Tprima[m] << std::endl;
  //}

  return Tprima;
}


AntSolution * BestWorstACO::getAntBest()  {
  return AntBest;
}

void BestWorstACO::setPheromone(std::map<std::vector<std::string>, fl::scalar > tao)  {
  Tao=tao;
}


void BestWorstACO::PheromoneMutation(AntSolution* Tbest, unsigned int numberOfInputs,float it, float Nit, float itr){

std::map<int, std::vector<std::vector<std::string>>>::iterator itCr;
std::vector<std::vector<std::string>> auxCr;
std::vector<std::string> auxR,auxA,pB;
std::string auxB;
fl::scalar auxTao,Taoth,mutTao;
float Pm,a,b,mut;
float sigma=4;
pB=Tbest->getPath();
unsigned int s,contMayor,contMenor,i;
auxTao=0;
s=0;
Pm=0.3;
contMenor=0;
contMayor=0;
for (itCr = Cr.begin(); itCr != Cr.end(); itCr++){
    auxCr=itCr->second;
    auxR=auxCr[0];
    auxA.clear();
    auxB=pB[s];
    if(auxB != "dc"){
      for (i = 0; i < numberOfInputs; i++){
        auxA.push_back(auxR[i]);
      }
      auxA.push_back(auxB);
    }else{
        auxA.push_back(auxB);
        auxA.push_back(std::to_string(itCr->first));
    }

    auxTao=auxTao + Tao[auxA];
    s++;
  }

Taoth=auxTao/Cr.size();
//std::cout<< "Taoth: " << Taoth << std::endl;

for (itCr = Cr.begin(); itCr != Cr.end(); itCr++){
    auxCr=itCr->second;
    a=static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    //std::cout<< "a: " << a << std::endl;
      if(a<=Pm){
        for (unsigned int l=0; l < auxCr.size();l++){
        auxR=auxCr[l];
        auxA.clear();
        auxB=auxR[numberOfInputs];
         if(auxR.size() !=  1){
           for (i = 0; i < numberOfInputs; i++){
             auxA.push_back(auxR[i]);
            }
          auxB=auxR[numberOfInputs];
          auxA.push_back(auxB);
         }else{
          auxB=auxR[0];
          auxA.push_back(auxB);
          auxA.push_back(std::to_string(itCr->first));
         }

        auxTao=Tao[auxA];
        b=static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        mut=((it-itr)/(Nit-itr))*sigma*Taoth;

        if (b>0.5){
          mutTao=auxTao-mut;
          //std::cout<< auxB << " " << auxTao << " mut: " << mut << " mutTao-: " << mutTao << std::endl;
        }else{
          mutTao=auxTao+mut;
          //std::cout<< auxB << " " << auxTao << " mut: " << mut << " mutTao+: " << mutTao << std::endl;
        }
        
        if (mutTao>1){
          contMayor++;
        }
        if (mutTao<0){
          contMenor++;
        }
       // std::cout<< "contMayor " << contMayor << " contMenor " << contMenor << std::endl;
        if (mutTao>=0 && mutTao<=1){
          Tao[auxA]=mutTao;
        }
      }
    
    }


 }

    


}
