%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yylex(void);
void yyerror(const char *s);
extern FILE *yyin;

#define MAX_GUESTS 50
#define MAX_AMENITIES 20
#define MAX_VARS 100

/* Fixed Room Inventory Caps */
#define TOTAL_STANDARD 20
#define TOTAL_DELUXE 15
#define TOTAL_SUITE 10
#define TOTAL_PRESIDENTIAL 5
#define TOTAL_HOTEL_ROOMS 50

/* ---------------- DATA STRUCTURES ---------------- */

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

typedef struct {
    char name[50];
    int value;
    int isDeclared;
} VarSymbol;

VarSymbol varTable[MAX_VARS];
int varCount = 0;

/* ---------------- AST DEFINITIONS ---------------- */

typedef enum {
    EXPR_NUM,
    EXPR_VAR,
    EXPR_ADD,
    EXPR_SUB,
    EXPR_MUL,
    EXPR_DIV,
    EXPR_EQ,
    EXPR_NEQ,
    EXPR_LT,
    EXPR_GT,
    EXPR_LTE,
    EXPR_GTE
} ExprType;

typedef struct ExprNode {
    ExprType type;
    int num;
    char varName[50];
    struct ExprNode *left;
    struct ExprNode *right;
} ExprNode;

typedef enum {
    STMT_GUEST,
    STMT_ROOM,
    STMT_NIGHTS,
    STMT_AMENITY,
    STMT_DISCOUNT,
    STMT_TAX,
    STMT_CHECKOUT,
    STMT_STATUS,
    STMT_PRINT_STR,
    STMT_PRINT_EXPR,
    STMT_RESET,
    STMT_MAKE,
    STMT_ASSIGN,
    STMT_TAKE,
    STMT_WHEN,
    STMT_REPEAT,
    STMT_BLOCK,
    STMT_STOP
} StmtType;

typedef struct StmtNode {
    StmtType type;
    char id1[50];
    char id2[100];
    ExprNode *expr1;
    struct StmtNode *body;
    struct StmtNode *elseBody;
    struct StmtNode *next;
} StmtNode;

/* ---------------- SYMBOL TABLE ROUTINES ---------------- */

int findGuest(const char *name) {
    for (int i = 0; i < guestCount; i++) {
        if (strcasecmp(guestTable[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

int getCategoryTotal(const char *roomType) {
    if (strcasecmp(roomType, "STANDARD") == 0) return TOTAL_STANDARD;
    if (strcasecmp(roomType, "DELUXE") == 0) return TOTAL_DELUXE;
    if (strcasecmp(roomType, "SUITE") == 0) return TOTAL_SUITE;
    if (strcasecmp(roomType, "PRESIDENTIAL") == 0) return TOTAL_PRESIDENTIAL;
    return 10;
}

int getCategoryOccupied(const char *roomType) {
    int count = 0;
    for (int i = 0; i < guestCount; i++) {
        if (!guestTable[i].isCheckedOut && strcasecmp(guestTable[i].roomType, roomType) == 0) {
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
    guestTable[guestCount].taxPercent = 12; // 12% GST
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

    if (strcasecmp(guestTable[idx].roomType, roomType) != 0 && occupied >= total) {
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

/* ---------------- VARIABLE SYMBOL TABLE ---------------- */

int findVar(const char *name) {
    for (int i = 0; i < varCount; i++) {
        if (strcmp(varTable[i].name, name) == 0) return i;
    }
    return -1;
}

void declareVar(const char *name) {
    if (findVar(name) != -1) {
        printf("[Semantic Error] Variable '%s' already declared.\n", name);
        return;
    }
    if (varCount >= MAX_VARS) {
        printf("[System Error] Variable table overflow.\n");
        return;
    }
    strcpy(varTable[varCount].name, name);
    varTable[varCount].value = 0;
    varTable[varCount].isDeclared = 1;
    varCount++;
}

void setVar(const char *name, int val) {
    int idx = findVar(name);
    if (idx == -1) {
        printf("[Semantic Error] Variable '%s' used without declaration. Auto-declaring.\n", name);
        declareVar(name);
        idx = findVar(name);
    }
    varTable[idx].value = val;
}

int getVar(const char *name) {
    int idx = findVar(name);
    if (idx == -1) {
        printf("[Semantic Error] Variable '%s' is not declared.\n", name);
        return 0;
    }
    return varTable[idx].value;
}

/* ---------------- AST CONSTRUCTORS ---------------- */

ExprNode *createNumExpr(int val) {
    ExprNode *e = (ExprNode *)malloc(sizeof(ExprNode));
    e->type = EXPR_NUM;
    e->num = val;
    e->left = e->right = NULL;
    return e;
}

ExprNode *createVarExpr(const char *name) {
    ExprNode *e = (ExprNode *)malloc(sizeof(ExprNode));
    e->type = EXPR_VAR;
    strcpy(e->varName, name);
    e->left = e->right = NULL;
    return e;
}

ExprNode *createOpExpr(ExprType type, ExprNode *left, ExprNode *right) {
    ExprNode *e = (ExprNode *)malloc(sizeof(ExprNode));
    e->type = type;
    e->left = left;
    e->right = right;
    return e;
}

StmtNode *createStmt(StmtType type) {
    StmtNode *s = (StmtNode *)malloc(sizeof(StmtNode));
    s->type = type;
    s->id1[0] = '\0';
    s->id2[0] = '\0';
    s->expr1 = NULL;
    s->body = NULL;
    s->elseBody = NULL;
    s->next = NULL;
    return s;
}

/* ---------------- AST INTERPRETER / EXECUTOR ---------------- */

int evalExpr(ExprNode *e) {
    if (!e) return 0;
    switch(e->type) {
        case EXPR_NUM: return e->num;
        case EXPR_VAR: return getVar(e->varName);
        case EXPR_ADD: return evalExpr(e->left) + evalExpr(e->right);
        case EXPR_SUB: return evalExpr(e->left) - evalExpr(e->right);
        case EXPR_MUL: return evalExpr(e->left) * evalExpr(e->right);
        case EXPR_DIV: {
            int d = evalExpr(e->right);
            if (d == 0) {
                printf("[Math Error] Division by zero encountered. Defaulting to 0.\n");
                return 0;
            }
            return evalExpr(e->left) / d;
        }
        case EXPR_EQ:  return evalExpr(e->left) == evalExpr(e->right);
        case EXPR_NEQ: return evalExpr(e->left) != evalExpr(e->right);
        case EXPR_LT:  return evalExpr(e->left) < evalExpr(e->right);
        case EXPR_GT:  return evalExpr(e->left) > evalExpr(e->right);
        case EXPR_LTE: return evalExpr(e->left) <= evalExpr(e->right);
        case EXPR_GTE: return evalExpr(e->left) >= evalExpr(e->right);
    }
    return 0;
}

int executionHalted = 0;
void executeStmtList(StmtNode *list);

void executeStmt(StmtNode *s) {
    if (!s || executionHalted) return;
    switch(s->type) {
        case STMT_GUEST:
            registerGuest(s->id1);
            break;
        case STMT_ROOM:
            assignRoom(s->id1, s->id2, evalExpr(s->expr1));
            break;
        case STMT_NIGHTS:
            assignNights(s->id1, evalExpr(s->expr1));
            break;
        case STMT_AMENITY:
            addAmenityService(s->id1, s->id2, evalExpr(s->expr1));
            break;
        case STMT_DISCOUNT:
            applyDiscount(s->id1, evalExpr(s->expr1));
            break;
        case STMT_TAX:
            applyTax(s->id1, evalExpr(s->expr1));
            break;
        case STMT_CHECKOUT:
            performCheckout(s->id1);
            break;
        case STMT_STATUS:
            printHotelStatus();
            break;
        case STMT_PRINT_STR:
            printf("[NOTICE] %s\n", s->id2);
            break;
        case STMT_PRINT_EXPR:
            printf("[OUTPUT] %d\n", evalExpr(s->expr1));
            break;
        case STMT_RESET:
            resetGuestRecord(s->id1);
            break;
        case STMT_MAKE:
            declareVar(s->id1);
            break;
        case STMT_ASSIGN:
            setVar(s->id1, evalExpr(s->expr1));
            break;
        case STMT_TAKE: {
            int val;
            printf("Input %s: ", s->id1);
            if (scanf("%d", &val) == 1) {
                setVar(s->id1, val);
            } else {
                printf("[Input Error] Invalid number.\n");
            }
            break;
        }
        case STMT_WHEN:
            if (evalExpr(s->expr1)) {
                executeStmtList(s->body);
            } else if (s->elseBody) {
                executeStmtList(s->elseBody);
            }
            break;
        case STMT_REPEAT: {
            int guard = 0;
            while (evalExpr(s->expr1) && !executionHalted) {
                executeStmtList(s->body);
                guard++;
                if (guard > 100000) {
                    printf("[Runtime Error] Infinite loop detected.\n");
                    break;
                }
            }
            break;
        }
        case STMT_BLOCK:
            executeStmtList(s->body);
            break;
        case STMT_STOP:
            printf("[STOP] Program execution halted.\n");
            executionHalted = 1;
            break;
    }
}

void executeStmtList(StmtNode *list) {
    StmtNode *curr = list;
    while (curr && !executionHalted) {
        executeStmt(curr);
        curr = curr->next;
    }
}

%}

/* ---------------- YACC DECLARATIONS ---------------- */

%union {
    int num;
    char *str;
    struct ExprNode *expr;
    struct StmtNode *stmt;
}

%token GUEST ROOM NIGHTS AMENITY DISCOUNT TAX CHECKOUT STATUS PRINT RESET
%token MAKE TAKE WHEN OTHERWISE REPEAT STOP
%token STANDARD DELUXE SUITE PRESIDENTIAL

%token <num> NUMBER
%token <str> ID
%token <str> STRING

%type <expr> expression term factor
%type <stmt> program statements statement statement_block
%type <stmt> guest_decl room_assignment nights_assignment amenity_addition
%type <stmt> discount_assignment tax_assignment checkout_stmt status_stmt
%type <stmt> print_stmt reset_stmt make_stmt take_stmt assign_stmt
%type <stmt> when_stmt repeat_stmt stop_stmt

%nonassoc LOWER_THAN_OTHERWISE
%nonassoc OTHERWISE

%left EQ NEQ
%left '<' '>' LTE GTE
%left '+' '-'
%left '*' '/'

%%

/* ---------------- GRAMMAR RULES ---------------- */

program:
    statements
    {
        $$ = $1;
        executeStmtList($$);
        printf("\n============================================================\n");
        printf("           HOTELSTAY COMPILATION & EXECUTION COMPLETE       \n");
        printf("============================================================\n");
    }
    ;

statements:
    statements statement
    {
        if ($1 == NULL) {
            $$ = $2;
        } else {
            StmtNode *curr = $1;
            while (curr->next != NULL) curr = curr->next;
            curr->next = $2;
            $$ = $1;
        }
    }
    | statement
    {
        $$ = $1;
    }
    | /* empty */
    {
        $$ = NULL;
    }
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
    | make_stmt
    | take_stmt
    | assign_stmt
    | when_stmt
    | repeat_stmt
    | stop_stmt
    | statement_block
    ;

statement_block:
    '{' statements '}'
    {
        StmtNode *s = createStmt(STMT_BLOCK);
        s->body = $2;
        $$ = s;
    }
    ;

guest_decl:
    GUEST ID ';'
    {
        StmtNode *s = createStmt(STMT_GUEST);
        strcpy(s->id1, $2);
        free($2);
        $$ = s;
    }
    ;

room_assignment:
    ROOM ID STANDARD ';'
    {
        StmtNode *s = createStmt(STMT_ROOM);
        strcpy(s->id1, $2);
        strcpy(s->id2, "STANDARD");
        s->expr1 = createNumExpr(2500);
        free($2);
        $$ = s;
    }
    | ROOM ID DELUXE ';'
    {
        StmtNode *s = createStmt(STMT_ROOM);
        strcpy(s->id1, $2);
        strcpy(s->id2, "DELUXE");
        s->expr1 = createNumExpr(4500);
        free($2);
        $$ = s;
    }
    | ROOM ID SUITE ';'
    {
        StmtNode *s = createStmt(STMT_ROOM);
        strcpy(s->id1, $2);
        strcpy(s->id2, "SUITE");
        s->expr1 = createNumExpr(8000);
        free($2);
        $$ = s;
    }
    | ROOM ID PRESIDENTIAL ';'
    {
        StmtNode *s = createStmt(STMT_ROOM);
        strcpy(s->id1, $2);
        strcpy(s->id2, "PRESIDENTIAL");
        s->expr1 = createNumExpr(18000);
        free($2);
        $$ = s;
    }
    | ROOM ID ID expression ';'
    {
        StmtNode *s = createStmt(STMT_ROOM);
        strcpy(s->id1, $2);
        strcpy(s->id2, $3);
        s->expr1 = $4;
        free($2);
        free($3);
        $$ = s;
    }
    ;

nights_assignment:
    NIGHTS ID expression ';'
    {
        StmtNode *s = createStmt(STMT_NIGHTS);
        strcpy(s->id1, $2);
        s->expr1 = $3;
        free($2);
        $$ = s;
    }
    ;

amenity_addition:
    AMENITY ID ID expression ';'
    {
        StmtNode *s = createStmt(STMT_AMENITY);
        strcpy(s->id1, $2);
        strcpy(s->id2, $3);
        s->expr1 = $4;
        free($2);
        free($3);
        $$ = s;
    }
    | AMENITY ID STRING expression ';'
    {
        StmtNode *s = createStmt(STMT_AMENITY);
        strcpy(s->id1, $2);
        strcpy(s->id2, $3);
        s->expr1 = $4;
        free($2);
        free($3);
        $$ = s;
    }
    ;

discount_assignment:
    DISCOUNT ID expression ';'
    {
        StmtNode *s = createStmt(STMT_DISCOUNT);
        strcpy(s->id1, $2);
        s->expr1 = $3;
        free($2);
        $$ = s;
    }
    ;

tax_assignment:
    TAX ID expression ';'
    {
        StmtNode *s = createStmt(STMT_TAX);
        strcpy(s->id1, $2);
        s->expr1 = $3;
        free($2);
        $$ = s;
    }
    ;

checkout_stmt:
    CHECKOUT ID ';'
    {
        StmtNode *s = createStmt(STMT_CHECKOUT);
        strcpy(s->id1, $2);
        free($2);
        $$ = s;
    }
    ;

status_stmt:
    STATUS ';'
    {
        $$ = createStmt(STMT_STATUS);
    }
    ;

print_stmt:
    PRINT STRING ';'
    {
        StmtNode *s = createStmt(STMT_PRINT_STR);
        strcpy(s->id2, $2);
        free($2);
        $$ = s;
    }
    | PRINT expression ';'
    {
        StmtNode *s = createStmt(STMT_PRINT_EXPR);
        s->expr1 = $2;
        $$ = s;
    }
    ;

reset_stmt:
    RESET ID ';'
    {
        StmtNode *s = createStmt(STMT_RESET);
        strcpy(s->id1, $2);
        free($2);
        $$ = s;
    }
    ;

make_stmt:
    MAKE ID ';'
    {
        StmtNode *s = createStmt(STMT_MAKE);
        strcpy(s->id1, $2);
        free($2);
        $$ = s;
    }
    ;

take_stmt:
    TAKE ID ';'
    {
        StmtNode *s = createStmt(STMT_TAKE);
        strcpy(s->id1, $2);
        free($2);
        $$ = s;
    }
    ;

assign_stmt:
    ID '=' expression ';'
    {
        StmtNode *s = createStmt(STMT_ASSIGN);
        strcpy(s->id1, $1);
        s->expr1 = $3;
        free($1);
        $$ = s;
    }
    ;

when_stmt:
    WHEN '(' expression ')' statement %prec LOWER_THAN_OTHERWISE
    {
        StmtNode *s = createStmt(STMT_WHEN);
        s->expr1 = $3;
        s->body = $5;
        s->elseBody = NULL;
        $$ = s;
    }
    | WHEN '(' expression ')' statement OTHERWISE statement
    {
        StmtNode *s = createStmt(STMT_WHEN);
        s->expr1 = $3;
        s->body = $5;
        s->elseBody = $7;
        $$ = s;
    }
    ;

repeat_stmt:
    REPEAT '(' expression ')' statement
    {
        StmtNode *s = createStmt(STMT_REPEAT);
        s->expr1 = $3;
        s->body = $5;
        $$ = s;
    }
    ;

stop_stmt:
    STOP ';'
    {
        $$ = createStmt(STMT_STOP);
    }
    ;

/* ---------------- EXPRESSIONS & PRECEDENCE ---------------- */

expression:
    expression EQ term     { $$ = createOpExpr(EXPR_EQ, $1, $3); }
    | expression NEQ term  { $$ = createOpExpr(EXPR_NEQ, $1, $3); }
    | expression '<' term  { $$ = createOpExpr(EXPR_LT, $1, $3); }
    | expression '>' term  { $$ = createOpExpr(EXPR_GT, $1, $3); }
    | expression LTE term  { $$ = createOpExpr(EXPR_LTE, $1, $3); }
    | expression GTE term  { $$ = createOpExpr(EXPR_GTE, $1, $3); }
    | expression '+' term  { $$ = createOpExpr(EXPR_ADD, $1, $3); }
    | expression '-' term  { $$ = createOpExpr(EXPR_SUB, $1, $3); }
    | term                 { $$ = $1; }
    ;

term:
    term '*' factor        { $$ = createOpExpr(EXPR_MUL, $1, $3); }
    | term '/' factor      { $$ = createOpExpr(EXPR_DIV, $1, $3); }
    | factor               { $$ = $1; }
    ;

factor:
    NUMBER                 { $$ = createNumExpr($1); }
    | ID                   { $$ = createVarExpr($1); free($1); }
    | '(' expression ')'   { $$ = $2; }
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
