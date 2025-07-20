#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "paradox.h"

char* readfile(const char* filename) {
	FILE* file = fopen(filename, "rb");
	if (!file) {fprintf(stderr, "File opening failed\n"); fflush(stderr); return NULL;}

	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	rewind(file);

	char* buffer = malloc(size+1);
	if (!buffer) {fclose(file); return NULL;}

	fread(buffer, 1, size, file);
	buffer[size] = '\0';

	fclose(file);
	return buffer;
}

void translateTag(Tag* tag) {

}

Tag* createTag(const char* name) {
	Tag* tag = malloc(sizeof(Tag));
	if (!tag) return NULL;

	tag->type = strdup(name);
	tag->attributes = NULL;
	tag->numOfAttributes = 0;
	tag->children = NULL;
	tag->numOfChildren = 0;

	return tag;
}

void addChild(Tag* parent, Tag* child) {
	Tag** temp;
	realloc(parent->children, (parent->numOfChildren+1)*sizeof(Tag*));
	if (temp == NULL) {fprintf(stderr, "Failed memory reallocation whilst attempting to add a child.\n"); fflush(stderr); exit(1);}
	
	parent->children = temp;
	parent->children[parent->numOfChildren] = child;
	parent->numOfChildren += 1;
}

Tag* createTagTree(const char* filename) {
	Tag* topTag = createTag("TOP");
	printf("%s\nlmnop\n", filename);
	char* file = readfile(filename);
	

	if (file==NULL) {
		fprintf(stderr, "Read file was null.\n");
		fflush(stderr);
		return NULL;
	}

	printf("Reading file:\n%s\n\n", file);

	size_t idx = 0;
	int nesting = 0;
	bool inTagDef = false;
	bool closingTag = false;
	char* currMode = "outside";
	Tag* parentTag = topTag;

	char* tagType = NULL;
	int tagTypeLen = 0;

	while (file[idx] != '\0') {
		printf("%c", file[idx]);
		
		switch(file[idx]) {
			case '<':
			{
				inTagDef = true;
				currMode = "name";
				break;
			}
			case '/':
			{
				closingTag = true;
				currMode = "name";
				break;
			}
			case '>':
			{
				if (!closingTag) nesting++; else nesting--;
				currMode = "outside";
				
				char* tempPtr = realloc(tagType, tagTypeLen+1);
				tagType = tempPtr;
				tagType[tagTypeLen] = '\0';
				printf("%s", tagType);

				//reset tag stuff
				tagType = realloc(tagType, 0);
				tagTypeLen = 0;

				closingTag = false;

				break;
			}
			default:
			{
				break;
			}
		}

		if ((!(file[idx] == '<' || file[idx] == '>' || file[idx] == '/')) && inTagDef) { //we're in a tag, now we need to build it.
			if (currMode == "name") {
				if (file[idx] == ' ') {
					currMode = "attributename";
				} else {
					char* tempPtr = realloc(tagType, tagTypeLen+1);
					tagType = tempPtr;
					tagType[tagTypeLen] = file[idx];
					tagTypeLen++;
				}
			}
		}
	
		idx++;
	}
	printf("\n");

	free(file);
	return topTag;
}