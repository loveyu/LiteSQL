#include"head.h"

bool LiteSQL::MemoryCopy(void *dst, const void *src, int count) {
	if(dst == NULL || src == NULL || dst == src)return false;

	void *dst2 = dst;
	count /= sizeof(char);

	while(count--) {
		*(char *)dst2 = *(char *)src;
		dst2 = (char *)dst2 + 1;
		src = (char *)src + 1;
	}

	return true;
}

void *LiteSQL::MemoryAlloc(SQLList *s) {
	return malloc(ListSizeCount(s));
}

void *LiteSQL::MemoryCount(SQLList *s, DataNode *data, const char *name) {
	int size = 0;

	for(int i = 0; i < s->length; i++) {
		if(!strcmp(s->name[i], name))break;

		size += s->size[i];
	}

	return data->data + size;
}

bool LiteSQL::FreeList(SQLList *p) {
	if(!p)return false;

	for(int i = 0; i < p->length; i++) {
		free(*(p->name + i));
		free(*(p->type + i));
	}

	free(p->size);
	free(p->name);
	free(p->type);
	free(p);
	return true;
}
bool LiteSQL::FreeDataList(DataListNode *p) {
	if(!p)return false;

	if(!FreeList(p->DataName))return false;

	DataNode *p2, *p3;
	p2 = p->List;

	while(p2) {
		p3 = p2->next;

		if(!FreeData(p2))return false;

		p2 = p3;
	}

	free(p);
	return true;
}
bool LiteSQL::FreeData(DataNode *p) {
	if(!p)return false;

	free(p->data);
	free(p);
	return true;
}