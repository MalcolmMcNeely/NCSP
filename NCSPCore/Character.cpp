//
// Implementation of Character.h
//
#include "Character.h"
#include "Utility.h"

#include <cassert>

Character::Character(eChar name) : charName_(name), alive_(true) {}
Character::~Character() {}

void Character::Overwrite(Character* pCharacter)
{   
	if (Character::IsNullCharacter(this))
	{
		throw std::invalid_argument("Can not overwrite a Null character object instance");
	}
	else if (Character::IsNullCharacter(pCharacter))
	{
		throw std::invalid_argument("Can not overwrite with a Null character object instance");
	}

	// Can only overwrite characters with the same name
	assert(this->charName_ == pCharacter->charName_);
   alive_ = pCharacter->alive_;
}