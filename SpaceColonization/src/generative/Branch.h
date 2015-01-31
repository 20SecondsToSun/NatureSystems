#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

namespace spacecolonization3D
{
	template <class Dim> class Branch
	{
		Branch<Dim>	  *parent;
		Dim growDirection;
		Dim OriginalGrowDirection;
		int	growCount;
		Dim position;

	public:
		Branch(Branch<Dim>* parent, Dim position, Dim growDirection);
		void reset();		
		Dim getPosition();
		void addGrowDirection(Dim vec);
		void increaseGrowCount();
		Dim getGrowDirection();
		int getGrowCount();
	};	
	template <typename Dim>
	void Branch<Dim>::reset()
	{
		growCount = 0;
		growDirection = OriginalGrowDirection;
	}

	template <typename Dim>
	Branch<Dim>::Branch(Branch<Dim>* parent, Dim position, Dim growDirection)
		:parent(parent),
		position(position),
		growDirection(growDirection),
		OriginalGrowDirection(growDirection),
		growCount(0)
	{

	}

	template <typename Dim>
	Dim Branch<Dim>::getPosition()
	{
		return position;
	}

	template <typename Dim>
	void Branch<Dim>::addGrowDirection(Dim vec)
	{
		growDirection += vec;
	}

	template <typename Dim>
	void Branch<Dim>::increaseGrowCount()
	{
		growCount++;
	}

	template <typename Dim>
	int Branch<Dim>::getGrowCount()
	{
		return growCount;
	}

	template <typename Dim>
	Dim Branch<Dim>::getGrowDirection()
	{
		return growDirection;
	}
}