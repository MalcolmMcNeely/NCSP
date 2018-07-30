//
// Implementation of Shot.h
//
#include "Shot.h"
#include "Character.h"
#include "Utility.h"

Shot::Shot()
{
   Shot(-1);
}

Shot::Shot(int id) : id_(id), gore_(0), thrill_(0), empathy_(0), threaten_(0), 
                     chase_(0), hide_(0), kill_(0), attack_(0), escape_(0), 
                     location_(eL_SIZE), timeDuration_(0)
{
}

Shot::~Shot() { }

void Shot::AddCharacter(Character& character)
{
   // Check if character is already in shot
   eChar charName = character.GetCharName();
   for (auto& pair : characterIndex_)
   {
      if (pair.first == charName)
      {
         // Character already added to shot so overwrite existing
         characters_[pair.second].Overwrite(&character);
         return;
      }
   }

   // Else add new character to the shot
   int charIndex = characters_.size();
   characterIndex_[charName] = charIndex;
   characters_.push_back(character);
}

std::vector<eChar> Shot::GetCharacterList() const
{
   std::vector<eChar> output;
   for (auto& pair : characterIndex_)
   {
      output.push_back(pair.first);
   }
   return output;
}

Character Shot::GetCharacter(eChar name) const
{
   for (auto& pair : characterIndex_)
   {
      if (pair.first == name)
      {
         return characters_[pair.second];
      }
   }
   // Returns null character if doesn't exist
   return Character::NullCharacter();
}