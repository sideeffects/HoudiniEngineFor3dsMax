#pragma once

#include "HEMAX_Input.h"

class HEMAX_Input_Transform : public HEMAX_Input
{
    public:
	HEMAX_Input_Transform(HEMAX_InputType Type, int Id, ULONG MaxNode);
	~HEMAX_Input_Transform();

	void RebuildAfterChange() override;

    private:
	HAPI_TransformEuler BuildTransform();
};
