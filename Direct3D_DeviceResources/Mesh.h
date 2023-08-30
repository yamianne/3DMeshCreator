#pragma once
#include "SceneObject.h"

enum GeometryType
{
	Triangle,
	Quad
};

class Mesh : public SceneObject
{
public:
	Mesh(std::string name, GeometryType meshType);

	void			Init(ID3D11Device* device) override;

	void			RecalculateGeometry(ID3D11Device* device);
	virtual void	RecalculateTriMesh(ID3D11Device* device) = 0;
	virtual void	RecalculateQuadMesh(ID3D11Device* device) = 0;

	int				MeshType;
protected:
	std::vector<Vector3> m_normals;
};