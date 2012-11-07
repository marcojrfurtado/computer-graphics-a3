#include <cstdio>
#include <algorithm>

#include "model.hpp"

using namespace std;

void Model::setup( std::vector< glm::vec3 > &v, std::vector< glm::vec3 > &vn, std::vector< std::vector< std::pair<int,int> >  > &f  ) {

	vertices = v;
	normals = vn;
	faces = f;

	calculateCenter();
	scale = calculateScale();
}


void Model::translate( float x, float y, float z) {
	
	pos.x+=x;
	pos.y+=y;
	pos.z+=z;

}


void Model::rotate( float x, float y, float z) {


	ang.x += x;
	if (ang.x > 360.0) ang.x -= 360.0;
	if (ang.x < 0.0) ang.x += 360.0;

	ang.y += y;
	if (ang.y > 360.0) ang.y -= 360.0;
	if (ang.y < 0.0) ang.y += 360.0;
	
	ang.z += z;
	if (ang.z > 360.0) ang.z -= 360.0;
	if (ang.z < 0.0) ang.z += 360.0;
}







void Model::calculateCenter() {

	std::vector< glm::vec3  >::iterator itV;
	center.x = center.y = center.z = 0.0;

	for( itV = vertices.begin() ; itV != vertices.end() ; itV++ ) {

		center.x+=itV->x;
		center.y+=itV->y;
		center.z+=itV->z;
	}

	center.x/=vertices.size();
	center.y/=vertices.size();
	center.z/=vertices.size();

}

float Model::calculateScale() {

	float retVal;	
	
	std::vector< glm::vec3  >::iterator itV = vertices.begin();
	glm::vec3 maxVec;
	glm::vec3 minVec;

	minVec.x= maxVec.x=itV->x;
	minVec.y= maxVec.y=itV->y;
	minVec.z= maxVec.z=itV->z;

	for( itV = vertices.begin() ; itV != vertices.end() ; itV++ ) {

		maxVec.x= ( itV->x > maxVec.x    )? itV->x : maxVec.x;
		maxVec.y= ( itV->y > maxVec.y    )? itV->y : maxVec.y;
		maxVec.z= ( itV->z > maxVec.z    )? itV->z : maxVec.z;
		
		
		minVec.x= ( itV->x < minVec.x    )? itV->x : minVec.x;
		minVec.y= ( itV->y < minVec.y    )? itV->y : minVec.y;
		minVec.z= ( itV->z < minVec.z    )? itV->z : minVec.z;
	}

	retVal = max( maxVec.x - minVec.x, max( maxVec.y - minVec.y , maxVec.z - minVec.z	)  );

	return retVal;



}

void Model::exportScheme(FILE *fp_out) {

	std::vector< glm::vec3 >::iterator itV;
	std::vector< std::vector< std::pair<int,int>  >  >::iterator itF;

	// Writes vertex vector to the output
	for( itV = vertices.begin() ; itV != vertices.end() ; itV++ ) {
		fprintf(fp_out,"v %f %f %f\n",itV->x,itV->y,itV->z);
	}
	
	// Writes normal vector to the output
	for( itV = normals.begin() ; itV != normals.end() ; itV++ ) {
		fprintf(fp_out,"vn %f %f %f\n",itV->x,itV->y,itV->z);
	}

	// Writes the faces list to the output
	for( itF = faces.begin() ; itF != faces.end() ; itF++  ) {
			fprintf(fp_out,"f %d//%d %d//%d %d//%d\n",(*itF)[0].first+1, (*itF)[0].second+1, (*itF)[1].first+1, (*itF)[1].second+1, (*itF)[2].first+1, (*itF)[2].second+1 );
	}
}
