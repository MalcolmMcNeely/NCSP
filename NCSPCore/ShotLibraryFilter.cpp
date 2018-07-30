//
// Implementation of ShotLibraryFilter.h
//
#include "ShotLibraryFilter.h"
#include "ShotLibrary.h"
#include "Shot.h"
#include "Character.h"

ShotLibraryFilter::ShotLibraryFilter()
{
}

ShotLibraryFilter::~ShotLibraryFilter()
{
}

void ShotLibraryFilter::FilterByLocation(ShotLibrary* in, ShotLibrary* out, eLocation loc, bool includeNoShot)
{
   if (in == nullptr) return;
   if (out == nullptr) return;

   out->Clear();
   out->SetViewerType(in->GetViewerType());

   for (auto& shot : in->GetShots())
   {
      if (shot.GetLocation() == loc)
      {
         out->AddShot(shot);
      }
   }

   if (includeNoShot)
   {
      for (auto& shot : in->GetShots())
      {
         if (shot.GetId() == NOSHOTID)
         {
            out->AddShot(shot);
         }
      }
   }

   out->Finalize();

   return;
}

void ShotLibraryFilter::FilterByCharacter(ShotLibrary* in, ShotLibrary* out, eChar ch, bool includeNoShot)
{
   if (in == nullptr) return;
   if (out == nullptr) return;

   out->Clear();
   out->SetViewerType(in->GetViewerType());

   for (auto& shot : in->GetShots())
   {
      for (auto character : shot.GetCharacterList())
      {
         if (character == ch)
         {
            out->AddShot(shot);
         }
      }
   }

   if (includeNoShot)
   {
      for (auto& shot : in->GetShots())
      {
         if (shot.GetId() == NOSHOTID)
         {
            out->AddShot(shot);
         }
      }
   }

   out->Finalize();

   return;
}
