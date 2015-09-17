#include "includes/datosBD/mongoDB.h"
#include "includes/SearchSpaceHeaders.h"
#include "includes/CorEngineHeaders.h"
#include "includes/AcoHeaders.h"
#include <map>
using namespace cor;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::finalize;

mongoDB::mongoDB() {}

void mongoDB::InsertDocumentProblema(std::string id, std::string nombre_problema,std::string tipo_problema,  int num_instancias,  int num_entradas,  int num_salidas,  int num_ejemplos_clase){


    mongocxx::instance inst{};
    mongocxx::client conn{};

    bsoncxx::builder::stream::document document{};

    auto collection = conn["CorDB"]["problemas"];

    document << "_id" << id
    		 << "nombre" << nombre_problema 
    		 << "tipo_problema" << tipo_problema 
    		 << "data_set" << open_array
                << open_document
                << "num_instancias" << num_instancias
                << "num_entradas" << num_entradas
                << "num_salidas" << num_salidas 
                << "num_ejemplos_clase" << num_ejemplos_clase << close_document << close_array;

    collection.insert_one(document.view());
    auto cursor = collection.find({});

    for (auto&& doc : cursor) {
        std::cout << bsoncxx::to_json(doc) << std::endl;
    }
 
}

void mongoDB::InsertDocumentExperimento(std::string id,int num_corridas ,std::string nombre_algoritmo, std::string id_problema,int num_VL,std::vector<std::pair<fl::scalar, fl::scalar>> domains, std::string nombre_CD,  int num_terminos_linguisticos,CorProblem * corProblem ){


    mongocxx::instance inst{};
    mongocxx::client conn{};
    std::map<std::string, fl::Triangle > A,B;
    SearchSpace *ss=corProblem->getSearchSpace();
    A=ss->getTermTriangleInput();
    B=ss->getTermTriangleOutput();
    std::map<std::string, fl::Triangle >::iterator it= A.begin();
    std::map<std::string, fl::Triangle >::iterator itO= B.begin();
    bsoncxx::builder::stream::document document{};
    
   
    auto collection = conn["CorDB"]["experimentos"];
    
    document << "_id" << id
    		 << "algoritmo" << nombre_algoritmo 
    		 << "problema" << id_problema
             << "num_corridas" << num_corridas
             << "variable_linguistica" << open_array
             << close_array
             << "conjunto_difuso" << open_array
             << close_array;

    collection.insert_one(document.view());

    for (int i = 0; i < num_VL+1; i++)
    		 {
    bsoncxx::builder::stream::document filter, update;
    filter      << "_id" << id;
    update      << "$push" << open_document 
            	<< "variable_linguistica" << open_document
                    << "num_VL" << i+1
                    << "rango_min" << domains[i].first 
                    << "rango_max" << domains[i].second
                    
                    << close_document
                << close_document;
                collection.update_one(filter, update);
            	
			 }

    for (int i = 0; i < num_VL+1; i++)
            { 
               for (int j = 0; j < num_terminos_linguisticos; j++)
                { 
                    bsoncxx::builder::stream::document filter, update;
                    if(i!=num_VL){
    filter              << "_id" << id;
    update              << "$push" << open_document
                        << "conjunto_difuso" << open_document
                                << "num_termino_linguistico" << j+1
                                << "variable_linguistica" << i+1
                                << "tipo_conjunto_difuso" <<  nombre_CD 
                                << "parametros" << it->second.parameters()  << close_document
                        << close_document;
                        it++;
                       collection.update_one(filter, update);

                    }
                    else{
    filter              << "_id" << id;
    update              << "$push" << open_document
                        << "conjunto_difuso" << open_document
                            << "num_termino_linguistico" << j+1
                            << "variable_linguistica" << i+1
                            << "tipo_conjunto_difuso" <<  nombre_CD 
                            << "parametros" << itO->second.parameters()  << close_document 
                        << close_document;
                        itO++;
                        collection.update_one(filter, update);
                    }
                }
            }


    auto cursor = collection.find({});

    for (auto&& doc : cursor) {
        std::cout << bsoncxx::to_json(doc) << std::endl;
    }
   
}


void mongoDB::InsertDocumentCorrida(std::string id, std::string fk_experimento, std::string enfoque_COR, CorProblem * corProblem,unsigned int numberOfInput,std::string heuristic){


    mongocxx::instance inst{};
    mongocxx::client conn{};

    bsoncxx::builder::stream::document document{};

    int nC,nP,nT;
    float cobertura,coberturaT;
    unsigned int sB;
    sB=1;
    cobertura=0;

    std::string consequent,antecedent;
   
    SearchSpace *ss=corProblem->getSearchSpace();

    std::map<std::string, fl::Triangle > B;
    B=ss->getTermTriangleOutput();

    std::map<int, std::vector<std::string>> C = ss->getCandidateConsecuent();
    std::map<int, std::vector<std::vector<std::string>>> Cr = ss->getCandidateRules();

    std::map<int, std::vector<std::string>>::iterator itC;
    std::map<int, std::vector<std::vector<std::string>>>::iterator itCr;

    std::map<int, std::vector<std::vector<fl::scalar>>> Es= ss->getEjemploPositivos();
    std::map<int, std::vector<std::vector<fl::scalar>>>::iterator itEs;

    std::map<std::vector<std::string>, fl::scalar > H = ss->getHeuristicInformation(); 

    std::vector<std::string> auxR,auxC;
    std::vector<std::vector<std::string>> auxCr;
    std::vector<std::vector<fl::scalar>> auxEs;
    std::vector<fl::scalar> auxE;

    char buffer[80];
    struct tm * timeinfo;
    time_t  timev;
    time(&timev);
    timeinfo = localtime (&timev);
    strftime(buffer,80,"%d-%m-%Y %I:%M:%S",timeinfo);
    std::string str(buffer);

    auto collection = conn["CorDB"]["corridas"];
   
    document << "_id" << id
             << "fecha" << str
             << "id_experimento" << fk_experimento 
            /* << "data_set" << open_array
                << open_document
                << "num_instancias" << num_instancias
                << "num_entradas" << num_entradas
                << "num_salidas" << num_salidas 
                << "num_ejemplos_clase" << num_ejemplos_clase << close_document << close_array*/
             << "num_subespacios" << ss->NumeberOfSubspace()
             << "enfoque_COR" << enfoque_COR
             << "heuristic" << heuristic
             << "num_subespacios_positivos" << ss->NumeberOfPositiveSubspace()
             << "cobertura_reglas" << open_array
             << close_array
             << "conjunto_reglas_candidatas" << open_array
             << close_array;

    collection.insert_one(document.view());

    for (itC=C.begin(); itC != C.end(); itC++){
    auxC=itC->second;
    auxEs=Es[itC->first];
    auxCr=Cr[itC->first];
      for (unsigned int m = 0; m < auxC.size(); m++){
      bsoncxx::builder::stream::document filter, update;
      cobertura=0;
          for (unsigned int l = 0; l < auxEs.size(); l++){
            auxE=auxEs[l];
            if (B[auxC[m]].membership(auxE[numberOfInput])!=0){
                cobertura++;
            }
           }
      coberturaT=cobertura/auxEs.size();
      nT=auxEs.size();
      filter          << "_id" << id;
      update          << "$push" << open_document
                      << "cobertura_reglas" << open_document
                            << "num_subespacio" << itC->first
                            << "consecuente" << auxC[m]
                            << "cobertura" << cobertura
                            << "total" << nT
                            << "cobertura_total" << coberturaT
                            << "heuristic_regla" << H[auxCr[m]]  << close_document
                      << close_document;
                  
      collection.update_one(filter, update); 
      }
    }

    for (itCr=Cr.begin(); itCr != Cr.end(); itCr++){
        bsoncxx::builder::stream::document filter, update; 
        auxCr=itCr->second;
        auxR=auxCr[0];
        nC=0;
        for (unsigned int k = 0; k < numberOfInput; k++){
                    if (k==0){
                        antecedent=antecedent+auxR[k];
                    }
                    else{
                        antecedent=antecedent+"-"+auxR[k];
                    }
                }
        for (unsigned int i = 0; i < auxCr.size(); i++){
            
            auxR=auxCr[i];
            
                for (unsigned int j = numberOfInput; j < auxR.size(); j++){
                    if (i==0){
                        consequent=consequent+auxR[j];
                    }
                    else{
                        consequent=consequent+"-"+auxR[j];
                    }
                    nC++;
                }

                if (auxR.size()==1){
                    consequent=consequent+"-"+auxR[0];
                    nC++;
                }

            
          }

          sB=sB*nC;
          nP=Es[itCr->first].size();
          

filter    << "_id" << id;
update    << "$push" << open_document
          << "conjunto_reglas_candidatas" << open_document
                << "num_subespacio" << itCr->first
                << "antecedentes" << antecedent  
                << "consecuentes" << consequent 
                << "num_puntos"   << nP
                << "num_consecuentes" << nC << close_document
          << close_document;
                  
          collection.update_one(filter, update); 
          consequent="";
          antecedent="";


    }

    
    bsoncxx::builder::stream::document filter, update; 
    std::stringstream ssB;
    ssB << sB;
    std::string strB = ssB.str();

    filter    << "_id" << id;
    update    << "$set" << open_document
              << "espacio_busqueda" << strB
             << close_document;
                  
    collection.update_one(filter, update); 
    
   /* auto cursor = collection.find({});

    for (auto&& doc : cursor) {
        std::cout << bsoncxx::to_json(doc) << std::endl;
    }*/
 
}




