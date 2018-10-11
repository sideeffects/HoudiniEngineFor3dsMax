#pragma once

#include "HEMAX_Hda.h"
#include "HEMAX_CustomAttributeContainer.h"
#include <icustattribcontainer.h>

class HEMAX_Modifier;

struct HEMAX_ModifierHda
{
    INode* ContainerNode;

    ICustAttribContainer* CustomAttributes;
    std::unordered_map<std::string, HEMAX_ParameterAttrib*> CustomAttributeMap;
    int MaxStampIndex;

    HEMAX_Modifier* DisplayGeometry;
};

bool
CreateNewModifierHda(HEMAX_ModifierHda& ModifierHda, HEMAX_Hda& Hda, HEMAX_Modifier* Modifier, INode* MaxNode);

void
RecreateExistingModifierHda(HEMAX_ModifierHda& ModifierHda, HEMAX_Hda& Hda, HEMAX_Modifier* Modifier, INode* MaxNode);

void
UpdateModifierHda(HEMAX_ModifierHda& ModifierHda, HEMAX_Hda& Hda);

void
DeleteMainInput(HEMAX_ModifierHda& ModifierHda);

static void
InitModifierPluginCustAttribContainer(HEMAX_ModifierHda& ModifierHda, HEMAX_Modifier* ModifierPlugin);

static void
GenerateBoilerplateModifierPluginCustomAttributes(HEMAX_ModifierHda& ModifierHda, HEMAX_Modifier* ModifierPlugin, HEMAX_Hda& Hda);