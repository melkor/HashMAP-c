#include <stdio.h>
#include <stdlib.h>

typedef struct node node;
struct node {
	int value;
	node *next;
	node *previous;
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
		while (map->tail != NULL) {
			node *previous = map->tail->previous;
			free(map->tail);
			map->tail = previous;
		}
		free(map);
	}
}

void hashmap_dump(HashMAP *map) {
	if (map == NULL) {
		return;
	}

	node *current_node = map->head;
	while (current_node) {
		printf("%d", current_node->value);
		if (current_node->next) {
			printf(" > ");
		}
		current_node = current_node->next;
	}
	printf("\n");
}

HashMAP* hashmap_append(HashMAP* map, int value) {
	if (map == NULL) {
		return map;
	}

	node *node_to_append = malloc(sizeof *node_to_append);
	if (node_to_append == NULL) {
		return map;
	}
	
	node_to_append->value = value;
	node_to_append->next = NULL;
	node_to_append->previous = map->tail;

	if (map->tail) {
		map->tail->next = node_to_append;
	}
	
	map->tail = node_to_append;

	if (!map->head) {
		map->head = node_to_append;
	}
	return map;
}

HashMAP* hashmap_prepend(HashMAP* map, int value) {
	if (map == NULL) {
		return map;
	}

	node *node_to_append = malloc(sizeof *node_to_append);
	if (node_to_append == NULL) {
		return map;
	}
	
	node_to_append->value = value;
	node_to_append->next = map->head;
	node_to_append->previous = NULL;

	if (map->head) {
		map->head->previous = node_to_append;
	}
	
	map->head = node_to_append;

	if (!map->tail) {
		map->tail = node_to_append;
	}
	return map;
}

int main() {
	
	// test only prepend
	HashMAP *test_prepend_map = hasmap_init(10);

	hashmap_prepend(test_prepend_map, 5);
	hashmap_prepend(test_prepend_map, 3);
	hashmap_prepend(test_prepend_map, 9);

	hashmap_dump(test_prepend_map);

	hashmap_free(test_prepend_map);

	// test only append
	HashMAP *test_append_map = hasmap_init(10);

	hashmap_append(test_append_map, 5);
	hashmap_append(test_append_map, 7);
	hashmap_append(test_append_map, 2);

	hashmap_dump(test_append_map);

	hashmap_free(test_append_map);

	return 0;
}
