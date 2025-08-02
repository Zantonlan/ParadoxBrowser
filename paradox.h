#ifndef PARADOXH
#define PARADOXH

//typedefs
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
	Tag* parent; // can be null if tag has no parent.
};

//functions

//makes strings uppercase and returns the upper'd string
char* strup(char* str);

//reads files and returns a char* of the entire file
char* readfile(const char* filename);

#endif