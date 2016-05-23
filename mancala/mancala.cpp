#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

ofstream outfile_next;  
ofstream outfile_log;

class Value
{
public:
	int v;
	vector<int> child_board;
};

class Node
{
public:
	string node_name;
	int index; 
	
	vector<int> board;
	vector<int> child_board; 


	int value; 
	Node(string node_name, vector<int> board);
};

Node::Node(string node_name, vector<int> board)
{
	this->node_name = node_name;
	this->board = board;
}

Value Min_value(Node parent, int index, int pit_num, int cutting_off_depth, int depth, int your_player
	,int task, int alpha, int beta);
Value Max_value(Node parent, int index, int pit_num, int cutting_off_depth, int depth, int your_player
	,int task, int alpha, int beta);
int Root_node(Node root, int pit_num, int cutting_off_depth, int depth, int your_player
	,int task, int alpha, int beta);

Value Min_value1(Node parent, int index, int pit_num, int cutting_off_depth, int depth, int your_player
	,int task, int alpha, int beta);
Value Max_value1(Node parent, int index, int pit_num, int cutting_off_depth, int depth, int your_player
	,int task, int alpha, int beta);
int Root_node1(Node root, int pit_num, int cutting_off_depth, int depth, int your_player
	,int task, int alpha, int beta);

string print(int value)
{
	string s;
	if(value >= 10000)
		s = "Infinity";
	else if(value <= -10000)
		s = "-Infinity";
	else
	{
		stringstream ss;
		ss << value;
		s = ss.str();
	}
	return s;
}

int Root_node(Node root, int pit_num, int cutting_off_depth, int depth, int your_player, 
	int task, int alpha, int beta)
{
		vector<int> gameover2;
		for(int k=0; k<root.board.size()/2-1; k++) 
		{
			if(root.board[k] == 0)
			{
				gameover2.push_back(0);
			}
		}
		vector<int> gameover3;
		for(int k=root.board.size()/2; k<root.board.size()-1; k++) 
		{
			if(root.board[k] == 0)
			{
				gameover3.push_back(0);
			}
		}
		if(gameover2.size() == root.board.size()/2-1 || gameover3.size() == root.board.size()/2-1)
		{
			return 0;
		}


	if(task != 1)
		ofstream outfile_log("traverse_log.txt",ios::app);

	Value value;
	Value tempValue = Value();
	value.v = -1000000;

	if(task == 2)
	{
		outfile_log << "Node,Depth,Value" << endl;
		outfile_log << "root," << depth << "," << print(value.v) << endl;
	}
	if(task == 3)
	{
		outfile_log << "Node,Depth,Value,Alpha,Beta" << endl;
		outfile_log << "root," << depth << "," << print(value.v) << "," << print(alpha) 
			<< "," << print(beta) << endl;
	}

	for(int i=0; i<pit_num; i++)
	{	
		if(root.board[i] != 0)
		{
			tempValue = Min_value(root, i, pit_num, cutting_off_depth, depth+1, your_player, 
						task, alpha,beta);

					if(tempValue.v > value.v)
					{
						value.v = tempValue.v;
						
						if(depth <= 1)
						{
							root.child_board = tempValue.child_board;
						}
					}
			
			if(task == 2)
			{
				outfile_log << "root," << depth << "," << print(value.v) << endl;
			}
			if(task == 3)
			{
				if(value.v >= beta)
				{
					outfile_log << "root," << depth << "," << print(value.v) << "," << 
						print(alpha) << "," << print(beta) << endl;

					break;
				}

				alpha = max(alpha, value.v);

				outfile_log << "root," << depth << "," << print(value.v) << "," << 
					print(alpha) << "," << print(beta) << endl;
			}
		}
	}
	
	for(int i=root.child_board.size()-2; i>=root.child_board.size()/2; i--) 
	{ 
		outfile_next << root.child_board[i]  << " ";
	}
	outfile_next << endl;
	for(int i=0; i<root.child_board.size()/2-1; i++) 
	{ 
		outfile_next <<  root.child_board[i] << " ";
	}
	outfile_next << endl;
	outfile_next << root.child_board[root.child_board.size()-1] << endl;
	outfile_next << root.child_board[root.child_board.size()/2-1] << endl;

	return 0;
}

Value Min_value(Node parent, int index, int pit_num, int cutting_off_depth, int depth, int your_player
	,int task, int alpha, int beta) 
{
	if(task != 1)
		ofstream outfile_log("traverse_log.txt",ios::app);

		Value value;
		Node next_node = parent; 
		int stones = next_node.board[index]; 
		int temp_stones = stones; 
		next_node.board[index] = 0; 
		value.v = 1000000;
						
		/////////////////// move stone ///////////////////////////
		for(int i=0; i<temp_stones; i++) 
		{
			next_node.board[(index+1+i) % (next_node.board.size()-1)]++;
		}
		///////////////////////////////////////////////////////

		/////////////////// catch opponent stone ///////////////////////////
		int index_laststone = (index+stones) % (next_node.board.size()-1);
		if(next_node.board[index_laststone] == 1 && index_laststone < next_node.board.size()/2-1)
		{
			next_node.board[index_laststone] = 0; 
			next_node.board[next_node.board.size()/2-1]++; 
			next_node.board[next_node.board.size()/2-1] 
			 = next_node.board[next_node.board.size()/2-1]
			 + next_node.board[next_node.board.size()-2-index_laststone];  
			next_node.board[next_node.board.size()-2-index_laststone] = 0; 
		}
		////////////////////////////////////////////////////////////////

		////////////// heuristic //////////////////////////////
		next_node.value = next_node.board[next_node.board.size()/2-1] - 
				next_node.board[next_node.board.size()-1];
		//////////////////////////////////////////////////////

		///////////////////////// end game ///////////////////////////
		vector<int> gameover1;
		for(int k=next_node.board.size()/2; k<next_node.board.size()-1; k++) 
		{
			if(next_node.board[k] == 0)
			{
				gameover1.push_back(0);
			}
		}
		if(gameover1.size() == next_node.board.size()/2-1)
		{
			for(int k=0; k<next_node.board.size()/2-1; k++)
			{
				next_node.board[next_node.board.size()/2-1] += next_node.board[k]; 
				next_node.board[k] = 0; 
			}

			if(task == 2)
			{
				outfile_log << "B" << index+2 << "," << depth << "," << 
					next_node.board[next_node.board.size()/2-1] -
						next_node.board[next_node.board.size()-1] << endl;
			}
			if(task == 3)
			{
				outfile_log << "B" << index+2 << "," << depth << "," << 
					next_node.board[next_node.board.size()/2-1] - 
						next_node.board[next_node.board.size()-1] 
							<< "," << print(alpha) << "," << print(beta) << endl;
			}
						
			value.v = next_node.board[next_node.board.size()/2-1] - 
				next_node.board[next_node.board.size()-1];

			if(depth == 1)
				value.child_board = next_node.board;

			return value;
		}
		////////////////////////////////////////////////////////////////////////////


		///////////////////////// end game ///////////////////////////
		vector<int> gameover;
		for(int k=0; k<next_node.board.size()/2-1; k++)
		{
			if(next_node.board[k] == 0)
			{
				gameover.push_back(0);
			}
		}

		if(gameover.size() == next_node.board.size()/2-1)
		{
			for(int k=next_node.board.size()/2; k<next_node.board.size()-1; k++)
			{
				next_node.board[next_node.board.size()-1] += next_node.board[k]; 
				next_node.board[k] = 0; 
			}
				
			if(task == 2)
			{
				outfile_log << "B" << index+2 << "," << depth << "," << 
					next_node.board[next_node.board.size()/2-1] -
						next_node.board[next_node.board.size()-1] << endl;
			}
			if(task == 3)
			{
				outfile_log << "B" << index+2 << "," << depth << "," << 
					next_node.board[next_node.board.size()/2-1] - 
						next_node.board[next_node.board.size()-1] 
							<< "," << print(alpha) << "," << print(beta) << endl;
			}
						
			value.v = next_node.board[next_node.board.size()/2-1] 
				- next_node.board[next_node.board.size()-1];

			if(depth == 1)
				value.child_board = next_node.board;

		    return value;
		}
		////////////////////////////////////////////////////////////////////////////
		

		///////////////////// one more chance //////////////////////////
		if( (stones+index) % (next_node.board.size()-1) == next_node.board.size()/2 - 1) 
		{		
			if(task == 2)
			{
				outfile_log << "B" << index+2 << "," << depth << "," 
					<< print(-value.v) << endl;
			}
			if(task == 3)
			{
				outfile_log << "B" << index+2 << "," << depth << "," 
						<< print(-value.v) << "," 
							<< print(alpha) << "," << print(beta) << endl;
			}


			Value tempValue = Value();
			value.v = -1000000;

			for(int j=0; j<pit_num; j++) 
			{
				if(next_node.board[j] != 0)
				{
					tempValue = Min_value(next_node, j, pit_num, cutting_off_depth, depth, your_player, 
						task, alpha,beta);

					if(tempValue.v > value.v)
					{
						value.v = tempValue.v;
						
						if(depth <= 1)
						{
							value.child_board
								= tempValue.child_board;
						}
					}

					if(task == 2)
					{
						outfile_log << "B" << index+2 << "," << depth << ","
							<< print(value.v) << endl;
					}
					if(task == 3)
					{
						if(value.v >= beta)
						{
							outfile_log << "B" << index+2 << "," << depth << "," <<
								print(value.v) << "," << print(alpha) << "," << print(beta) 
									<< endl;
						
							return value;
						}

						alpha = max(alpha,value.v);

						outfile_log << "B" << index+2 << "," << depth << "," << 
							print(value.v) << ","
								<< print(alpha) << "," << print(beta) << endl;
					}
				}
			}
			return value;
		}
		////////////////////////////////////////////////////////////

		if(cutting_off_depth != depth)
			{
				if(task == 2)
				{
					outfile_log << "B" << index+2 << "," << depth << "," 
						<< print(value.v) << endl;
				}
				if(task == 3)
				{
					outfile_log << "B" << index+2 << "," << depth << "," 
						<< print(value.v) << 
							"," << print(alpha) << ","	<< print(beta) << endl;
				}
			}
		else if(cutting_off_depth == depth)
		{
			if(task == 2)
			{
				outfile_log << "B" << index+2 << "," << depth << "," 
					<< next_node.value << endl;
			}
			if(task == 3)
			{
				outfile_log << "B" << index+2 << "," << depth << "," 
					<< next_node.value << "," << 
						print(alpha) <<	"," << print(beta) << endl;
			}	

			if(depth == 1)
				value.child_board = next_node.board;

			value.v = next_node.value;
			return value;
		}
			

		for(int j=0; j<pit_num; j++) 
		{
			if(next_node.board[next_node.board.size()-2-j] != 0) 
			{	
				value.v = min(value.v, Max_value(next_node, next_node.board.size()-2-j, pit_num, 
						cutting_off_depth, depth+1, your_player, task, alpha, beta).v);

				if(task == 2) 
				{
					outfile_log << "B" << index+2 << "," << depth << "," 
						<< print(value.v) << endl;
				}

				if(task == 3) 
				{ 
					if(value.v <= alpha)
					{
						outfile_log << "B" << index+2 << "," << depth << "," << 
							print(value.v) << 
								"," << print(alpha) << "," << print(beta) << endl;
					
						if(depth == 1)
							value.child_board = next_node.board;

						return value;
					}
					
					beta = min(beta,value.v);

					outfile_log << "B" << index+2 << "," << depth << "," << print(value.v) << 
						"," << print(alpha) << "," << print(beta) << endl;
				}
			}
		}

		if(depth == 1)
				value.child_board = next_node.board;

		return value;
}	

Value Max_value(Node parent, int index, int pit_num, int cutting_off_depth, int depth, int your_player
	,int task, int alpha, int beta) 
{
	if(task != 1)
		ofstream outfile_log("traverse_log.txt",ios::app);

		Value value;
		Node next_node = parent; 
		int stones = next_node.board[index]; 
		int temp_stones = stones; 
		next_node.board[index] = 0; 
		value.v = -1000000;	
		
		/////////////////// move stone ///////////////////////////
		for(int i=0; i<temp_stones; i++)
		{
			int temp_index1 = index - next_node.board.size()/2;
				
			next_node.board[(((temp_index1+1+i) % (next_node.board.size()-1)) 
				+ next_node.board.size()/2) % next_node.board.size()]++;
		}
		///////////////////////////////////////////////////////

		/////////////////// catch opponent stone ///////////////////////////
		int temp_index = index - next_node.board.size()/2;
		int temp_index_laststone = (temp_index+stones) % (next_node.board.size()-1);
		int index_laststone = (temp_index_laststone + next_node.board.size()/2)
			% next_node.board.size();
		if(next_node.board[index_laststone] == 1 && index_laststone > next_node.board.size()/2-1
			&& index_laststone < next_node.board.size()-1)
		{
			next_node.board[index_laststone] = 0; 
			next_node.board[next_node.board.size()-1]++; 
			next_node.board[next_node.board.size()-1] 
			 = next_node.board[next_node.board.size()-1]
			 + next_node.board[next_node.board.size()-2-index_laststone];  
			next_node.board[next_node.board.size()-2-index_laststone] = 0; 
		}
		////////////////////////////////////////////////////////////////

		////////////// heuristic //////////////////////////////
		next_node.value = next_node.board[next_node.board.size()/2-1] - 
			next_node.board[next_node.board.size()-1];
		//////////////////////////////////////////////////////

		///////////////////////// end game ///////////////////////////
		vector<int> gameover1;
		for(int k=0; k<next_node.board.size()/2-1; k++) 
		{
			if(next_node.board[k] == 0)
			{
				gameover1.push_back(0);
			}
		}

		if(gameover1.size() == next_node.board.size()/2-1)
		{
			for(int k=next_node.board.size()/2; k<next_node.board.size()-1; k++)
			{
				next_node.board[next_node.board.size()-1] += next_node.board[k]; 
				next_node.board[k] = 0;
			}
				
			if(task == 2)
			{
				outfile_log << "A" << parent.board.size()-index << "," << depth << "," <<
					 next_node.board[next_node.board.size()/2-1] - 
						next_node.board[next_node.board.size()-1] << endl;
			}
			if(task == 3)
			{
				outfile_log << "A" << parent.board.size()-index << "," << depth << "," <<
					 next_node.board[next_node.board.size()/2-1] - 
						next_node.board[next_node.board.size()-1] <<
							"," << print(alpha) << "," << print(beta) << endl;
			}
		
			value.v = next_node.board[next_node.board.size()/2-1] - 
				next_node.board[next_node.board.size()-1];

		    return value;
		}
		////////////////////////////////////////////////////////////////////////////

		///////////////////////// end game ///////////////////////////
		vector<int> gameover;
		for(int k=next_node.board.size()/2; k<next_node.board.size()-1; k++) 
		{
			if(next_node.board[k] == 0)
			{
				gameover.push_back(0);
			}
		}
		if(gameover.size() == next_node.board.size()/2-1)
		{
			for(int k=0; k<next_node.board.size()/2-1; k++)
			{
				next_node.board[next_node.board.size()/2-1] += next_node.board[k]; 
				next_node.board[k] = 0; 
			}

			if(task == 2)
			{
				outfile_log << "A" << parent.board.size()-index << "," << depth << "," <<
					 next_node.board[next_node.board.size()/2-1] - 
						next_node.board[next_node.board.size()-1] << endl;
			}
			if(task == 3)
			{
				outfile_log << "A" << parent.board.size()-index << "," << depth << "," <<
					 next_node.board[next_node.board.size()/2-1] - 
						next_node.board[next_node.board.size()-1] <<
							"," << print(alpha) << "," << print(beta) << endl;
			}
						
			value.v = next_node.board[next_node.board.size()/2-1] - 
				next_node.board[next_node.board.size()-1];

			return value;
		}
		////////////////////////////////////////////////////////////////////////////

	
		/////////////////////// one more chance //////////////////////////
		if( (stones+index) % (next_node.board.size()-1) == 0) 
		{
			if(task == 2)
			{
				outfile_log << "A" << parent.board.size()-index << "," << depth << "," << 
					print(-value.v) << endl;
			}
			if(task == 3)
			{
				outfile_log << "A" << parent.board.size()-index << "," << depth << "," 
					<< print(-value.v) 
						<< "," << print(alpha) << "," << print(beta) << endl;
			}

			value.v = 1000000;

			for(int j=0; j<pit_num; j++) 
			{
				if(next_node.board[next_node.board.size()-2-j] != 0) 
				{
					value.v = min(value.v, Max_value(next_node, next_node.board.size()-2-j, 
							pit_num, cutting_off_depth, depth, your_player, task, alpha, beta).v);

					if(task == 2)
					{
						outfile_log << "A" << parent.board.size()-index << "," << depth << "," << 
							print(value.v) << endl;
					}
					if(task == 3)
					{
						if(value.v <= alpha)
						{
							outfile_log << "A" << parent.board.size()-index << "," << depth 
								<< "," << print(value.v) 
									<< "," << print(alpha) << "," << print(beta) << endl;
	
							return value;
						}

						beta = min(beta, value.v);

						outfile_log << "A" << parent.board.size()-index << "," << depth << 
							"," << print(value.v) << "," << print(alpha) << "," << print(beta) << endl;
					}
				}
			}
			return value;
		}
		///////////////////////////////////////////////////////////////

		if(cutting_off_depth != depth)
		{
			if(task == 2)
			{
				outfile_log << "A" << parent.board.size()-index << "," << depth 
					<< "," << print(value.v) << endl;
			}
			if(task == 3)
			{
				outfile_log << "A" << parent.board.size()-index << "," << depth 
					<< "," << print(value.v) 
						<< "," << print(alpha) << "," << print(beta) << endl;
			}
		}
		else if(cutting_off_depth == depth)
		{
			if(task == 2)
			{
				outfile_log << "A" << parent.board.size()-index << "," << depth << 
					"," << next_node.value << endl;
			}
			if(task == 3)
			{
				outfile_log << "A" << parent.board.size()-index << "," << depth << 
					"," << next_node.value << "," << print(alpha) << "," << print(beta) << endl;
			}
			value.v = next_node.value;
			return value; 
		}
	
		
		for(int j=0; j<pit_num; j++) 
		{
			if(next_node.board[j] != 0) 
			{
				value.v = max(value.v,Min_value(next_node, j, pit_num, cutting_off_depth, depth+1, your_player
					,task, alpha, beta).v);
				
				if(task == 2) 
				{
					outfile_log << "A" << parent.board.size()-index << "," << depth 
						<< "," << print(value.v) << endl;
				}
				
				if(task == 3) 
				{
					if(value.v >= beta)
					{
						outfile_log << "A" << parent.board.size()-index << "," << depth 
							<< "," << print(value.v) 
								<< "," << print(alpha) << "," << print(beta) << endl;
	
						return value;
					}

					alpha = max(alpha, value.v);

					outfile_log << "A" << parent.board.size()-index << "," << depth 
						<< "," << print(value.v) 
							<< "," << print(alpha) << "," << print(beta) << endl;
				}
			}
		}
		return value;
}


int Root_node1(Node root, int pit_num, int cutting_off_depth, int depth, int your_player, 
	int task, int alpha, int beta)
{

		vector<int> gameover2;
		for(int k=0; k<root.board.size()/2-1; k++) 
		{
			if(root.board[k] == 0)
			{
				gameover2.push_back(0);
			}
		}
		vector<int> gameover3;
		for(int k=root.board.size()/2; k<root.board.size()-1; k++) 
		{
			if(root.board[k] == 0)
			{
				gameover3.push_back(0);
			}
		}
		if(gameover2.size() == root.board.size()/2-1 || gameover3.size() == root.board.size()/2-1)
		{
			return 0;
		}


	if(task != 1)
		ofstream outfile_log("traverse_log.txt",ios::app);

	Value value;
	Value tempValue = Value();
	value.v = -1000000;

	if(task == 2)
	{
		outfile_log << "Node,Depth,Value" << endl;

		outfile_log << "root," << depth << "," << print(value.v) << endl;
	}
	if(task == 3)
	{
		outfile_log << "Node,Depth,Value,Alpha,Beta" << endl;

		outfile_log << "root," << depth << "," << print(value.v) << "," << print(alpha) 
			<< "," << print(beta) << endl;
	}

	for(int i=root.board.size()-2; i>root.board.size()/2-1; i--)
	{	
		if(root.board[i] != 0)
		{
			tempValue = Min_value1(root, i, pit_num, cutting_off_depth, depth+1, your_player,
				task, alpha, beta);

			if(tempValue.v > value.v)
			{
				value.v = tempValue.v;

				if(depth <= 1)
				{
					root.child_board = tempValue.child_board;
				}
			}
			
			if(task == 2)
			{
				outfile_log << "root," << depth << "," << print(value.v) << endl;
			}
			if(task == 3)
			{
				if(value.v >= beta)
				{
					outfile_log << "root," << depth << "," << print(value.v) << "," << 
						print(alpha) << "," << print(beta) << endl;

					break;
				}

				alpha = max(alpha, value.v);

				outfile_log << "root," << depth << "," << print(value.v) << "," << 
					print(alpha) << "," << print(beta) << endl;
			}
		}
	}

	for(int i=root.child_board.size()-2; i>=root.child_board.size()/2; i--) 
	{
		outfile_next << root.child_board[i];

		if(i != root.child_board.size()/2)
			outfile_next << " ";
	}		
	outfile_next << endl;		
	for(int i=0; i<root.child_board.size()/2-1; i++)
	{ 		
		outfile_next <<  root.child_board[i];

		if(i != root.child_board.size()/2-2)
			outfile_next << " ";
	}		
	outfile_next << endl;		
	outfile_next << root.child_board[root.child_board.size()-1] << endl;		
	outfile_next << root.child_board[root.child_board.size()/2-1] << endl;		

	return 0;
}

Value Min_value1(Node parent, int index, int pit_num, int cutting_off_depth, int depth, int your_player
	,int task, int alpha, int beta) 
{
	if(task != 1)
		ofstream outfile_log("traverse_log.txt",ios::app);
		
		Value value;
		Node next_node = parent; 
		int stones = next_node.board[index]; 
		int temp_stones = stones; 
		next_node.board[index] = 0; 
		value.v = 1000000;	
			
		/////////////////// move stone ///////////////////////////
		for(int i=0; i<temp_stones; i++) 
		{
			int temp_index1 = index - next_node.board.size()/2;
				
			next_node.board[(((temp_index1+1+i) % (next_node.board.size()-1)) 
				+ next_node.board.size()/2) % next_node.board.size()]++;
		}
		///////////////////////////////////////////////////////
		
		/////////////////// catch opponent stone ///////////////////////////
		int temp_index = index - next_node.board.size()/2;
		int temp_index_laststone = (temp_index+stones) % (next_node.board.size()-1);
		int index_laststone = (temp_index_laststone + next_node.board.size()/2)
			% next_node.board.size();
		if(next_node.board[index_laststone] == 1 && index_laststone > next_node.board.size()/2-1
			&& index_laststone < next_node.board.size()-1)
		{
			next_node.board[index_laststone] = 0; 
			next_node.board[next_node.board.size()-1]++; 
			next_node.board[next_node.board.size()-1] 
			 = next_node.board[next_node.board.size()-1]
			 + next_node.board[next_node.board.size()-2-index_laststone];  
			next_node.board[next_node.board.size()-2-index_laststone] = 0; 
		}
		////////////////////////////////////////////////////////////////
		
		////////////// heuristic //////////////////////////////
		next_node.value = next_node.board[next_node.board.size()-1]
		- next_node.board[next_node.board.size()/2-1];
		//////////////////////////////////////////////////////

		///////////////////////// end game ///////////////////////////
		vector<int> gameover1;
		for(int k=0; k<next_node.board.size()/2-1; k++)
		{
			if(next_node.board[k] == 0)
			{
				gameover1.push_back(0);
			}
		}

		if(gameover1.size() == next_node.board.size()/2-1)
		{
			for(int k=next_node.board.size()/2; k<next_node.board.size()-1; k++)
			{
				next_node.board[next_node.board.size()-1] += next_node.board[k]; 
				next_node.board[k] = 0; 
			}
				
			if(task == 2)
			{
				outfile_log << "A" << parent.board.size()-index << "," << depth << "," <<
					next_node.board[next_node.board.size()-1]
					 - next_node.board[next_node.board.size()/2-1] << endl;
			}
			if(task == 3)
			{
				outfile_log << "A" << parent.board.size()-index << "," << depth << "," <<
					next_node.board[next_node.board.size()-1]
						- next_node.board[next_node.board.size()/2-1] <<
							"," << print(alpha) << "," << print(beta) << endl;
			}
		
			value.v = next_node.board[next_node.board.size()-1]
					- next_node.board[next_node.board.size()/2-1];

			if(depth == 1)		
				value.child_board = next_node.board;

		    return value;
		}
		////////////////////////////////////////////////////////////////////////////

		///////////////////////// end game ///////////////////////////
		vector<int> gameover;
		for(int k=next_node.board.size()/2; k<next_node.board.size()-1; k++)
		{
			if(next_node.board[k] == 0)
			{
				gameover.push_back(0);
			}
		}
		if(gameover.size() == next_node.board.size()/2-1)
		{
			for(int k=0; k<next_node.board.size()/2-1; k++)
			{
				next_node.board[next_node.board.size()/2-1] += next_node.board[k];
				next_node.board[k] = 0;
			}

			if(task == 2)
			{
				outfile_log << "A" << parent.board.size()-index << "," << depth << "," <<
					next_node.board[next_node.board.size()-1]
					 - next_node.board[next_node.board.size()/2-1] << endl;
			}
			if(task == 3)
			{
				outfile_log << "A" << parent.board.size()-index << "," << depth << "," <<
					next_node.board[next_node.board.size()-1]
						- next_node.board[next_node.board.size()/2-1] <<
							"," << print(alpha) << "," << print(beta) << endl;
			}
						
			value.v = next_node.board[next_node.board.size()-1] - 
				next_node.board[next_node.board.size()/2-1];

			if(depth == 1)		
				value.child_board = next_node.board;

			return value;
		}
		////////////////////////////////////////////////////////////////////////////
		
		/////////////////////// one more chance //////////////////////////
		if( (stones+index) % (next_node.board.size()-1) == 0) 
		{
			if(task == 2)
			{
				outfile_log << "A" << parent.board.size()-index << "," << depth << "," << 
					print(-value.v) << endl;
			}
			if(task == 3)
			{
				outfile_log << "A" << parent.board.size()-index << "," << depth << "," << print(-value.v) 
					<< "," << print(alpha) << "," << print(beta) << endl;
			}

			Value tempValue = Value();
			value.v = -1000000;
	
			for(int j=0; j<pit_num; j++) 
			{
				if(next_node.board[next_node.board.size()-2-j] != 0) 
				{
					tempValue = Min_value1(next_node, next_node.board.size()-2-j, 
							pit_num, cutting_off_depth, depth, your_player, task, alpha, beta);

					if(tempValue.v > value.v)
					{
						value.v = tempValue.v;

						if(depth <= 1)
						{
							value.child_board = tempValue.child_board;
						}
					}

					if(task == 2)
					{
						outfile_log << "A" << parent.board.size()-index << "," << depth << "," << 
							print(value.v) << endl;
					}
					if(task == 3)
					{
						if(value.v >= beta)
						{
							outfile_log << "A" << parent.board.size()-index << "," << depth << "," 
									<< print(value.v) 
										<< "," << print(alpha) << "," << print(beta) << endl;
	
							return value;
						}

						alpha = max(alpha,value.v);

						outfile_log << "A" << parent.board.size()-index << "," << depth << 
							"," << print(value.v) << "," << print(alpha) << "," << print(beta) << endl;
					}
				}
			}
			return value;
		}
		///////////////////////////////////////////////////////////////


		if(cutting_off_depth != depth) 
		{
			if(task == 2)
			{
				outfile_log << "A" << parent.board.size()-index << "," << depth << "," << print(value.v) << endl;
			}
			if(task == 3)
			{
				outfile_log << "A" << parent.board.size()-index << "," << depth << "," << print(value.v) 
					<< "," << print(alpha) << "," << print(beta) << endl;
			}
		}
		else if(cutting_off_depth == depth) 
		{
			if(task == 2)
			{
				outfile_log << "A" << parent.board.size()-index << "," << depth << 
					"," << next_node.value << endl;
			}
			if(task == 3)
			{
				outfile_log << "A" << parent.board.size()-index << "," << depth << 
					"," << next_node.value << "," << print(alpha) << "," << print(beta) << endl;
			}

			if(depth == 1)		
				value.child_board = next_node.board;

			value.v = next_node.value;
			return value; 
		}
	
		
		for(int j=0; j<pit_num; j++) 
		{
			if(next_node.board[j] != 0) 
			{
				value.v = min(value.v,Max_value1(next_node, j, pit_num, cutting_off_depth, depth+1, your_player
					,task, alpha, beta).v);
				
				if(task == 2) 
				{
					outfile_log << "A" << parent.board.size()-index << "," << depth << "," << print(value.v) << endl;
				}
				
				if(task == 3) 
				{
					if(value.v <= alpha)
					{
						outfile_log << "A" << parent.board.size()-index << "," << depth << "," << print(value.v) 
							<< "," << print(alpha) << "," << print(beta) << endl;
	
						if(depth == 1)		
							value.child_board = next_node.board;

						return value;
					}

					beta = min(beta, value.v);

					outfile_log << "A" << parent.board.size()-index << "," << depth << "," << print(value.v) 
						<< "," << print(alpha) << "," << print(beta) << endl;
				}
			}
		}

		if(depth == 1)		
				value.child_board = next_node.board;

		return value;
}

Value Max_value1(Node parent, int index, int pit_num, int cutting_off_depth, int depth, int your_player
	,int task, int alpha, int beta) 
{
	if(task != 1)
		ofstream outfile_log("traverse_log.txt",ios::app);

		Value value;
		Node next_node = parent; 
		int stones = next_node.board[index]; 
		int temp_stones = stones; 
		next_node.board[index] = 0; 
		value.v = -1000000;

		/////////////////// move stone ///////////////////////////
		for(int i=0; i<temp_stones; i++) 
		{
			next_node.board[(index+1+i) % (next_node.board.size()-1)]++;
		}
		///////////////////////////////////////////////////////

		/////////////////// catch opponent stone ///////////////////////////
		int index_laststone = (index+stones) % (next_node.board.size()-1); 
		if(next_node.board[index_laststone] == 1 && index_laststone < next_node.board.size()/2-1)
		{
			next_node.board[index_laststone] = 0; 
			next_node.board[next_node.board.size()/2-1]++;
			next_node.board[next_node.board.size()/2-1] 
			 = next_node.board[next_node.board.size()/2-1]
			 + next_node.board[next_node.board.size()-2-index_laststone];  
			next_node.board[next_node.board.size()-2-index_laststone] = 0; 
		}
		////////////////////////////////////////////////////////////////

		////////////// heuristic //////////////////////////////
		next_node.value = next_node.board[next_node.board.size()-1]
			- next_node.board[next_node.board.size()/2-1]; 
		//////////////////////////////////////////////////////

		///////////////////////// end game ///////////////////////////
		vector<int> gameover1;
		for(int k=next_node.board.size()/2; k<next_node.board.size()-1; k++)
		{
			if(next_node.board[k] == 0)
			{
				gameover1.push_back(0);
			}
		}
		if(gameover1.size() == next_node.board.size()/2-1)
		{
			for(int k=0; k<next_node.board.size()/2-1; k++)
			{
				next_node.board[next_node.board.size()/2-1] += next_node.board[k]; 
				next_node.board[k] = 0; 
			}

			if(task == 2)
			{
				outfile_log << "B" << index+2 << "," << depth << "," << 
					next_node.board[next_node.board.size()-1] - 
						next_node.board[next_node.board.size()/2-1] << endl;
			}
			if(task == 3)
			{
				outfile_log << "B" << index+2 << "," << depth << "," << 
					next_node.board[next_node.board.size()-1] -
						next_node.board[next_node.board.size()/2-1]  
							<< "," << print(alpha) << "," << print(beta) << endl;
			}
						
			value.v = next_node.board[next_node.board.size()-1] - 
				next_node.board[next_node.board.size()/2-1];

			return value;
		}
		////////////////////////////////////////////////////////////////////////////

		///////////////////////// end game ///////////////////////////
		vector<int> gameover;
		for(int k=0; k<next_node.board.size()/2-1; k++)
		{
			if(next_node.board[k] == 0)
			{
				gameover.push_back(0);
			}
		}

		if(gameover.size() == next_node.board.size()/2-1)
		{
			for(int k=next_node.board.size()/2; k<next_node.board.size()-1; k++)
			{
				next_node.board[next_node.board.size()-1] += next_node.board[k];
				next_node.board[k] = 0; 
			}
				
			if(task == 2)
			{
				outfile_log << "B" << index+2 << "," << depth << "," << 
					next_node.board[next_node.board.size()-1] - 
						next_node.board[next_node.board.size()/2-1] << endl;
			}
			if(task == 3)
			{
				outfile_log << "B" << index+2 << "," << depth << "," << 
					next_node.board[next_node.board.size()-1] - 
						next_node.board[next_node.board.size()/2-1]  
							<< "," << print(alpha) << "," << print(beta) << endl;
			}
			
			value.v = next_node.board[next_node.board.size()-1]
						- next_node.board[next_node.board.size()/2-1];

		    return value;
		}
		////////////////////////////////////////////////////////////////////////////

		///////////////////// one more chance //////////////////////////
		if( (stones+index) % (next_node.board.size()-1) == next_node.board.size()/2 - 1) 
		{		
			if(task == 2)
			{
				outfile_log << "B" << index+2 << "," << depth << "," << print(-value.v) << endl;
			}
			if(task == 3)
			{
				outfile_log << "B" << index+2 << "," << depth << "," << print(-value.v) << "," 
					<< print(alpha) << "," << print(beta) << endl;
			}

			value.v = 1000000;

			for(int j=0; j<pit_num; j++) 
			{
				if(next_node.board[j] != 0) 
				{
					value.v = min(value.v, Max_value1(next_node, j, pit_num, cutting_off_depth, depth, your_player
						,task, alpha, beta).v); 
					
					if(task == 2)
					{
						outfile_log << "B" << index+2 << "," << depth << "," << print(value.v) << endl;
					}
					if(task == 3)
					{
						if(value.v <= alpha)
						{
							outfile_log << "B" << index+2 << "," << depth << "," << print(value.v) << 
								"," << print(alpha) << "," << print(beta) << endl;

							return value;
						}

						beta = min(beta, value.v);
						
						outfile_log << "B" << index+2 << "," << depth << "," << print(value.v) << ","
							<< print(alpha) << "," << print(beta) << endl;
					}
				}
			}
			
			return value;
		}
		////////////////////////////////////////////////////////////

		if(cutting_off_depth != depth)
		{
			if(task == 2)
			{
				outfile_log << "B" << index+2 << "," << depth << "," << print(value.v) << endl;
			}
			if(task == 3)
			{
				outfile_log << "B" << index+2 << "," << depth << "," << print(value.v) << 
					"," << print(alpha) << ","	<< print(beta) << endl;
			}
		}
		else if(cutting_off_depth == depth)
		{
			if(task == 2)
			{
				outfile_log << "B" << index+2 << "," << depth << "," << next_node.value << endl;
			}
			if(task == 3)
			{
				outfile_log << "B" << index+2 << "," << depth << "," << next_node.value << "," << 
					print(alpha) <<	"," << print(beta) << endl;
			}

			value.v = next_node.value;
			return value; 
		}
			
				
		for(int j=0; j<pit_num; j++) 
		{
			if(next_node.board[next_node.board.size()-2-j] != 0)
			{	
				value.v = max(value.v, Min_value1(next_node, next_node.board.size()-2-j, pit_num, 
						cutting_off_depth, depth+1, your_player, task, alpha, beta).v);

				if(task == 2)
				{
					outfile_log << "B" << index+2 << "," << depth << "," << print(value.v) << endl;
				}

				if(task == 3) 
				{ 
					
					if(value.v >= beta)
					{
						outfile_log << "B" << index+2 << "," << depth << "," << print(value.v) << 
							"," << print(alpha) << "," << print(beta) << endl;

						return value;
					}
					
					alpha = max(alpha, value.v);

					outfile_log << "B" << index+2 << "," << depth << "," << print(value.v) << 
						"," << print(alpha) << "," << print(beta) << endl;
				}
			}
		}
		return value;	
}	


int main(int argc, const char* argv[])
{
	outfile_next.open("next_state.txt",ofstream::out);
	
	ifstream infile(argv[2],ifstream::in);
	
	int task; // Greedy=1, Min_valueiMax_value=2, Alpha-Beta=3, Competition=4

	int your_player; // choose 1P or 2P
	int cutting_off_depth;
	int depth = 0; 

	vector<int> P1; 
	vector<int> P2; 

	int P2_number_of_stones; 
	int P1_number_of_stones; 

	vector<int> board;

	string space; 
	
	infile >> task;
	getline(infile,space);


	if(task != 1)
		outfile_log.open("traverse_log.txt",ofstream::out);


	infile >> your_player;
	getline(infile,space);

	infile >> cutting_off_depth;
	getline(infile,space); 

	string temp;
	int temp1;

	getline(infile,temp);
	if (temp[temp.size() - 1] == '\r')
		temp.resize(temp.size() - 1);
	
	istringstream ss(temp);
	while(!ss.eof())
	{
		int j;
		ss >> j;
		P2.push_back(j);
	}

	getline(infile,temp);
	if (temp[temp.size() - 1] == '\r')
		temp.resize(temp.size() - 1);
	
	istringstream ss1(temp);
	while(!ss1.eof())
	{
		int i;
		ss1 >> i;
		P1.push_back(i);
	}

	infile >> P2_number_of_stones;
	getline(infile,space); 

	infile >> P1_number_of_stones;
	getline(infile,space); 

	//////////// create boead //////////////
	for(int i=0; i<P1.size(); i++)
	{
		board.push_back(P1[i]);
	}
	board.push_back(P1_number_of_stones);
	for(int i=P1.size()-1; i>=0; i--)
	{
		board.push_back(P2[i]);
	}
	board.push_back(P2_number_of_stones); 
	////////////////////////////////////////

	Node root("root",board); 
	Node root1("root",board); 

	int alpha = -1000000;
	int beta = 1000000;
	
	if(your_player == 1)
	{
		if(task == 1) 
			cutting_off_depth = 1;

		Root_node(root, P2.size(), cutting_off_depth, depth, your_player, task, alpha, beta);
		
	}
	else if(your_player == 2)
	{
		if(task == 1)
			cutting_off_depth = 1;

		Root_node1(root, P2.size(), cutting_off_depth, depth, your_player, task, alpha, beta);
	}

	return 0;
}