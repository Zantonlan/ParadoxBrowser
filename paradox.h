#ifndef PARADOXH
#define PARADOXH

typedef struct {
	char* name;
	char* value;
} Attribute;

typedef struct Tag Tag;
struct Tag {
	char* type;
	Attribute** attributes;
	int numOfAttributes;
	Tag** children; //can be null if there are no nested tags
	int numOfChildren;
};

#endif