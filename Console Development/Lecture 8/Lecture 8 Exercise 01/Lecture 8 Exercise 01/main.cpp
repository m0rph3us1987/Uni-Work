// System includes
#include <stdio.h>
#include <stdlib.h>
#include <scebase.h>
#include <kernel.h>
#include <gnmx.h>


// PS4 Math Libraries (Matrices/Vectors)
#include <vectormath.h>

using namespace sce;
using namespace sce::Gnmx;
using namespace sce::Vectormath::Scalar::Aos;


// Program Entry Point
int main()
{
	Vector4 up(0, 1, 0, 1);

	Matrix4 identity(Vector4(1, 0, 0, 0),
		Vector4(0, 1, 0, 0),
		Vector4(0, 0, 1, 0),
		Vector4(0, 0, 0, 1));

	// val should be the same as up (i.e., multiplied
	// by an identity
	Vector4 val = identity * up;
	// val => 0,1,0,0

	Matrix4 rotZ;
	rotZ = rotZ.rotationZ(3.14f); // radians

	// val2 should be pointing in the opposite direction
	// as we rotated by 180 degrees (i.e., PI radians)
	Vector4 val2 = rotZ * up;
	// val2 => 0, -1, 0, 0


	return 0;
} // End main(..)