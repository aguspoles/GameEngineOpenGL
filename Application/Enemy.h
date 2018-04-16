#pragma once
#include "MeshRenderer.h"

class Enemy : public MeshRenderer
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

