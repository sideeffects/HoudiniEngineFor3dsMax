#include "HEMAX_Events.h"

#include "UI/HEMAX_UI.h"

HEMAX_Events::HEMAX_Events(HEMAX_UI* _UserInterface)
    : UserInterface(_UserInterface)
{

}

void
HEMAX_Events::SessionChanged()
{
    UserInterface->Update();
}

void
HEMAX_Events::ShelfUpdated()
{
    UserInterface->Update();
}

void
HEMAX_Events::SelectionSetChanged(HEMAX_3dsmaxHda* Hda,
                                  bool ForceUnlock)
{
    UserInterface->ChangeHdaSelection(Hda, ForceUnlock);
}

void
HEMAX_Events::AssetLoaded()
{
    UserInterface->UpdateLoadedAssetLibrariesList();
}

void
HEMAX_Events::AssetRemoved()
{
    UserInterface->UpdateLoadedAssetLibrariesList();
}

void
HEMAX_Events::HdaPreDelete(HEMAX_3dsmaxHda* Hda)
{
    UserInterface->HandleHdaPreDeleteEvent(Hda);
}

void
HEMAX_Events::InputChanged()
{
    UserInterface->Update();
}

void
HEMAX_Events::ParameterChanged()
{
    UserInterface->Update();
}

void
HEMAX_Events::NodeChanged()
{
    UserInterface->Update();
}
