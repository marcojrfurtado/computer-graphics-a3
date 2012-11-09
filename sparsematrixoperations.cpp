#include "sparsematrixoperations.hpp"



MatrixXd * SparseMatrixOperations::matrix_c( const Joint &root, const Model &mdl) {

	std::vector< std::pair<glm::vec3, glm::vec3> > bones;

	root.get_bones(bones);


	for( auto it = bones.begin(); it != bones.end() ; it++) {

	}	

	return 0;

}
