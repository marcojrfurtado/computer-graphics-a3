#include "sparsematrixoperations.hpp"
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;


using namespace Eigen;


SparseMatrixOperations::SparseMatrixF * SparseMatrixOperations::matrix_s( const Joint &root, const Model &mdl) {


	// Get the list of bones in the hierarchy
	std::vector< std::pair<glm::vec3, glm::vec3> > bones;
	root.get_bones(bones);


	for( auto it = bones.begin(); it != bones.end() ; it++) {

		printf(" ( %lf, %lf, %lf)   ( %lf, %lf, %lf))\n",it->first.x,it->first.y,it->first.z,it->second.x,it->second.y,it->second.z);

	}

	SparseMatrixF *sp_mat_s = new SparseMatrixF(mdl.vertices.size(),bones.size());
	sp_mat_s->reserve(mdl.vertices.size()); // The number of triplets depends on the number of vertices,


	// Iterate through all the vertices, looking for the closest bone
	for ( auto iti = mdl.vertices.begin() ; iti != mdl.vertices.end() ; iti++ ) {

		// S_ij
		int i = iti - mdl.vertices.begin();


		std::vector<int> j;
		std::vector< std::pair< glm::vec3, glm::vec3 > >::iterator minj;
		// Iterate on each bone
		for( auto itj = minj  = bones.begin() ; itj != bones.end() ; itj++ ) {


			segment newSeg = minimalSeg(*iti,*itj), minSeg = minimalSeg(*iti,*minj);

			double minDist = segLength(minSeg), newDist = segLength(newSeg);

			if (  newDist < minDist   ) {
				minj = itj;
				j.clear(); // If we found a smaller distance, ignore previous j

				j.push_back( (int) (minj - bones.begin()));

			} else if ( newDist == minDist ) {

				minj = itj;
				j.push_back( (int) (minj - bones.begin()) );
			}

		}


		for( auto itJval = j.begin() ; itJval !=j.end() ; itJval++ )
		{
			sp_mat_s->insert(i,*itJval) =  1/ (double) j.size();
		}

	}




	return sp_mat_s;

}

SparseMatrixOperations::SparseMatrixF * SparseMatrixOperations::matrix_c( const Joint &root, const Model &mdl) {
	
	// Get the list of bones in the hierarchy
	std::vector< std::pair<glm::vec3, glm::vec3> > bones;
	root.get_bones(bones);


	for( auto it = bones.begin(); it != bones.end() ; it++) {

		printf(" ( %lf, %lf, %lf)   ( %lf, %lf, %lf))\n",it->first.x,it->first.y,it->first.z,it->second.x,it->second.y,it->second.z);

	}

	SparseMatrixF *sp_mat_c = new SparseMatrixF(mdl.vertices.size(),bones.size());
	sp_mat_c->reserve(mdl.vertices.size()); // The number of triplets depends on the number of vertices,


	// Iterate through all the vertices, looking for the closest bone
	for ( auto iti = mdl.vertices.begin() ; iti != mdl.vertices.end() ; iti++ ) {

		// S_ij
		int i = iti - mdl.vertices.begin();


		std::vector<int> j;
		std::vector< std::pair< glm::vec3, glm::vec3 > >::iterator minj;
		// Iterate on each bone
		for( auto itj = minj  = bones.begin() ; itj != bones.end() ; itj++ ) {

			// If this segment intersects any face from the model, it is not visible
			if( has_intersections(minimalSeg(*iti,*itj),mdl) )
				continue;

			double minDist = segLength(minimalSeg(*iti,*minj)), newDist = segLength(minimalSeg(*iti,*itj));

			if (  newDist < minDist   ) {
				minj = itj;
				j.clear(); // If we found a smaller distance, ignore previous j

				j.push_back( (int) (minj - bones.begin()));

			} else if ( newDist == minDist ) {

				minj = itj;
				j.push_back( (int) (minj - bones.begin()) );
			}

		}


		for( auto itJval = j.begin() ; itJval !=j.end() ; itJval++ )
		{
			sp_mat_c->insert(i,*itJval) =  1/ (double) j.size();
		}

	}




	return sp_mat_c;


}
/*
SparseMatrixOperations::segment SparseMatrixOperations::minimalSeg(glm::vec3 point, segment seg ) {


	// If segment is just a single point, return a segment from point to it
	if ( seg.first == seg.second  )
		return segment(point,seg.first);



	glm::vec3 c = seg.first - point; // vector from the beginning of seg to point

	glm::vec3 unit = glm::normalize( seg.second - seg.first ); // Unit vector for seg 

	float length = glm::distance(seg.first,seg.second); // Length of segment

	float t = glm::dot(unit,c); // Intersection point Distance from seg.first


	// Check to see the point is on the line

	if ( t < 0 ) 
		return segment(point,seg.first);
	else if ( t > length  )
		return segment(point,seg.second);

	// Get the distance to move from point seg.first
	unit*=t;
	

	// Move from seg.first to the nearst point in segment
	return segment(point,seg.first+unit);


}	*/

void SparseMatrixOperations::print_out(SparseMatrixF *s, const char *filename) {

	ofstream outfile(filename);

	for (int k=0; k<s->outerSize(); ++k) {
		outfile << k ;
		for (SparseMatrixF::InnerIterator it(*s,k); it; ++it)
		{
			outfile << " " << it.col();
		}
		outfile << endl;
	}

	outfile.close();
}


bool SparseMatrixOperations::has_intersections( segment seg, const Model &mdl) {

	return true;

}
