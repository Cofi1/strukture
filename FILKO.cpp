#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Struktura za čvor stabla
typedef struct Node {
    int data;
    struct Node* left;
    struct Node* right;
} Node;

// Funkcija za stvaranje novog čvora
Node* noviCvor(int data) {
    Node* cvor = (Node*)malloc(sizeof(Node));
    cvor->data = data;
    cvor->left = NULL;
    cvor->right = NULL;
    return cvor;
}

// Dodavanje novog elementa u stablo
Node* dodaj(Node* root, int data) {
    if (root == NULL) {
        return noviCvor(data);
    }
    if (data < root->data) {
        root->left = dodaj(root->left, data);
    }
    else if (data > root->data) {
        root->right = dodaj(root->right, data);
    }
    return root;
}

// Ispis inorder (lijevo, korijen, desno)
void inorder(Node* root) {
    if (root != NULL) {
        inorder(root->left);
        printf("%d ", root->data);
        inorder(root->right);
    }
}

// Ispis preorder (korijen, lijevo, desno)
void preorder(Node* root) {
    if (root != NULL) {
        printf("%d ", root->data);
        preorder(root->left);
        preorder(root->right);
    }
}

// Ispis postorder (lijevo, desno, korijen)
void postorder(Node* root) {
    if (root != NULL) {
        postorder(root->left);
        postorder(root->right);
        printf("%d ", root->data);
    }
}

// Ispis level-order (po razinama)
void levelOrder(Node* root) {
    if (root == NULL) return;

    Node* queue[100];
    int front = 0, rear = 0;

    queue[rear++] = root;

    while (front < rear) {
        Node* current = queue[front++];
        printf("%d ", current->data);

        if (current->left != NULL) {
            queue[rear++] = current->left;
        }
        if (current->right != NULL) {
            queue[rear++] = current->right;
        }
    }
}

// Pronalaženje elementa u stablu
bool pronadi(Node* root, int data) {
    if (root == NULL) {
        return false;
    }
    if (data == root->data) {
        return true;
    }
    if (data < root->data) {
        return pronadi(root->left, data);
    }
    else {
        return pronadi(root->right, data);
    }
}

// Najmanji element u stablu (za pomoć pri brisanju)
Node* najmanjiCvor(Node* root) {
    while (root->left != NULL) {
        root = root->left;
    }
    return root;
}

// Brisanje elementa iz stabla
Node* obrisi(Node* root, int data) {
    if (root == NULL) {
        return NULL;
    }
    if (data < root->data) {
        root->left = obrisi(root->left, data);
    }
    else if (data > root->data) {
        root->right = obrisi(root->right, data);
    }
    else {
        if (root->left == NULL) {
            Node* temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL) {
            Node* temp = root->left;
            free(root);
            return temp;
        }

        Node* temp = najmanjiCvor(root->right);
        root->data = temp->data;
        root->right = obrisi(root->right, temp->data);
    }
    return root;
}

// Glavni program
int main() {
    Node* root = NULL;
    int izbor, vrijednost;

    do {
        printf("\nIzbornik:\n");
        printf("1. Dodaj element\n");
        printf("2. Ispis (inorder)\n");
        printf("3. Ispis (preorder)\n");
        printf("4. Ispis (postorder)\n");
        printf("5. Ispis (level-order)\n");
        printf("6. Pronadi element\n");
        printf("7. Obrisi element\n");
        printf("8. Izlaz\n");
        printf("Odabir: ");
        scanf("%d", &izbor);

        switch (izbor) {
        case 1:
            printf("Unesite vrijednost za dodavanje: ");
            scanf("%d", &vrijednost);
            root = dodaj(root, vrijednost);
            break;
        case 2:
            printf("Inorder ispis: ");
            inorder(root);
            printf("\n");
            break;
        case 3:
            printf("Preorder ispis: ");
            preorder(root);
            printf("\n");
            break;
        case 4:
            printf("Postorder ispis: ");
            postorder(root);
            printf("\n");
            break;
        case 5:
            printf("Level-order ispis: ");
            levelOrder(root);
            printf("\n");
            break;
        case 6:
            printf("Unesite vrijednost za pronalazak: ");
            scanf("%d", &vrijednost);
            if (pronadi(root, vrijednost)) {
                printf("Element %d pronađen.\n", vrijednost);
            }
            else {
                printf("Element %d nije pronađen.\n", vrijednost);
            }
            break;
        case 7:
            printf("Unesite vrijednost za brisanje: ");
            scanf("%d", &vrijednost);
            root = obrisi(root, vrijednost);
            printf("Element %d obrisan (ako je postojao).\n", vrijednost);
            break;
        case 8:
            printf("Izlaz iz programa.\n");
            break;
        default:
            printf("Nepoznata opcija.\n");
        }
    } while (izbor != 8);

    return 0;
}