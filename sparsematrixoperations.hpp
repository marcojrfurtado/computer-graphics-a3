



#ifndef _SPARSE_MATRIX_OPERATIONS_
 #define _SPARSE_MATRIX_OPERATIONS_

// Define this variable, so we can use the sparse matrix module
#define EIGEN_YES_I_KNOW_SPARSE_MODULE_IS_NOT_STABLE_YET 
#include <Eigen/Eigen>
#include <Eigen/Sparse>

#include "model.hpp"
#include "joint.hpp"
#include <cstdio>
#include <utility>
#include <glm/glm.hpp>






class SparseMatrixOperations {

	public:
		// Return a newly allocated matrix 
		static Eigen::SparseMatrix<float> * matrix_s(const Joint &root, const Model &mdl );

	private:
		// Calculates the distance from a point to a segment
		static double distPointSeg(glm::vec3 point, std::pair< glm::vec3, glm::vec3 > seg );


};


#endif
