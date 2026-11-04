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