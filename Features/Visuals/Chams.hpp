#pragma once

struct ModelRenderInfo_t;
struct matrix3x4_t;

namespace Chams
{
	void CreateMaterials();
	void DrawModelExecute(void *ThisPtr, void *Context, void *State, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld);
};