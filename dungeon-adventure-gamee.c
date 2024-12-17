#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_INVENTORY_SIZE 3
#define MAX_ROOM_DESC_LENGTH 100
#define MAX_ITEM_DESC_LENGTH 50
#define MAX_CREATURE_NAME_LENGTH 50
#define MAX_SAVE_NAME_LENGTH 50
#define MAX_TRAP_DESC_LENGTH 100

// Struct definitions
typedef struct Creature {
    char name[MAX_CREATURE_NAME_LENGTH];
    int health;
    int strength;
} Creature;

typedef enum {
    PHYSICAL_DAMAGE, // Fiziksel hasar
    POISON_DAMAGE,   // Zehir hasarı
    MAGIC_DAMAGE     // Büyüsel hasar
} DamageType;


typedef struct Trap {
    char description[MAX_TRAP_DESC_LENGTH];
    int damage;
    DamageType damage_type; // Hasar türü
} Trap;


typedef struct Room {
    char description[MAX_ROOM_DESC_LENGTH];
    char contains[MAX_ITEM_DESC_LENGTH];
    struct Room* up;
    struct Room* down;
    struct Room* left;
    struct Room* right;
    Creature* creature;
    Trap* trap;
} Room;

typedef enum {
    SWORD,        // Kılıç: +5 Strength
    BANDAGE,      // Sağlık Bandı: +20 Health
    ARMOR,        // Zırh: +10 Health
    XP_BOOST,     // XP Power: +5 Strength, +5 Health, +10 XP
    ITEM_NONE     // No item in the room
} ItemType;



typedef struct Player {
    int health;
    int strength;
    int xp;
    char inventory[MAX_INVENTORY_SIZE][MAX_ITEM_DESC_LENGTH];
    int inventory_count;
    Room* current_room;
} Player;

// Function prototypes
Room* create_room(const char* description, const char* item);
void connect_rooms(Room* room1, Room* room2, const char* direction);
Creature* create_creature(const char* name, int health, int strength);
Trap* create_trap(const char* description, int damage);
void populate_creatures(Room* rooms[], int room_count);
void populate_traps(Room* rooms[], int room_count);
void print_room(Room* room);
void print_player(Player* player);
void move_player(Player* player, const char* direction);
void add_item_to_inventory(Player* player, Room* room); 
void attack_creature(Player* player);
void save_game(Player* player, Room* rooms[], int room_count, const char* save_name);
Player* load_game(Room* rooms[], int room_count, const char* save_name);
void free_resources(Room* rooms[], int room_count, Player* player);
void end_game(Player* player);

// Main function
int main() {
    srand(time(NULL));

    // Create rooms
    Room* rooms[7];
    rooms[0] = create_room("You are in a dimly lit cave.", "SWORD" );
    rooms[1] = create_room("You are in a forest clearing." , "XP_BOOST");
    rooms[2] = create_room("You are in a ruined temple.", "BANDAGE");
    rooms[3] = create_room("You are in a dark dungeon.", "ITEM_NONE");
    rooms[4] = create_room("You are in an abandoned mansion.", "ARMOR");
    rooms[5] = create_room("You are on a misty mountain top.","BANDAGE");
    rooms[6] = create_room("You are in a deep underground chamber.", "ITEM_NONE");

    // Connect rooms (making sure all are connected in some way)
    connect_rooms(rooms[0], rooms[1], "right");
    connect_rooms(rooms[1], rooms[2], "down");
    connect_rooms(rooms[2], rooms[3], "left");
    connect_rooms(rooms[3], rooms[4], "up");
    connect_rooms(rooms[4], rooms[5], "right");
    connect_rooms(rooms[5], rooms[6], "down");
    connect_rooms(rooms[6], rooms[0], "left");  // Connecting back to room 0 to create a loop

    // Populate creatures
    populate_creatures(rooms, 7);

    // Populate traps
    populate_traps(rooms, 7);
    // Create player
    Player player = {100, 10, 0, {""}, 0, rooms[0]};

    // Display the welcome message
    printf("Welcome to the Dungeon Adventure Game!\n");
    printf("Player Info:\n");
    printf("- Health: %d\n", player.health);
    printf("- Strength: %d\n", player.strength);
    printf("- XP: %d\n", player.xp);
    printf("- Inventory Slots: %d\n\n", MAX_INVENTORY_SIZE);

    printf("Available Commands:\n");
    printf("- move [u/d/l/r]: Move up, down, left, or right\n");
    printf("- look: Inspect the current room\n");
    printf("- inventory: Show your inventory\n");
    printf("- attack: Engage in battle\n");
    printf("- save <filepath>: Save the game\n");
    printf("- load <filepath>: Load a saved game\n");
    printf("- exit: Exit the game\n\n");

    printf("Let the adventure begin!\n\n");

    // Main game loop
    char command[50];
    while (player.health > 0) {
        printf("> Command: ");
        scanf("%s", command);

        if (strcmp(command, "look") == 0) {
            print_room(player.current_room);
        } else if (strcmp(command, "move") == 0) {
            printf("Direction (up/down/left/right): ");
            char direction[10];
            scanf("%s", direction);
            move_player(&player, direction);
        } else if (strcmp(command, "inventory") == 0) {
            print_player(&player);
        } else if (strcmp(command, "pickup") == 0) {
            add_item_to_inventory(&player, player.current_room);
        } else if (strcmp(command, "attack") == 0) {
            attack_creature(&player);
        } else if (strcmp(command, "save") == 0) {
            printf("Save name: ");
            char save_name[MAX_SAVE_NAME_LENGTH];
            scanf("%s", save_name);
            save_game(&player, rooms, 4, save_name);
        } else if (strcmp(command, "load") == 0) {
            printf("Save name: ");
            char save_name[MAX_SAVE_NAME_LENGTH];
            scanf("%s", save_name);
            Player* loaded_player = load_game(rooms, 4, save_name);
            if (loaded_player) {
                player = *loaded_player;
                free(loaded_player);
            }
        } else if (strcmp(command, "exit") == 0) {
            free_resources(rooms, 4, &player);
            printf("Exiting game. Goodbye!\n");
            break;
        } else {
            printf("Unknown command. Try again.\n");
        }

        if (player.health <= 0) {
            end_game(&player);
            break;
        }
    }

    return 0;
}

// Function implementations

Room* create_room(const char* description, const char* item) {
    Room* room = (Room*)malloc(sizeof(Room));
    strcpy(room->description, description);
    if (item != NULL) {
        strcpy(room->contains, item);
    } else {
        room->contains[0] = '\0'; // Eğer item yoksa, boş bırak
    }
    room->up = room->down = room->left = room->right = NULL;
    room->creature = NULL;
    room->trap = NULL;
    return room;
}

void connect_rooms(Room* room1, Room* room2, const char* direction) {
    if (strcmp(direction, "up") == 0) {
        room1->up = room2;
        room2->down = room1;
    } else if (strcmp(direction, "down") == 0) {
        room1->down = room2;
        room2->up = room1;
    } else if (strcmp(direction, "left") == 0) {
        room1->left = room2;
        room2->right = room1;
    } else if (strcmp(direction, "right") == 0) {
        room1->right = room2;
        room2->left = room1;
    }
}


Creature* create_creature(const char* name, int health, int strength) {
    Creature* creature = (Creature*)malloc(sizeof(Creature));
    strcpy(creature->name, name);
    creature->health = health;
    creature->strength = strength;
    return creature;
}


Trap* create_trap(const char* description, int damage) {
    Trap* trap = (Trap*)malloc(sizeof(Trap));
    strcpy(trap->description, description);
    trap->damage = damage;
    return trap;
}

void populate_creatures(Room* rooms[], int room_count) {
    const char* creature_names[] = {
        "Goblin", "Orc", "Skeleton", "Troll", 
        "Dragon", "Vampire", "Zombie", "Werewolf", "Griffin", "Giant"
    };

    for (int i = 0; i < room_count; ++i) {
        if (rand() % 2 == 0) {
            // Randomly assign creatures with random health and strength values
            rooms[i]->creature = create_creature(creature_names[rand() % 10], 20 + rand() % 50, 5 + rand() % 20);
        }
    }
}


void populate_traps(Room* rooms[], int room_count) {
    const char* trap_descriptions[] = {"A hidden spike trap", "A swinging blade", "A poison dart trap", "A collapsing floor"};
    for (int i = 0; i < room_count; ++i) {
        if (rand() % 2 == 0) {
            rooms[i]->trap = create_trap(trap_descriptions[rand() % 4], 10 + rand() % 20);
        }
    }
}

void print_room(Room* room) {
    printf("\n%s\n", room->description);
    printf("Item here: %s\n", room->contains);
    if (room->creature) {
        printf("A %s is here! Health: %d, Strength: %d\n", room->creature->name, room->creature->health, room->creature->strength);
    } else {
        printf("No creatures here.\n");
    }
    if (room->trap) {
        printf("Beware! %s\n", room->trap->description);
    }
}

void print_player(Player* player) {
    printf("\nPlayer Stats:\n");
    printf("Health: %d\n", player->health);
    printf("Strength: %d\n", player->strength);
    printf("XP: %d\n", player->xp);
    printf("Inventory: ");
    for (int i = 0; i < player->inventory_count; ++i) {
        printf("%s ", player->inventory[i]);
    }
    printf("\n");
}

void show_possible_directions(Player* player) {
    printf("You can move in the following directions:\n");

    if (player->current_room->up != NULL) {
        printf("- up\n");
    }
    if (player->current_room->down != NULL) {
        printf("- down\n");
    }
    if (player->current_room->left != NULL) {
        printf("- left\n");
    }
    if (player->current_room->right != NULL) {
        printf("- right\n");
    }
}
void move_player(Player* player, const char* direction) {
    // Mevcut odadan gidebileceğiniz yönleri göster
    show_possible_directions(player);

    Room* new_room = NULL;

    if (strcmp(direction, "up") == 0) {
        new_room = player->current_room->up;
    } else if (strcmp(direction, "down") == 0) {
        new_room = player->current_room->down;
    } else if (strcmp(direction, "left") == 0) {
        new_room = player->current_room->left;
    } else if (strcmp(direction, "right") == 0) {
        new_room = player->current_room->right;
    }

    if (new_room) {
        player->current_room = new_room;
        printf("You moved to a new room: %s\n", new_room->description);
    } else {
        printf("Cannot move in that direction. There's no room in that direction.\n");
    }
}

void apply_item_effect(Player* player, ItemType item) {
    switch (item) {
        case SWORD:
            player->strength += 5;
            printf("Sword used! Strength +5\n");
            break;
        case BANDAGE:
            player->health += 20;
            printf("Healing Bandage used! Health +20\n");
            break;
        case ARMOR:
            player->health += 10;
            printf("Armor used! Health +10\n");
            break;
        case XP_BOOST:
            player->strength += 5;
            player->health += 5;
            player->xp += 10;
            printf("XP Power used! Strength +5, Health +5, XP +10\n");
            break;
        default:
            printf("Unknown item.\n");
            break;
    }
}


void add_item_to_inventory(Player* player, Room* room) {
    
    
    
    if (player->inventory_count < MAX_INVENTORY_SIZE) {
        // Eşyayı envantere ekle
        strcpy(player->inventory[player->inventory_count], room->contains);
        player->inventory_count++;
        printf("Picked up: %s\n", room->contains);
        
        // Odaya ait eşyayı enum'a çevir ve etkilerini uygula
        ItemType item = ITEM_NONE;  // Varsayılan olarak ITEM_NONE
        if (strcmp(room->contains, "SWORD") == 0) {
            item = SWORD;
        } else if (strcmp(room->contains, "BANDAGE") == 0) {
            item = BANDAGE;
        } else if (strcmp(room->contains, "ARMOR") == 0) {
            item = ARMOR;
        } else if (strcmp(room->contains, "XP_BOOST") == 0) {
            item = XP_BOOST;
        }

        apply_item_effect(player, item);

        // Odaya ait eşyayı sıfırla (artık odada olmayacak)
        strcpy(room->contains, "");
    } else {
        printf("Inventory is full!\n");
    }
}


void attack_creature(Player* player) {
    if (player->current_room->creature) {
        Creature* creature = player->current_room->creature;
        
        // Player attacks the creature
        int damage_dealt = player->strength;
        printf("You attack the %s for %d damage.\n", creature->name, damage_dealt);
        creature->health -= damage_dealt;

        // Check if creature is defeated
        if (creature->health <= 0) {
            printf("You defeated the %s!\n", creature->name);
            player->xp += 10;
            free(creature);
            player->current_room->creature = NULL;
        } else {
            printf("The %s has %d health left.\n", creature->name, creature->health);
            
            // Creature retaliates
            int creature_damage = creature->strength;
            player->health -= creature_damage;
            printf("The %s retaliates and deals %d damage to you!\n", creature->name, creature_damage);
            
            // Check if player is dead after retaliation
            if (player->health <= 0) {
                printf("You have been defeated by the %s. Game over.\n", creature->name);
                player->health = 0;  // Ensure health doesn't go below zero
            } else {
                printf("Your health is now %d.\n", player->health);
            }
        }
    } else {
        printf("No creatures to attack in this room.\n");
    }
}


void save_game(Player* player, Room* rooms[], int room_count, const char* save_name) {
    FILE* file = fopen(save_name, "wb");
    if (!file) {
        printf("Error saving game.\n");
        return;
    }

    // Player bilgilerini kaydet
    fwrite(player, sizeof(Player), 1, file);

    // Oda bilgilerini kaydet
    for (int i = 0; i < room_count; ++i) {
        fwrite(rooms[i], sizeof(Room), 1, file);
        
        // Her odadaki yaratıkları kaydet
        if (rooms[i]->creature != NULL) {
            fwrite(rooms[i]->creature, sizeof(Creature), 1, file);
        } else {
            // Yaratık yoksa NULL olarak kaydet
            Creature dummy = {0};
            fwrite(&dummy, sizeof(Creature), 1, file);
        }
        
        // Her odadaki tuzakları kaydet
        if (rooms[i]->trap != NULL) {
            fwrite(rooms[i]->trap, sizeof(Trap), 1, file);
        } else {
            // Tuzak yoksa NULL olarak kaydet
            Trap dummy_trap = {0};
            fwrite(&dummy_trap, sizeof(Trap), 1, file);
        }
    }

    fclose(file);
    printf("Game saved to %s.\n", save_name);
}

Player* load_game(Room* rooms[], int room_count, const char* save_name) {
    FILE* file = fopen(save_name, "rb");
    if (!file) {
        printf("Error loading game.\n");
        return NULL;
    }

    Player* player = (Player*)malloc(sizeof(Player));
    fread(player, sizeof(Player), 1, file);

    for (int i = 0; i < room_count; ++i) {
        fread(rooms[i], sizeof(Room), 1, file);

        // Yaratığı yükle
        Creature* creature = NULL;
        fread(&creature, sizeof(Creature), 1, file);
        if (creature->name[0] != '\0') {
            rooms[i]->creature = creature;  // Yaratığı odada yeniden oluştur
        } else {
            rooms[i]->creature = NULL;  // Yaratık yoksa NULL
        }

        // Tuzakları yükle
        Trap* trap = NULL;
        fread(&trap, sizeof(Trap), 1, file);
        if (trap->description[0] != '\0') {
            rooms[i]->trap = trap;  // Tuzak varsa odada tekrar oluştur
        } else {
            rooms[i]->trap = NULL;  // Tuzak yoksa NULL
        }
    }

    fclose(file);
    printf("Game loaded from %s.\n", save_name);
    return player;
}

void free_resources(Room* rooms[], int room_count, Player* player) {
    for (int i = 0; i < room_count; ++i) {
        free(rooms[i]);
    }
    free(player);
}

void end_game(Player* player) {
    printf("You have died. Game over.\n");
}
