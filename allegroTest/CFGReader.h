#ifndef INC_CFGREADER_H
#define INC_CFGREADER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct cfgOption
{
	string optionName;
	vector<string> optionValueArray;
	string optionValue;
	bool isList;
};

class CFGReader
{
	public:
	bool readProfile(string file, char delim);
	bool getOptionBooleanValue(string optionName);
	int getOptionIntValue(string optionName);
	float getOptionFloatValue(string optionName);
	string getOptionStringValue(string optionName);
	vector<string> getOptionListValue(string optionName);
	
	//these methods are to grab individual data from a list we already read in
	bool getOptionListBooleanValue(string listName, string optionName);
	int getOptionListIntValue(string listName,string optionName);
	float getOptionListFloatValue(string listName,string optionName);
	string getOptionListStringValue(string listName,string optionName);
	string getOptionListRawString(string listName,string optionName);
	string getRawListData(string listName,string optionName);
	
	void clearCfgFile();
	string getAllStates();

	private:
		char delimiter;
		vector<cfgOption> cfgFile;
		cfgOption* getCfgOption(string optionName);
		string getListData(string listName,string optionName);
		bool getBooleanData(string data);
		int getIntData(string data);
		float getFloatData(string data);
		vector<int> getIntArrayData(string data);
		vector<float> getFloatArrayData(string data);
		vector<string> getStringArrayData(string data);
		//good for getting a particular piece of data from a csv line	
		int findInStringArray(vector<string> data, string searchItem);
		vector<string> getStringTokens(string origString, string delims);
		vector<string> getStringTokens(string origString, char delim);
		inline string trim(string& str)
		{
			str.erase(0, str.find_first_not_of(' '));       //prefixing spaces
			str.erase(str.find_last_not_of(' ')+1);         //surfixing spaces
			return str;
		}
		inline string StringToLower(string strToConvert)
		{
			string returnString = strToConvert;
			for(size_t i=0; i < strToConvert.size(); i++)
				returnString[i] = tolower(strToConvert[i]);
			return returnString;
		}
};
#endif //INC_CFGREADER_H