#define byte uint8_t

#include "SHA256.h"
#include "hex2bin.h"

#include <iostream>
#include <ctime>
#include <string>
#include <sstream>

using namespace std;

struct book
{
	string title;
	string isbn;
	bool ability_to_share;
	bool checkOut;
}

//function to get currrent timestamp
long int unix_timestamp()
{
	time_t t = time(0);
	long int now = static_cast<long int> (t);
	return now;
}

//define block class
class Block
{
	public:
	int index;							//index of current block
	string timeStamp;				//time of creation of block
	string blockData;					//data of block
	string prevHash;				//hash of previous block
	string blockHash;				//hash of current block
	
	//function to compute hash of block
	string hashBlock()
	{
		//convert index from int to string
		string s = to_string(this->index);
		
		//create string to store all block data and append data into that data string
		string data;
		data.append(s);
		data.append(this->timeStamp);
		data.append(this->blockData);
		data.append(this->prevHash);	
		
		int endNum = checkHash(data);
		//cout << endNum << endl;
		
		data.append(to_string(endNum));

		//convert data string into const char array
		const char *cstr = data.c_str();
		//compute hash of data with SHA256
		this->blockHash = SHA256(cstr);
			
	}
	
	//proof of work function
	int checkHash(string data)
	{
		//int for how many zeros should each hash begin with
		int n = 10;
		//boolean for whether hash has been found or not
		bool properHash = false;
		//defining variables used in function
		string tmp;
		string checkHash;
		string check;
		string binHash;
		string zeros = "0000000000000000000000000000000000000000000000";
		//this is the special number
		int hashNum = 0;
		while(properHash == false)
		{
			//cout << hashNum << endl;
			
			//copy data into temporary variable
			tmp = data;
			// adding number to test hash
			tmp.append(to_string(hashNum));
			//hasing temp variable
			char * writable = new char[tmp.size() + 1];
			copy(tmp.begin(), tmp.end(), writable);
			writable[tmp.size()] = '\0';
			checkHash = SHA256(writable);
			binHash = hex_str_to_bin_str(checkHash);
			//check if first n characters of hash are 0s
			//cout << binHash << "\n" << checkHash << endl;
			
			if(binHash.substr(0, n) != zeros.substr(0, n))
			{
				//if not, increase hashNum by one and try again
				hashNum++;
			}
			else
				//if so, return the special number
				properHash = true;
			
			delete[] writable;		
		}
		cout << binHash << endl;
		return hashNum;
	}
	
};

//function to create next block
Block nextBlock(Block lastBlock, string tm)
{
	//define temporary block
	Block returnBlock;
	//assign data to block
	returnBlock.index = lastBlock.index+1;
	returnBlock.timeStamp = tm;
	returnBlock.blockData = "I'm block " + to_string(returnBlock.index);
	returnBlock.prevHash = lastBlock.blockHash;
	//return new block
	return returnBlock;
}

int main()
{
	
	//index of block to maintain order
	int GIndex = 0;
	
	//get current timestamp
	long int now = unix_timestamp();
	//create first block and assign data
	Block genesisBlock;
	genesisBlock.index = GIndex;
	genesisBlock.timeStamp = to_string(now);
	genesisBlock.blockData = "GenesisBlock";
	genesisBlock.prevHash = "0";
	genesisBlock.hashBlock();
	
	//make genesis block the previous block
	Block lastBlock = genesisBlock;
	
	
	
	/*//loop to create new blocks
	while(GIndex < 10)
	{
		//create new block to be processed
		Block currentBlock;
		//get current timestamp
		now = unix_timestamp();
		//assign current block data
		currentBlock = nextBlock(lastBlock, to_string(now));
		//compute hash of current block
		currentBlock.hashBlock();
		cout << currentBlock.blockData << " " << currentBlock.blockHash << endl;
		//make current block previous block
		lastBlock = currentBlock;
		//increase index by 1
		GIndex++;
	}*/
	
	return 0;
}