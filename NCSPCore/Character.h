//
// Class containing information about a character in a single shot
//
#pragma once

#include "common.h"
#include <vector>
#include <map>

class Character
{
public:

   Character(eChar name);
   ~Character();

   void SetAlive(bool b) { alive_ = b; }
   eChar GetCharName() const  { return charName_; }
   bool GetAlive() const { return alive_; }

   // Overwrites with information from another Character object instance
	// Can only overwrite characters with the same name otherwise fail assert
	// Will not work on objects returned from NullCharacter()
   void Overwrite(Character* pCharacter);

	// Creates 'NULL' character instance which should only be used for error
	// checking purposes as this will not work with the Overwrite function
   static Character NullCharacter() { return Character(eC_NONE); }
	static bool IsNullCharacter(Character* pCharacter) { return pCharacter->charName_ == eC_NONE; }
   static int NumMembers() { return 2; }

private:

   eChar charName_;
   bool alive_;
};