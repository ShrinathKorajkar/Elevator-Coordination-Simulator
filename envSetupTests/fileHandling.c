#include <stdio.h>
// #include <stdlib.h>
#include <string.h>

typedef struct
{
    int id;
    char name[50];
    int age;
} User;

void create(FILE *file)
{
    User user;
    int isUnique = 0;

    while (!isUnique)
    {
        isUnique = 1;

        printf("Enter User_ID: ");
        if (scanf("%d", &user.id) != 1)
        {
            printf("Invalid input. Please enter a valid number.\n");
            while (getchar() != '\n')
                ;
            isUnique = 0;
            continue;
        }

        User tempUser;
        fseek(file, 0, SEEK_SET);
        while (fread(&tempUser, sizeof(User), 1, file))
        {
            if (tempUser.id == user.id)
            {
                printf("ID already exists.\nPlease enter a unique ID.\n");
                isUnique = 0;
                break;
            }
        }
    }

    getchar();
    printf("Enter the Name: ");
    fgets(user.name, sizeof(user.name), stdin);
    user.name[strcspn(user.name, "\n")] = '\0';

    int validAge = 0;
    while (!validAge)
    {
        printf("Enter Age: ");
        if (scanf("%d", &user.age) != 1)
        {
            printf("Invalid input. Please enter a valid number.\n");
            while (getchar() != '\n')
                ;
        }
        else if (user.age < 0 || user.age > 120)
        {
            printf("Please enter a realistic age.\n");
        }
        else
        {
            validAge = 1;
        }
    }

    fseek(file, 0, SEEK_END);
    fwrite(&user, sizeof(User), 1, file);
    fflush(file);
    printf("User added successfully.\n");
}

void display(FILE *file)
{
    User user;
    fseek(file, 0, SEEK_SET);

    printf("\nID\tName\t\tAge\n");
    printf("------------------------------------\n");
    while (fread(&user, sizeof(User), 1, file))
    {
        printf("%d\t%s\t\t%d\n", user.id, user.name, user.age);
    }
}

void update(FILE *file)
{
    int id, found = 0;
    User user;
    FILE *tempFile = fopen("temp.txt", "wb");
    if (!tempFile)
    {
        printf("Error creating temporary file.\n");
        return;
    }

    printf("Enter User ID to update: ");
    scanf("%d", &id);

    fseek(file, 0, SEEK_SET);
    while (fread(&user, sizeof(User), 1, file))
    {
        if (user.id == id)
        {
            found = 1;
            printf("Enter new Name: ");
            getchar();
            fgets(user.name, sizeof(user.name), stdin);
            user.name[strcspn(user.name, "\n")] = '\0';
            printf("Enter new Age: ");
            scanf("%d", &user.age);
            printf("User updated successfully\n");
        }
        fwrite(&user, sizeof(User), 1, tempFile);
    }

    fclose(tempFile);
    if (!found)
    {
        printf("User ID not found.\n");
        remove("temp.txt");
        return;
    }

    fclose(file);
    remove("users.txt");
    rename("temp.txt", "users.txt");
    file = fopen("users.txt", "rb+");
}

void delete(FILE *file)
{
    int id, found = 0;
    User user;
    FILE *tempFile = fopen("temp.txt", "wb");
    if (!tempFile)
    {
        printf("Error creating temporary file.\n");
        return;
    }

    printf("Enter User ID to delete: ");
    scanf("%d", &id);

    fseek(file, 0, SEEK_SET);
    while (fread(&user, sizeof(User), 1, file))
    {
        if (user.id != id)
        {
            fwrite(&user, sizeof(User), 1, tempFile);
        }
        else
        {
            found = 1;
        }
    }

    fclose(tempFile);
    if (!found)
    {
        printf("User ID not found.\n");
        remove("temp.txt");
        return;
    }

    fclose(file);
    remove("users.txt");
    rename("temp.txt", "users.txt");
    file = fopen("users.txt", "rb+");
    printf("User deleted successfully!\n");
}

FILE *initializeFile()
{
    FILE *file = fopen("users.txt", "rb+");
    if (!file)
    {
        file = fopen("users.txt", "wb+");
        if (!file)
        {
            printf("Error creating file.\n");
            exit(1);
        }
    }
    return file;
}

int main()
{
    FILE *file = fopen("users.txt", "a+");
    if (file == NULL)
    {
        file = fopen("users.txt", "w+");
        if (file == NULL)
        {
            printf("Error: Unable to open file.\n");
            return 1;
        }
    }

    int choice;
    do
    {
        printf("\n1. Create User\n2. Display Users\n3. Update User\n4. Delete User\n5. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input. Please enter a valid choice.\n");
            while (getchar() != '\n')
                ;
            choice = -1;
            continue;
        }

        switch (choice)
        {
        case 1:
            create(file);
            break;
        case 2:
            display(file);
            break;
        case 3:
            update(file);
            break;
        case 4:
            delete (file);
            break;
        case 5:
            printf("Exiting...\n");
            break;
        default:
            printf("Invalid choice. please try again.\n");
        }
    } while (choice != 5);

    fclose(file);
    return 0;
}
