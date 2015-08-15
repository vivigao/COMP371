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
	WaterModel(int textureID);
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
  std::vector<Vertex> vertexBuffer;
  
  int mTextureID;
  
  float timer;
  
  unsigned int mRows = 51;
  unsigned int mColumns = 51;
  
  float waveTime = 3,
  waveWidth = 1,
  waveHeight = 0.8,
  waveFreq = 0.03;
  
  int waveTimeLocation,
  waveWidthLocation,
  waveHeightLocation;
  
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