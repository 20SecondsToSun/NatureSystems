#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

namespace tree3d
{

using namespace ci;
using namespace ci::app;
using namespace ci::gl;
using namespace std;



class Branch;
typedef std::pair<float, float> vectorKey;

class Leaf
{        
    public:
		ci::Vec3f position;
		Branch* closestBranch;
 
		Leaf(ci::Vec3f _position)
		{
			position = _position;
		}
};

class Branch
{
	public :
		Branch* parent;
		Vec3f growDirection;
		Vec3f OriginalGrowDirection;
		int   growCount;
        Vec3f position;
 
		Branch(Branch* _parent, Vec3f _position, Vec3f _growDirection)
		{
			parent = _parent;
			position = _position;
			growDirection = growDirection;
			OriginalGrowDirection = growDirection;
			growCount = 0;
		}
 
		void reset()
		{
			growCount = 0;
			growDirection = OriginalGrowDirection;
		}
};


class Tree3d
{
	public:
		 bool DoneGrowing;    
		 Vec3f position;
 
		static int LeafCount;
		static int TreeWidth ;    
		static int TreeHeight ;   
		static int TreeDeep ;
		static int trunkHeight ;
		static int MinDistance ;
		static int MaxDistance ;
		static int branchLength ;
 
		Branch* root;
		std::list<Leaf> leaves;
		std::map <vectorKey, Branch*> branches;

		Rectf crown;
 
		Tree3d(Vec3f _position):DoneGrowing(false)
		{	

			position = _position;
			GenerateCrown();
			GenerateTrunk();		
		}

		void clear()
		{
			branches.clear();
			leaves.clear();
		}

		void generate()
		{
			DoneGrowing = false;
			GenerateCrown();
			GenerateTrunk();		
		}



		void draw()
		{
			gl::color(Color(0, 0, 1));

			for (auto it=branches.begin(); it != branches.end(); ++it)			
			{				
				gl::drawSphere((*it).second->position, 1, 16);
			}

			gl::color(Color(0, 1, 0));
			gl::drawSphere(position, 1.5, 16);
		
			gl::color(Color(1, 0, 0));
			for (auto it=leaves.begin(); it != leaves.end(); ++it)			
			{
				gl::drawSphere((*it).position, 1, 16);
			}
		}

	private:
		void GenerateCrown()
		{	
			Vec3f cornerPoint = Vec3f(position.x - TreeWidth *0.5, position.y +  trunkHeight, position.z - TreeDeep *0.5);
			
			crown = Rectf(cornerPoint.x, cornerPoint.y ,cornerPoint.x+TreeWidth, cornerPoint.y +TreeHeight);
			Vec2f deepZ  = Vec2f(cornerPoint.z, cornerPoint.x+TreeDeep );
			for (int i = 0; i < LeafCount; i++)
			{
				Vec3f location = Vec3f(randFloat(crown.x1, crown.x2),randFloat(crown.y1, crown.y2), randFloat(deepZ.x, deepZ.y));
				leaves.push_back(Leaf(location));  
			}

		}

		void GenerateTrunk()
		{
			root = new Branch(NULL, position, Vec3f(0, 1, 0));	
			vectorKey vec = make_pair(root->position.x,root->position.y);
			branches[vec] = root;
			//branches.insert(make_pair(vec, root));

			Branch* current = new Branch(root, Vec3f(position.x, position.y + branchLength, position.z), Vec3f(0, 1, 0));
			vec = make_pair(current->position.x,current->position.y);
			branches[vec] = current;			

			// //Keep growing trunk upwards until we reach a leaf       
			while ((root->position - current->position).length() < trunkHeight)
			{
				Branch* trunk = new Branch(current, Vec3f(current->position.x, current->position.y + branchLength, current->position.z), Vec3f(0, 1, 0));
				vec = make_pair(trunk->position.x,trunk->position.y);
				branches[vec] = trunk;	
				current = trunk;  				
			}     

		}
public:
		void grow()
		{

			if (DoneGrowing) return;
			
			//If no leaves left, we are done
			if (leaves.size() == 0) { 
				DoneGrowing = true; 
				return; 
			}


			for (auto leave = leaves.begin(); leave != leaves.end(); ++leave)			
			{
				bool leafRemoved = false;

				 leave->closestBranch = NULL;
				 Vec3f direction = Vec3f::zero();

				 for (auto branch = branches.begin(); branch!=branches.end(); branch++)
				 {
					direction = leave->position - branch->second->position;                       //direction to branch from leaf
					float distance =   floor(direction.length());            //distance to branch from leaf
					direction.normalize();

					if (distance <= MinDistance)            //Min leaf distance reached, we remove it
					{
						leaves.erase(leave--);							
						leafRemoved = true;
						break;
					}
					else if (distance <= MaxDistance)       //branch in range, determine if it is the nearest
					{
						if (!leave->closestBranch)
							leave->closestBranch = branch->second;
						else if ((leave->position - leave->closestBranch->position).length() > distance)
							leave->closestBranch = branch->second;
						 //console()<< " leave->closestBranch "<<leave->closestBranch<<endl;
					}
				 }
				//ci::app::console()<<"--------leafRemoved-----------"<<leafRemoved<<std::endl;
 
				//if the leaf was removed, skip
				 if (!leafRemoved)
				 {
					
					//Set the grow parameters on all the closest branches that are in range
					if (leave->closestBranch)
					{
						Vec3f dir = leave->position - leave->closestBranch->position;
						dir.normalize();
						leave->closestBranch->growDirection += dir;       //add to grow direction of branch
						leave->closestBranch->growCount++;
						//console()<< " leave->closestBranch->growCount "<<leave->closestBranch->growCount<<endl;
					}
				 }
			  }			
				//Generate the new branches
				//HashSet<Branch> newBranches = new HashSet<Branch>();
			   vector<Branch*> newBranches;
			   for (auto branch = branches.begin(); branch!=branches.end(); branch++)
			   {
					Branch * _branch = branch->second;
					if (_branch->growCount > 0)    //if at least one leaf is affecting the branch
					{
						Vec3f avgDirection = _branch->growDirection / _branch->growCount;
						avgDirection.normalize();


						Branch* newBranch = new Branch(_branch, _branch->position + avgDirection * branchLength, avgDirection);
						//vec = make_pair(current->position.x,current->position.y);
						//branches[vec] = current;					
						newBranches.push_back(newBranch);
						_branch->reset();
					}
				}			  
				 //Add the new branches to the tree
				bool branchAdded = false;
				for (int i = 0; i < newBranches.size(); i++)
				{				
					//Check if branch already exists.  These cases seem to happen when leaf is in specific areas
					//Branch existing;
					vectorKey vec = make_pair(newBranches[i]->position.x, newBranches[i]->position.y);
						
					Branch* findingBranch =  branches.find(vec)->second;					
					if ( findingBranch!= NULL)
					{						
						vectorKey vec = make_pair(newBranches[i]->position.x,newBranches[i]->position.y);
						branches[vec] = newBranches[i];	

						branchAdded = true;
					}					 
				}

				//if no branches were added - we are done
				//this handles issues where leaves equal out each other, making branches grow without ever reaching the leaf
				if (!branchAdded) 
					DoneGrowing = true;
		}
};

int Tree3d::branchLength = 2;
int Tree3d::LeafCount = 500;
int Tree3d::TreeWidth = 600;    
int Tree3d::TreeHeight  = 400;   
int Tree3d::TreeDeep    = 700;   
int Tree3d::trunkHeight = 150;
int Tree3d::MinDistance = 60;
int Tree3d::MaxDistance = 105;
};

//http://www.jgallant.com/procedurally-generating-trees-with-space-colonization-algorithm-in-xna/