//
// COMP 371 Final Project
//
// Created by Emma Saboureau
//
//

#pragma once

#include "Model.h"

class WaterModel : public Model
{
public:
	WaterModel(glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f));
	virtual ~WaterModel();

	virtual void Update(float dt);
	virtual void Draw();

protected:
	virtual bool ParseLine(const std::vector<ci_string> &token);

private:
	// The vertex format could be different for different types of models
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 color;
	};
  
  std::vector<unsigned short> mIndexBuffer;
  
  unsigned int mRows = 5;
  unsigned int mColumns = 5;

  unsigned int mElementBuffer;
	unsigned int mVertexArrayID;
	unsigned int mVertexBufferID;
};


// From BSpline
//  float* GetPoint(const int& i, float(&cpts)[92][3]);
//  static void setFunctions();
//  static vector<vec3> Spline1, Spline2;
//  unsigned int numOfVertices;	//*/
//  int noOfPoints;
//  unsigned int LOD;