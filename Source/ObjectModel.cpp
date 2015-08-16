// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.

#include "ObjectModel.h"
#include "Renderer.h"
#include "ObjectLoader.h"

// Include GLEW - OpenGL Extension Wrangler
#include <GL/glew.h>
using namespace glm;

ObjectModel::ObjectModel(vec3 size) : Model(){
  // Read our .obj file
  std::vector<glm::vec3> vertices;
  std::vector<glm::vec2> uvs;
  std::vector<glm::vec3> normals; // Won't be used at the moment.
  bool res = ObjectLoader::LoadObject("Objects/map.obj", vertices, uvs, normals);
  
  if (!res) {
    printf("Could not load...");
    return;
  }
  
  mVertexSize = (int) vertices.size();
  
  // Create a vertex array !DEV
  glGenVertexArrays(1, &mVertexArrayID);
  
  // Load it into a VBO
  glGenBuffers(1, &mVertexBufferID);
  glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
  
  glGenBuffers(1, &mUVBufferID);
  glBindBuffer(GL_ARRAY_BUFFER, mUVBufferID);
  glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
}

ObjectModel::~ObjectModel(){
	// Free the GPU from the Vertex Buffer
	glDeleteBuffers(1, &mVertexBufferID);
  glDeleteBuffers(1, &mUVBufferID);
	glDeleteVertexArrays(1, &mVertexArrayID);
}

void ObjectModel::Update(float dt){
	// If you are curious, un-comment this line to have spinning cubes!
	// That will only work if your world transform is correct...
	// mRotationAngleInDegrees += 90 * dt; // spins by 90 degrees per second
	Model::Update(dt);
}

void ObjectModel::Draw(){
	// Draw the Vertex Buffer
	// Note this draws a unit Cube
	// The Model View Projection transforms are computed in the Vertex Shader
	glBindVertexArray(mVertexArrayID);

	GLuint WorldMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldTransform"); 
	glUniformMatrix4fv(WorldMatrixLocation, 1, GL_FALSE, &GetWorldMatrix()[0][0]);
  
  // 1rst attribute buffer : vertices
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
  glVertexAttribPointer(0,  // The attribute we want to configure
                        3,                            // size
                        GL_FLOAT,                     // type
                        GL_FALSE,                     // normalized?
                        0,                            // stride
                        (void*)0                      // array buffer offset
                        );
	
  // 2nd attribute buffer : UVs
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, mUVBufferID);
  glVertexAttribPointer(1,                   // The attribute we want to configure
                        2,                            // size : U+V => 2
                        GL_FLOAT,                     // type
                        GL_FALSE,                     // normalized?
                        0,                            // stride
                        (void*)0                      // array buffer offset
                        );
  
	// Draw the triangles !
  glDrawArrays(GL_TRIANGLES, 0, mVertexSize);

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
}

bool ObjectModel::ParseLine(const std::vector<ci_string> &token){
	if (token.empty())
		return true;
	else
		return Model::ParseLine(token);
}