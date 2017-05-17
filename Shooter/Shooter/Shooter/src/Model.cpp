#include <stdafx.h>
#include "Model.h"



Model::Model()
{
	m_Rotation = 0;
	spin = 0;
}

Model::~Model()
{
	delete(m_mr);
	delete(m_tl);
	m_mr = NULL;
	m_tl = NULL;
}



Model::Model(string objectPath, string texturePath)
{
	m_mr = new ModelReader(objectPath);
	m_tl = new TextureLoader();
	m_tl->LoadBMP(texturePath, m_textureID);

	m_Rotation = 0;
	spin = 0;
}

void Model::DrawUsingVBO(bool drawWithNormals, bool drawWithTexture)
{
	glPushMatrix();

	glTranslatef(m_position.x, m_position.y, m_position.z);	// draw robot at desired coordinates
	// activate and specify pointer to vertex array
	glRotatef(m_Rotation, 0.0f, m_Rotation,  0.0f);

	glEnableClientState(GL_VERTEX_ARRAY);
	vector<float>& vertices = m_mr->GetVertices();
	glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);

	// pointer to normal array
	if (drawWithNormals)
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		vector<float>& normals = m_mr->GetNormals();
		glNormalPointer(GL_FLOAT, 0, &normals[0]);

	}

	if (drawWithTexture)
	{
		glEnable(GL_TEXTURE_2D);
		vector<float>& textureCoordinates = m_mr->GetTextureCoordinates();
		glBindTexture(GL_TEXTURE_2D, m_textureID);

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, &textureCoordinates[0]);
	}

	// draw the shape
	glDrawArrays(GL_TRIANGLES, 0, (unsigned int)vertices.size() / 3);

	// deactivate vertex arrays after drawing
	glDisableClientState(GL_VERTEX_ARRAY);

	if (drawWithNormals)
	{
		glDisableClientState(GL_NORMAL_ARRAY);
	}

	if (drawWithTexture)
	{
		glBindTexture(GL_TEXTURE_2D, NULL);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisable(GL_TEXTURE_2D);

	}
	glPopMatrix();	// pop back to original coordinate system

	m_Rotation += spin;
	if (m_Rotation < 0)
	{
		m_Rotation = 360 + m_Rotation;
	}
}

void Model::tanslate(glm::vec3 translation,bool set)
{
	if (set)
	{
		m_position = translation;
	}
	else
	{
		m_position += translation;
	}

}

void Model::rotate(float amount, bool set)
{
	if (set)
	{
		m_Rotation = amount;
	}
	else
	{
		spin = amount;
		
	}
}
