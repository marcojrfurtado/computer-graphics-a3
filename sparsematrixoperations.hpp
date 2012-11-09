



#ifndef _SPARSE_MATRIX_OPERATIONS_
 #define _SPARSE_MATRIX_OPERATIONS_

#include <Eigen/Dense>

#include "model.hpp"
#include "joint.hpp"


using Eigen::MatrixXd;


class SparseMatrixOperations {

	public:
		static MatrixXd * matrix_c(const Joint &root, const Model &mdl );


};


#endif
