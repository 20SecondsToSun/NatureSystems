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
	typedef Branch<ci::Vec2f> Branch2D;
	typedef Leaf<ci::Vec2f> Leaf2D;

	class Tree2D : public ITree
	{	
	private:
		Branch2D *root;
		std::list<Leaf2D> leaves;
		std::map <vectorKey, Branch2D*> branches;
		ci::Rectf crown;
		bool doneGrowing;    
		ci::Vec2f position;
		bool growing;

		void GenerateCrown();
		void GenerateTrunk();
		void addBranch(Branch2D *branch);		
		void growThreaded();
		boost::mutex branchMutex;

	public:
		Tree2D(ci::Vec2f position = ci::Vec2f::zero());
		void clear();
		void generate();
		void draw();
		void grow();
		void runInThread();
	};
}

//http://www.jgallant.com/procedurally-generating-trees-with-space-colonization-algorithm-in-xna/