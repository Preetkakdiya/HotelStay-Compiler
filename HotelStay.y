%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yylex(void);
void yyerror(const char *s);
extern FILE *yyin;

#define MAX_GUESTS 50
#define MAX_AMENITIES 20

/* Fixed Room Inventory Caps */
#define TOTAL_STANDARD 20
#define TOTAL_DELUXE 15
#define TOTAL_SUITE 10
#define TOTAL_PRESIDENTIAL 5
#define TOTAL_HOTEL_ROOMS 50

typedef struct {
    char name[50];
    int cost;
} AmenityItem;

typedef struct {
    char name[50];
    char roomType[30];
    int roomRate;
    int nights;
    AmenityItem amenities[MAX_AMENITIES];
    int amenityCount;
    int discountPercent;
    int taxPercent;
    int isCheckedOut;
} GuestRecord;

GuestRecord guestTable[MAX_GUESTS];
int guestCount = 0;

/* ---------------- INVENTORY & SYMBOL TABLE HELPERS ---------------- */

int findGuest(const char *name) {
    for (int i = 0; i < guestCount; i++) {
        if (strcasecmp(guestTable[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

int getCategoryTotal(const char *roomType) {
    if (strcmp(roomType, "STANDARD") == 0) return TOTAL_STANDARD;
    if (strcmp(roomType, "DELUXE") == 0) return TOTAL_DELUXE;
    if (strcmp(roomType, "SUITE") == 0) return TOTAL_SUITE;
    if (strcmp(roomType, "PRESIDENTIAL") == 0) return TOTAL_PRESIDENTIAL;
    return 10; // default cap for custom rooms
}

int getCategoryOccupied(const char *roomType) {
    int count = 0;
    for (int i = 0; i < guestCount; i++) {
        if (!guestTable[i].isCheckedOut && strcmp(guestTable[i].roomType, roomType) == 0) {
            count++;
        }
    }
    return count;
}

void registerGuest(const char *name) {
    if (findGuest(name) != -1) {
        printf("[Semantic Error] Guest '%s' is already registered in the system.\n", name);
        return;
    }
    if (guestCount >= MAX_GUESTS) {
        printf("[System Error] Hotel guest capacity reached (%d guests max).\n", MAX_GUESTS);
        return;
    }

    strcpy(guestTable[guestCount].name, name);
    strcpy(guestTable[guestCount].roomType, "UNASSIGNED");
    guestTable[guestCount].roomRate = 0;
    guestTable[guestCount].nights = 1;
    guestTable[guestCount].amenityCount = 0;
    guestTable[guestCount].discountPercent = 0;
    guestTable[guestCount].taxPercent = 12; // Standard 12% GST
    guestTable[guestCount].isCheckedOut = 0;

    printf("[Registration] Guest '%s' checked in. Ready for room assignment.\n", name);
    guestCount++;
}

void assignRoom(const char *name, const char *roomType, int rate) {
    int idx = findGuest(name);
    if (idx == -1) {
        printf("[Semantic Error] Cannot assign room. Guest '%s' not found.\n", name);
        return;
    }
    if (guestTable[idx].isCheckedOut) {
        printf("[Semantic Error] Guest '%s' is already checked out.\n", name);
        return;
    }

    int occupied = getCategoryOccupied(roomType);
    int total = getCategoryTotal(roomType);

    // If guest is already holding this room type, allow re-assignment/rate update
    if (strcmp(guestTable[idx].roomType, roomType) != 0 && occupied >= total) {
        printf("[Availability Alert] All %d '%s' rooms are currently FULL! Please choose another type.\n", total, roomType);
        return;
    }

    strcpy(guestTable[idx].roomType, roomType);
    guestTable[idx].roomRate = rate;

    int remaining = total - getCategoryOccupied(roomType);
    printf("[Room Allocated] %s allocated '%s' Room @ Rs.%d/night. (%d %s room(s) still available)\n", 
           name, roomType, rate, remaining, roomType);
}

void assignNights(const char *name, int nights) {
    int idx = findGuest(name);
    if (idx == -1) {
        printf("[Semantic Error] Cannot set nights. Guest '%s' not found.\n", name);
        return;
    }
    if (nights <= 0) {
        printf("[Semantic Error] Duration of stay must be at least 1 night. Provided: %d\n", nights);
        return;
    }
    guestTable[idx].nights = nights;
    printf("[Stay Updated] %s stay duration updated to %d night(s).\n", name, nights);
}

void addAmenityService(const char *name, const char *serviceName, int cost) {
    int idx = findGuest(name);
    if (idx == -1) {
        printf("[Semantic Error] Cannot add service. Guest '%s' not found.\n", name);
        return;
    }
    if (cost < 0) {
        printf("[Semantic Error] Amenity cost cannot be negative.\n");
        return;
    }
    if (guestTable[idx].amenityCount >= MAX_AMENITIES) {
        printf("[Semantic Error] Amenity limit exceeded for guest '%s'.\n", name);
        return;
    }

    int aIdx = guestTable[idx].amenityCount;
    strcpy(guestTable[idx].amenities[aIdx].name, serviceName);
    guestTable[idx].amenities[aIdx].cost = cost;
    guestTable[idx].amenityCount++;

    printf("[Amenity Added] Added '%s' (Rs.%d) to %s's folio.\n", serviceName, cost, name);
}

void applyDiscount(const char *name, int discount) {
    int idx = findGuest(name);
    if (idx == -1) {
        printf("[Semantic Error] Cannot apply discount. Guest '%s' not found.\n", name);
        return;
    }
    if (discount < 0 || discount > 100) {
        printf("[Semantic Error] Invalid discount percent: %d%% (Must be between 0 and 100).\n", discount);
        return;
    }
    guestTable[idx].discountPercent = discount;
    printf("[Discount Applied] %d%% promotional discount applied for %s.\n", discount, name);
}

void applyTax(const char *name, int tax) {
    int idx = findGuest(name);
    if (idx == -1) {
        printf("[Semantic Error] Cannot apply tax. Guest '%s' not found.\n", name);
        return;
    }
    if (tax < 0) {
        printf("[Semantic Error] Tax percentage cannot be negative.\n");
        return;
    }
    guestTable[idx].taxPercent = tax;
    printf("[Tax Configured] %d%% tax rate set for %s.\n", tax, name);
}

void performCheckout(const char *name) {
    int idx = findGuest(name);
    if (idx == -1) {
        printf("[Semantic Error] Cannot checkout. Guest '%s' not found.\n", name);
        return;
    }
    if (guestTable[idx].isCheckedOut) {
        printf("[Warning] Guest '%s' has already been checked out.\n", name);
        return;
    }

    GuestRecord *g = &guestTable[idx];
    int roomTotal = g->roomRate * g->nights;
    int amenitiesTotal = 0;
    for (int i = 0; i < g->amenityCount; i++) {
        amenitiesTotal += g->amenities[i].cost;
    }

    int grossTotal = roomTotal + amenitiesTotal;
    int discountAmt = (grossTotal * g->discountPercent) / 100;
    int netBeforeTax = grossTotal - discountAmt;
    int taxAmt = (netBeforeTax * g->taxPercent) / 100;
    int finalBill = netBeforeTax + taxAmt;

    printf("\n+==========================================================+\n");
    printf("|              HOTEL GRAND HORIZON - TAX INVOICE           |\n");
    printf("+==========================================================+\n");
    printf("  Guest Name    : %-38s\n", g->name);
    printf("  Room Type     : %-15s (Rs.%d / night)\n", g->roomType, g->roomRate);
    printf("  Duration      : %d night(s)\n", g->nights);
    printf("------------------------------------------------------------\n");
    printf("  Room Charges  : %d nights x Rs.%d = Rs.%-10d\n", g->nights, g->roomRate, roomTotal);

    if (g->amenityCount > 0) {
        printf("  Additional Amenities & Folio Services:\n");
        for (int i = 0; i < g->amenityCount; i++) {
            printf("    * %-25s : Rs.%d\n", g->amenities[i].name, g->amenities[i].cost);
        }
        printf("  Amenities Total Charges      : Rs.%-10d\n", amenitiesTotal);
    } else {
        printf("  Amenities Charges            : Rs.0 (None)\n");
    }

    printf("------------------------------------------------------------\n");
    printf("  Gross Subtotal                : Rs.%-10d\n", grossTotal);
    printf("  Discount (%2d%%)                : -Rs.%-10d\n", g->discountPercent, discountAmt);
    printf("  Net Taxable Amount            : Rs.%-10d\n", netBeforeTax);
    printf("  Taxes / GST (%2d%%)             : +Rs.%-10d\n", g->taxPercent, taxAmt);
    printf("+==========================================================+\n");
    printf("  GRAND TOTAL PAYABLE           : Rs.%-10d\n", finalBill);
    printf("+==========================================================+\n");
    printf("  Thank you for choosing Hotel Grand Horizon! Have a safe trip!\n\n");

    g->isCheckedOut = 1;

    // Show release message
    int avail = getCategoryTotal(g->roomType) - getCategoryOccupied(g->roomType);
    printf("[Room Released] %s room released! Total %s rooms now available: %d\n\n", 
           g->roomType, g->roomType, avail);
}

void printHotelStatus() {
    int stdOcc = getCategoryOccupied("STANDARD");
    int dlxOcc = getCategoryOccupied("DELUXE");
    int steOcc = getCategoryOccupied("SUITE");
    int prsOcc = getCategoryOccupied("PRESIDENTIAL");
    int totalOcc = stdOcc + dlxOcc + steOcc + prsOcc;
    int totalAvail = TOTAL_HOTEL_ROOMS - totalOcc;

    printf("\n+==========================================================+\n");
    printf("|          HOTEL GRAND HORIZON - ROOM AVAILABILITY         |\n");
    printf("+==========================================================+\n");
    printf("  %-16s %-8s %-10s %-12s\n", "Room Category", "Total", "Occupied", "Available");
    printf("  --------------------------------------------------------\n");
    printf("  %-16s %-8d %-10d %-12d\n", "STANDARD", TOTAL_STANDARD, stdOcc, TOTAL_STANDARD - stdOcc);
    printf("  %-16s %-8d %-10d %-12d\n", "DELUXE", TOTAL_DELUXE, dlxOcc, TOTAL_DELUXE - dlxOcc);
    printf("  %-16s %-8d %-10d %-12d\n", "SUITE", TOTAL_SUITE, steOcc, TOTAL_SUITE - steOcc);
    printf("  %-16s %-8d %-10d %-12d\n", "PRESIDENTIAL", TOTAL_PRESIDENTIAL, prsOcc, TOTAL_PRESIDENTIAL - prsOcc);
    printf("  --------------------------------------------------------\n");
    printf("  TOTAL HOTEL      %-8d %-10d %-12d\n", TOTAL_HOTEL_ROOMS, totalOcc, totalAvail);
    printf("+==========================================================+\n");

    printf("\n+----------------------------------------------------------+\n");
    printf("|                 ACTIVE GUEST ROSTER LOG                  |\n");
    printf("+----------------------------------------------------------+\n");
    if (guestCount == 0) {
        printf("  No registered guests currently.\n");
    } else {
        printf("  %-15s %-14s %-8s %-12s\n", "Guest", "Room", "Nights", "Status");
        printf("  --------------------------------------------------------\n");
        for (int i = 0; i < guestCount; i++) {
            printf("  %-15s %-14s %-8d %-12s\n", 
                guestTable[i].name, 
                guestTable[i].roomType, 
                guestTable[i].nights,
                guestTable[i].isCheckedOut ? "Checked Out" : "Occupied");
        }
    }
    printf("+----------------------------------------------------------+\n\n");
}

void resetGuestRecord(const char *name) {
    int idx = findGuest(name);
    if (idx == -1) {
        printf("[Semantic Error] Cannot reset. Guest '%s' not found.\n", name);
        return;
    }
    for (int i = idx; i < guestCount - 1; i++) {
        guestTable[i] = guestTable[i + 1];
    }
    guestCount--;
    printf("[Record Cleared] Guest record '%s' removed from system.\n", name);
}

%}

/* ---------------- YACC DECLARATIONS ---------------- */

%union {
    int num;
    char *str;
}

%token GUEST ROOM NIGHTS AMENITY DISCOUNT TAX CHECKOUT STATUS PRINT RESET
%token STANDARD DELUXE SUITE PRESIDENTIAL

%token <num> NUMBER
%token <str> ID
%token <str> STRING

%type <num> expression
%type <num> term
%type <num> factor

%left '+' '-'
%left '*' '/'

%%

/* ---------------- GRAMMAR RULES ---------------- */

program:
    statements
    {
        printf("\n============================================================\n");
        printf("           HOTELSTAY COMPILATION & EXECUTION COMPLETE       \n");
        printf("============================================================\n");
    }
    ;

statements:
    statements statement
    | statement
    ;

statement:
    guest_decl
    | room_assignment
    | nights_assignment
    | amenity_addition
    | discount_assignment
    | tax_assignment
    | checkout_stmt
    | status_stmt
    | print_stmt
    | reset_stmt
    ;

guest_decl:
    GUEST ID ';'
    {
        registerGuest($2);
        free($2);
    }
    ;

room_assignment:
    ROOM ID STANDARD ';'
    {
        assignRoom($2, "STANDARD", 2500);
        free($2);
    }
    | ROOM ID DELUXE ';'
    {
        assignRoom($2, "DELUXE", 4500);
        free($2);
    }
    | ROOM ID SUITE ';'
    {
        assignRoom($2, "SUITE", 8000);
        free($2);
    }
    | ROOM ID PRESIDENTIAL ';'
    {
        assignRoom($2, "PRESIDENTIAL", 18000);
        free($2);
    }
    | ROOM ID ID expression ';'
    {
        assignRoom($2, $3, $4);
        free($2);
        free($3);
    }
    ;

nights_assignment:
    NIGHTS ID expression ';'
    {
        assignNights($2, $3);
        free($2);
    }
    ;

amenity_addition:
    AMENITY ID ID expression ';'
    {
        addAmenityService($2, $3, $4);
        free($2);
        free($3);
    }
    | AMENITY ID STRING expression ';'
    {
        addAmenityService($2, $3, $4);
        free($2);
        free($3);
    }
    ;

discount_assignment:
    DISCOUNT ID expression ';'
    {
        applyDiscount($2, $3);
        free($2);
    }
    ;

tax_assignment:
    TAX ID expression ';'
    {
        applyTax($2, $3);
        free($2);
    }
    ;

checkout_stmt:
    CHECKOUT ID ';'
    {
        performCheckout($2);
        free($2);
    }
    ;

status_stmt:
    STATUS ';'
    {
        printHotelStatus();
    }
    ;

print_stmt:
    PRINT STRING ';'
    {
        printf("[NOTICE] %s\n", $2);
        free($2);
    }
    | PRINT expression ';'
    {
        printf("[CALCULATION] Result = %d\n", $2);
    }
    ;

reset_stmt:
    RESET ID ';'
    {
        resetGuestRecord($2);
        free($2);
    }
    ;

/* ---------------- ARITHMETIC EXPRESSIONS ---------------- */

expression:
    expression '+' term
    {
        $$ = $1 + $3;
    }
    | expression '-' term
    {
        $$ = $1 - $3;
    }
    | term
    {
        $$ = $1;
    }
    ;

term:
    term '*' factor
    {
        $$ = $1 * $3;
    }
    | term '/' factor
    {
        if ($3 == 0) {
            printf("[Math Error] Division by zero encountered. Defaulting to 0.\n");
            $$ = 0;
        } else {
            $$ = $1 / $3;
        }
    }
    | factor
    {
        $$ = $1;
    }
    ;

factor:
    NUMBER
    {
        $$ = $1;
    }
    | '(' expression ')'
    {
        $$ = $2;
    }
    ;

%%

/* ---------------- C MAIN & ERROR HANDLERS ---------------- */

void yyerror(const char *s) {
    printf("\n[Syntax Error] %s\n", s);
}

int main(int argc, char *argv[]) {
    printf("============================================================\n");
    printf("       HOTELSTAY DOMAIN-SPECIFIC LANGUAGE (LEX & YACC)      \n");
    printf("============================================================\n\n");

    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            printf("[Error] Cannot open input file '%s'\n", argv[1]);
            return 1;
        }
        printf("[HotelStay] Executing input script: '%s'\n\n", argv[1]);
    } else {
        printf("[HotelStay] Enter HotelStay commands (Press Ctrl+Z then Enter to finish):\n\n");
    }

    yyparse();

    if (argc > 1 && yyin != NULL) {
        fclose(yyin);
    }

    return 0;
}
