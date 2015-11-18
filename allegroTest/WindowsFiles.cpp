#include "WindowsFiles.h"


WindowsFiles::WindowsFiles(string mainPath)
{
	MAX_LOOP_COUNT = 5;
	masterFolderList = getFolders(mainPath.c_str());
}
//--------------------------------------------------------------------------------------------------------
WindowsFiles::~WindowsFiles()
{
	// erase the first 3 elements:
  masterFolderList.erase (masterFolderList.begin(),masterFolderList.end());
  newListOfDirs.erase (newListOfDirs.begin(),newListOfDirs.end());
  ignoreList.erase (ignoreList.begin(),ignoreList.end());

}
//--------------------------------------------------------------------------------------------------------
int WindowsFiles::getRandomNum(int min, int max)
{
//	if(rand()%10 > 5)
	{
		int diff = ((max - min) + 1);
		return ((diff * rand())/RAND_MAX) + min;
	}
//	return rand()%max + min;
}
//--------------------------------------------------------------------------------------------------------
int WindowsFiles::getNextMasterListIndex(int curIndex)
{
	if(curIndex < masterFolderList.size()-1)
		curIndex++;
	else 
		curIndex = 0;

	return curIndex;
}
//--------------------------------------------------------------------------------------------------------
vector<string> WindowsFiles::getFolders(string path)//needs to have *.fileExt to work
{
	HANDLE hFind;
	WIN32_FIND_DATA FindFileData;
	string curDir = prepPathForTraversal(path);
	char sTmp[MAX_PATH]="\0";
	vector<string> folderList;
	
	//make sure that the path ends with '\' otherwise, weid shit happens!


	hFind = FindFirstFile(curDir.c_str(), &FindFileData);

	int removeAst = curDir.length()-1;
	curDir.resize (removeAst); //curDir[removeAst]='\0';//remove the asterisk

	if(hFind != INVALID_HANDLE_VALUE)
	do{
		if( strcmp(FindFileData.cFileName,".") == 0 || strcmp(FindFileData.cFileName,"..") == 0)//ignore anything we put in this list
			continue;
 
		if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{		
			strcpy(sTmp, curDir.c_str());
			strcat(sTmp, FindFileData.cFileName);//cur dir + new folder name = new dir to explre
			strcat(sTmp, "\\");
			folderList.push_back(sTmp);
		}

	}while(FindNextFile(hFind, &FindFileData));
	FindClose(hFind);
	
	/*if(folderList.size() <= 0)//no dirs!
		return NULL;

	int index = rand()%folderList.size();
	return folderList[index];*/
	return folderList;
}
//--------------------------------------------------------------------------------------------------------
int WindowsFiles::countFoldersinDir(string path)
{
	HANDLE hFind;
	WIN32_FIND_DATA FindFileData;
	string curDir = prepPathForTraversal(path);
	char sTmp[MAX_PATH]="\0";
	int folderNum = 0;

	hFind = FindFirstFile(curDir.c_str(), &FindFileData);

	int removeAst = curDir.length()-1;
	curDir.resize (removeAst); //curDir[removeAst]='\0';//remove the asterisk

	if(hFind != INVALID_HANDLE_VALUE)
	do{
		if( strcmp(FindFileData.cFileName,".") == 0 || strcmp(FindFileData.cFileName,"..") == 0)
			continue;
 
		if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			folderNum++;


	}while(FindNextFile(hFind, &FindFileData));
	FindClose(hFind);
	
	return folderNum;
}
//--------------------------------------------------------------------------------------------------------
int WindowsFiles::myDeleteFile(string file, bool permanetDelete)
{
	if(permanetDelete)
	{
		//If the function succeeds, the return value is nonzero.
		//If the function fails, the return value is zero (0). To get extended error information, call GetLastError.
		return DeleteFile(file.c_str());
	}

	int len = file.size()+2;
    char* pathToFile = new char[len];
    strcpy(pathToFile, file.c_str());
    pathToFile[len-1] = pathToFile[len-2] = '\0';

	SHFILEOPSTRUCT operation;
	ZeroMemory(&operation, sizeof(operation));
	
	operation.wFunc = FO_DELETE;
	operation.pFrom = pathToFile;//"c:\\file\to\delete.txt";
	operation.fFlags = FOF_ALLOWUNDO|FOF_NO_UI|FOF_NORECURSION;

	//Returns zero if successful; otherwise nonzero. Applications normally should simply check for zero or nonzero.
	//http://msdn.microsoft.com/en-us/library/windows/desktop/bb762164(v=vs.85).aspx
	return SHFileOperation(&operation);

}
//--------------------------------------------------------------------------------------------------------
string WindowsFiles::deleteAllFilesInDir(string path)
{
	HANDLE hFind;
	WIN32_FIND_DATA FindFileData;
	string curDir =  prepPathForTraversal(path);
	char sTmp[MAX_PATH]="\0";
	string returnString = "";

	hFind = FindFirstFile(curDir.c_str(), &FindFileData);

	int removeAst = curDir.length()-1;
	curDir.resize (removeAst); //curDir[removeAst]='\0';//remove the asterisk

	if(hFind != INVALID_HANDLE_VALUE)
	do{
		if( strcmp(FindFileData.cFileName,".") == 0 || strcmp(FindFileData.cFileName,"..") == 0)//ignore anything we put in this list
			continue;
 
		if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			continue;
		else
		{
			string file = curDir + FindFileData.cFileName;
			if(myDeleteFile( file ) == false )
			{
				returnString += "Error deleting file: ";
				returnString += file;
				returnString += "\n";
			}
		}

	}while(FindNextFile(hFind, &FindFileData));
	FindClose(hFind);
	return returnString;
}
//--------------------------------------------------------------------------------------------------------
string WindowsFiles::getRandomFileFromDir(string path)
{
	HANDLE hFind;
	WIN32_FIND_DATA FindFileData;
	string curDir = prepPathForTraversal(path);
	char sTmp[MAX_PATH]="\0";

	vector<string> files;

	hFind = FindFirstFile(curDir.c_str(), &FindFileData);

	int removeAst = curDir.length()-1;
	curDir.resize (removeAst); //curDir[removeAst]='\0';//remove the asterisk

	if(hFind != INVALID_HANDLE_VALUE)
	do {
		if( strcmp(FindFileData.cFileName,".") == 0 || strcmp(FindFileData.cFileName,"..") == 0 || strcmp(FindFileData.cFileName,"Thumbs.db")==0)//ignore anything we put in this list
			continue;
 
		if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			continue;
		else
		{
			
			files.push_back(curDir + FindFileData.cFileName);
		}

	}while(FindNextFile(hFind, &FindFileData));
	FindClose(hFind);
	if(files.size() > 0)
	{
		//used to crash here
		int num = getRandomNum(0, files.size()-1);
		return files[num];
	}
	return ""; //this dir was empty, return empty string
}
//--------------------------------------------------------------------------------------------------------
string WindowsFiles::toUpper(string word)
{
	int i=0;

	char *newWord =new char[word.size()+1];
	char c;
	while (i < (int)word.size())
	{
		c=word[i];
		newWord[i] = toupper(c);
		newWord[i+1] = '\0';
		i++;
	}
	string returnWord = newWord;
	return returnWord;
}
//--------------------------------------------------------------------------------------------------------
bool WindowsFiles::checkForIgnorePathList(vector<string> ignoreList, string curPath)
{
	size_t found;
	string path = toUpper(curPath);

	for(int i = 0; i < (int)ignoreList.size(); i++)
	{
		string comp = toUpper(ignoreList[i]);
		
		found=path.find(comp);
		if (found!=string::npos && comp.size() > 1)
		{
			//cout << curPath << " vs: " << ignoreList[i];
			return true;
		}
	}
	return false;
}
//--------------------------------------------------------------------------------------------------------
vector<string> WindowsFiles::randomizeFolderList(int numFolders)
{
	cout << "getting random folders...\n";
	vector<string> folderList;

	for(int i = 0; i < numFolders; i++)
	{
		bool validFile = false;
		int loopCpunter = 0;
		do{
			if(loopCpunter >= MAX_LOOP_COUNT)
			{
				validFile = true;
				break;
			}

			int randomDir = getRandomNum(0,masterFolderList.size()-1);//doesnt seem to get last dir with the size() -1
			if(checkForIgnorePathList(ignoreList, masterFolderList[randomDir]))
			{
				validFile = false;
				loopCpunter++;
			}
			else
			{
				folderList.push_back(masterFolderList[randomDir]);
				validFile = true;
			}
			
		}while(!validFile);
	}
	return folderList;
}
//--------------------------------------------------------------------------------------------------------
vector<string> WindowsFiles::queueFolderList(int numFolders)
{
	vector<string> folderList;
	cout << "making a queue, starting from a random folder...\n";
	
	int curDir = getRandomNum(0,masterFolderList.size()-1);//doesnt seem to get last dir with the size() -1
	int seqIndex = 0;
	int *seqList = new int[masterFolderList.size()];
	for(int i = 0; i < numFolders; i++)
	{
		bool validFile = false;
		do{

			if(checkForIgnorePathList(ignoreList, masterFolderList[curDir]))
			{
				curDir = getNextMasterListIndex(curDir);
			}
			else
			{
				string dir = masterFolderList[curDir];
				if(seqList[curDir] < countFoldersinDir(dir) &&  seqList[curDir] > -1)	
					seqList[curDir] ++;
				else
					seqList[curDir] = 0;
				
				folderList.push_back(getDirFromIndex(dir, seqList[curDir]));
				validFile = true;
				if(curDir < masterFolderList.size()-1)
					curDir++;
				else
					curDir = 0;
			}
			
		}while(!validFile);
	}
	return folderList;
}
//--------------------------------------------------------------------------------------------------------
vector<string> WindowsFiles::shuffleFolderList(int numFolders)
{
	vector<string> folderList;
	cout << "shuffling folders...\n";
	//if this number is too big, make it smaller so we arnt in an endless loop
	if(numFolders > (masterFolderList.size() - ignoreList.size()) )
		numFolders = (masterFolderList.size() - ignoreList.size());

	for(int i = 0; i < numFolders; i++)
	{
		bool validFile = false;
		int loopCpunter = 0;
		int randomDir = getRandomNum(0,masterFolderList.size()-1);

		do{
			if(loopCpunter >= MAX_LOOP_COUNT)
			{
				validFile = true;
			}

			
			string curDir = masterFolderList[randomDir];
			
			if(checkForIgnorePathList(ignoreList,curDir))
			{
				validFile = false;
				loopCpunter++;
				randomDir = getNextMasterListIndex(randomDir);
			}
			else
			{
				bool isIncluded = false;
				for(int j = 0; j < (int)folderList.size(); j++)
				{
					if(folderList[j] == curDir)//if this dir is alraedy in the list, ignore it
					{
						isIncluded = true;
						loopCpunter++;
						break;
					}
				}
				if(!isIncluded)
				{
					folderList.push_back(curDir);
					validFile = true;
				}
			}
			
		}while(!validFile);
	}
	return folderList;
}
//--------------------------------------------------------------------------------------------------------
vector<string> WindowsFiles::getMasterFolderList()
{
	return masterFolderList;
}
//--------------------------------------------------------------------------------------------------------
string WindowsFiles::getRandomDirFromMasterFolderList()
{
	return masterFolderList[getRandomNum(0,masterFolderList.size()-1)];
}
//--------------------------------------------------------------------------------------------------------
string WindowsFiles::getRandomDirFromFolderList(vector<string> dirList)
{
	return dirList[getRandomNum(0,dirList.size()-1)];
}
//--------------------------------------------------------------------------------------------------------
string WindowsFiles::getDirFromIndex(string baseDir, int index, bool useIgnoreList)
{
		//short circuit for ignore stuff
	if(useIgnoreList)
	{
		if(checkForIgnorePathList(ignoreList, baseDir)) 
			return "";
	}

	vector<string> curDirList = getFolders(baseDir.c_str());
	if(index < curDirList.size()-1 && index > -1)
	{
		string returnDir = curDirList[index];
		if(useIgnoreList)
		{
			if(checkForIgnorePathList(ignoreList, returnDir)) 
				return "";
		}
		return returnDir;
	}

	//if we are here, then we prob had an invalid index
	return "";
}

//--------------------------------------------------------------------------------------------------------
//pics a dir from the master list, then "digs" down to get soething new
string WindowsFiles::getRandomDir(string dir, bool useIgnoreList)
{
	//short circuit for ignore stuff
	if(useIgnoreList)
	{
		if(checkForIgnorePathList(ignoreList, dir)) 
			return "";
	}

	int curDepth = 0;
	string curDir = dir;
	bool done = false;
	int loopCount = 0;
	vector<string> digPath; //to keep track of the way we get to our final folder

	while(!done)//keep "digging" until theres no more folders, just images remain
	{
		vector<string> curDirList = getFolders(curDir.c_str());
		
		if(curDirList.size() == 0)//there are no more folders
			done = true;
		else if(loopCount > 10)
		{
			curDir = "";//return an empty string, since no valid directorys were "found" (more like stumbled upon)
			done = true;
		}

		else
		{
			int nextDir = 0;
			if(curDirList.size()-1 > 1)
				nextDir = getRandomNum(0, curDirList.size()-1);
			string newDir = curDirList[nextDir];

			if(useIgnoreList)
			{
				if(checkForIgnorePathList(ignoreList, newDir)) //if we find something to ignore
				{
					loopCount++;
					continue;//try again
				}
				else
				{
					curDir = newDir;//oitehrwise, keep digging
					loopCount = 0;//reset loop count
				}
			}
			else
				curDir = newDir;//no directory is off limits

		}
	}
	return curDir;
}

//--------------------------------------------------------------------------------------------------------
void WindowsFiles::addRangeToIgnoreList(vector<string> range)
{
	for(int i = 0; i < (int)range.size(); i++)
		ignoreList.push_back(range[i]);
}
//--------------------------------------------------------------------------------------------------------
string WindowsFiles::prepPathForTraversal(string path)
{
	char lastChar = *path.rbegin();
	string newPath = path;
	if(lastChar == '\\')
		newPath += "*";//add asterisk to look for all "files" in cur dir
	else
		newPath += "\\*";//if we forgot to add the last back slash, add it, so we dont crash
	return  newPath;
}