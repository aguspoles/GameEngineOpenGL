#pragma once
#include "stdafx.h"
#include <map>
#include <memory>
#include <string>

template <class Resource, class Identifier>
class ResourceHolder{
private:
	std::map<Identifier, std::unique_ptr<Resource>> _resourceMap;

public:
	void LoadFromFile(Identifier id, const LPWSTR filename);
	void LoadFromFile(Identifier id, const string& filename);
	Resource& GetResource(Identifier id);
	const Resource& GetResource(Identifier id) const;


};

template<class Resource, class Identifier>
inline void ResourceHolder<Resource, Identifier>::LoadFromFile(Identifier id, const LPWSTR fileName)
{
	std::unique_ptr<Resource> resource(new Resource);
	if (!resource->LoadFromFile(fileName))
		throw std::runtime_error("ResourceHolder::LoadFromFile - failed to load " + *fileName);
	else
	{
		auto inserted = _resourceMap.insert(std::make_pair(id, std::move(resource)));
		assert(inserted.second);
	}
}

template<class Resource, class Identifier>
inline void ResourceHolder<Resource, Identifier>::LoadFromFile(Identifier id, const string& fileName)
{
	std::unique_ptr<Resource> resource(new Resource);
	if (!resource->LoadFromFile(fileName))
		throw std::runtime_error("ResourceHolder::LoadFromFile - failed to load " + fileName);
	else
	{
		auto inserted = _resourceMap.insert(std::make_pair(id, std::move(resource)));
		assert(inserted.second);
	}
}

template<class Resource, class Identifier>
inline Resource & ResourceHolder<Resource, Identifier>::GetResource(Identifier id)
{
	auto found = _resourceMap.find(id);
	assert(found != _resourceMap.end());
	return *found->second;
}

template<class Resource, class Identifier>
inline const Resource & ResourceHolder<Resource, Identifier>::GetResource(Identifier id) const
{
	auto found = _resourceMap.find(id);
	return *found->second;
}
