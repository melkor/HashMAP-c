#include <stdlib.h>

typedef struct node node;
struct node {
	int value;
	node *next_node;
	node *previous_node;
};

typedef struct HashMAP HashMAP;
struct HashMAP {
	size_t size;
	node *head;
	node *tail;
};

HashMAP* hasmap_init(size_t size)  {
	HashMAP *map = malloc(sizeof *map);
	if (map == NULL) {
		return map;
	}

	map->size = size;
	map->head = NULL;
	map->tail = NULL;
	return map;
}

void hashmap_free(HashMAP *map) {
	if (map != NULL) {
		free(map);
	}
}

int main() {
	HashMAP *map = hasmap_init(10);
	hashmap_free(map);
	return 0;
}
