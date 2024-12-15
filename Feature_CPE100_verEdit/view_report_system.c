#include <stdio.h>
#include <string.h>

typedef struct {
    int id;             
    char name[999];     
    char range[999];    
    int sold;           
    char desc[999];     
} Product;

void search_by_price(FILE *file,float min_price,float max_price);
void search_by_name(FILE *file,const char *name);
void summary(FILE *file);
void find_best_selling(FILE *file);

int main() {
    FILE *file = fopen("data_products.csv","r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    int choice;
    char search_name[100];
    float min_price, max_price;

    printf("Menu:\n");
    printf("1. Search by price range\n");
    printf("2. Search by name\n");
    printf("3. Summary of all products\n");
    printf("4. Find best-selling product\n");
    printf("Enter your choice: ");
    scanf("%d",&choice);

    switch (choice) {
        case 1:
            printf("Enter min price: ");
            scanf("%f",&min_price);
            printf("Enter max price: ");
            scanf("%f",&max_price);
            search_by_price(file,min_price,max_price);
            break;

        case 2:
            printf("Enter product name: ");
            scanf("%s",search_name);
            search_by_name(file,search_name);
            break;

        case 3:
            summary(file);
            break;

        case 4:
            find_best_selling(file);
            break;

        default:
            printf("Invalid choice.\n");
    }

    fclose(file);
    return 0;
}

void search_by_price(FILE *file,float min_price,float max_price) {
    Product p;
    rewind(file);
    char input[999];
    printf("Products in price range %.2f - %.2f:\n",min_price,max_price);

    while (fgets(input,sizeof(input),file)) { 
        if (sscanf(input,"%d,%998[^,],%998[^,],%d,%998[^\n]",&p.id,p.name,p.range,&p.sold,p.desc) == 5) {
            float min,max;
            if (sscanf(p.range,"%f-%f",&min,&max) == 2 && min <= max_price && max >= min_price) {
                printf("ID: %d,Name: %s,Sold: %d,Range: %s,Description: %s\n",p.id,p.name,p.sold,p.range,p.desc);
            }
        }
    }
}

void search_by_name(FILE *file, const char *name) {
    Product p;
    rewind(file);
    char input[999];
    printf("Searching for products containing '%s':\n",name);

    while (fgets(input,sizeof(input),file)) {
        if (sscanf(input,"%d,%998[^,],%998[^,],%d,%998[^\n]",&p.id,p.name,p.range,&p.sold,p.desc) == 5) {
            if (strstr(p.name,name) != NULL) {
                printf("ID: %d,Name: %s,Sold: %d,Range: %s,Description: %s\n",p.id,p.name,p.sold,p.range,p.desc);
            }
        }
    }
}

void summary(FILE *file) {
    Product p;
    rewind(file);

    int total_products = 0,total_sold = 0;
    float total_revenue = 0;
    char input[999]; 
    printf("Summary of Products:\n");
    while (fgets(input,sizeof(input),file)) { 
        if (sscanf(input,"%d,%998[^,],%998[^,],%d,%998[^\n]",&p.id,p.name,p.range,&p.sold,p.desc) == 5) {
            float min,max;
            if (sscanf(p.range,"%f-%f",&min,&max) == 2) {
                total_products++;
                total_sold += p.sold;
                total_revenue += p.sold * ((min + max) / 2.0); 
            }
        }
    }
    printf("Total Products: %d\n",total_products);
    printf("Total Sold: %d\n",total_sold);
    printf("Total Revenue: %.2f\n",total_revenue);
}

void find_best_selling(FILE *file) {
    Product p,best;
    best.sold = 0;
    char input[999];

    rewind(file);
    while (fgets(input,sizeof(input),file)) { 
        if (sscanf(input,"%d,%998[^,],%998[^,],%d,%998[^\n]",&p.id,p.name,p.range,&p.sold,p.desc) == 5) {
            if (p.sold > best.sold) {
                best = p;
            }
        }
    }
    printf("Best Selling Product:\n");
    printf("ID: %d, Name: %s, Sold: %d, Range: %s, Description: %s\n",best.id,best.name,best.sold,best.range,best.desc);
}
