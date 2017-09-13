#pragma once

struct D3DXMATRIX;

class CRender
{
public:
	const D3DXMATRIX& WorldToScreenMatrix();
};