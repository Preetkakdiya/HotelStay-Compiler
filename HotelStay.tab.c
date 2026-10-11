/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "HotelStay.y"

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


#line 638 "HotelStay.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "HotelStay.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_GUEST = 3,                      /* GUEST  */
  YYSYMBOL_ROOM = 4,                       /* ROOM  */
  YYSYMBOL_NIGHTS = 5,                     /* NIGHTS  */
  YYSYMBOL_AMENITY = 6,                    /* AMENITY  */
  YYSYMBOL_DISCOUNT = 7,                   /* DISCOUNT  */
  YYSYMBOL_TAX = 8,                        /* TAX  */
  YYSYMBOL_CHECKOUT = 9,                   /* CHECKOUT  */
  YYSYMBOL_STATUS = 10,                    /* STATUS  */
  YYSYMBOL_PRINT = 11,                     /* PRINT  */
  YYSYMBOL_RESET = 12,                     /* RESET  */
  YYSYMBOL_MAKE = 13,                      /* MAKE  */
  YYSYMBOL_TAKE = 14,                      /* TAKE  */
  YYSYMBOL_WHEN = 15,                      /* WHEN  */
  YYSYMBOL_OTHERWISE = 16,                 /* OTHERWISE  */
  YYSYMBOL_REPEAT = 17,                    /* REPEAT  */
  YYSYMBOL_STOP = 18,                      /* STOP  */
  YYSYMBOL_STANDARD = 19,                  /* STANDARD  */
  YYSYMBOL_DELUXE = 20,                    /* DELUXE  */
  YYSYMBOL_SUITE = 21,                     /* SUITE  */
  YYSYMBOL_PRESIDENTIAL = 22,              /* PRESIDENTIAL  */
  YYSYMBOL_NUMBER = 23,                    /* NUMBER  */
  YYSYMBOL_ID = 24,                        /* ID  */
  YYSYMBOL_STRING = 25,                    /* STRING  */
  YYSYMBOL_LOWER_THAN_OTHERWISE = 26,      /* LOWER_THAN_OTHERWISE  */
  YYSYMBOL_EQ = 27,                        /* EQ  */
  YYSYMBOL_NEQ = 28,                       /* NEQ  */
  YYSYMBOL_29_ = 29,                       /* '<'  */
  YYSYMBOL_30_ = 30,                       /* '>'  */
  YYSYMBOL_LTE = 31,                       /* LTE  */
  YYSYMBOL_GTE = 32,                       /* GTE  */
  YYSYMBOL_33_ = 33,                       /* '+'  */
  YYSYMBOL_34_ = 34,                       /* '-'  */
  YYSYMBOL_35_ = 35,                       /* '*'  */
  YYSYMBOL_36_ = 36,                       /* '/'  */
  YYSYMBOL_37_ = 37,                       /* '{'  */
  YYSYMBOL_38_ = 38,                       /* '}'  */
  YYSYMBOL_39_ = 39,                       /* ';'  */
  YYSYMBOL_40_ = 40,                       /* '='  */
  YYSYMBOL_41_ = 41,                       /* '('  */
  YYSYMBOL_42_ = 42,                       /* ')'  */
  YYSYMBOL_YYACCEPT = 43,                  /* $accept  */
  YYSYMBOL_program = 44,                   /* program  */
  YYSYMBOL_statements = 45,                /* statements  */
  YYSYMBOL_statement = 46,                 /* statement  */
  YYSYMBOL_statement_block = 47,           /* statement_block  */
  YYSYMBOL_guest_decl = 48,                /* guest_decl  */
  YYSYMBOL_room_assignment = 49,           /* room_assignment  */
  YYSYMBOL_nights_assignment = 50,         /* nights_assignment  */
  YYSYMBOL_amenity_addition = 51,          /* amenity_addition  */
  YYSYMBOL_discount_assignment = 52,       /* discount_assignment  */
  YYSYMBOL_tax_assignment = 53,            /* tax_assignment  */
  YYSYMBOL_checkout_stmt = 54,             /* checkout_stmt  */
  YYSYMBOL_status_stmt = 55,               /* status_stmt  */
  YYSYMBOL_print_stmt = 56,                /* print_stmt  */
  YYSYMBOL_reset_stmt = 57,                /* reset_stmt  */
  YYSYMBOL_make_stmt = 58,                 /* make_stmt  */
  YYSYMBOL_take_stmt = 59,                 /* take_stmt  */
  YYSYMBOL_assign_stmt = 60,               /* assign_stmt  */
  YYSYMBOL_when_stmt = 61,                 /* when_stmt  */
  YYSYMBOL_repeat_stmt = 62,               /* repeat_stmt  */
  YYSYMBOL_stop_stmt = 63,                 /* stop_stmt  */
  YYSYMBOL_expression = 64,                /* expression  */
  YYSYMBOL_term = 65,                      /* term  */
  YYSYMBOL_factor = 66                     /* factor  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  61
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   249

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  43
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  24
/* YYNRULES -- Number of rules.  */
#define YYNRULES  61
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  126

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   285


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      41,    42,    35,    33,     2,    34,     2,    36,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    39,
      29,    40,    30,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    37,     2,    38,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    31,    32
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   605,   605,   616,   627,   632,   638,   639,   640,   641,
     642,   643,   644,   645,   646,   647,   648,   649,   650,   651,
     652,   653,   654,   658,   667,   677,   686,   695,   704,   713,
     726,   737,   747,   760,   771,   782,   792,   799,   806,   815,
     825,   835,   845,   856,   864,   875,   885,   894,   895,   896,
     897,   898,   899,   900,   901,   902,   906,   907,   908,   912,
     913,   914
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "GUEST", "ROOM",
  "NIGHTS", "AMENITY", "DISCOUNT", "TAX", "CHECKOUT", "STATUS", "PRINT",
  "RESET", "MAKE", "TAKE", "WHEN", "OTHERWISE", "REPEAT", "STOP",
  "STANDARD", "DELUXE", "SUITE", "PRESIDENTIAL", "NUMBER", "ID", "STRING",
  "LOWER_THAN_OTHERWISE", "EQ", "NEQ", "'<'", "'>'", "LTE", "GTE", "'+'",
  "'-'", "'*'", "'/'", "'{'", "'}'", "';'", "'='", "'('", "')'", "$accept",
  "program", "statements", "statement", "statement_block", "guest_decl",
  "room_assignment", "nights_assignment", "amenity_addition",
  "discount_assignment", "tax_assignment", "checkout_stmt", "status_stmt",
  "print_stmt", "reset_stmt", "make_stmt", "take_stmt", "assign_stmt",
  "when_stmt", "repeat_stmt", "stop_stmt", "expression", "term", "factor", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-55)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      39,   -10,    -1,    10,    14,    16,    31,    35,    23,    -4,
      42,    48,    56,    40,    44,    45,    47,    39,    83,    39,
     -55,   -55,   -55,   -55,   -55,   -55,   -55,   -55,   -55,   -55,
     -55,   -55,   -55,   -55,   -55,   -55,   -55,   -55,    51,    49,
      41,    -7,    41,    41,    52,   -55,   -55,   -55,    53,    41,
     111,    43,   -55,    54,    55,    57,    41,    41,   -55,    41,
      -2,   -55,   -55,   -55,    60,    61,    62,    63,    41,   124,
      41,    41,   137,   150,   -55,   -55,    -3,    41,    41,    41,
      41,    41,    41,    41,    41,   -55,    41,    41,   -55,   -55,
     -55,    79,    95,   163,   -55,   -55,   -55,   -55,   -55,   176,
     -55,   189,   202,   -55,   -55,   -55,    43,    43,    43,    43,
      43,    43,    43,    43,   -55,   -55,    39,    39,   -55,   -55,
     -55,   -55,    87,   -55,    39,   -55
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       5,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     5,     0,     2,
       4,    22,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,     0,     0,
       0,     0,     0,     0,     0,    36,    59,    60,     0,     0,
       0,    55,    58,     0,     0,     0,     0,     0,    46,     0,
       0,     1,     3,    24,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    35,    37,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    38,     0,     0,    39,    40,
      41,     0,     0,     0,    23,    25,    26,    27,    28,     0,
      30,     0,     0,    33,    34,    61,    47,    48,    49,    50,
      51,    52,    53,    54,    56,    57,     0,     0,    42,    29,
      31,    32,    43,    45,     0,    44
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -55,   -55,    78,   -19,   -55,   -55,   -55,   -55,   -55,   -55,
     -55,   -55,   -55,   -55,   -55,   -55,   -55,   -55,   -55,   -55,
     -55,    18,   165,   -54
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    50,    51,    52
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      62,     1,     2,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    38,    14,    15,    70,    71,    46,
      47,    48,    16,    39,    77,    78,    79,    80,    81,    82,
      83,    84,   114,   115,    40,    17,    94,    49,    41,   105,
      42,    62,     1,     2,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    43,    14,    15,    69,    44,
      72,    73,    45,    16,    46,    47,    53,    76,    64,    65,
      66,    67,    54,    68,    91,    92,    17,    93,    86,    87,
      55,    56,    49,    61,    58,    57,    99,    59,   101,   102,
      63,    74,    75,    88,    89,    60,    90,   122,   123,    95,
      96,    97,    98,   124,     0,   125,    77,    78,    79,    80,
      81,    82,    83,    84,     0,     0,     0,     0,     0,     0,
       0,   116,    77,    78,    79,    80,    81,    82,    83,    84,
       0,     0,     0,     0,     0,     0,     0,   117,    77,    78,
      79,    80,    81,    82,    83,    84,     0,     0,     0,     0,
      85,    77,    78,    79,    80,    81,    82,    83,    84,     0,
       0,     0,     0,   100,    77,    78,    79,    80,    81,    82,
      83,    84,     0,     0,     0,     0,   103,    77,    78,    79,
      80,    81,    82,    83,    84,     0,     0,     0,     0,   104,
      77,    78,    79,    80,    81,    82,    83,    84,     0,     0,
       0,     0,   118,    77,    78,    79,    80,    81,    82,    83,
      84,     0,     0,     0,     0,   119,    77,    78,    79,    80,
      81,    82,    83,    84,     0,     0,     0,     0,   120,    77,
      78,    79,    80,    81,    82,    83,    84,     0,     0,     0,
       0,   121,   106,   107,   108,   109,   110,   111,   112,   113
};

static const yytype_int8 yycheck[] =
{
      19,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    24,    17,    18,    24,    25,    23,
      24,    25,    24,    24,    27,    28,    29,    30,    31,    32,
      33,    34,    86,    87,    24,    37,    38,    41,    24,    42,
      24,    60,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    24,    17,    18,    40,    24,
      42,    43,    39,    24,    23,    24,    24,    49,    19,    20,
      21,    22,    24,    24,    56,    57,    37,    59,    35,    36,
      24,    41,    41,     0,    39,    41,    68,    40,    70,    71,
      39,    39,    39,    39,    39,    17,    39,   116,   117,    39,
      39,    39,    39,    16,    -1,   124,    27,    28,    29,    30,
      31,    32,    33,    34,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    42,    27,    28,    29,    30,    31,    32,    33,    34,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    42,    27,    28,
      29,    30,    31,    32,    33,    34,    -1,    -1,    -1,    -1,
      39,    27,    28,    29,    30,    31,    32,    33,    34,    -1,
      -1,    -1,    -1,    39,    27,    28,    29,    30,    31,    32,
      33,    34,    -1,    -1,    -1,    -1,    39,    27,    28,    29,
      30,    31,    32,    33,    34,    -1,    -1,    -1,    -1,    39,
      27,    28,    29,    30,    31,    32,    33,    34,    -1,    -1,
      -1,    -1,    39,    27,    28,    29,    30,    31,    32,    33,
      34,    -1,    -1,    -1,    -1,    39,    27,    28,    29,    30,
      31,    32,    33,    34,    -1,    -1,    -1,    -1,    39,    27,
      28,    29,    30,    31,    32,    33,    34,    -1,    -1,    -1,
      -1,    39,    77,    78,    79,    80,    81,    82,    83,    84
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    17,    18,    24,    37,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    24,    24,
      24,    24,    24,    24,    24,    39,    23,    24,    25,    41,
      64,    65,    66,    24,    24,    24,    41,    41,    39,    40,
      45,     0,    46,    39,    19,    20,    21,    22,    24,    64,
      24,    25,    64,    64,    39,    39,    64,    27,    28,    29,
      30,    31,    32,    33,    34,    39,    35,    36,    39,    39,
      39,    64,    64,    64,    38,    39,    39,    39,    39,    64,
      39,    64,    64,    39,    39,    42,    65,    65,    65,    65,
      65,    65,    65,    65,    66,    66,    42,    42,    39,    39,
      39,    39,    46,    46,    16,    46
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    43,    44,    45,    45,    45,    46,    46,    46,    46,
      46,    46,    46,    46,    46,    46,    46,    46,    46,    46,
      46,    46,    46,    47,    48,    49,    49,    49,    49,    49,
      50,    51,    51,    52,    53,    54,    55,    56,    56,    57,
      58,    59,    60,    61,    61,    62,    63,    64,    64,    64,
      64,    64,    64,    64,    64,    64,    65,    65,    65,    66,
      66,    66
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     0,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     3,     4,     4,     4,     4,     5,
       4,     5,     5,     4,     4,     3,     2,     3,     3,     3,
       3,     3,     4,     5,     7,     5,     2,     3,     3,     3,
       3,     3,     3,     3,     3,     1,     3,     3,     1,     1,
       1,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: statements  */
#line 606 "HotelStay.y"
    {
        (yyval.stmt) = (yyvsp[0].stmt);
        executeStmtList((yyval.stmt));
        printf("\n============================================================\n");
        printf("           HOTELSTAY COMPILATION & EXECUTION COMPLETE       \n");
        printf("============================================================\n");
    }
#line 1801 "HotelStay.tab.c"
    break;

  case 3: /* statements: statements statement  */
#line 617 "HotelStay.y"
    {
        if ((yyvsp[-1].stmt) == NULL) {
            (yyval.stmt) = (yyvsp[0].stmt);
        } else {
            StmtNode *curr = (yyvsp[-1].stmt);
            while (curr->next != NULL) curr = curr->next;
            curr->next = (yyvsp[0].stmt);
            (yyval.stmt) = (yyvsp[-1].stmt);
        }
    }
#line 1816 "HotelStay.tab.c"
    break;

  case 4: /* statements: statement  */
#line 628 "HotelStay.y"
    {
        (yyval.stmt) = (yyvsp[0].stmt);
    }
#line 1824 "HotelStay.tab.c"
    break;

  case 5: /* statements: %empty  */
#line 632 "HotelStay.y"
    {
        (yyval.stmt) = NULL;
    }
#line 1832 "HotelStay.tab.c"
    break;

  case 23: /* statement_block: '{' statements '}'  */
#line 659 "HotelStay.y"
    {
        StmtNode *s = createStmt(STMT_BLOCK);
        s->body = (yyvsp[-1].stmt);
        (yyval.stmt) = s;
    }
#line 1842 "HotelStay.tab.c"
    break;

  case 24: /* guest_decl: GUEST ID ';'  */
#line 668 "HotelStay.y"
    {
        StmtNode *s = createStmt(STMT_GUEST);
        strcpy(s->id1, (yyvsp[-1].str));
        free((yyvsp[-1].str));
        (yyval.stmt) = s;
    }
#line 1853 "HotelStay.tab.c"
    break;

  case 25: /* room_assignment: ROOM ID STANDARD ';'  */
#line 678 "HotelStay.y"
    {
        StmtNode *s = createStmt(STMT_ROOM);
        strcpy(s->id1, (yyvsp[-2].str));
        strcpy(s->id2, "STANDARD");
        s->expr1 = createNumExpr(2500);
        free((yyvsp[-2].str));
        (yyval.stmt) = s;
    }
#line 1866 "HotelStay.tab.c"
    break;

  case 26: /* room_assignment: ROOM ID DELUXE ';'  */
#line 687 "HotelStay.y"
    {
        StmtNode *s = createStmt(STMT_ROOM);
        strcpy(s->id1, (yyvsp[-2].str));
        strcpy(s->id2, "DELUXE");
        s->expr1 = createNumExpr(4500);
        free((yyvsp[-2].str));
        (yyval.stmt) = s;
    }
#line 1879 "HotelStay.tab.c"
    break;

  case 27: /* room_assignment: ROOM ID SUITE ';'  */
#line 696 "HotelStay.y"
    {
        StmtNode *s = createStmt(STMT_ROOM);
        strcpy(s->id1, (yyvsp[-2].str));
        strcpy(s->id2, "SUITE");
        s->expr1 = createNumExpr(8000);
        free((yyvsp[-2].str));
        (yyval.stmt) = s;
    }
#line 1892 "HotelStay.tab.c"
    break;

  case 28: /* room_assignment: ROOM ID PRESIDENTIAL ';'  */
#line 705 "HotelStay.y"
    {
        StmtNode *s = createStmt(STMT_ROOM);
        strcpy(s->id1, (yyvsp[-2].str));
        strcpy(s->id2, "PRESIDENTIAL");
        s->expr1 = createNumExpr(18000);
        free((yyvsp[-2].str));
        (yyval.stmt) = s;
    }
#line 1905 "HotelStay.tab.c"
    break;

  case 29: /* room_assignment: ROOM ID ID expression ';'  */
#line 714 "HotelStay.y"
    {
        StmtNode *s = createStmt(STMT_ROOM);
        strcpy(s->id1, (yyvsp[-3].str));
        strcpy(s->id2, (yyvsp[-2].str));
        s->expr1 = (yyvsp[-1].expr);
        free((yyvsp[-3].str));
        free((yyvsp[-2].str));
        (yyval.stmt) = s;
    }
#line 1919 "HotelStay.tab.c"
    break;

  case 30: /* nights_assignment: NIGHTS ID expression ';'  */
#line 727 "HotelStay.y"
    {
        StmtNode *s = createStmt(STMT_NIGHTS);
        strcpy(s->id1, (yyvsp[-2].str));
        s->expr1 = (yyvsp[-1].expr);
        free((yyvsp[-2].str));
        (yyval.stmt) = s;
    }
#line 1931 "HotelStay.tab.c"
    break;

  case 31: /* amenity_addition: AMENITY ID ID expression ';'  */
#line 738 "HotelStay.y"
    {
        StmtNode *s = createStmt(STMT_AMENITY);
        strcpy(s->id1, (yyvsp[-3].str));
        strcpy(s->id2, (yyvsp[-2].str));
        s->expr1 = (yyvsp[-1].expr);
        free((yyvsp[-3].str));
        free((yyvsp[-2].str));
        (yyval.stmt) = s;
    }
#line 1945 "HotelStay.tab.c"
    break;

  case 32: /* amenity_addition: AMENITY ID STRING expression ';'  */
#line 748 "HotelStay.y"
    {
        StmtNode *s = createStmt(STMT_AMENITY);
        strcpy(s->id1, (yyvsp[-3].str));
        strcpy(s->id2, (yyvsp[-2].str));
        s->expr1 = (yyvsp[-1].expr);
        free((yyvsp[-3].str));
        free((yyvsp[-2].str));
        (yyval.stmt) = s;
    }
#line 1959 "HotelStay.tab.c"
    break;

  case 33: /* discount_assignment: DISCOUNT ID expression ';'  */
#line 761 "HotelStay.y"
    {
        StmtNode *s = createStmt(STMT_DISCOUNT);
        strcpy(s->id1, (yyvsp[-2].str));
        s->expr1 = (yyvsp[-1].expr);
        free((yyvsp[-2].str));
        (yyval.stmt) = s;
    }
#line 1971 "HotelStay.tab.c"
    break;

  case 34: /* tax_assignment: TAX ID expression ';'  */
#line 772 "HotelStay.y"
    {
        StmtNode *s = createStmt(STMT_TAX);
        strcpy(s->id1, (yyvsp[-2].str));
        s->expr1 = (yyvsp[-1].expr);
        free((yyvsp[-2].str));
        (yyval.stmt) = s;
    }
#line 1983 "HotelStay.tab.c"
    break;

  case 35: /* checkout_stmt: CHECKOUT ID ';'  */
#line 783 "HotelStay.y"
    {
        StmtNode *s = createStmt(STMT_CHECKOUT);
        strcpy(s->id1, (yyvsp[-1].str));
        free((yyvsp[-1].str));
        (yyval.stmt) = s;
    }
#line 1994 "HotelStay.tab.c"
    break;

  case 36: /* status_stmt: STATUS ';'  */
#line 793 "HotelStay.y"
    {
        (yyval.stmt) = createStmt(STMT_STATUS);
    }
#line 2002 "HotelStay.tab.c"
    break;

  case 37: /* print_stmt: PRINT STRING ';'  */
#line 800 "HotelStay.y"
    {
        StmtNode *s = createStmt(STMT_PRINT_STR);
        strcpy(s->id2, (yyvsp[-1].str));
        free((yyvsp[-1].str));
        (yyval.stmt) = s;
    }
#line 2013 "HotelStay.tab.c"
    break;

  case 38: /* print_stmt: PRINT expression ';'  */
#line 807 "HotelStay.y"
    {
        StmtNode *s = createStmt(STMT_PRINT_EXPR);
        s->expr1 = (yyvsp[-1].expr);
        (yyval.stmt) = s;
    }
#line 2023 "HotelStay.tab.c"
    break;

  case 39: /* reset_stmt: RESET ID ';'  */
#line 816 "HotelStay.y"
    {
        StmtNode *s = createStmt(STMT_RESET);
        strcpy(s->id1, (yyvsp[-1].str));
        free((yyvsp[-1].str));
        (yyval.stmt) = s;
    }
#line 2034 "HotelStay.tab.c"
    break;

  case 40: /* make_stmt: MAKE ID ';'  */
#line 826 "HotelStay.y"
    {
        StmtNode *s = createStmt(STMT_MAKE);
        strcpy(s->id1, (yyvsp[-1].str));
        free((yyvsp[-1].str));
        (yyval.stmt) = s;
    }
#line 2045 "HotelStay.tab.c"
    break;

  case 41: /* take_stmt: TAKE ID ';'  */
#line 836 "HotelStay.y"
    {
        StmtNode *s = createStmt(STMT_TAKE);
        strcpy(s->id1, (yyvsp[-1].str));
        free((yyvsp[-1].str));
        (yyval.stmt) = s;
    }
#line 2056 "HotelStay.tab.c"
    break;

  case 42: /* assign_stmt: ID '=' expression ';'  */
#line 846 "HotelStay.y"
    {
        StmtNode *s = createStmt(STMT_ASSIGN);
        strcpy(s->id1, (yyvsp[-3].str));
        s->expr1 = (yyvsp[-1].expr);
        free((yyvsp[-3].str));
        (yyval.stmt) = s;
    }
#line 2068 "HotelStay.tab.c"
    break;

  case 43: /* when_stmt: WHEN '(' expression ')' statement  */
#line 857 "HotelStay.y"
    {
        StmtNode *s = createStmt(STMT_WHEN);
        s->expr1 = (yyvsp[-2].expr);
        s->body = (yyvsp[0].stmt);
        s->elseBody = NULL;
        (yyval.stmt) = s;
    }
#line 2080 "HotelStay.tab.c"
    break;

  case 44: /* when_stmt: WHEN '(' expression ')' statement OTHERWISE statement  */
#line 865 "HotelStay.y"
    {
        StmtNode *s = createStmt(STMT_WHEN);
        s->expr1 = (yyvsp[-4].expr);
        s->body = (yyvsp[-2].stmt);
        s->elseBody = (yyvsp[0].stmt);
        (yyval.stmt) = s;
    }
#line 2092 "HotelStay.tab.c"
    break;

  case 45: /* repeat_stmt: REPEAT '(' expression ')' statement  */
#line 876 "HotelStay.y"
    {
        StmtNode *s = createStmt(STMT_REPEAT);
        s->expr1 = (yyvsp[-2].expr);
        s->body = (yyvsp[0].stmt);
        (yyval.stmt) = s;
    }
#line 2103 "HotelStay.tab.c"
    break;

  case 46: /* stop_stmt: STOP ';'  */
#line 886 "HotelStay.y"
    {
        (yyval.stmt) = createStmt(STMT_STOP);
    }
#line 2111 "HotelStay.tab.c"
    break;

  case 47: /* expression: expression EQ term  */
#line 894 "HotelStay.y"
                           { (yyval.expr) = createOpExpr(EXPR_EQ, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2117 "HotelStay.tab.c"
    break;

  case 48: /* expression: expression NEQ term  */
#line 895 "HotelStay.y"
                           { (yyval.expr) = createOpExpr(EXPR_NEQ, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2123 "HotelStay.tab.c"
    break;

  case 49: /* expression: expression '<' term  */
#line 896 "HotelStay.y"
                           { (yyval.expr) = createOpExpr(EXPR_LT, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2129 "HotelStay.tab.c"
    break;

  case 50: /* expression: expression '>' term  */
#line 897 "HotelStay.y"
                           { (yyval.expr) = createOpExpr(EXPR_GT, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2135 "HotelStay.tab.c"
    break;

  case 51: /* expression: expression LTE term  */
#line 898 "HotelStay.y"
                           { (yyval.expr) = createOpExpr(EXPR_LTE, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2141 "HotelStay.tab.c"
    break;

  case 52: /* expression: expression GTE term  */
#line 899 "HotelStay.y"
                           { (yyval.expr) = createOpExpr(EXPR_GTE, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2147 "HotelStay.tab.c"
    break;

  case 53: /* expression: expression '+' term  */
#line 900 "HotelStay.y"
                           { (yyval.expr) = createOpExpr(EXPR_ADD, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2153 "HotelStay.tab.c"
    break;

  case 54: /* expression: expression '-' term  */
#line 901 "HotelStay.y"
                           { (yyval.expr) = createOpExpr(EXPR_SUB, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2159 "HotelStay.tab.c"
    break;

  case 55: /* expression: term  */
#line 902 "HotelStay.y"
                           { (yyval.expr) = (yyvsp[0].expr); }
#line 2165 "HotelStay.tab.c"
    break;

  case 56: /* term: term '*' factor  */
#line 906 "HotelStay.y"
                           { (yyval.expr) = createOpExpr(EXPR_MUL, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2171 "HotelStay.tab.c"
    break;

  case 57: /* term: term '/' factor  */
#line 907 "HotelStay.y"
                           { (yyval.expr) = createOpExpr(EXPR_DIV, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2177 "HotelStay.tab.c"
    break;

  case 58: /* term: factor  */
#line 908 "HotelStay.y"
                           { (yyval.expr) = (yyvsp[0].expr); }
#line 2183 "HotelStay.tab.c"
    break;

  case 59: /* factor: NUMBER  */
#line 912 "HotelStay.y"
                           { (yyval.expr) = createNumExpr((yyvsp[0].num)); }
#line 2189 "HotelStay.tab.c"
    break;

  case 60: /* factor: ID  */
#line 913 "HotelStay.y"
                           { (yyval.expr) = createVarExpr((yyvsp[0].str)); free((yyvsp[0].str)); }
#line 2195 "HotelStay.tab.c"
    break;

  case 61: /* factor: '(' expression ')'  */
#line 914 "HotelStay.y"
                           { (yyval.expr) = (yyvsp[-1].expr); }
#line 2201 "HotelStay.tab.c"
    break;


#line 2205 "HotelStay.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 917 "HotelStay.y"


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
