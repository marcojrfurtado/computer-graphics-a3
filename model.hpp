#ifndef _MODEL_
  #define _MODEL_

#include <glm/glm.hpp>
#include <vector>


class Model {

	public: 
		Model() {
			reset();
		}


		// Resets this model
		void reset() {
			scale = 1.0;

			ang.x = ang.y = ang.z = 1.0;
			pos.x = pos.y = pos.z = 0.0;
		}



		// Vertex vector
		std::vector< glm::vec3 > vertices;

		// Normal vector
		std::vector< glm::vec3 > normals;

		// Faces vector
		std::vector< std::vector< std::pair<int,int> > > faces;

		// Model Center 
		glm::vec3 center;

		// Model Scale
		float scale;

		// Used for model rotations
		glm::vec3 ang;

		// Used for model translations
		glm::vec3 pos;

		void setup( std::vector< glm::vec3 > &v, std::vector< glm::vec3 > &vn, std::vector< std::vector< std::pair<int,int> >  > &f  );

		void translate(float x, float y, float z);
		void rotate(float x, float y, float z);

		// Responsible for task 2 of this exercise
		// Writes a model out an .obj file
		void exportScheme(FILE *fp_out);
	private:
		// Calculates the mean vertex
		void calculateCenter();
		
		// Calculate the scale to be used 
		float calculateScale();
};

#endif
