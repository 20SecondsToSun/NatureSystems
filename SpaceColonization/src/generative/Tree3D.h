#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "ITree.h"
#include "Branch.h"
#include "Leaf.h"
#include "cinder/MayaCamUI.h"
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

		std::list<Leaf3D> leavesDrawing;
		std::map <vectorKey, Branch3D*> branchesDrawing;

		ci::Rectf crown;
		bool doneGrowing;    
		ci::Vec3f position;
		bool growing;

		void GenerateCrown();
		void GenerateTrunk(ci::Vec3f startPoint = ci::Vec3f(0, 1, 0), ci::Vec3f steerPoint = ci::Vec3f(0, 1, 0));

		void addBranch(Branch3D *branch);		
		void growThreaded();
		boost::mutex branchMutex;
		boost::mutex leaveMutex;		
		ci::MayaCamUI* cam;
	public:
		Tree3D(ci::MayaCamUI* cam, ci::Vec3f position = ci::Vec3f::zero());
		Tree3D(ci::MayaCamUI* cam, ci::Vec3f position, std::vector<ci::Vec3f> _leaves);
		void clear();
		void generate();
		void draw();
		void grow();
		void setCrown(std::vector<ci::Vec3f> leaves);
		void runInThread();		

	};
}

//http://www.jgallant.com/procedurally-generating-trees-with-space-colonization-algorithm-in-xna/