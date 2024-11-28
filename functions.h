// functions.h
#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#define MAX_USERS 10000
#define HASH_MAP_SIZE 10000 // this is size of the hash map (basically number of buckets in the table).
#define MAX_NAME_LEN 50
#define MAX_EMAIL_LEN 50
#define MAX_FRIENDS 100 // this is the example max friends per user damn dawg 100 is alot
#include <stdlib.h>
#include <string.h>

typedef struct HashNode {
    char key[MAX_NAME_LEN];  // unique key (for name or email)
    int int_key;              // key for integer-based hashmaps (user_map)
    void* value;              // value (e.g., user_id, or any value we want to associate with the key)
    struct HashNode* next;   // pointer for the next hashnode (for chaining in case of collisions, linkedlisting lol)
} HashNode;

typedef struct HashMap { // defining the hashmap itself
    HashNode* table[HASH_MAP_SIZE];
} HashMap;

// essential HashMap functions
unsigned int hash(const char* key);

void put_in_hash(HashMap* map, const char* key, void* value);
void* get_from_hash(HashMap* map, const char* key);
int remove_from_hash(HashMap* map, const char* key);

void init_hash_map(HashMap* map);
void put_in_hash_int(HashMap* map, int key, void* value); // MAKING SURE THE KEYS ARE INTS BRO AHHHHHHH
void* get_from_hash_int(HashMap* map, int key);
int remove_from_hash_int(HashMap* map, int key);

typedef struct User {
    int user_id;
    char name[MAX_NAME_LEN];
    char email[MAX_EMAIL_LEN];
    struct User *friends[MAX_FRIENDS];
    int friend_count;
} User;

// global HashMaps for user management, i learned that using 'extern' is helpful when calling variables from other files
extern HashMap user_map;  // maps user_id -> User*
extern HashMap name_map;  // maps name -> user_id
extern HashMap email_map; // maps email -> user_id

// initialize all hashmaps woohoo!
void init_maps();

typedef struct {
    // add attributes
} Post;

typedef struct {
    // add attributes
} Message;

void testingParser(int arg1, char *arg2);
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

#endif
