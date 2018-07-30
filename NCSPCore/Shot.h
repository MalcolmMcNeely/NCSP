//
// Class which represents a single shot in narrative
//
#pragma once

#include "common.h"
#include "Character.h"

#include <vector>
#include <map>

class Shot
{
public:

   Shot();
   Shot(int id);
   ~Shot();

   void SetID(int id) { id_ = id; }
   void SetStructure(int s) { structure_ = s; }
   void SetGore(int gore) { gore_ = gore; }
   void SetThrill(int thrill) { thrill_ = thrill; }
   void SetEmpathy(int empathy) { empathy_ = empathy; }
   void SetThreaten(int threaten) { threaten_ = threaten; }
   void SetChase(int chase) { chase_ = chase; }
   void SetHide(int hide) { hide_ = hide; }
   void SetKill(int kill) { kill_ = kill; }
   void SetAttack(int attack) { attack_ = attack; }
   void SetEscape(int escape) { escape_ = escape; }
   void SetLocation1(eLocation loc) { location_ = loc; }
   void SetTimeDuration(int timeDuration) { timeDuration_ = timeDuration; }
	void SetFileLocation(std::string file) { fileLocation_ = file; }

   int GetId() const { return id_; }
   int GetStructure() const{ return structure_; }
   int GetGore() const { return gore_; }
   int GetThrill() const { return thrill_; }
   int GetEmpathy() const { return empathy_; }
   int GetThreaten() const { return threaten_; }
   int GetChase() const { return chase_; }
   int GetHide() const { return hide_; }
   int GetKill() const { return kill_; }
   int GetAttack() const { return attack_; }
   int GetEscape() const { return escape_; }   
   eLocation GetLocation() const { return location_; }
   int GetTimeDuration() const { return timeDuration_; }
	std::string GetFileLocation() const { return fileLocation_; }
   
   // Will overwrite characters already in shot
   void AddCharacter(Character& character);
   bool HasCharacters() const { return !characters_.empty(); }
   std::vector<eChar> GetCharacterList() const;
   // Returns null character if doesn't exist
   Character GetCharacter(eChar name) const;
   
   std::vector<Character> GetCharacters() const { return characters_; }

   // Number of members not including characters
   // ID, structure, gore, thrill, empathy, threaten, chase, hide, kill
   // attack, escape, location1, duration
   static int NumBasicMembers() { return 13; }

private:

   int id_;
   int structure_;
   int gore_;
   int thrill_;
   int empathy_;
   int threaten_;
   int chase_;
   int hide_;
   int kill_;
   int attack_;
   int escape_;
   eLocation location_;
	int timeDuration_;

   std::map<eChar, int> characterIndex_;
   std::vector<Character> characters_;
	std::string fileLocation_;
};

