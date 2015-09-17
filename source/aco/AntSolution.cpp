#include "includes/SearchSpaceHeaders.h"
#include "includes/CorEngineHeaders.h"
#include "includes/AcoHeaders.h"
#include <list>

using namespace cor;

AntSolution::AntSolution(unsigned int ant,unsigned int numberOfRules,std::vector<std::string> path,fl::scalar MSE) {

	this->ant=ant;
	this->numberOfRules=numberOfRules;
	this->path=path;
	this->MSE=MSE;
}

AntSolution::AntSolution(){}

unsigned int AntSolution::getAnt() {
	return this->ant;
}

unsigned int AntSolution::getNumberOfRules() {
	return this->numberOfRules;
}

std::vector<std::string> AntSolution::getPath() {
	return this->path;
}

fl::scalar AntSolution::getMSE() {
	return this->MSE;
}

void AntSolution::setPath(std::vector<std::string> path) {
	this->path=path;
}

void AntSolution::setMSE(fl::scalar mse) {
	this->MSE=mse;
}

void AntSolution::setNumberOfRules(unsigned int numberOfRules) {
    this->numberOfRules = numberOfRules;
}



