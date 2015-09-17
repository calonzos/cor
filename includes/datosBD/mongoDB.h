#ifndef mongoDB_h
#define mongoDB_h

#include <iostream>
#include <vector>
#include "fl/Headers.h"

#include "includes/aco/CorProblem.h"

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>



namespace cor {
class mongoDB{
	
	
	
		public:
		 mongoDB();
                 void InsertDocumentProblema(std::string id, std::string nombre_problema,std::string tipo_problema,  int num_instancias,  int num_entradas,  int num_salidas,  int num_ejemplos_clase);


		 void InsertDocumentExperimento(std::string id,int num_corridas ,std::string nombre_algoritmo, std::string id_problema,int num_VL,std::vector<std::pair<fl::scalar, fl::scalar>> domains, std::string nombre_CD,  int num_terminos_linguisticos,CorProblem * corProblem );

		void InsertDocumentCorrida(std::string id, std::string fk_experimento, std::string enfoque_COR, CorProblem * corProblem);


		
		void InsertDocumentCorrida(std::string id, std::string fk_experimento, std::string enfoque_COR, CorProblem * corProblem,unsigned int numberOfInput,std::string heuristic);
	};
}
#endif
