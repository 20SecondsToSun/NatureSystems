#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "Branch.h"

namespace spacecolonization3D
{
	template <class Dim> class Leaf
	{
		Dim position;
		Branch<Dim>* closestBranch; 

	public:		
		Leaf(Dim position);
		Dim getPosition();
		void setClosestBranch(Branch<Dim>* closestBranch);
		Branch<Dim>* getClosestBranch();
		Dim getClosestBranchPosition();
	};

	template <typename Dim>
	Leaf<Dim>::Leaf(Dim position):position(position)
	{

	}

	template <typename Dim>
	void Leaf<Dim>::setClosestBranch(Branch<Dim>* closestBranch)
	{
		this->closestBranch = closestBranch;
	}

	template <typename Dim>
	Branch<Dim>* Leaf<Dim>::getClosestBranch()
	{
		return closestBranch;
	}

	template <typename Dim>
	Dim Leaf<Dim>::getClosestBranchPosition()
	{
		return closestBranch->getPosition();
	}

	template <typename Dim>
	Dim Leaf<Dim>::getPosition()
	{
		return position;
	}
}