#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

// Utility function to test parser
void testingParser(int arg1, char *arg2) {
    printf("The parser was called with arguments: %d and %s\n", arg1, arg2);
}

User* create_user(const char* name, const char* email); // int user_id is auto-generated to be unique
void add_friend(User* user1, User* user2); // users user1 and user2 are now friends
void delete_friend(User* user1, User* user2); // users user1 and user2 are no longer friends
void delete_user(User* user); // user is deleted
void print_users(); // prints all user names in ascending order

void change_user_name(User* user, char* new_name);
void change_user_email(User* user, char* new_email);
void print_friends(User* user); // prints user's friends in ascending order

User* search_user_by_name(const char* name);
User* search_user_by_email(const char* email);
User** mutual_friends(User* user1, User* user2); // returns an array of pointers to the mutual friends
void print_mutual_friends(User** friends); // prints mutual friends' user names in acsending order

Message* create_message(User* sender, User* receiver, const char* content); // int message_id is auto-generated to be unique
void print_message(Message* message);
void display_chat(User* user1, User* user2); // print messages in FIFO

Post* new_post(User* user, const char* content); // post id is auto-generated to be unique
void add_like(Post* post, User* user); // user is the individual who liked the post
void display_feed(User* user1);

HashMap user_map;  // user_id -> User*
HashMap name_map;  // name -> user_id
HashMap email_map; // email -> user_id
int current_user_id = 0; // starts at 0, auto incrementer (is that a word?) for user generation

// Initialize hashmaps
void init_maps() {
    init_hash_map(&user_map);
    init_hash_map(&name_map);
    init_hash_map(&email_map);
}

/* above ^ : this is so that my int main can utilize calling the hashmaps (create_user, delete_user, and other functions
can use put_in_hash, get_from_hash, and remove_from_hash. */

// WRITE FUNCTIONS BELOW

// INT SPECIFIC FUNCTIONS BRO
// this is the hash function to compute an index for a given key
unsigned int hash(const char* key) {
    unsigned int hash = 0;
    while (*key) {
        hash = (hash * 31) + *key++;  // simple hash multiplier
    }
    return hash % HASH_MAP_SIZE;
}

// this is to initialize the hash map
void init_hash_map(HashMap* map) {
    for (int i = 0; i < HASH_MAP_SIZE; i++) {
        map->table[i] = NULL;
    }
}

void put_in_hash_int(HashMap* map, int key, void* value) {
    unsigned int index = key % HASH_MAP_SIZE; // Hashing with modulo
    HashNode* node = map->table[index];
    while (node) {
        if (node->int_key == key) { // Check integer key
            node->value = value;  // Update value if the key exists
            return;
        }
        node = node->next;
    }
    // Add new node
    node = (HashNode*)malloc(sizeof(HashNode));
    node->int_key = key; // Assign integer key
    node->value = value;
    node->next = map->table[index];
    map->table[index] = node;
}

// this is to retrieve a value by key from the hash map
void* get_from_hash_int(HashMap* map, int key) {
    unsigned int index = key % HASH_MAP_SIZE; // using da moduloooooooooo
    HashNode* node = map->table[index];

    while (node) {
        if (node->int_key == key) {
            return node->value;
        }
        node = node->next;
    }
    return NULL; // sorry dawg not found :(
}

// this is to remove a key-value pair from the hash map
int remove_from_hash_int(HashMap* map, int key) {
    unsigned int index = key % HASH_MAP_SIZE; // hashing with modulooooooo
    HashNode* node = map->table[index];
    HashNode* prev = NULL;

    while (node) {
        if (node->int_key == key) {
            if (prev) {
                prev->next = node->next;
            } else {
                map->table[index] = node->next;
            }
            free(node);
            return 1; // successfully removed!!
        }
        prev = node;
        node = node->next;
    }
    return 0; // return 0, key not found sorry :(
}

/* before starting on the ACTUAL FUNCTIONS for adhering to the project guidelines, i've added implementation for the
HashMap functions in here since functions.h is shared by all source files and contains only declarations; whereas
functions.c contains the actual code for functions which basically ensures there’s only one definition during linking */

int hash_string_to_int(const char* key) {
    unsigned int hash = 0;
    while (*key) {
        hash = (hash * 31) + *key++; // compute hash using ASCII values
    }
    return hash % HASH_MAP_SIZE; // return an integer hash, cuz only ints around here
}

// F'N: this is to create a user with a unique user ID, name, and email
User* create_user(const char* name, const char* email) {
    int name_key = hash(name);
    int email_key = hash(email);

    // Check if name or email already exists
    if (get_from_hash_int(&name_map, name_key) != NULL || get_from_hash_int(&email_map, email_key) != NULL) {
        printf("Name or email already exists.\n");
        return NULL;
    }

    // Allocate memory for new user
    User* new_user = (User*)malloc(sizeof(User));
    if (!new_user) {
        printf("Memory allocation failed.\n");
        return NULL;
    }
    new_user->user_id = current_user_id++;
    strncpy(new_user->name, name, MAX_NAME_LEN);
    strncpy(new_user->email, email, MAX_EMAIL_LEN);
    new_user->friend_count = 0;

    put_in_hash_int(&user_map, new_user->user_id, new_user); // integer key bc we are using int :D
    put_in_hash_int(&name_map, name_key, new_user);          // integer key for name
    put_in_hash_int(&email_map, email_key, new_user);        // integer key for email

    return new_user;
}

// F'N: add a friend between user1 and user2... who knows how many friends user1 got... dont judge .-.
void add_friend(User *user1, User *user2) {
    if (user1->friend_count >= MAX_FRIENDS || user2->friend_count >= MAX_FRIENDS) {
        printf("Maximum friends limit reached for one of the users.\n");
        return;
    }

    // homie im just checking if yall are already friends
    for (int i = 0; i < user1->friend_count; i++) {
        if (user1->friends[i] == user2) {
            printf("%s and %s are already friends.\n", user1->name, user2->name);
            return;
        }
    }

    // add friends
    user1->friends[user1->friend_count++] = user2;
    user2->friends[user2->friend_count++] = user1;
    printf("%s and %s are now friends.\n", user1->name, user2->name);
}


// F'N delete a user and then free its memory
void delete_user(User* user) {
    if (!user) return;

    // here, yo boy is converting name and email to integer keys CUZ WE LOVE INTS
    int name_key = hash(user->name);
    int email_key = hash(user->email);

    // Remove user from friends' lists
    for (int i = 0; i < user->friend_count; i++) {
        User* friend = user->friends[i];
        for (int j = 0; j < friend->friend_count; j++) {
            if (friend->friends[j] == user) {
                // im now shifting the last friend to the current position and reduce the count
                friend->friends[j] = friend->friends[friend->friend_count - 1];
                friend->friend_count--;
                break;
            }
        }
    }

    // remove user from all hashmaps using integer keys
    remove_from_hash_int(&user_map, user->user_id);
    remove_from_hash_int(&name_map, name_key);
    remove_from_hash_int(&email_map, email_key);

    free(user);
    printf("User deleted successfully.\n");
}

// F'N print all users' names in alphabetical, ascending order
void print_users() {
    User* user_array[MAX_USERS];
    int count = 0;

    // sooooo collect users from hashmap
    for (int i = 0; i < HASH_MAP_SIZE; i++) {
        HashNode* node = user_map.table[i];
        while (node) {
            user_array[count++] = (User*)node->value;
            node = node->next;
        }
    }

    // thennnnn sort users by their name
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (strcmp(user_array[i]->name, user_array[j]->name) > 0) {
                User* temp = user_array[i];
                user_array[i] = user_array[j];
                user_array[j] = temp;
            }
        }
    }

    // LASTLY print the sorted names (abc order)
    for (int i = 0; i < count; i++) {
        printf("%s", user_array[i]->name);
        if (i < count - 1) printf(", ");
    }
    printf("\n");
}


// so ik its weird but print friends is different compared to print users...
void print_friends(User* user) {
    if (!user || user->friend_count == 0) {
        printf("%s has no friends.\n", user->name);
        return;
    }

    // copy friends to a temporary array
    User* friends[MAX_FRIENDS];
    for (int i = 0; i < user->friend_count; i++) {
        friends[i] = user->friends[i];
    }

    // here, we sort the friends by name
    for (int i = 0; i < user->friend_count - 1; i++) {
        for (int j = i + 1; j < user->friend_count; j++) {
            if (strcmp(friends[i]->name, friends[j]->name) > 0) {
                User* temp = friends[i];
                friends[i] = friends[j];
                friends[j] = temp;
            }
        }
    }

    // now we actually are printing friends' names
    for (int i = 0; i < user->friend_count; i++) {
        printf("%s", friends[i]->name);
        if (i < user->friend_count - 1) printf(", ");
    }
    printf("\n");
}


// F'N search a user by their name
User* search_user_by_name(const char* name) {
    int name_key = hash(name); // we convert name to integer key CUZ <3 INTS
    return (User*)get_from_hash_int(&name_map, name_key);
}

// F'N search a user by their email
User* search_user_by_email(const char* email) {
    int email_key = hash(email); // we convert email to integer key CUZ >3 INTS
    return (User *) get_from_hash_int(&email_map, email_key);
}