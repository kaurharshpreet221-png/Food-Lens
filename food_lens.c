#include <stdio.h>
#include <string.h>

#define MAX_INGREDIENTS 100 //max no of ingredients in DB
#define MAX_NAME 100  //max length of ingredient name
#define MAX_TEXT 200 //max length of description in report

//structure
struct Ingredient {
    char name[MAX_NAME];
    char category[MAX_NAME];
    char function[MAX_TEXT];
    char awareness[MAX_TEXT];
};

// structure: result ke liye
struct Product {
    char productName[MAX_NAME];
    char ingredientNames[MAX_INGREDIENTS][MAX_NAME];
    int databaseIndex[MAX_INGREDIENTS];
    int total;
    int found;
    int notFound;
};

struct Ingredient database[MAX_INGREDIENTS];
int databaseCount = 0;

// spaces remove krega
void trim(char text[]) {
    int start = 0;
    int end;
    int i;

    while (text[start] == ' ' || text[start] == '\t') {
        start++;
    }

    if (start > 0) {
        for (i = 0; text[i] != '\0'; i++) {
            text[i] = text[i + start];
        }
    }

    end = (int)strlen(text) - 1;

    while (end >= 0 &&
          (text[end] == ' ' || text[end] == '\t' ||
           text[end] == '\n' || text[end] == '\r')) {
        text[end] = '\0';
        end--;
    }
}

//convert inputs into lowercase taaki case sensitive na ho
char lowerChar(char ch) {
    if (ch >= 'A' && ch <= 'Z') {
        return ch + ('a' - 'A');
    }

    return ch;
}

//compare two strings 
int sameText(char first[], char second[]) {
    int i = 0;

    while (first[i] != '\0' && second[i] != '\0') {
        if (lowerChar(first[i]) != lowerChar(second[i])) {
            return 0;
        }

        i++;
    }

    return first[i] == '\0' && second[i] == '\0';
}

//database load krega ingredient wali file se
void loadDatabase() {
    FILE *file;
    char line[MAX_TEXT * 3];
    char *part;

    databaseCount = 0;

    file = fopen("ingredients.txt", "r");

    if (file == NULL) {
        printf("\nError: ingredients.txt not found.\n");
        return;
    }

    while (fgets(line, sizeof(line), file) != NULL &&
           databaseCount < MAX_INGREDIENTS) {

        trim(line);

        if (strlen(line) == 0) {
            continue;
        }

        part = strtok(line, "|");
        if (part == NULL) 
        {
            continue;
        }
        strncpy(database[databaseCount].name, part, MAX_NAME - 1);
        database[databaseCount].name[MAX_NAME - 1] = '\0';

        part = strtok(NULL, "|");
        if (part == NULL) 
        {
            continue;
        }
        strncpy(database[databaseCount].category, part, MAX_NAME - 1);
        database[databaseCount].category[MAX_NAME - 1] = '\0';

        part = strtok(NULL, "|");
        if (part == NULL) 
        {
            continue;
        }
        strncpy(database[databaseCount].function, part, MAX_TEXT - 1);
        database[databaseCount].function[MAX_TEXT - 1] = '\0';

        part = strtok(NULL, "|");
        if (part == NULL) 
        {
            continue;
        }
        strncpy(database[databaseCount].awareness, part, MAX_TEXT - 1);
        database[databaseCount].awareness[MAX_TEXT - 1] = '\0';

        databaseCount++;
    }

    fclose(file);
}

//database me ingredient search krega
int linearSearch(char searchName[]) {
    int i;

    for (i = 0; i < databaseCount; i++) {
        if (sameText(database[i].name, searchName)) {
            return i;
        }
    }

    return -1;
}

//matched ingredient ka details display krega
void displayIngredient(int index) {
    printf("\nIngredient: %s\n", database[index].name);
    printf("Category: %s\n", database[index].category);
    printf("Function: %s\n", database[index].function);
    printf("Awareness: %s\n", database[index].awareness);
}

//ingredient search krega aur display krega
void searchIngredient() {
    char name[MAX_NAME];
    int index;

    if (databaseCount == 0) {
        printf("\nThe database is empty or could not be loaded.\n");
        return;
    }

    printf("\nEnter ingredient name: ");
    fgets(name, sizeof(name), stdin);
    trim(name);

    index = linearSearch(name);

    if (index != -1) {
        printf("\nIngredient found!");
        displayIngredient(index);
    } else {
        printf("\nIngredient NOT FOUND in the database.\n");
    }
}

//input karaega product ka naam aur ingredients
void inputProduct(struct Product *p) {
    char input[MAX_TEXT * 3];
    char *part;

    p->total = 0;
    p->found = 0;
    p->notFound = 0;

    printf("\nEnter packaged food product name: ");
    fgets(p->productName, sizeof(p->productName), stdin);
    trim(p->productName);

    printf("Enter ingredients separated by commas:\n");
    fgets(input, sizeof(input), stdin);

    part = strtok(input, ",");

    while (part != NULL && p->total < MAX_INGREDIENTS) {
        trim(part);

        if (strlen(part) > 0) {
            strncpy(p->ingredientNames[p->total], part, MAX_NAME - 1);
            p->ingredientNames[p->total][MAX_NAME - 1] = '\0';

            p->databaseIndex[p->total] =
                linearSearch(p->ingredientNames[p->total]);

            if (p->databaseIndex[p->total] != -1) {
                p->found++;
            } else {
                p->notFound++;
            }

            p->total++;
        }

        part = strtok(NULL, ",");
    }
}

// count krega konse ingredients database me hai aur konse nahi
void countIngredients(struct Product *p) {
    int i;

    p->found = 0;
    p->notFound = 0;

    for (i = 0; i < p->total; i++) {
        if (p->databaseIndex[i] != -1) {
            p->found++;
        } else {
            p->notFound++;
        }
    }
}

// final report display krega
void displayReport(struct Product *p) {
    int i;

    printf("\n");
    printf("FOOD LENS ANALYSIS REPORT\n");
    printf("\n");

    printf("Product name: %s\n", p->productName);
    printf("Total ingredients: %d\n", p->total);
    printf("Matched ingredients: %d\n", p->found);
    printf("Unmatched ingredients: %d\n", p->notFound);

    for (i = 0; i < p->total; i++) {
        printf("\n");
        printf("Ingredient: %s\n", p->ingredientNames[i]);

        if (p->databaseIndex[i] != -1) {
            printf("Database status: FOUND\n");
            printf("Category: %s\n",
                   database[p->databaseIndex[i]].category);
            printf("Function: %s\n",
                   database[p->databaseIndex[i]].function);
            printf("Awareness: %s\n",
                   database[p->databaseIndex[i]].awareness);
        } else {
            printf("NOT FOUND IN DATABASE\n");
        }
    }
}

/* Analyse a packaged food product */
void analyseProduct() {
    struct Product p;

    if (databaseCount == 0) {
        printf("\nThe database is empty or could not be loaded.\n");
        return;
    }

    inputProduct(&p);
    countIngredients(&p);
    displayReport(&p);
}

/* Display every ingredient in the database */
void displayDatabase() {
    int i;

    if (databaseCount == 0) {
        printf("\nNo ingredients available in the database.\n");
        return;
    }

    printf("\nINGREDIENT DATABASE\n");

    for (i = 0; i < databaseCount; i++) {
        printf("\n%d. %s\n", i + 1, database[i].name);
        printf("Category: %s\n", database[i].category);
        printf("Function: %s\n", database[i].function);
        printf("Awareness: %s\n", database[i].awareness);
    }
}

/* Main menu */
int main(int argc, char *argv[]) {
    int choice;
    char input[20];

    loadDatabase();

    if (argc > 1 && strcmp(argv[1], "search") == 0) {
        char name[MAX_NAME];
        int index;

        if (argc < 3) return 1;

        strncpy(name, argv[2], MAX_NAME - 1);
        name[MAX_NAME - 1] = '\0';

        index = linearSearch(name);

        if (index != -1) {
            displayIngredient(index);
        } else {
            printf("Ingredient not found in the database.\n");
        }

        return 0;
    }

    if (argc > 1 && strcmp(argv[1], "analyse") == 0) {
        struct Product p;
        char ingredients[MAX_TEXT * 3];

        if (argc < 4) return 1;

        strncpy(p.productName, argv[2], MAX_NAME - 1);
        p.productName[MAX_NAME - 1] = '\0';

        strncpy(ingredients, argv[3], sizeof(ingredients) - 1);
        ingredients[sizeof(ingredients) - 1] = '\0';

        p.total = 0;
        p.found = 0;
        p.notFound = 0;

        char *part = strtok(ingredients, ",");

        while (part != NULL && p.total < MAX_INGREDIENTS) {
            trim(part);

            if (strlen(part) > 0) {
                strncpy(p.ingredientNames[p.total], part, MAX_NAME - 1);
                p.ingredientNames[p.total][MAX_NAME - 1] = '\0';

                p.databaseIndex[p.total] =
                    linearSearch(p.ingredientNames[p.total]);

                p.total++;
            }

            part = strtok(NULL, ",");
        }

        countIngredients(&p);
        displayReport(&p);
        return 0;
    }

    while (1) {
        printf("\n\n========== FOOD LENS ==========\n");
        printf("1. Search an Ingredient\n");
        printf("2. Analyse Packaged Food\n");
        printf("3. Display Ingredient Database\n");
        printf("4. Exit\n");
        printf("===============================\n");
        printf("Enter your choice: ");

        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        if (sscanf(input, "%d", &choice) != 1) {
            printf("\nInvalid input. Enter a number from 1 to 4.\n");
            continue;
        }

        switch (choice) {
            case 1:
                searchIngredient();
                break;

            case 2:
                analyseProduct();
                break;

            case 3:
                displayDatabase();
                break;

            case 4:
                printf("\nThank you for using Food Lens!\n");
                return 0;

            default:
                printf("\nInvalid choice. Please select 1 to 4.\n");
        }
    }

    return 0;
}