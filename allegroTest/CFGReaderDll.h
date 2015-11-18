
#include <string>
#include <vector>

#include "CFGReader.h"

using namespace std;

namespace CFG
{
	CFGReader cfgr;//can't use it as a member variable due to the static conflict
	class CFGReaderDLL
	{
			
	public:	
		static __declspec(dllexport) bool readCfgFile(string filePath, char delim);

		static __declspec(dllexport) bool getCfgBoolValue(string optionName);
		static __declspec(dllexport) int getCfgIntValue(string optionName);
		static __declspec(dllexport) float getCfgFloatValue(string optionName);
		static __declspec(dllexport) string getCfgStringValue(string optionName);
		static __declspec(dllexport) vector<string> getCfgListValue(string optionName);

		static __declspec(dllexport) bool getBoolValueFromList(string listName, string optionName);
		static __declspec(dllexport) int getIntValueFromList(string listName, string optionName);
		static __declspec(dllexport) float getFloatValueFromList(string listName, string optionName);
		static __declspec(dllexport) string getStringValueFromList(string listName, string optionName);

		static __declspec(dllexport) string getRawValueFromList(string listName, string optionName);//dont parse he csv data


	};
}//end namespace
