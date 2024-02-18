#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to represent each product
typedef struct Product {
    char name[100];
    int id;
    float price;
    int quantity;
    struct Product *next;
} Product;

// Node for the stack to store product updates
typedef struct Update {
    int id;
    int old_quantity;
    int new_quantity;
    struct Update *next;
} Update;

// Function to create a new product node
Product* createProduct(char name[], int id, float price, int quantity) {
    Product *newProduct = (Product*)malloc(sizeof(Product));
    strcpy(newProduct->name, name);
    newProduct->id = id;
    newProduct->price = price;
    newProduct->quantity = quantity;
    newProduct->next = NULL;
    return newProduct;
}

// Function to push an update onto the stack
void pushUpdate(Update **top, int id, int old_quantity, int new_quantity) {
    Update *newUpdate = (Update*)malloc(sizeof(Update));
    newUpdate->id = id;
    newUpdate->old_quantity = old_quantity;
    newUpdate->new_quantity = new_quantity;
    newUpdate->next = *top;
    *top = newUpdate;
}

// Function to pop an update from the stack
void popUpdate(Update **top) {
    if (*top == NULL) {
        printf("Update stack is empty\n");
        return;
    }
    Update *temp = *top;
    *top = (*top)->next;
    free(temp);
}

// Function to add a new product to the end of the list
void addProduct(Product **head, Product *newProduct) {
    if (*head == NULL) {
        *head = newProduct;
        return;
    }
    Product *temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newProduct;
}

// Function to display all products in the list
void displayProducts(Product *head) {
    if (head == NULL) {
        printf("No products available\n");
        return;
    }
    printf("Product ID\tName\tPrice\tQuantity\n");
    while (head != NULL) {
        printf("%d\t\t%s\t%.2f\t%d\n", head->id, head->name, head->price, head->quantity);
        head = head->next;
    }
}

// Function to read products from a file and populate the list
void readProductsFromFile(Product **head, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file\n");
        exit(1);
    }

    char name[100];
    int id, quantity;
    float price;

    while (fscanf(file, "%d %s %f %d", &id, name, &price, &quantity) != EOF) {
        Product *newProduct = createProduct(name, id, price, quantity);
        addProduct(head, newProduct);
    }

    fclose(file);
}

// Function to write products from the list to a file
void writeProductsToFile(Product *head, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file\n");
        exit(1);
    }

    while (head != NULL) {
        fprintf(file, "%d %s %.2f %d\n", head->id, head->name, head->price, head->quantity);
        head = head->next;
    }

    fclose(file);
}

int main() {
    Product *inventory = NULL;
    Update *updateStack = NULL;

    // Read products from file
    readProductsFromFile(&inventory, "pos.txt");

    int choice;
    do {
        printf("\n1. Display Products\n");
        printf("2. Add Product\n");
        printf("3. Update Product\n");
        printf("4. Save and Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                displayProducts(inventory);
                break;
            case 2: {
                // Add product
                char name[100];
                int id, quantity;
                float price;
                printf("Enter product details:\n");
                printf("Name: ");
                scanf("%s", name);
                printf("ID: ");
                scanf("%d", &id);
                printf("Price: ");
                scanf("%f", &price);
                printf("Quantity: ");
                scanf("%d", &quantity);
                Product *newProduct = createProduct(name, id, price, quantity);
                addProduct(&inventory, newProduct);
                break;
            }
            case 3: {
                // Update product
                int productId, newQuantity;
                printf("Enter product ID to update: ");
                scanf("%d", &productId);
                printf("Enter new quantity: ");
                scanf("%d", &newQuantity);

                // Find the product and update its quantity
                Product *temp = inventory;
                while (temp != NULL) {
                    if (temp->id == productId) {
                        pushUpdate(&updateStack, temp->id, temp->quantity, newQuantity);
                        temp->quantity = newQuantity;
                        printf("Product quantity updated successfully\n");
                        break;
                    }
                    temp = temp->next;
                }
                if (temp == NULL) {
                    printf("Product not found\n");
                }
                break;
            }
            case 4:
                // Save products to file and exit
                writeProductsToFile(inventory, "pos.txt");
                printf("Products saved successfully. Exiting...\n");
                break;
            default:
                printf("Invalid choice\n");
        }
    } while (choice != 4);
}

    // Free
