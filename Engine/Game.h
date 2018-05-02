#ifndef GAME_H
#define GAME_H
#include <vector>
#include "Composite.h"
#include <map>
class Mesh;
class Shader;
class Texture;
class Model;

class ENGINE_API Game
{
public:
	Game();
	virtual ~Game();

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	void Run();
	void HandleEvents();
	static bool isRunning;

	void ManageTime();
	void SetFPSCapped(bool value);
	void ShowInfo();

	void AddMesh(Mesh* mesh);
	void AddModel(Model* model);
	void AddTexture(Texture* texture);
	void AddShader(string name, Shader* shader);

	Shader* GetShader(string name);

protected:
	Composite Root;
private:
	double _lastFrameTime;
	double _currentFrameTime;
	double _timePerFrame;
	double _timeSinceLastUpdate;
	double _deltaTime;
	bool _fpsCapped;

	const unsigned int FRAME_CAP = 60;
	unsigned int _frames;

	std::vector<Mesh*> m_meshes;
	std::map<string, Shader*> m_shaders;
	std::vector<Texture*> m_textures;
	std::vector<Model*> m_models;


};

#endif
