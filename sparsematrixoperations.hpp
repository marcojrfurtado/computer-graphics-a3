



#ifndef _SPARSE_MATRIX_OPERATIONS_
 #define _SPARSE_MATRIX_OPERATIONS_

// Define this variable, so we can use the sparse matrix module
#define EIGEN_YES_I_KNOW_SPARSE_MODULE_IS_NOT_STABLE_YET 

// Sets the type of the default marixx to be row major
// It did not work, had to specify EIgen::RowMajor attribute.
//#define EIGEN_DEFAULT_TO_ROW_MAJOR

#include <Eigen/Eigen>
#include <Eigen/Sparse>

#include "model.hpp"
#include "joint.hpp"
#include <cstdio>
#include <utility>
#include <glm/glm.hpp>






class SparseMatrixOperations {

	public:

		typedef Eigen::SparseMatrix<float,Eigen::RowMajor> SparseMatrixS;

		// Return a newly allocated matrix 
		static SparseMatrixS * matrix_s(const Joint &root, const Model &mdl );

		// Prints he sparse matrix s to a file
		static void print_out(SparseMatrixS *s, const char *filename);

	private:
		// Calculates the distance from a point to a segment
		static double distPointSeg(glm::vec3 point, std::pair< glm::vec3, glm::vec3 > seg );


};


#endif
