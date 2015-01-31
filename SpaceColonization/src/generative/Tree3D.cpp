#include "Tree3D.h"

using namespace spacecolonization3D;
using namespace ci;
using namespace ci::app;
using namespace ci::gl;
using namespace std;

Tree3D::Tree3D(Vec3f position)
	:position(position),
	doneGrowing(false)
{
	GenerateCrown();
	GenerateTrunk();	
}

void Tree3D::GenerateCrown()
{	
	Vec3f cornerPoint = position + Vec3f(-treeWidth *0.5f, trunkHeight, -treeDeep *0.5);

	Vec2f xy = Vec2f(cornerPoint.x, cornerPoint.y);
	crown = Rectf(xy, xy + Vec2f(treeWidth, treeHeight));

	Vec2f deepZ  = Vec2f(cornerPoint.z, cornerPoint.x + treeDeep);
	
	randSeed(std::time(nullptr));

	for (int i = 0; i < leafCount; i++)
	{		
		Vec3f location = Vec3f(randFloat(crown.x1, crown.x2), randFloat(crown.y1, crown.y2), randFloat(deepZ.x, deepZ.y));
		leaves.push_back(Leaf3D(location));  
	}
}

void Tree3D::GenerateTrunk()
{
	root = new Branch3D(NULL, position, Vec3f(0, 1, 0));	
	addBranch(root);

	Branch3D* current = root; 

	//Keep growing trunk upwards until we reach a leaf       
	while ((root->getPosition() - current->getPosition()).length() < trunkHeight)
	{
		Vec3f  upNormal = Vec3f(0.0f, 1.0f, 0.0f);
		Branch3D *trunk   = new Branch3D(current, current->getPosition() + Vec3f( 0.0f, branchLength, 0.0f), upNormal);
		addBranch(trunk);		
		current = trunk;  				
	} 
}

void Tree3D::addBranch(Branch3D *branch)
{
	vectorKey vec = make_pair(branch->getPosition().x, branch->getPosition().y);
	branches[vec] = branch;	
}

void Tree3D::runInThread()
{
	growing = true;
	ThreadRef(new boost::thread(bind( &Tree3D::growThreaded, this)));
}

void Tree3D::growThreaded()
{	
	while(growing)
	{
		grow();
		// sleep a while
		boost::this_thread::sleep( boost::posix_time::milliseconds(10) );
	}
}

void Tree3D::grow()
{
	 boost::lock_guard<boost::mutex> lock(branchMutex);

	if (leaves.empty())
		doneGrowing = true; 

	if (doneGrowing) return;

	for (auto leave = leaves.begin(); leave != leaves.end(); ++leave)			
	{
		bool leafRemoved = false;

		leave->setClosestBranch(NULL);
		Vec3f direction = Vec3f::zero();

		for (auto branch =branches.begin(); branch != branches.end(); ++branch)	
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
				Vec3f dir = leave->getPosition() - leave->getClosestBranchPosition();
				dir.normalize();
				leave->getClosestBranch()->addGrowDirection(dir);       //add to grow direction of branch
				leave->getClosestBranch()->increaseGrowCount();
				//console()<< " leave->closestBranch->growCount "<<leave->closestBranch->growCount<<endl;
			}
		}
	}			
	////Generate the new branches
	////HashSet<Branch> newBranches = new HashSet<Branch>();

	vector<Branch3D*> newBranches;
	for (auto branch = branches.begin(); branch!=branches.end(); branch++)
	{
		Branch3D * _branch = branch->second;
		if (_branch->getGrowCount() > 0)    //if at least one leaf is affecting the branch
		{
			Vec3f avgDirection = _branch->getGrowDirection() / _branch->getGrowCount();
			avgDirection.normalize();

			Branch3D* newBranch = new Branch3D(_branch, _branch->getPosition() + avgDirection * branchLength, avgDirection);
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

		Branch3D* findingBranch = branches.find(vec)->second;					
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

void Tree3D::draw()
{
	boost::lock_guard<boost::mutex> lock(branchMutex);

	gl::color(Color(0, 0, 1));

	for (auto it : branches)					
		gl::drawSphere(it.second->getPosition(), 1, 16);	

	gl::color(Color(0, 1, 0));
	gl::drawSphere(position, 1.5, 16);

	gl::color(Color(1, 0, 0));
	for (auto it : leaves)		
		gl::drawSphere(it.getPosition(), 1, 16);	
}

void Tree3D::clear()
{
	boost::lock_guard<boost::mutex> lock(branchMutex);
	branches.clear();
	leaves.clear();
}

void Tree3D::generate()
{
	boost::lock_guard<boost::mutex> lock(branchMutex);
	doneGrowing = false;
	GenerateCrown();
	GenerateTrunk();		
}