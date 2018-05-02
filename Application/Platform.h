#pragma once
#include "MeshRenderer.h"

class Platform : public MeshRenderer
{
public:
	Platform();
	virtual ~Platform();

	void UpdateComposite() override;
	void InitComposite() override;
	void SetShaderProperties() override final;

	void HandleInput();
};

