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

HashMAP* hasmap_init()  {
	HashMAP *map = malloc(sizeof *map);
	if (map == NULL) {
		return map;
	}

	map->size = 0;
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
		return NULL;
	}

	node *node_to_append = malloc(sizeof *node_to_append);
	if (node_to_append == NULL) {
		return NULL;
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
	map->size++;
	return map;
}

HashMAP* hashmap_prepend(HashMAP* map, int value) {
	if (map == NULL) {
		return NULL;
	}

	node *node_to_append = malloc(sizeof *node_to_append);
	if (node_to_append == NULL) {
		return NULL;
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
	map->size++;
	return map;
}

HashMAP* hashmap_insert(HashMAP* map, int value, int index) {
	if (map == NULL) {
		return NULL;
	}

	if (index > map->size) {
		return NULL;
	}

	if (index == map->size) {
		return hashmap_append(map, value);
	}

	node *node_to_replace = map->head;
	int i = 0;
	while (i < index) {
		if (!node_to_replace->next) {
			return NULL;
		}
		node_to_replace= node_to_replace->next;
		i++;
	}

	if (!node_to_replace->previous) {
		return hashmap_prepend(map, value);
	}

	node *node_to_append = malloc(sizeof *node_to_append);
	if (node_to_append == NULL) {
		return NULL;
	}
	node *previous_node = node_to_replace->previous;
	node *next_node = previous_node->next;
	node_to_append->value = value;

	previous_node->next = node_to_append;
	node_to_append->previous = previous_node;
	node_to_append->next = next_node;
	next_node->previous = node_to_append;

	map->size++;
	return map;
}

HashMAP* hashmap_delete_at(HashMAP* map, int index) {
	if (map == NULL) {
		return NULL;
	}

	if (index > map->size) {
		return NULL;
	}

	node *node_to_remove = map->head;
	int i = 0;
	while (i < index) {
		if (!node_to_remove->next) {
			return NULL;
		}
		node_to_remove= node_to_remove->next;
		i++;
	}
	node *previous_node = node_to_remove->previous;
	node *next_node = node_to_remove->next;

	previous_node->next = next_node;
	next_node->previous = previous_node;

	free(node_to_remove);
	map->size--;
	return map;
}

int main() {
	
	printf("--- test prepend:\n");
	HashMAP *test_prepend_map = hasmap_init();

	hashmap_prepend(test_prepend_map, 5);
	hashmap_prepend(test_prepend_map, 3);
	hashmap_prepend(test_prepend_map, 9);
	hashmap_dump(test_prepend_map);

	hashmap_free(test_prepend_map);

	printf("--- test append:\n");
	HashMAP *test_append_map = hasmap_init();

	hashmap_append(test_append_map, 5);
	hashmap_append(test_append_map, 7);
	hashmap_append(test_append_map, 2);
	hashmap_dump(test_append_map);

	hashmap_free(test_append_map);

	printf("--- test append and prepend:\n");
	HashMAP *test_map = hasmap_init();

	hashmap_append(test_map, 5);
	hashmap_prepend(test_map, 8);
	hashmap_append(test_map, 7);
	hashmap_prepend(test_map, 1);
	hashmap_append(test_map, 2);
	hashmap_dump(test_map);

	hashmap_free(test_map);

	printf("--- test insert and remove:\n");
	HashMAP *test_insert_map = hasmap_init();

	hashmap_append(test_insert_map, 5);
	hashmap_append(test_insert_map, 7);

	printf("insert 0 at position 0\n");
	hashmap_insert(test_insert_map, 0, 0);
	hashmap_dump(test_insert_map);

	printf("insert 9 at position 2\n");
	hashmap_insert(test_insert_map, 9, 2);
	hashmap_dump(test_insert_map);
	
	printf("insert 1 at position 1\n");
	hashmap_insert(test_insert_map, 1, 1);
	hashmap_dump(test_insert_map);
	
	printf("insert 8 at position 3\n");
	hashmap_insert(test_insert_map, 8, 3);
	hashmap_dump(test_insert_map);
	
	printf("insert 6 at position 5\n");
	hashmap_insert(test_insert_map, 6, 5);
	hashmap_dump(test_insert_map);

	printf("insert 8 after last element at position %lu\n", test_insert_map->size);
	hashmap_insert(test_insert_map, 8, 7);
	hashmap_dump(test_insert_map);

	printf("delete node at position 3\n");
	hashmap_delete_at(test_insert_map, 3);
	hashmap_dump(test_insert_map);

	hashmap_free(test_insert_map);

	return 0;
}
