#include"head.h"

LiteSQL::LiteSQL() {
	DataList = (DataListNode *)malloc(sizeof(DataListNode));
	DataList->DataName = NULL;
	DataList->List = NULL;
	DataList->next = NULL;
	ReadingFile = false;
	SelectTableStatus = DataList;
}
int LiteSQL::CheckType(char *type) {
	if(!strcmp(type, "int"))return sizeof(int);

	if(!strcmp(type, "long"))return sizeof(long);

	if(!strcmp(type, "double"))return sizeof(double);

	if(!strcmp(type, "folat"))return sizeof(float);

	if(!strcmp(type, "char"))return sizeof(char);

	return 0;
}
int LiteSQL::GetNameSize(SQLList *s, char *name) {
	for(int i = 0; i < s->length; i++) {
		if(!strcmp(s->name[i], name))return s->size[i];
	}

	return 0;
}
void *LiteSQL::GetNameType(SQLList *s, char *name) {
	for(int i = 0; i < s->length; i++) {
		if(!strcmp(s->name[i], name))return s->type[i];
	}

	return NULL;
}
int LiteSQL::ListSizeCount(SQLList *s) {
	int size = 0;

	for(int i = 0; i < s->length; i++)
		size += s->size[i];

	return size;
}
DATATYPE LiteSQL::GetSelectDataType(const char *name) {
	s = SelectStatus.Datalist->DataName;

	for(int i = 0; i < s->length; i++) {
		if(!strcmp(s->name[i], name)) {
			if(!strcmp(s->type[i], "int"))return INT;

			if(!strcmp(s->type[i], "long"))return LONG;

			if(!strcmp(s->type[i], "double"))return DOUBLE;

			if(!strcmp(s->type[i], "folat"))return FLOAT;

			if(!strcmp(s->type[i], "char"))return CHAR;
		}
	}

	return OTHER;
}