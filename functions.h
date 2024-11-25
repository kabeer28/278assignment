// functions.h
#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#define MAX_USERS 10000
#define MAX_NAME_LEN 50
#define MAX_EMAIL_LEN 50
#define MAX_FRIENDS 100 // this is the example max friends per user damn dawg 100 is alot

typedef struct {
    int user_id;
    char name[MAX_NAME_LEN];
    char email[MAX_EMAIL_LEN];
    struct User *friends[MAX_FRIENDS];
    int friend_count;
} User;

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
