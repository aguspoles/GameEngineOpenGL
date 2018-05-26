#pragma once
#include "ModelRenderer.h"

class Enemy : public ModelRenderer
{
public:
	Enemy();
	~Enemy();

	void UpdateComposite() override;
	void InitComposite() override;
	void SetShaderProperties() override final;

	void HandleInput();

private:
	bool m_moveLeft, m_moveRight;
};

