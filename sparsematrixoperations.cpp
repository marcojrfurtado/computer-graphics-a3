#include "sparsematrixoperations.hpp"
#include <vector>

using namespace Eigen;


SparseMatrix<float> * SparseMatrixOperations::matrix_s( const Joint &root, const Model &mdl) {


	// Get the list of bones in the hierarchy
	std::vector< std::pair<glm::vec3, glm::vec3> > bones;
	root.get_bones(bones);

/*
	for( auto it = bones.begin(); it != bones.end() ; it++) {

		printf(" ( %lf, %lf, %lf)   ( %lf, %lf, %lf))\n",it->first.x,it->first.y,it->first.z,it->second.x,it->second.y,it->second.z);

	}
*/
	DynamicSparseMatrix<float>
	triplets.reserve(mdl.vertices.size()); // The number of triplets depends on the number of vertices,


	// Iterate through all the vertices, looking for the closest bone
	for ( auto iti = mdl.vertices.begin() ; iti != mdl.vertices.end() ; iti++ ) {

		// S_ij
		int i = iti - mdl.vertices.begin();


		std::vector<int> j;
		auto minj;
		// Iterate on each bone
		for( auto itj = minj  = bones.begin() ; itj != bones.end() ; itj++ ) {

			double minDist = distPointSeg(*iti,*minj), newDist = distPointSeg(*iti,*itj);
			


			if (  newDist < minDist   ) {
				minj = itj;
				j.clear(); // If we found a smaller distance, ignore previous j

				j.push_back( (double) minj - bones.begin());

			} else if ( newDist == minDist ) {

				minj = itj;
				j.push_back( (double) minj - bones.begin() );
			}

		}

		
		// Create a triplet at position i,j, so we know i is the closest vertex to bone j.
		// The value should be 1/j.size()
		for( auto itJval = j.begin() ; itJval !=j.end() ; itJval++ )
			tripletList.push_back(Triplet<float>(i,*itJval, 1/j.size() ));

	}


	SparseMatrix<float> *sp_mat_s = new SparseMatrix<float>(mdl.vertices.size(),bones.size());

	sp_mat_s->setFromTriplets(tripletList.begin(),tripletList.end());




	return sp_mat;

}


double SparseMatrixOperations::distPointSeg(glm::vec3 point, std::pair< glm::vec3, glm::vec3 > seg ) {

	return 0;

}	
