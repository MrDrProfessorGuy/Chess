#include "map.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
//notmine
/* Type for definning the list */
typedef struct node {
    MapKeyElement key;
    MapDataElement data;
    struct node* next;
}*Node;

/* Type for definning the map which is imlpemented as a list with the first node as deme */
struct Map_t {
    Node head;
    Node iterator; // initialized to head at first
    int size;
    copyMapDataElements copyDataElements;
    copyMapKeyElements copyKeyElements;
    freeMapDataElements freeDataElements;
    freeMapKeyElements freeKeyElements;
    compareMapKeyElements compareKeyElements;
};



/* declaring functions */
static Node nodeAllocate();
static Node nodeCreate(Map map, MapKeyElement key, MapDataElement data);
static void nodeDestroy(Map map, Node node);
static Node nodeCopy(Map map, MapKeyElement key, MapDataElement data);
static Node mapContainsKey(Map map, MapKeyElement key);


/**
 * nodeAllocate: Allocates a new empty node. 
 *
 * @param - None.
 * @return
 * NULL - If allocation failed.
 * node_ptr - Pointer for the new node to be assigned to in case of succsses.
 */
static Node nodeAllocate() {
    Node node_ptr = malloc(sizeof(*node_ptr));
    if (!node_ptr) {
        return NULL;
    }
    
    node_ptr->key = NULL;
    node_ptr->data = NULL;
    node_ptr->next = NULL;
    return node_ptr;
}

/**
 * nodeCreate: create a new node holding a key-data pair 
 * using nodeCopy function in case it's not the deme node
 * pointer to next node is set to NULL by default
 * @param node_ptr - pointer for the new node to be assigned to 
 * @param key - the key element to assign
 * @param data - the data element to assign
 *
 * @return
 * NULL- If NULL pointer was sent to it or in case allocation memory failed
 * node_ptr- Pointer for the new node to be assigned to in case of succsses.
 */
static Node nodeCreate(Map map, MapKeyElement key, MapDataElement data) {
    if (!map) {
        return NULL;
    }
    //for the deme one we send nodeCreate(map,NULL,NULL)
    if (!data && !key) {
        //if it's not empty then it's not the deme one and NULL should be returned
        if (map->size != 0) {
            return NULL;
        }
        //else it's the deme one and needs to be allocated
        Node node_ptr = nodeAllocate();
        map->size++;
        return node_ptr;
    }

    //if we arrive here then only one of them is NULL and it's not the deme node
    if (!data || !key) {
        return NULL;
    }

    //if data and key aren't NULL and size isn't 0 then it's a new node and we must create a copy 
    //of it before inserting it
    Node node_ptr = nodeCopy(map, key, data);
    if (!node_ptr) {
        return NULL;
    }

    map->size++;
    return node_ptr;
}

/**
 * nodeCopy: create a new node holding a copied key-data pair
 * pointer to next node is set to NULL by default
 * @param node_ptr - pointer for the new copied node to be assigned to
 * @param key - the key element to copy and assign
 * @param data - the data element to copy and assign
 *
 * @return
 * NULL- If NULL pointer was sent to it or in case allocation memory failed
 * node_ptr- Pointer for the new copied node to be assigned to in case of succsses.
 */
static Node nodeCopy(Map map, MapKeyElement key, MapDataElement data) {
    
    Node node_ptr = nodeAllocate();
    if (!node_ptr) {
       return NULL;
    }
    //copy key
    MapKeyElement key_copy = map->copyKeyElements(key);
    if (!key_copy) {
        nodeDestroy(map, node_ptr);
        return NULL;
    }
    //copy data
    MapDataElement data_copy = map->copyDataElements(data);
    if (!data_copy) {
        //free what has alrady been allocated
        nodeDestroy(map, node_ptr);
        map->freeKeyElements(key_copy);
        return NULL;
    }
    //inser copies to the new node
    node_ptr->key = key_copy;
    node_ptr->data = data_copy;
    node_ptr->next = NULL;
    return node_ptr;
}

  
Map mapCreate(copyMapDataElements copyDataElement,
    copyMapKeyElements copyKeyElement,
    freeMapDataElements freeDataElement,
    freeMapKeyElements freeKeyElement,
    compareMapKeyElements compareKeyElements) {

    if (!(copyDataElement) || !(copyKeyElement) ||
        !(freeDataElement) || !(freeKeyElement) ||
        !(compareKeyElements)) {
        return NULL;
    }

    Map map = malloc(sizeof(*map));
    //allocation failed
    if (!(map)) {
        return NULL;
    }

    map->size = 0;//map is empty at first and it was put it here because nodeCreate addresses the size
    
    map->head = nodeCreate(map,NULL,NULL);
    //allocation failed
    if (!(map->head)) {
        free(map);//free what has already been allocated
        return NULL;
    }

    map->iterator = map->head; //head and iterator, at first, point to the head of the list which is the map
   
    map->copyDataElements = copyDataElement;
    map->copyKeyElements = copyKeyElement;
    map->freeDataElements = freeDataElement;
    map->freeKeyElements = freeKeyElement;
    map->compareKeyElements = compareKeyElements;

    return map;
}

void mapDestroy(Map map) {
    if (!map) {
        return;
    }
    mapClear(map);//certainly it doesn't return MAP_NULL_ARGUMENT  because NULL has already been checked above
    nodeDestroy(map, map->head);//destroy deme
    free(map);
}

Map mapCopy(Map map) {
    if (!map) {
        return NULL;
    }
    Map map_copy = mapCreate(map->copyDataElements, map->copyKeyElements, map->freeDataElements,
        map->freeKeyElements, map->compareKeyElements);
    if (!map_copy) {
        return NULL;
    }

    Node current = map->head->next;//we need the loop to start copying from the actual first node and not from 
                                   //the deme because the deme node is created along with map_copy

    while (current) {
        MapResult result = mapPut(map_copy, current->key, current->data);//start copy 
        if (result != MAP_SUCCESS) {
            mapDestroy(map_copy);
            return NULL;
        }
        current = current->next;
    }
    return map_copy;
}

int mapGetSize(Map map) {
    if (!map) {
        return -1;
    }

    return map->size-1;
}

static Node mapContainsKey(Map map, MapKeyElement key) {
    if (!map || !key || map->size<=1) {//if it's map->size==1 then it's only deme 
                                      
        return NULL;
    }

    Node current_node = map->head->next;
    while (current_node) {
        if (map->compareKeyElements(current_node->key, key) == 0) {
            return current_node;//no need to copy since it's an internal function 
                           //and we don't return it to the user
                           //also it's used for getKey function
                           //where you guys asked to return the key itself and not a copy of it     
        }
        current_node = current_node->next;
    }
    return NULL; //if we're here then key wasn't not found

}

bool mapContains(Map map, MapKeyElement element) {

    Node node = mapContainsKey(map, element);
    if (!node) {
        return 0;
    }
    return 1;
}

MapResult mapPut(Map map, MapKeyElement keyElement, MapDataElement dataElement) {
    if (!map || !keyElement || !dataElement) {
        return MAP_NULL_ARGUMENT;
    }
    int compare_keys;
    Node current_node = map->head;
    Node next_node = current_node->next;
    while (next_node) {
        compare_keys = map->compareKeyElements(next_node->key, keyElement);

        if (compare_keys == 0) {
            MapDataElement data_copy = map->copyDataElements(dataElement);
            if (!data_copy) {
                return MAP_OUT_OF_MEMORY;
            }
            map->freeDataElements(next_node->data);
            next_node->data = data_copy;
            return MAP_SUCCESS;
        }

        else if (compare_keys > 0) {
            Node new_node = nodeCreate(map, keyElement, dataElement); //data not copied data because in create it copies
            if (!new_node) {
                return MAP_OUT_OF_MEMORY;
            }
            //insert between current_node and next_node
            new_node->next = current_node->next;
            current_node->next = new_node;
            return MAP_SUCCESS;
        }
        current_node = next_node;//it's needed in case we need to insert the node not first
        next_node = next_node->next;
    }
    //if we're here then for sure a new node is supposed to be inserted last
    Node new_node = nodeCreate(map, keyElement, dataElement); //not copied data because in nodecreate copies them
    if (!new_node) {
        return MAP_OUT_OF_MEMORY;
    }
    current_node->next = new_node;  
    return MAP_SUCCESS;
}

MapDataElement mapGet(Map map, MapKeyElement keyElement) {
    if (!map || !keyElement) {
        return NULL;
    }

    Node node_key = mapContainsKey(map, keyElement);
    if (!node_key) {
        return NULL;
    }
    assert(node_key->data);//data is not supposed to be NULL as it's not allowed in mapPut
    return node_key->data; // no copy as required
}

static void nodeDestroy(Map map, Node node) {
    if (!map || !node) {
        return;
    }
    map->freeDataElements(node->data);
    map->freeKeyElements(node->key);
    free(node);
}

MapResult mapRemove(Map map, MapKeyElement keyElement) {
    if (!map|| !keyElement) {
        return MAP_NULL_ARGUMENT;
    }

    Node previous_node = map->head;
    Node next_node = previous_node->next;
    while (next_node) {
        if (map->compareKeyElements(next_node->key, keyElement) == 0) {
            previous_node->next = next_node->next;//link previous node to next node 
                                                      //before removing current iterator node
            nodeDestroy(map, next_node);
            map->size--;
            return MAP_SUCCESS;
        }
        previous_node = next_node;
        next_node = next_node->next;
    }
    assert(mapContains(map,keyElement)==0);
    return MAP_ITEM_DOES_NOT_EXIST;
}

MapKeyElement mapGetFirst(Map map) {
    if (!map) {
        return NULL;
    }

    map->iterator = map->head;
    MapKeyElement copy_key = mapGetNext(map);//next since the first one is deme
                                                 //assume success as requested

    return copy_key;
}
MapKeyElement mapGetNext(Map map) {
    if (!map || !map->iterator->next) {
        return NULL;
    }
    map->iterator = map->iterator->next;

    MapKeyElement copy_key = map->copyKeyElements(map->iterator->key);//assume success as requested                                         //assume success as requested

    return copy_key;
}

MapResult mapClear(Map map) {
    if (!map) {
        return MAP_NULL_ARGUMENT;
    }

    Node current_node = map->head;

    while (map->size>1) { //when it's one, we're left with the deme and we'll release it separately
        mapRemove(map, current_node->next->key);
    }

    assert(map->size == 1);//asserting it's 1 as supposed to be in order to remain only with
                          //the deme node so if the user wants to use mapPut after mapClear would be okay
    return MAP_SUCCESS;
}

