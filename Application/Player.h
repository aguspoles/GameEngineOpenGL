#pragma once
#include "MeshRenderer.h"

class Player :
	public MeshRenderer
{
public:
	Player();
	~Player();

	void UpdateComposite() override;
	void InitComposite() override;
	void SetShaderProperties() override final;

	void HandleInput();


private:
	bool m_moveLeft, m_moveRight, m_moveUp, m_moveDown, m_moveForward, m_moveBackward;
};

