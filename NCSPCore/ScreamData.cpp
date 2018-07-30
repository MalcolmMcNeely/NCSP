//
// Implementation of ScreamData.h
//
#include "ScreamData.h"
#include "Shot.h"
#include "Character.h"
#include "tinyxml2.h"

#define SCREAMSHOTS 140
#define SCREAMXMLDATAPATH "Data/ScreamData.xml"

#ifndef XMLCheckResult
#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
#endif

using namespace tinyxml2;
using namespace NarrativeEnums;
using namespace std;

namespace ScreamData
{
   Shot ParseShotFromXMLElement(XMLElement* e);

   int GetData(std::vector<Shot>& data)
   {
      data.clear();

      XMLDocument doc;
      doc.LoadFile(SCREAMXMLDATAPATH);

      if (doc.ErrorID() == XML_NO_ERROR)
      {
         XMLNode* pRoot = doc.FirstChild();
         if (pRoot == nullptr) return XML_ERROR_FILE_READ_ERROR;

         // XML Document begins with "Data"
         assert(((std::string)pRoot->Value()).compare("Data") == 0);
         XMLElement* shotElement = pRoot->FirstChildElement("Shot");

         while (shotElement != nullptr)
         {
				// Parse each shot
            data.push_back(ParseShotFromXMLElement(shotElement));
            shotElement = shotElement->NextSiblingElement();
         }
      }
      else
      {
         return doc.ErrorID();
      }
      
      return 1;
   }

   Shot ParseShotFromXMLElement(XMLElement* e)
   {
		int tempInt;
      eLocation loc;
		XMLError eResult;

      XMLElement* pElement = e->FirstChildElement();
      if (pElement == nullptr) return XML_ERROR_PARSING_ELEMENT;
      assert(((std::string)pElement->Value()).compare("ShotId") == 0);

		eResult = pElement->QueryIntText(&tempInt);
      XMLCheckResult(eResult);
		Shot s;

      pElement = e->FirstChildElement("Structure");
      if (pElement != nullptr)
      {
         eResult = pElement->QueryIntText(&tempInt);
         XMLCheckResult(eResult);
         s.SetStructure(tempInt);
      }

      pElement = e->FirstChildElement("Gore");
      if (pElement != nullptr)
      {
         eResult = pElement->QueryIntText(&tempInt);
         XMLCheckResult(eResult);
         s.SetGore(tempInt);
      }

      pElement = e->FirstChildElement("Thrill");
      if (pElement != nullptr)
      {
         eResult = pElement->QueryIntText(&tempInt);
         XMLCheckResult(eResult);
         s.SetThrill(tempInt);
      }

      pElement = e->FirstChildElement("Empathy");
      if (pElement != nullptr)
      {
         eResult = pElement->QueryIntText(&tempInt);
         XMLCheckResult(eResult);
         s.SetEmpathy(tempInt);
      }

      pElement = e->FirstChildElement("Threaten");
      if (pElement != nullptr)
      {
         eResult = pElement->QueryIntText(&tempInt);
         XMLCheckResult(eResult);
         s.SetThreaten(tempInt);
      }

      pElement = e->FirstChildElement("Chase");
      if (pElement != nullptr)
      {
         eResult = pElement->QueryIntText(&tempInt);
         XMLCheckResult(eResult);
         s.SetChase(tempInt);
      }

      pElement = e->FirstChildElement("Hide");
      if (pElement != nullptr)
      {
         eResult = pElement->QueryIntText(&tempInt);
         XMLCheckResult(eResult);
         s.SetHide(tempInt);
      }

      pElement = e->FirstChildElement("Kill");
      if (pElement != nullptr)
      {
         eResult = pElement->QueryIntText(&tempInt);
         XMLCheckResult(eResult);
         s.SetKill(tempInt);
      }

      pElement = e->FirstChildElement("Attack");
      if (pElement != nullptr)
      {
         eResult = pElement->QueryIntText(&tempInt);
         XMLCheckResult(eResult);
         s.SetAttack(tempInt);
      }

      pElement = e->FirstChildElement("Escape");
      if (pElement != nullptr)
      {
         eResult = pElement->QueryIntText(&tempInt);
         XMLCheckResult(eResult);
         s.SetEscape(tempInt);
      }

      pElement = e->FirstChildElement("Location1");
      if (pElement != nullptr)
      {
         loc = GetLocationFromString(pElement->GetText());
         s.SetLocation1(loc);
      }

      //pElement = e->FirstChildElement("Location2");
      //if (pElement != nullptr)
      //{
      //   loc = GetLocationFromString(pElement->GetText());
      //   s.SetLocation2(loc);
      //}
      //else
      //{
      //   s.SetLocation2(loc);
      //}

      pElement = e->FirstChildElement("TimeIndex");
      eResult = pElement->QueryIntText(&tempInt);
      XMLCheckResult(eResult);
      s.SetID(tempInt);

      pElement = e->FirstChildElement("TimeDuration");
      eResult = pElement->QueryIntText(&tempInt);
      XMLCheckResult(eResult);
      s.SetTimeDuration(tempInt);

      // Add characters if present
      pElement = e->FirstChildElement("Character");
		
      if (pElement != nullptr)
      {
			while (pElement != nullptr)
			{
				XMLElement* pSubElement = pElement->FirstChildElement("CharacterName");
				eChar name = GetCharFromString(pSubElement->GetText());
				assert(name != eC_SIZE);
				Character c(name);

            pSubElement = pElement->FirstChildElement("Alive");
            if (pSubElement != nullptr)
            {
               eResult = pSubElement->QueryIntText(&tempInt);
               if (eResult == XML_SUCCESS)
               {
                  c.SetAlive(!((bool)tempInt));
               }
            }
            
            s.AddCharacter(c);
            pElement = pElement->NextSiblingElement("Character");
			}
      }

		pElement = e->FirstChildElement("FileLocation");
		if (pElement != nullptr)
		{
			string file = string(pElement->GetText());
			s.SetFileLocation(file);
		}

      return s;
   }

}