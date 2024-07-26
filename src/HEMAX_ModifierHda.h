#pragma once

#include "HEMAX_Hda.h"

#include "HEMAX_3dsmaxHda.h"
#include "HEMAX_CustomAttributeContainer.h"

#pragma warning(push, 0)
#include <icustattribcontainer.h>
#pragma warning(pop)

class HEMAX_Modifier;

class HEMAX_ModifierHda : public HEMAX_3dsmaxHda
{
    public:
	HEMAX_ModifierHda();

	bool	        Create(HAPI_AssetLibraryId AssetId,
                               const std::string& AssetPath, int AssetIndex,
			       HEMAX_Modifier* Modifier, INode* MaxNode);

	void		Recreate(HAPI_AssetLibraryId AssetId,
                                 const std::string& AssetPath, int AssetIndex,
				 HEMAX_Modifier* Modifier, INode* ContainerNode);

	bool		CreateNewModifierHda(HEMAX_Hda& Hda,
                            HEMAX_Modifier* Modifier, INode* MaxNode);
	void		RecreateExistingModifierHda(HEMAX_Hda& Hda,
                            HEMAX_Modifier* Modifier, INode* MaxNode);

	void		UpdateModifierHda();

	void		DeleteMainInput();

	void		Update3dsmaxHda();

        virtual std::string Get3dsMaxContainerName() override;

	INode*		ContainerNode;

	int		MaxStampIndex;

	HEMAX_Modifier*	DisplayGeometry;

    private:
	void		InitModifierPluginCustAttribContainer(
                            HEMAX_Modifier* ModifierPlugin);

	void	        GenerateBoilerplateModifierPluginCustomAttributes(
                            HEMAX_Modifier* ModifierPlugin, HEMAX_Hda& Hda);

        virtual HEMAX_NodeListParameterAttrib* CreateNodeListCustAttrib(
                                                    int Subnetwork) override;
};



