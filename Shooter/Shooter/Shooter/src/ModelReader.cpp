#include "stdafx.h"
#include "ModelReader.h"


ModelReader::ModelReader(string filename)
{
	ReadModelObjData(filename);

	//expand the data suitable for lDrawArrays()
	CreateExpandedVertices();
	CreateExpandedNormals();
	CreateExpandedTextureCoordinates();
}


void ModelReader::ReadModelObjData(string filename)
{
	
	std::fstream modelfile(filename, std::ios_base::in);

	if (!modelfile.is_open())
	{

		//DebugBreak();
		throw std::invalid_argument("File not found");
		return;	//ERROR!!!
	}

	string line;
	string token = "";

	while (getline(modelfile, line))
	{
		istringstream iss(line);

		// process the line
		token = "";

		iss >> token; // read to first whitespace

		if (token == "#")
		{
			// ignore rest of line
			// iss >> s;
		}
		else if (token == "g")
		{
			// read the model name
			iss >> m_modelName;
			// ignore rest of line
		}
		else if (token == "v")
		{
			ProcessVertexLine(iss);
		}
		else if (token == "vn")
		{
			ProcessVertexNormalLine(iss);
		}
		else if (token == "vt")
		{
			ProcessVertexTextureLine(iss);
		}
		else if (token == "f")
		{
			ProcessFaceLine(iss);
		}
		else
		{
			// ignore any line without these qualifiers
			// ignore rest of line
		}
	}
	modelfile.close();

}

void ModelReader::ProcessVertexLine(istringstream& iss)
{
	for (int i = 0; i < 3; i++)
	{
		float fCoord;
		iss >> fCoord;
		m_vertices.push_back(fCoord);
	}
	// TODO - store 3 floats in m_vertices vector

}

void ModelReader::ProcessVertexNormalLine(istringstream& iss)
{
	for (int i = 0; i < 3; i++)
	{
		float fCoord;
		iss >> fCoord;
		m_vertexNormals.push_back(fCoord);
	}
	// TODO - store 3 floats in m_vertexNormals vector
}

void ModelReader::ProcessVertexTextureLine(istringstream& iss)
{
	float fCoord;
	while (iss >> fCoord)
		m_vertexTextureCoordinates.push_back(fCoord);
	// TODO - store 2 floats in m_vertexTextureCoordinates vector

}


void ModelReader::ProcessFaceLine(istringstream& iss)
{
	static const int forwardSlash = 0x2F;
	for (int i = 0; i < 3; i++)
	{
		unsigned int iCoord;
		iss >> iCoord;

		m_faceVertexIndices.push_back(iCoord - 1);

		int lookAhead = iss.peek();  	// peek character
		if (lookAhead == forwardSlash)    // "/"
		{

			lookAhead = iss.get();
			lookAhead = iss.peek();
			if (lookAhead == forwardSlash)
			{
				lookAhead = iss.get();
				iss >> iCoord;
				m_faceNormalIndices.push_back(iCoord - 1);

			}
			else
			{
				iss >> iCoord;
				m_faceTextureIndices.push_back(iCoord - 1);
				lookAhead = iss.get();
				iss >> iCoord;
				m_faceNormalIndices.push_back(iCoord - 1);

			}

		}

	}

	// TODO - process 3 types of f line data
}


void ModelReader::CreateExpandedVertices()
{
	for (std::vector<unsigned int>::iterator it = m_faceVertexIndices.begin(); it != m_faceVertexIndices.end(); ++it)
	{
		int vertexNumber = (*it);
		// 3 floats per triangular face
		for (int i = 0; i < 3; i++)
		{
			m_vertexTriplets.push_back(m_vertices[3 * vertexNumber + i]);
		}
		// TODO - index and copy 3 floats to the m_vertexTriplets vector

	}
}
void ModelReader::CreateExpandedNormals()
{
	// we create a list of normal triplets for each face (with duplicates)
	// this will then be the same size as the vertexTriplets vector

	for (std::vector<unsigned int>::iterator it = m_faceNormalIndices.begin(); it != m_faceNormalIndices.end(); ++it)
	{
		//here we have a 1 base index
		// get the face number as 0 indexed
		int vertexNormalNumber = (*it);
		for (int i = 0; i < 3; i++)
		{
			m_vertexNormalTriplets.push_back(m_vertexNormals[3 * vertexNormalNumber + i]);
		}


		// TODO - index and copy 3 floats to the m_vertexNormalTriplets vector

	}
}
void ModelReader::CreateExpandedTextureCoordinates()
{
	for (std::vector<unsigned int>::iterator it = m_faceTextureIndices.begin(); it != m_faceTextureIndices.end(); ++it)
	{
		//here we have a 1 base index
		// get the face number as 0 indexed
		int vertexTextureNumber = (*it);
		for (int i = 0; i < 2; i++)
		{
			m_vertexTexturePairs.push_back(m_vertexTextureCoordinates[2 * vertexTextureNumber + i]);
		}
	}


}


// Get methods gove access to the vector data

vector<float>& ModelReader::GetVertices()
{
	return m_vertexTriplets;
}
vector<float>& ModelReader::GetNormals()
{
	return m_vertexNormalTriplets;
}
vector<float>& ModelReader::GetTextureCoordinates()
{
	return m_vertexTexturePairs;
}

void ModelReader::tanslate(glm::vec3 translation)
{


}
