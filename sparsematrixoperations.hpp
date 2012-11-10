



#ifndef _SPARSE_MATRIX_OPERATIONS_
 #define _SPARSE_MATRIX_OPERATIONS_

// Define this variable, so we can use the sparse matrix module
#define EIGEN_YES_I_KNOW_SPARSE_MODULE_IS_NOT_STABLE_YET 


// When compiling GLM, this option print important statements about the included modules
//#define GLM_MESSAGES

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
#include <glm/gtx/closest_point.hpp>
#undef glm_gtx_closest_point // Simple hack to make this include work. Newer GLM versions fixed this issue.
#include <glm/gtx/closest_point.inl>






class SparseMatrixOperations {

	public:

		typedef Eigen::SparseMatrix<float,Eigen::RowMajor> SparseMatrixF;
		typedef std::pair< glm::vec3, glm::vec3 > segment;

		// Return a newly allocated matrix FOR TASK 2A (simplified skin-bone connection matrix)
		static SparseMatrixF * matrix_s(const Joint &root, const Model &mdl );
		
		// Return a newly allocated matrix FOR TASK 2B (skin to visible-bone connection matrix)
		static SparseMatrixF * matrix_c(const Joint &root, const Model &mdl );

		// Prints he sparse matrix s to a file
		static void print_out(SparseMatrixF *s, const char *filename);

	private:
		// Computes the minimal segment that connects  a point to a segment
		static segment minimalSeg(glm::vec3 point, segment seg ) { 

			if ( seg.first == seg.second )
				return segment(point,seg.first);

			return segment(point,glm::closestPointOnLine(point,seg.first,seg.second));  
		};

		// Calculates  the length of a segment
		static double segLength( segment seg ) { return glm::distance(seg.first,seg.second); };

		static bool has_intersections( segment seg, const Model &mdl);


};


#endif
