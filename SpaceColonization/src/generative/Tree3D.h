#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "ITree.h"
#include "Branch.h"
#include "Leaf.h"
#include <boost/thread/thread.hpp>

namespace spacecolonization3D
{
	typedef std::pair<float, float> vectorKey;
	typedef boost::shared_ptr<boost::thread> ThreadRef;
	typedef Branch<ci::Vec3f> Branch3D;
	typedef Leaf<ci::Vec3f> Leaf3D;

	class Tree3D : public ITree
	{	
	private:
		Branch3D *root;
		std::list<Leaf3D> leaves;
		std::map <vectorKey, Branch3D*> branches;
		ci::Rectf crown;
		bool doneGrowing;    
		ci::Vec3f position;
		bool growing;

		void GenerateCrown();
		void GenerateTrunk();
		void addBranch(Branch3D *branch);		
		void growThreaded();
		boost::mutex branchMutex;

	public:
		Tree3D(ci::Vec3f position = ci::Vec3f::zero());
		void clear();
		void generate();
		void draw();
		void grow();
		void runInThread();		
	};
}

//http://www.jgallant.com/procedurally-generating-trees-with-space-colonization-algorithm-in-xna/