#include "Tree2D.h"

using namespace spacecolonization3D;
using namespace ci;
using namespace ci::app;
using namespace ci::gl;
using namespace std;

Tree2D::Tree2D(Vec2f position)
	:position(position),
	doneGrowing(false)
{
	GenerateCrown();
	GenerateTrunk();	
}

void Tree2D::GenerateCrown()
{	
	Vec2f cornerPoint = position + Vec2f(-treeWidth *0.5f, - treeHeight - trunkHeight);

	Vec2f xy = Vec2f(cornerPoint.x, cornerPoint.y);
	crown = Rectf(xy, xy + Vec2f(treeWidth, treeHeight));
		
	randSeed(std::time(nullptr));

	for (int i = 0; i < leafCount; i++)
	{		
		Vec2f location = Vec2f(randFloat(crown.x1, crown.x2), randFloat(crown.y1, crown.y2));
		leaves.push_back(Leaf2D(location));  
	}
}

void Tree2D::GenerateTrunk()
{
	root = new Branch2D(NULL, position, Vec2f(0, -1));	
	addBranch(root);

	Branch2D* current = root; 

	//Keep growing trunk upwards until we reach a leaf       
	while ((root->getPosition() - current->getPosition()).length() < trunkHeight)
	{
		Vec2f upNormal = Vec2f(0.0f, -1.0f);
		Branch2D *trunk   = new Branch2D(current, current->getPosition() + Vec2f( 0.0f, -branchLength), upNormal);
		addBranch(trunk);		
		current = trunk;  				
	} 
}

void Tree2D::addBranch(Branch2D *branch)
{
	vectorKey vec = make_pair(branch->getPosition().x, branch->getPosition().y);
	branches[vec] = branch;	
}

void Tree2D::runInThread()
{
	growing = true;
	ThreadRef(new boost::thread(bind( &Tree2D::growThreaded, this)));
}

void Tree2D::growThreaded()
{	
	while(growing)
	{
		grow();
		// sleep a while
		boost::this_thread::sleep( boost::posix_time::milliseconds(10) );
	}
}

void Tree2D::grow()
{
	 boost::lock_guard<boost::mutex> lock(branchMutex);

	if (leaves.empty())
		doneGrowing = true; 

	if (doneGrowing) return;

	for (auto leave = leaves.begin(); leave != leaves.end(); ++leave)			
	{
		bool leafRemoved = false;

		leave->setClosestBranch(NULL);
		Vec2f direction = Vec2f::zero();

		for (auto branch = branches.begin(); branch != branches.end(); ++branch)	
		{
			direction = leave->getPosition() - branch->second->getPosition();//direction to branch from leaf
			float distance = floor(direction.length());            //distance to branch from leaf
			direction.normalize();

			if (distance <= minDistance)            //Min leaf distance reached, we remove it
			{
				leaves.erase(leave--);							
				leafRemoved = true;
				break;
			}
			else if (distance <= maxDistance)       //branch in range, determine if it is the nearest
			{
				if (!leave->getClosestBranch() || 
					(leave->getPosition() - leave->getClosestBranchPosition()).length() > distance)
					leave->setClosestBranch(branch->second);
				//console()<< " leave->closestBranch "<<leave->closestBranch<<endl;
			}
		}
		//ci::app::console()<<"--------leafRemoved-----------"<<leafRemoved<<std::endl;

		//if the leaf was removed, skip
		if (!leafRemoved)
		{

			//Set the grow parameters on all the closest branches that are in range
			if (leave->getClosestBranch())
			{
				Vec2f dir = leave->getPosition() - leave->getClosestBranchPosition();
				dir.normalize();
				leave->getClosestBranch()->addGrowDirection(dir);       //add to grow direction of branch
				leave->getClosestBranch()->increaseGrowCount();
				//console()<< " leave->closestBranch->growCount "<<leave->closestBranch->growCount<<endl;
			}
		}
	}			
	////Generate the new branches
	////HashSet<Branch> newBranches = new HashSet<Branch>();

	vector<Branch2D*> newBranches;
	for (auto branch = branches.begin(); branch!=branches.end(); branch++)
	{
		Branch2D * _branch = branch->second;
		if (_branch->getGrowCount() > 0)    //if at least one leaf is affecting the branch
		{
			Vec2f avgDirection = _branch->getGrowDirection() / _branch->getGrowCount();
			avgDirection.normalize();

			Branch2D* newBranch = new Branch2D(_branch, _branch->getPosition() + avgDirection * branchLength, avgDirection);
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
		vectorKey vec = make_pair(newBranches[i]->getPosition().x, newBranches[i]->getPosition().y);

		Branch2D* findingBranch = branches.find(vec)->second;					
		if ( findingBranch!= NULL)
		{						
			addBranch(newBranches[i]);
			branchAdded = true;
		}					 
	}

	////if no branches were added - we are done
	////this handles issues where leaves equal out each other, making branches grow without ever reaching the leaf
	if (!branchAdded) 
		doneGrowing = true;
}

void Tree2D::draw()
{
	boost::lock_guard<boost::mutex> lock(branchMutex);

	gl::color(Color(0, 0, 1));

	for (auto it : branches)					
		gl::drawSolidCircle(it.second->getPosition(), 1, 16);	

	gl::color(Color(0, 1, 0));
	gl::drawSolidCircle(position, 1.5, 16);

	gl::color(Color(1, 0, 0));
	for (auto it : leaves)		
		gl::drawSolidCircle(it.getPosition(), 1, 16);	
}

void Tree2D::clear()
{
	boost::lock_guard<boost::mutex> lock(branchMutex);
	branches.clear();
	leaves.clear();
}

void Tree2D::generate()
{
	boost::lock_guard<boost::mutex> lock(branchMutex);
	doneGrowing = false;
	GenerateCrown();
	GenerateTrunk();		
}