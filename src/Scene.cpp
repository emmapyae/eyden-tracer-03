#include "Scene.h"
#include "PrimTriangle.h"
#include "ShaderEyelight.h"
#include "ShaderPhong.h"
// #include <fstream>
// #include <iostream>

void CScene::ParseOBJ(const std::string &fileName)
{
	std::cout << "Parsing OBJFile : " << fileName << std::endl;

	std::shared_ptr<IShader> pShader = std::make_shared<CShaderEyelight>(RGB(1, 1, 1));
	// --- PUT YOUR CODE HERE --
	// reference: https://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/

	// Opening the OBJ file
	std::ifstream openFile(fileName);
	if (!openFile.is_open())
	{
		std::cerr << "Cannot open the File! " << std::endl;
		exit(1); // exit the program if openFile not found
	}

	std::string readLine; // to read each line in the obj file
	std::vector<Vec3f> vertexIndices; 	// save all values for v
	Vec3f temp_v;
	Vec3i temp_f;
	// read the file until an empty string is found
	// read line by line
	while (getline(openFile, readLine))
	{
		// check with the first character in the line
		if (readLine[0] == 'v')
		{
			std::istringstream get_v(readLine.substr(2));
			// we start reading from index 2 of the line because the first two
			// chars are either v or f with a space followed.
			// put all 3 vertexes to each index of the vector
			get_v >> temp_v[0] >> temp_v[1] >> temp_v[2];
			vertexIndices.push_back(temp_v * 99.99f);
		}
		else if (readLine[0] == 'f')
		{
			std::istringstream get_f(readLine.substr(2));
			// read 3 vertexes for each face 
			get_f >> temp_f[0] >> temp_f[1] >> temp_f[2];
			this->Add(std::make_shared<CPrimTriangle>(vertexIndices[temp_f[0]-1], vertexIndices[temp_f[1]-1], vertexIndices[temp_f[2]-1], pShader));
			// subtracting 1 from indecies because they start from 1 in the OBJ file 
		}
	}

	// close file after reading
	openFile.close();

	std::cout << "Finished Parsing" << std::endl;
}