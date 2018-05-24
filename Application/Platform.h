#pragma once
#include "ModelRenderer.h"

class Platform : public ModelRenderer
{
public:
	Platform();
	virtual ~Platform();

	void UpdateComposite() override;
	void InitComposite() override;
	void SetShaderProperties() override final;

	void HandleInput();
};

