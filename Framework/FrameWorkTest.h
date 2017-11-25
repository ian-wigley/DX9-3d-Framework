#include "BoundingBox.h"
#include "BoundingSphere.h"
#include "Castle.h"
#include "DirectionalLight.h"
#include "ExplosionNode.h"
#include "FrameWorkResourceManager.h"
#include "Light.h"
#include "SceneNode.h"
#include "SpotLight.h"
#include "SkyDome.h"
#include "Tank.h"
#include "Tree.h"
#include "TerrainNode.h"
#include "WoodenCrate.h"
#include "BulletNode.h"

#pragma once

class FrameWorkTest : public Framework
{
public:
	FrameWorkTest(void);
	~FrameWorkTest(void);
	FrameWorkTest(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int showCmd);
	int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE prevInstance, LPWSTR cmdLine, int showCmd);
	Framework GetFrameWork(void)const;
	Framework frameWork;
	FrameWorkTest Test(void);

private:
	void Shutdown();
	SceneNode * _sNode;
};

