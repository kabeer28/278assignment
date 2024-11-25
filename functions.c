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

// WRITE FUNCTIONS BELOW

User *users[MAX_USERS] = {NULL}; // this creates an array of users, initially all NULL
int current_user_id = 0;         // this is the global variable for auto-generating user IDs

// F'N this is to create a user with a unique user ID, name, and email
User* create_user(const char* name, const char* email) {

    if (current_user_id >= MAX_USERS) { // this is if there are already more than 10000 users
        printf("Maximum user limit reached.\n");
        return NULL;
    }

    // check for a unique name and email
//    for (int i = 0; i < current_user_id; i++) {
//        if (users[i] && (strcmp(users[i]->name, name) == 0 || strcmp(users[i]->email, email) == 0)) {
//            printf("Name or email already exists.\n");
//            return NULL;
//        }
//    }

    // allocate enough memory for new user and assign their corresponding values
    User *new_user = (User *)malloc(sizeof(User));
    new_user->user_id = current_user_id++;
    strncpy(new_user->name, name, MAX_NAME_LEN);
    strncpy(new_user->email, email, MAX_EMAIL_LEN);
    new_user->friend_count = 0;

    users[new_user->user_id] = new_user;
    return new_user;
}
// F'N add a friend relationship between user1 and user2
void add_friend(User *user1, User *user2) {
    if (user1->friend_count >= MAX_FRIENDS || user2->friend_count >= MAX_FRIENDS) {
        printf("Maximum friends limit reached for one of the users.\n");
        return;
    }

    user1->friends[user1->friend_count++] = user2;
    user2->friends[user2->friend_count++] = user1;
}

// F'N delete a user and then free memory
void delete_user(User *user) {
    if (user == NULL) return;

    // remove user from friends lists
    for (int i = 0; i < user->friend_count; i++) {
        User *friend = user->friends[i];
        for (int j = 0; j < friend->friend_count; j++) {
            if (friend->friends[j] == user) {
                friend->friends[j] = friend->friends[friend->friend_count - 1];
                friend->friend_count--;
                break;
            }
        }
    }

    // remove user from global users array and free memory
    users[user->user_id] = NULL;
    free(user);
}

// F'N print all users' names in alphabetical, ascending order
void print_users() {
    User *sorted_users[MAX_USERS];
    int count = 0;

    // collect all non-NULL users
    for (int i = 0; i < MAX_USERS; i++) {
        if (users[i] != NULL) {
            sorted_users[count++] = users[i];
        }
    }

    // sort users by name (alphabetical, ascending)
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (strcmp(sorted_users[i]->name, sorted_users[j]->name) > 0) {
                User *temp = sorted_users[i];
                sorted_users[i] = sorted_users[j];
                sorted_users[j] = temp;
            }
        }
    }

    // print the sorted user names
    for (int i = 0; i < count; i++) {
        printf("%s", sorted_users[i]->name);
        if (i < count - 1) printf(", ");
    }
    printf("\n");
}

// F'N search a user by their name
User* search_user_by_name(const char* name) {
    for (int i = 0; i < current_user_id; i++) {
        if (users[i] && strcmp(users[i]->name, name) == 0) {
            return users[i];
        }
    }
    return NULL;
}