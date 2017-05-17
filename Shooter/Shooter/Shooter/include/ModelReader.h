#ifndef MODELREADER_H
#define MODELREADER_H

#include "stdafx.h"
#include "glm\glm.hpp"

using namespace std;

class ModelReader
{
private:

	// need to know ascii for forward slash
	static const int forwardSlash = 0x2F;

	vector<float> m_vertices;					// v
	vector<float> m_vertexNormals;				// vn
	vector<float> m_vertexTextureCoordinates;	// vt only U V

												// f   v/vt/vn  (vt and vn optional)
												// these are unsigned as opengl expected unsigned data
	vector<unsigned int> m_faceVertexIndices;
	vector<unsigned int> m_faceTextureIndices;
	vector<unsigned int> m_faceNormalIndices;

	string m_modelName;

	vector<float> m_vertexTriplets;
	vector<float> m_vertexNormalTriplets;
	vector<float> m_vertexTexturePairs;

public:
	ModelReader(string filename);


	void ModelReader::ReadModelObjData(string filename);

	void ProcessVertexLine(istringstream& iss);
	void ProcessVertexNormalLine(istringstream& iss);
	void ProcessVertexTextureLine(istringstream& iss);
	void ProcessFaceLine(istringstream& iss);


	void CreateExpandedVertices();
	void CreateExpandedNormals();
	void CreateExpandedTextureCoordinates();

	vector<float>& GetVertices();
	vector<float>& GetNormals();
	vector<float>& GetTextureCoordinates();

	void tanslate(glm::vec3 translation);

};

#endif