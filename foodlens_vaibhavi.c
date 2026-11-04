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

int linearSearch(char searchName[]) {
    int i;

    for (i = 0; i < databaseCount; i++) {
        if (sameText(database[i].name, searchName)) {
            return i;
        }
    }

    return -1;
}

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