void displayIngredient(int index) {
    printf("\nIngredient: %s\n", database[index].name);
    printf("Category: %s\n", database[index].category);
    printf("Function: %s\n", database[index].function);
    printf("Awareness: %s\n", database[index].awareness);
}

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