#include <stdio.h>
#include <stdlib.h>

typedef struct node node;
struct node {
	int value;
	node *next;
	node *previous;
};

typedef struct DoubleList DoubleList;
struct DoubleList {
	size_t size;
	node *head;
	node *tail;
};

DoubleList* hasmap_init()  {
	DoubleList *map = malloc(sizeof *map);
	if (map == NULL) {
		return map;
	}

	map->size = 0;
	map->head = NULL;
	map->tail = NULL;
	return map;
}

void double_list_free(DoubleList *map) {
	if (map != NULL) {
		while (map->tail != NULL) {
			node *previous = map->tail->previous;
			free(map->tail);
			map->tail = previous;
		}
		free(map);
	}
}

void double_list_dump(DoubleList *map) {
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

DoubleList* double_list_append(DoubleList* map, int value) {
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

DoubleList* double_list_prepend(DoubleList* map, int value) {
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

DoubleList* double_list_insert(DoubleList* map, int value, int index) {
	if (map == NULL) {
		return NULL;
	}

	if (index > map->size) {
		return NULL;
	}

	if (index == map->size) {
		return double_list_append(map, value);
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
		return double_list_prepend(map, value);
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

DoubleList* double_list_delete_at(DoubleList* map, int index) {
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

DoubleList* double_list_delete_value(DoubleList* map, int value) {
	if (map == NULL) {
		return NULL;
	}

	node *current_node = map->head;
	while (current_node) {
		node *next_node = current_node->next;
		if (current_node->value == value) {
			if (current_node->previous) {
				current_node->previous->next = next_node;
				if (next_node) {
					next_node->previous = current_node->previous;
				}
			}
			free(current_node);
			map->size--;
		}
		current_node = next_node;
	}
	return map;
}

node* double_list_find(DoubleList* map, int value) {
	if (map == NULL) {
		return NULL;
	}

	node *current_node = map->head;
	node *return_node = NULL;
	while (current_node && !return_node) {
		if (current_node->value == value) {
			break;
		}
		current_node = current_node->next;
	}
	return current_node;
}


int main() {
	
	printf("--- test prepend:\n");
	DoubleList *test_prepend_map = hasmap_init();

	double_list_prepend(test_prepend_map, 5);
	double_list_prepend(test_prepend_map, 3);
	double_list_prepend(test_prepend_map, 9);
	double_list_dump(test_prepend_map);

	double_list_free(test_prepend_map);

	printf("--- test append:\n");
	DoubleList *test_append_map = hasmap_init();

	double_list_append(test_append_map, 5);
	double_list_append(test_append_map, 7);
	double_list_append(test_append_map, 2);
	double_list_dump(test_append_map);

	double_list_free(test_append_map);

	printf("--- test append and prepend:\n");
	DoubleList *test_map = hasmap_init();

	double_list_append(test_map, 5);
	double_list_prepend(test_map, 8);
	double_list_append(test_map, 7);
	double_list_prepend(test_map, 1);
	double_list_append(test_map, 2);
	double_list_dump(test_map);

	double_list_free(test_map);

	printf("--- test insert and remove:\n");
	DoubleList *test_insert_map = hasmap_init();

	double_list_append(test_insert_map, 5);
	double_list_append(test_insert_map, 7);

	printf("insert 0 at position 0\n");
	double_list_insert(test_insert_map, 0, 0);
	double_list_dump(test_insert_map);

	printf("insert 9 at position 2\n");
	double_list_insert(test_insert_map, 9, 2);
	double_list_dump(test_insert_map);
	
	printf("insert 1 at position 1\n");
	double_list_insert(test_insert_map, 1, 1);
	double_list_dump(test_insert_map);
	
	printf("insert 8 at position 3\n");
	double_list_insert(test_insert_map, 8, 3);
	double_list_dump(test_insert_map);
	
	printf("insert 6 at position 5\n");
	double_list_insert(test_insert_map, 6, 5);
	double_list_dump(test_insert_map);

	printf("insert 8 after last element at position %lu\n", test_insert_map->size);
	double_list_insert(test_insert_map, 8, 7);
	double_list_dump(test_insert_map);

	printf("delete node at position 3\n");
	double_list_delete_at(test_insert_map, 3);
	double_list_dump(test_insert_map);

	printf("delete value 3\n");
	double_list_insert(test_insert_map, 3, 3);
	double_list_insert(test_insert_map, 3, 5);
	double_list_insert(test_insert_map, 3, 7);
	double_list_dump(test_insert_map);
	double_list_delete_value(test_insert_map, 3);
	double_list_dump(test_insert_map);

	
	printf("get node with value 5\n");
	node *test_node = double_list_find(test_insert_map, 5);
	if (test_node) {
		printf("node next: %d\n", test_node->next->value);
		printf("node previous: %d\n", test_node->previous->value);
	}

	double_list_free(test_insert_map);



	return 0;
}
