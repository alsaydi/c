#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
class Bead{
	friend ostream & operator<<(ostream & out,const Bead & bead);
	friend istream & operator>>(istream & in, Bead & bead);
public:
	Bead();
	Bead(int leftColor,int rightColor);
	~Bead();
	Bead(const Bead& copy); /* copy constructor */
	bool operator==(const Bead& right);
	Bead& operator=(const Bead& right);
	bool canLink(const Bead& other);
	int leftColor;
	int rightColor;	
	
};

Bead::Bead()
{
	this->leftColor = 0;
	this->rightColor = 0;
}

Bead::Bead(int leftColor,int rightColor)
{
	this->leftColor = leftColor;
	this->rightColor = rightColor;

}

Bead::~Bead()
{
}
Bead::Bead(const Bead & copy)
{
	if(this != &copy) /* avoid copying to the object to itself */
	{
		this->leftColor = copy.leftColor;
		this->rightColor = copy.rightColor;
	}
}
Bead& Bead::operator =(const Bead &right)
{
	if(this != &right) /* avoid assigning the object to itself */
	{
		this->leftColor = right.leftColor;
		this->rightColor = right.rightColor;
	}
	return *this; /* useful for doing things like a = b = c; where a,b and c are Bead objects */
}
bool Bead::operator==(const Bead& right)
{
	return this->leftColor == right.leftColor && this->rightColor == right.rightColor;	
}
bool Bead::canLink(const Bead & other)
{
	return this->leftColor == other.rightColor || this->leftColor == other.leftColor
		|| this->rightColor == other.rightColor || this->rightColor == other.leftColor;
}
ostream & operator<<(ostream & out,const Bead& bead)
{
	out << bead.leftColor << " " << bead.rightColor;
	return out;
}
istream& operator>>(istream &in,Bead& bead)
{
	in >> bead.leftColor >> bead.rightColor;
	return in;
}


/*
*  find a hamiltanion cylce in a graph
*  beadsPtr is the a pointer to the vector needed because we want to swap the colors of some beads
*  matrix: the notation int ** is the same in usage as int [][] 
*  visited: an array of ints to keep track of what's been visited 
*  cycle: an array of vertices which will make a cycle 
*  vertex: the starting vertex
*  currentV: where we at
*  size: how many vertices
*  done:
*/
bool findHC(vector<Bead> * beadsPtr, int ** matrix,int * visited,int * cycle,
			int vertex,int currentV,int size, bool done)
{
	
	visited[vertex] = 1;
	for(int i=0;i<size && !done;i++)
	{
		if( i==0 && matrix[vertex][i] && currentV == size) 
		{

			cycle[currentV] = 0; /* add the first bead to the end of the cycle to make "a real" cycle; not really needed for this*/
			done = true;
		
		}
		else {
			if(!visited[i] && matrix[vertex][i]){
				if( findHC(beadsPtr,matrix,visited,cycle,i,currentV+1,size,done))
					done = true;
				else visited[i] = 0;
			}
		}
	}

	if(done)
	{		
		cycle[currentV-1] = vertex; 
	}
	return done;
}

int findSuccessiveBead(vector<Bead> &beads,int * visited,int rightColor)
{
	int retIndex = -1;
	for(int i=0;i<beads.size();i++)
	{
		if((beads[i].leftColor == rightColor || beads[i].rightColor == rightColor )&& !visited[i])
		{
			if(beads[i].rightColor != beads[i].leftColor && beads[i].rightColor == rightColor)
			{
				Bead temp;
				temp.leftColor = beads[i].rightColor;
				temp.rightColor = beads[i].leftColor;
				beads[i] = temp;
			}
			visited[i] = 1;
			retIndex = i;
			break;
		}
	}
	return retIndex;
}
int run()
{
	int numCases = 0;
	cin >> numCases;
	int caseNo = 1;
	while(caseNo <= numCases)
	{
		vector<Bead> beads; /* vector to hold the vertixes */
		int numBeads = 0;
		cin >> numBeads;
		int ** matrix = new int*[numBeads];  /* for edges representation */
		for(int i=0;i<numBeads;i++){
			matrix[i] = new int[numBeads];
			memset(matrix[i],0,sizeof(int)*numBeads);
		
		}
		while(numBeads > 0)
		{
			Bead bead;
			cin >> bead;
			cin.ignore();
			beads.push_back(bead);
			numBeads--;
		}

		int size = beads.size();
		int colors[51]; /* a bead  may have one of 50 colors */
		for(int i=0;i<51;i++)
			colors[i] = 0;
		for(int i=0;i<beads.size();i++)
		{
			colors[beads[i].leftColor]++;
			colors[beads[i].rightColor]++;
		}
		bool validCase = true;
		for(int i=0;i<51;i++){
			if((colors[i] % 2)) /* if any color appears an odd number of times, then this case is invalid */
			{
				validCase = false;				
			}
		}
		if(!validCase)
		{
			cout << "Case #"<<caseNo++ <<endl << "some beads may be lost" << endl << endl;
			continue;
		}
		for(int i=0;i<size;i++)
		{
			
			for(int j=0;j<size;j++)
			{
				if(i==j) matrix[i][j]=0;
				if(beads[i].canLink(beads[j] ))
				{
					matrix[i][j] = 1;
				}
				else
				{
					matrix[i][j] = 0;
				}
			}		
		}	
		int * cycle = new int[beads.size()+1]; /* a cycle array has the first element in the beginning and the end of the cycle*/
		int * visited = new int[beads.size()];
		memset(cycle,-1,sizeof(int)*(beads.size()+1));
		memset(visited,0,sizeof(int)*beads.size());

		cout << "Case #" << caseNo++ << endl;
		if(findHC(&beads, matrix,visited,cycle,0,1,beads.size(),false)){
			cout << beads[cycle[0]] << endl;/* findHC when returning true ensures that if we start from cycle[0]; 
											we can make a cycle that includes all beads in the area and run into 
											sub-cycles */
			int counter = 1;
			int rightColor = beads[cycle[0]].rightColor;
			memset(visited,0,sizeof(int)*beads.size());
			visited[cycle[0]] = 1;
			while(counter < beads.size())
			{
				int index = findSuccessiveBead(beads,visited,rightColor);
				cout << beads[index] << endl;
				rightColor = beads[index].rightColor;
				counter++;
			}
			cout << endl;
		}
		else
		{
			cout << "some beads may be lost" << endl << endl;
		}
		delete cycle;
		delete visited;
		for(int i=0;i<beads.size();i++)
			delete matrix[i];
		delete matrix;
		beads.clear();		
	}
	return 0;	
}

int main()
{
	run();
	return 0;
}
