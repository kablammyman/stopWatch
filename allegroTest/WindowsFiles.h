#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sys/stat.h>

using namespace std;

class WindowsFiles
{
public:
	WindowsFiles(string mainPath);
	~WindowsFiles();
	vector<string> getFolders(string path);//needs to have *.fileExt to work
	int countFoldersinDir(string path);
	int myDeleteFile(string file, bool permanetDelete = false);
	string deleteAllFilesInDir(string path);
	string getRandomDir(string dir, bool useIgnoreList = true);
	string getRandomFileFromDir(string path);
	void addRangeToIgnoreList(vector<string> range);
	string getRandomDirFromMasterFolderList();
	string getRandomDirFromFolderList(vector<string> dirList);
	string getDirFromIndex(string baseDir, int index, bool useIgnoreList = true);
	vector<string> getMasterFolderList();
	vector<string> shuffleFolderList(int numFolders);
	vector<string> queueFolderList(int numFolders);
	vector<string> randomizeFolderList(int numFolders);
private:
	struct tm* clock;               // create a time structure
	struct stat attrib;         // create a file attribute structure
	int MAX_LOOP_COUNT;
	vector<string> masterFolderList;
	vector<string> newListOfDirs;
	vector<string> ignoreList;
	int getRandomNum(int min, int max);
	int getNextMasterListIndex(int curIndex);
	string toUpper(string word);
	bool checkForIgnorePathList(vector<string> ignoreList, string curPath);
	string prepPathForTraversal(string path);
	
};

