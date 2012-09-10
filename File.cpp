#include"head.h"
bool LiteSQL::Read() {
	ReadingFile = true;

	if(DataList->next)return false;

	if(!FileOpenSystem())return false;

	ReadingFile = false;
	return true;
}
bool LiteSQL::FileOpenSystem() {
	FILE *SystemFile;
	char SystemFilePath[280];

	if(!FilePathGet(SystemFilePath, "info.sd"))return false;

	SystemFile = fopen(SystemFilePath, "rb");

	if(!SystemFile) return false;

	char tmp[20];

	while(fread(tmp, sizeof(char), 20, SystemFile) == 20) {
		FileOpenDataList(tmp);
	}

	return true;
}
bool LiteSQL::FileOpenDataList(const char *name) {
	if(!name)return false;

	FILE *ListFile;
	long FileSize;
	char ListFilePath[280];
	char tmp[24];
	strncpy(tmp, name, 20);

	if(!FilePathGet(ListFilePath, strcat(tmp, ".ls")))return false;

	ListFile = fopen(ListFilePath, "rb");

	if(!ListFile)return false;

	FileSize = FileCountSize(ListFilePath);

	if(FileSize < (long)((ListNameSize + ListTypeSize)*sizeof(char) + sizeof(int)))return false;

	int l = (int)(FileSize / ((ListNameSize + ListTypeSize) * sizeof(char) + sizeof(int)));

	if(!CreatList(l))return false;

	char datatmp[((ListNameSize+ListTypeSize)*sizeof(char)+sizeof(int))/sizeof(char)];

	for(int i = 0; i < l; i++) {
		fread(datatmp, sizeof(char), ((ListNameSize + ListTypeSize)*sizeof(char) + sizeof(int)) / sizeof(char), ListFile);
		CreatListData(datatmp, LName, i);
		CreatListData(datatmp + ListNameSize, LSize, i);
		CreatListData(datatmp + ListNameSize + sizeof(int) / sizeof(char), LType, i);
	}

	//CreatListDebugOut();
	if(!CreatListAddList(name))return false;

	if(fclose(ListFile))return false;

	return FileOpenData(name);
}
bool LiteSQL::FileOpenData(const char *name) {
	FILE *DataFile;
	char DataFilePath[280];
	char tmp[24];
	strncpy(tmp, name, 20);

	if(!FilePathGet(DataFilePath, strcat(tmp, ".dat")))return false;

	DataFile = fopen(DataFilePath, "rb");

	if(!DataFile)return false;

	int CountSize = ListSizeCount(s);
	char *DataTmp;
	DataTmp = (char *)malloc(CountSize);

	while(fread(DataTmp, CountSize, 1, DataFile)) {
		CreatData(name);

		if(!MemoryCopy(status.data->data, DataTmp, CountSize))return false;
	}

	free(DataTmp);

	if(fclose(DataFile))return false;

	return true;
}
bool LiteSQL::FileSetPath(char *path) {
	if(!path || strlen(path) > 255) {
		strncpy(DataFileDir, "", 255);
		return false;
	}

	strncpy(DataFileDir, path, 255);

	if(!FileExists(DataFileDir))FileMakeDir(DataFileDir);

	return true;
}

bool LiteSQL::FileWriteSystem() {
	if(ReadingFile)return true;//文件读取中放弃操作

	FILE *SystemFile;
	char SystemFilePath[280];

	if(!FilePathGet(SystemFilePath, "info.sd"))return false;

	SystemFile = fopen(SystemFilePath, "wb");

	if(!SystemFile) return false;

	DataListNode *node = DataList;

	while(node->next) {
		node = node->next;
		fwrite(node->TableName, sizeof(char), 20, SystemFile);
	}

	if(!fclose(SystemFile) && !node->next)return true;
	else return false;
}

bool LiteSQL::FilePathGet(char *path, const char *filename) {
	if(!filename || strlen(filename) > 23 || strlen(filename) < 1)return false;

	strcpy(path, DataFileDir);

	if(strlen(DataFileDir) > 0 && DataFileDir[strlen(DataFileDir)-1] != '\\')
		strcat(path, "\\");

	strcat(path, filename);
	return true;
}
bool LiteSQL::FileExists(const char *path) {
	if( (_access( path, 0 )) != -1 )return true;
	else return false;
}
long LiteSQL::FileCountSize(const char *pathname) {
	if(!pathname)return false;

	FILE *ListFile;
	ListFile = fopen(pathname, "rb");

	if(!ListFile)return -1;

	fseek(ListFile, 0, SEEK_END);
	return  ftell(ListFile);
}
bool LiteSQL::FileMakeDir(const char *path) {
	if(!path)return false;

	int i = 0;
	char tmp1[256], tmp2[256];
	strncpy(tmp1, path, 256);

	while(tmp1[i] && i < 256) {
		if(tmp1[i] == '\\') {
			tmp1[i] = '\0';
			strcpy(tmp2, tmp1);
			tmp1[i] = '\\';

			if(!FileExists(tmp2))mkdir(tmp2);
		}

		++i;
	}

	return true;
}
bool LiteSQL::FileWriteDataList(DataListNode *data) {
	if(ReadingFile)return true;//文件读取中放弃操作

	if(!data)return false;

	FILE *ListFile;
	char ListFilePath[280];
	char tmp[24];
	strncpy(tmp, data->TableName, 20);

	if(!FilePathGet(ListFilePath, strcat(tmp, ".ls")))return false;

	ListFile = fopen(ListFilePath, "wb");

	if(!ListFile)return false;

	for(int i = 0; i < data->DataName->length; ++i) {
		fwrite(data->DataName->name[i], sizeof(char), ListNameSize, ListFile);
		fwrite(data->DataName->size + i, sizeof(int), 1, ListFile);
		fwrite(data->DataName->type[i], sizeof(char), ListTypeSize, ListFile);
	}

	return !fclose(ListFile);
}
bool LiteSQL::FileWriteData(DataListNode *data) {
	if(ReadingFile)return true;//文件读取中放弃操作

	if(!data || !data->List->next)return false;

	FILE *DataFile;
	char DataFilePath[280];
	char tmp[24];
	DataNode *node = data->List->next;
	strncpy(tmp, data->TableName, 20);

	if(!FilePathGet(DataFilePath, strcat(tmp, ".dat")))return false;

	DataFile = fopen(DataFilePath, "wb");

	if(!DataFile)return false;

	do {
		fwrite(node->data, ListSizeCount(data->DataName), 1, DataFile);
		node = node->next;
	} while(node);

	return !fclose(DataFile);
}
bool LiteSQL::FileDeleteTable(const char *tablename) {
	if(ReadingFile)return true;//文件读取中放弃操作

	char path[280];
	char tmp[24];
	strncpy(tmp, tablename, 20);

	if(!FilePathGet(path, strcat(tmp, ".ls")))return false;

	if(remove(path) != 0)return false;

	strncpy(tmp, tablename, 20);

	if(!FilePathGet(path, strcat(tmp, ".dat")))return false;

	if(remove(path) != 0)return false;

	return true;
}
void LiteSQL::FileReadingStatusChange() {
	ReadingFile = !ReadingFile;
}