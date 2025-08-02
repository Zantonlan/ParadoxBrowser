//parses html and 

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "paradox.h"

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
	tag->parent = NULL;

	return tag;
}

Attribute* createAttribute(const char* name, const char* value) {
	Attribute* attr = malloc(sizeof(Attribute));
	if (!attr) return NULL;

	attr->name = strdup(name);
	attr->value = strdup(value);

	return attr;
}

void addChild(Tag* parent, Tag* child) {
	Tag** temp = realloc(parent->children, (parent->numOfChildren+1)*sizeof(Tag*));
	if (temp == NULL) {fprintf(stderr, "Failed memory reallocation whilst attempting to add a child.\n"); fflush(stderr); exit(1);}
	
	parent->children = temp;
	parent->children[parent->numOfChildren] = child;
	parent->numOfChildren += 1;

	child->parent = parent;
}

void addAttribute(Attribute*** table, int* numOfAttributes, Attribute* attrToAdd) {
	Attribute** temp;
	temp = realloc(*table, (*numOfAttributes+1)*sizeof(Attribute*));
	if (temp == NULL) {fprintf(stderr, "Failed memory reallocation whilst attempting to add an attribute.\n"); fflush(stderr); exit(1);}

	*table = temp;
	*table[*numOfAttributes] = attrToAdd;
	*numOfAttributes += 1;
}

Tag* createTagTree(const char* filename) {
	Tag* topTag = createTag("TOP");
	printf("%s\nlmnop\n", filename);
	fflush(stdout);
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
	bool makingTextTag = false;
	char* currMode = "outside";
	Tag* parentTag = topTag;

	char* tagType = NULL;
	int tagTypeLen = 0;
	Attribute** attributes = NULL;
	int numberOfAttributes = 0;
	int amntAttr = 0;
	char* attributeName = NULL;
	int attrNameLen = 0;
	char* attributeValue = NULL;
	int attrValLen = 0;
	bool inAttrVal = false;

	while (file[idx] != '\0') {
		printf("%c", file[idx]);
		fflush(stdout);
		
		switch(file[idx]) {
			case '<':
			{
				if (makingTextTag) {
					//printf("%s", tagType);

					if (attributeName != NULL) {
						//printf("%s", attributeName);
					}

					if (attributeValue != NULL) {
						//printf("%s", attributeValue);
					}

					Tag* newTag = createTag(tagType);
					printf("a");
					fflush(stdout);
					Attribute** tagAttributes = malloc(sizeof(Attribute*)*numberOfAttributes);
					printf("b");
					fflush(stdout);
					memcpy(tagAttributes, attributes, sizeof(Attribute*)*numberOfAttributes);
					printf("c");
					fflush(stdout);
					newTag->attributes = tagAttributes;
					printf("d");
					fflush(stdout);
					newTag->numOfAttributes = numberOfAttributes;
					printf("e");
					fflush(stdout);

					free(tagType);
					tagType = NULL;
					tagTypeLen = 0;
					free(attributeName);
					attributeName = NULL;
					attrNameLen = 0;
					free(attributeValue);
					attributeValue = NULL;
					attrValLen = 0;
					numberOfAttributes = 0;
					inAttrVal = false;
					free(attributes);
					attributes = NULL;
					
					/*
					char* tempPtr = realloc(attributeValue, attrValLen+1);
					attributeValue = tempPtr;
					attributeValue[attrValLen] = '\0';
					Attribute* attr = createAttribute(attributeName, attributeValue);
					addAttribute(&attributes, &numberOfAttributes, attr);

					tempPtr = realloc(tagType, tagTypeLen+1);
					tagType = tempPtr;
					tagType[tagTypeLen] = '\0';

					Tag* newTag = createTag(tagType);
					Attribute** tagAttributes = malloc(sizeof(Attribute*)*numberOfAttributes);
					memcpy(tagAttributes, attributes, sizeof(Attribute*)*numberOfAttributes);
					newTag->attributes = tagAttributes;
					newTag->numOfAttributes = numberOfAttributes;

					free(tagType);
					tagType = NULL;
					tagTypeLen = 0;
					free(attributeName);
					attributeName = NULL;
					attrNameLen = 0;
					free(attributeValue);
					attributeValue = NULL;
					attrValLen = 0;
					numberOfAttributes = 0;
					inAttrVal = false;
					free(attributes);
					attributes = NULL;
					*/
				}

				inTagDef = true;
				currMode = "name";
				printf("It's a name");
				fflush(stdout);
				break;
			}
			case '/':
			{
				if (inTagDef) closingTag = true;
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

				if (attributeName != NULL) {
					printf("%s", attributeName);
				}

				if (attributeValue != NULL) {
					printf("%s", attributeValue);
				}

				if (!closingTag) {
					Tag* newTag = createTag(tagType);
					if (numberOfAttributes > 0) {
						Attribute** tagAttributes = malloc(sizeof(Attribute*)*numberOfAttributes);
						memcpy(tagAttributes, attributes, sizeof(Attribute*)*numberOfAttributes);
						newTag->attributes = tagAttributes;
						newTag->numOfAttributes = numberOfAttributes;
					}

					addChild(parentTag, newTag);
					if (!(strup(tagType) == "BR")) { //add more ors and stuff for every tag to exclude from the re-parenting
						parentTag = newTag;
					}
				} else {
					parentTag = parentTag->parent;
				}

				//reset tag stuff
				free(tagType);
				tagType = NULL;
				tagTypeLen = 0;
				free(attributeName);
				attributeName = NULL;
				attrNameLen = 0;
				free(attributeValue);
				attributeValue = NULL;
				attrValLen = 0;
				numberOfAttributes = 0;
				inAttrVal = false;
				free(attributes);
				attributes = NULL;

				closingTag = false;

				inTagDef = false;

				break;
			}
			default:
			{
				break;
			}
		}

		if (!(file[idx] == '<' || file[idx] == '>' || file[idx] == '/') && inTagDef) { //we're in a tag, now we need to build it.
			if (currMode == "name") {
				if (file[idx] == ' ') {
					currMode = "attributename";
				} else {
					char* tempPtr = realloc(tagType, tagTypeLen+1);
					tagType = tempPtr;
					tagType[tagTypeLen] = file[idx];
					tagTypeLen++;
				}
			} else if (currMode == "attributename") {
				if (file[idx] == '=') {
					char* tempPtr = realloc(attributeName, attrNameLen+1);
					attributeName = tempPtr;
					attributeName[attrNameLen] = '\0';

					currMode = "attributeval";
				} else {
					char* tempPtr = realloc(attributeName, attrNameLen+1);
					attributeName = tempPtr;
					attributeName[attrNameLen] = file[idx];
					attrNameLen++;
				}
			} else if (currMode == "attributeval") {
				if (file[idx] == '\"') {
					inAttrVal = !inAttrVal;

					if (!inAttrVal) {
						char* tempPtr = realloc(attributeValue, attrValLen+1);
						attributeValue = tempPtr;
						attributeValue[attrValLen] = '\0';
						Attribute* attr = createAttribute(attributeName, attributeValue);
						addAttribute(&attributes, &numberOfAttributes, attr);
					}
				} else if (file[idx] == ' ') {
					attributeName = realloc(attributeName, 0);
					attributeName = NULL;
					attrNameLen = 0;
					attributeValue = realloc(attributeValue, 0);
					attributeValue = NULL;
					attrValLen = 0;

					currMode = "attributename";
				} else {
					char* tempPtr = realloc(attributeValue, attrValLen+1);
					attributeValue = tempPtr;
					attributeValue[attrValLen] = file[idx];
					attrValLen++;
				}
			}
		} else if (!inTagDef && file[idx] != '>') {
			if (!makingTextTag && file[idx] != '\n' && file[idx] != '\t') {
				makingTextTag = true;
				tagType = "TEXT";
				tagTypeLen = strlen("TEXT");
				attributeName = "CONTENTS";
				attrNameLen = strlen("CONTENTS");
				numberOfAttributes = 1;
			}
			
			/*
			if (makingTextTag) {
				char* tempPtr = realloc(attributeValue, attrValLen+1);
				attributeValue = tempPtr;
				attributeValue[attrValLen] = file[idx];
				attrValLen++;
			}

			*/
		}
	
		idx++;
	}
	printf("\n");

	free(file);
	return topTag;
}