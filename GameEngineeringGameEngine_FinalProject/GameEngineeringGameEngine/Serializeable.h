#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include "StandardIncludes.h"

class Serializable {
public :
	Serializable(){}
	virtual ~Serializable(){}

	virtual void Serialize(std::ostream& _stream) = 0;
	virtual void DeSerialize(std::istream& _stream)= 0;

};




#endif // !SERIALIZABLE_H
