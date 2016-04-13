#include "PlaneMesh.h"

PlaneMesh::PlaneMesh() 
{
}

PlaneMesh::~PlaneMesh()
{

}

void PlaneMesh::LoadModel(int width, int height, float vertexOffset)
{
	m_isDynamicVertexBuffer = true;
	m_width = width;
	m_height = height;

	float offset = vertexOffset;


	//Set the vertices count
	m_vertexCount = (m_width + 1)*(m_height + 1);
	//Set the indices count
	int faceCount = (m_width*m_height);
	m_indexCount = faceCount*6;



	//Create the vertex and index arrays
	m_vertices = new Vertex[m_vertexCount];
	m_indices = new unsigned long[m_indexCount];


	//Load the vertex array
	int faceIndex = 0;
	for (int z = 0; z <= m_height; z++)
	{
		
		for (int x = 0; x <= m_width; x++)
		{
			int v = (z*(m_width+1))+x;
			m_vertices[v].position = XMFLOAT3(x*offset, 0.0f, z*offset);  //bottom
			m_vertices[v].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);

			//m_vertices[v].colour = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
			m_vertices[v].texture = XMFLOAT2(x*offset*0.1f, z*offset*0.1f);
			//m_vertices[v].texture = XMFLOAT2(x/m_width, z/m_height);

			if (x < m_width && z<m_height) 
			{
				// Load the index array with data.
				int i = (faceIndex * 6);

				int bottomL = ((z)*(m_width + 1)) + x;
				int topL = ((z+1)*(m_width + 1)) + x;
				m_indices[i] = bottomL;  // Bottom left.
				m_indices[i + 1] = topL;  // Top left.
				m_indices[i + 2] = bottomL+1;  // Bottom Right.

				m_indices[i + 3] = topL + 1;// Top Right.
				m_indices[i + 4] = bottomL+1;// Bottom Right.
				m_indices[i + 5] = topL;// Bottom Left.
				faceIndex++;

			}
		}

	}
}