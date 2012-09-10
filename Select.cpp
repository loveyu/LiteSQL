#include"head.h"

bool LiteSQL::SelectTable(char *tablename) {
	DataListNode *p = NULL;

	if(!DataListSelectTable(tablename, &p)) {
		SelectStatus.Datalist = NULL;
		return false;
	}

	//设置当前操作的表状态地址
	SelectStatus.Datalist = p;
	SelectStatus.data = p->List;
	SelectStatus.WhereData = NULL;
	SelectStatus.WhereName = NULL;

	return true;
}
bool LiteSQL::Select() {
	if(!SelectStatus.Datalist)return false;

	SelectWhereClean();

	if(!SelectStatus.data || !SelectStatus.data->next)return false;
	else SelectStatus.data = SelectStatus.data->next;

	return true;
}
bool LiteSQL::Select(const void *data, char *name) {
	if(!Select())return false;

	SelectWhereMake(data, name);

	while(SelectStatus.data && SelectWhereCompare(SelectStatus.data)) {
		SelectStatus.data = SelectStatus.data->next;
	}

	if(!SelectStatus.data)return false;

	return true;
}
bool LiteSQL::SelectWhereClean() {
	if(!SelectStatus.WhereData)free(SelectStatus.WhereData);

	if(!SelectStatus.WhereName)free(SelectStatus.WhereName);

	return true;
}
bool LiteSQL::SelectWhereMake(const void *data, char *name) {
	int size;

	if(strlen(name) + 1 > ListNameSize)return false;

	size = GetNameSize(SelectStatus.Datalist->DataName, name);
	SelectStatus.WhereData = (char *)malloc(size);

	if(!SelectStatus.WhereData)return false;

	SelectStatus.WhereName = (char *)malloc(ListNameSize * sizeof(char));

	if(!SelectStatus.WhereName)return false;

	return (MemoryCopy(SelectStatus.WhereData, data, size)
	        && MemoryCopy(SelectStatus.WhereName, name, sizeof(char) * (strlen(name) + 1)));
}
int LiteSQL::SelectWhereCompare(DataNode *p) {
	if(!p)return -2;//匹配错误

	if(!SelectStatus.WhereData || !SelectStatus.WhereName)return 0;//无匹配，默认成功

	char type[ListTypeSize];
	strncpy(type, (char *)GetNameType(SelectStatus.Datalist->DataName, SelectStatus.WhereName), ListTypeSize);

	if(!strcmp(type, "int")) {
		if(*(int *)MemoryCount(SelectStatus.Datalist->DataName, p, SelectStatus.WhereName)
		        == *(int *)SelectStatus.WhereData)return 0;
	}

	if(!strcmp(type, "long")) {
		if(*(long *)MemoryCount(SelectStatus.Datalist->DataName, p, SelectStatus.WhereName)
		        == *(long *)SelectStatus.WhereData)
			return 0;
	}

	if(!strcmp(type, "double")) {
		if(*(double *)MemoryCount(SelectStatus.Datalist->DataName, p, SelectStatus.WhereName)
		        == *(double *)SelectStatus.WhereData )
			return 0;
	}

	if(!strcmp(type, "folat")) {
		if(*(float *)MemoryCount(SelectStatus.Datalist->DataName, p, SelectStatus.WhereName)
		        == *(float *)SelectStatus.WhereData )
			return 0;
	}

	if(!strcmp(type, "char")) {
		if(!strcmp((char *)MemoryCount(SelectStatus.Datalist->DataName, p, SelectStatus.WhereName),
		           (char *)SelectStatus.WhereData))
			return 0;
	}

	return -1;
}
bool LiteSQL::SelectNext() {
	if(!SelectStatus.data)return false;

	SelectStatus.data = SelectStatus.data->next;

	if(SelectStatus.WhereData && SelectStatus.WhereName) {
		while(SelectStatus.data && SelectWhereCompare(SelectStatus.data)) {
			SelectStatus.data = SelectStatus.data->next;
		}
	}

	if(!SelectStatus.data)return false;

	return true;
}
void *LiteSQL::SelectGet(const char *name) {
	if(!name || !SelectStatus.data || !SelectStatus.Datalist)return NULL;

	return MemoryCount(SelectStatus.Datalist->DataName, SelectStatus.data, name);
}
bool LiteSQL::SelectListName(char tablename[20]) {
	if(!SelectTableStatus->next) {
		SelectTableStatus = DataList;
		return false;
	}

	SelectTableStatus = SelectTableStatus->next;
	strncpy(tablename, SelectTableStatus->TableName, 20);
	return true;
}
int LiteSQL::SelectListFieldNumber() {
	return SelectStatus.Datalist->DataName->length;
}