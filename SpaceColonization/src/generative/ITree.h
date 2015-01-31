#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

namespace spacecolonization3D
{
	class ITree  
	{
	public:
		virtual void generate() = 0;
		virtual void clear() = 0;
		virtual void grow() = 0;	
		virtual void draw() = 0;
		virtual void runInThread() = 0;

		static int leafCount;
		static int treeWidth;    
		static int treeHeight;   
		static int treeDeep;
		static int trunkHeight;
		static int minDistance;
		static int maxDistance;
		static int branchLength;
	};
}