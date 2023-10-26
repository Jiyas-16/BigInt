#include"BigInteger.h"
#include<stdio.h>
#include<stdlib.h>

int isEqualToZero(struct BigInteger number) {
    struct node *itr = number.head;
    while (itr != 0) {
        if (itr->data != 0) {
            return 0;
        }
        itr = itr->next;
    }
    return 1;
}


void AddAtEnd(struct BigInteger *number, int element) {
    struct node *temp = (struct node *)malloc(sizeof(struct node));
    if (temp == NULL) {
        printf("Memory Allocation Failed.\n");
        exit(1);
    }
    temp->data = element;
    temp->next = NULL;
    number->size = number->size + 1;
    if (number->head == NULL) {
        number->head = temp;
        return;
    }
    struct node *itr = number->head;
    while (itr->next != NULL) {
        itr = itr->next;
    }
    itr->next = temp;
}

void AddAtBegan(int element, struct BigInteger *number) {
    if (number == NULL) {
        exit(1);
    }
    struct node *temp = (struct node *)malloc(sizeof(struct node));
    if (temp == NULL) {
        printf("Memory allocation failed .\n");
        exit(1);
    }
    temp->data = element;
    temp->next = number->head;
    number->head = temp;
    number->size = number->size + 1;
}

struct BigInteger initialize(char *str) {
    struct BigInteger number;
    number.head = NULL;
    number.size = 0;
    int i = 0;
    if (str[0] == '-') {
        number.length = -1;
        i++;
    } else {
        number.length = 1;
    }

    while (str[i] != '\0') {
        if (str[i] >= '0' && str[i] <= '9') {
            int digit = str[i] - '0';
            AddAtBegan(digit, &number);
        } else {
            printf("Invalid input.\n");
            exit(1);
        }
        i++;
    }

    return number;
    
}

void display_reverse(struct node *head) {
    if (head == NULL) {
        return;
    }
    display_reverse(head->next);
    printf("%d", head->data);
}

void display(struct BigInteger number) {
    if (number.length == -1) {
        printf("%c", '-');
    }
    if (number.head == NULL) {
        return;
    }
    display_reverse(number.head);
}

struct BigInteger makeBigInt() {
    struct BigInteger number;
    number.head = NULL;
    number.length = 1;
    number.size = 0;
    return number;
}

struct BigInteger add(struct BigInteger a, struct BigInteger b) {
    struct BigInteger result = makeBigInt();

    int carry = 0, sum = 0;
    if (a.length == -1 && b.length == -1) {
        result.length = -1;
    } else if (a.length == -1) {
        a.length = 1;
        result = sub(b, a);
        a.length = -1;
        return result;
    } else if (b.length == -1) {
        b.length = 1;
        result = sub(a, b);
        b.length = -1;
        return result;
    }
    struct node *ptr1 = a.head;
    struct node *ptr2 = b.head;

    while (ptr1 || ptr2 || carry) {
        sum = carry;
        if (ptr1) {
            sum += ptr1->data;
            ptr1 = ptr1->next;
        }
        if (ptr2) {
            sum += ptr2->data;
            ptr2 = ptr2->next;
        }
        carry = sum / 10;
        AddAtEnd(&result, sum % 10);
    }

    return result;
}

struct node *reverse(struct node *head) {
    struct node *itr = head, *prev = NULL, *next;
    while (itr != NULL) {
        next = itr->next;
        itr->next = prev;
        prev = itr;
        itr = next;
    }
    head = prev;
    return head;
}

struct BigInteger truncate(struct BigInteger number) {
    if (number.head == NULL || number.head->next == NULL) {
        return number;
    }
    number.head = reverse(number.head);
    while (number.head != NULL && (number.head)->next != NULL) {
        if ((number.head)->data == 0) {
            struct node *temp = number.head;
            number.head = (number.head)->next;
            number.size = number.size - 1;
            free(temp);
        } else {
            break;
        }
    }
    number.head = reverse(number.head);
    return number;
}

int compare(struct BigInteger a, struct BigInteger b) {
    truncate(a);
    truncate(b);
    int return_val = -1;
    if ((b.size) > (a.size)) {
        return 1;
    } else if ((b.size) < (a.size)) {
        return 0;
    } else if (b.size == a.size) {
        a.head = reverse(a.head);
        b.head = reverse(b.head);
        struct node *head1 = a.head, *head2 = b.head;
        while (head1 != NULL && head2 != NULL) {
            if ((head2->data) > (head1->data)) {
                return_val = 1;
                break;
            } else if ((head2->data) < (head1->data)) {
                return_val = 0;
                break;
            }
            head1 = head1->next;
            head2 = head2->next;
        }
        a.head = reverse(a.head);
        b.head = reverse(b.head);
    }
    return return_val;
}

struct BigInteger sub(struct BigInteger a, struct BigInteger b) {
    a = truncate(a);
    b = truncate(b);
    struct node *l1 = a.head, *l2 = b.head;
    struct BigInteger result = makeBigInt();
    
    if (a.length == 1 && b.length == -1) {
        b.length = 1;
        result = add(a, b);
        b.length = -1;
        result.length = 1;
        return result;
    } else if (a.length == -1 && b.length == 1) {
        a.length = 1;
        result = add(b, a);
        a.length = -1;
        result.length = -1;
        return result;
    }

    if (compare(a, b) == 1) {
        struct node *temp = l1;
        l1 = l2;
        l2 = temp;
        result.length = -1;
    }
    
    int sub = 0;
    int borrow = 0;
    
    while (l1 != NULL && l2 != NULL) {
        sub = l1->data - l2->data + borrow;
        
        if (sub < 0) {
            borrow = -1;
            sub = sub + 10;
        } else {
            borrow = 0;
        }
        
        AddAtEnd(&result, sub);
        l1 = l1->next;
        l2 = l2->next;
    }
    
    while (l1 != NULL) {
        sub = l1->data + borrow;
        
        if (sub < 0) {
            borrow = -1;
            sub = sub + 10;
        } else {
            borrow = 0;
        }
        
        AddAtEnd(&result, sub);
        l1 = l1->next;
    }
    
    if (a.length == -1 && b.length == -1) {
        if (result.length == -1) {
            result.length = 1;
        } else {
            result.length = -1;
        }
    }
    
    result = truncate(result);
    
    return result;
}

struct BigInteger mul(struct BigInteger n1, struct BigInteger n2) {
    struct BigInteger result = makeBigInt(), partial_product;
    struct node *a = n1.head, *b = n2.head;
    if (a == NULL || b == NULL) {
        printf("the numbers are not initialized yet\n");
        return result;
    }
    int carry = 0, product = 0, i = 0;
    while (b != NULL) {
        struct node *a = n1.head;
        carry = 0;
        partial_product = makeBigInt();
        for (int j = 0; j < i; j++) {
            AddAtEnd(&partial_product, 0);
        }
        while (a != NULL) {
            product = (a->data) * (b->data) + carry;
            AddAtEnd(&partial_product, product % 10);
            carry = product / 10;
            a = a->next;
        }
        if (carry > 0) {
            AddAtEnd(&partial_product, carry);
        }
        result = add(partial_product, result);
        free(partial_product.head);
        b = b->next;
        i++;
    }
    if (n1.length != n2.length) {
        result.length = -1;
    }
    return result;
}

struct BigInteger copy(struct BigInteger number) {
    struct BigInteger copy = makeBigInt();
    struct node *itr = number.head;
    while (itr != NULL) {
        AddAtEnd(&copy, itr->data);
        itr = itr->next;
    }
    return copy;
}

struct BigInteger add_one(struct BigInteger number) {
    struct BigInteger temp = makeBigInt();
    AddAtBegan(1, &temp);
    struct BigInteger new_node = add(number, temp);
    return new_node;
}

void long_division(struct BigInteger a, struct BigInteger b, struct BigInteger *q, struct BigInteger *remain) {
    a = truncate(a);
    b = truncate(b);

    int count = 0;

    if (compare(a, b) == 1) {
        *remain = copy(a);
        AddAtBegan(0, q);
        return;
    }

    a.length = 1;
    b.length = 1;

    if (isEqualToZero(b)) {
        printf("Division by zero not possible.\n");
        exit(1);
    }

    struct BigInteger rem;
    rem = makeBigInt();
    rem = copy(a);
    int comparision;
    do {
        rem = sub(rem, b);
        count++;
        if (rem.length == -1) {
            break;
        }
        comparision = compare(b, rem);
    } while (comparision != 0);

    AddAtBegan(count, q);
    *remain = rem;
}

struct BigInteger div1(struct BigInteger a, struct BigInteger b) {
    if (isEqualToZero(b)) {
        printf("Division by zero not possible.\n");
        exit(1);
    }

    struct BigInteger count = makeBigInt();
    AddAtBegan(0, &count);
    if (compare(a, b) == 1) {
        return count;
    }

    struct BigInteger new_a, q, remain, rev_a;
    rev_a = makeBigInt();
    struct node *itr = a.head;
    while (itr != NULL) {
        AddAtBegan(itr->data, &rev_a);
        itr = itr->next;
    }

    remain = makeBigInt();
    q = makeBigInt();
    new_a = makeBigInt();
    itr = rev_a.head;
    int i;
    for (i = 0; i < b.size; i++) {
        AddAtBegan(itr->data, &new_a);
        if (i == b.size) {
            break;
        }
        itr = itr->next;
    }
    int length = 1;
    if (a.length == -1 && b.length == 1) {
        length = -1;
    } else if (a.length == 1 && b.length == -1) {
        length = -1;
    }
    b.length = 1;
    new_a.length = 1;

    while (itr != NULL) {
        long_division(new_a, b, &q, &remain);
        AddAtBegan(itr->data, &remain);
        free(new_a.head);
        new_a.head = NULL;
        new_a = remain;
        itr = itr->next;
    }

    long_division(new_a, b, &q, &remain);

    if (length == -1) {
        q.length = -1;
    }
    q = truncate(q);
    return q;
}

struct BigInteger mod(struct BigInteger a, struct BigInteger b) {
    if (isEqualToZero(b)) {
        printf("Division by zero not possible.\n");
        exit(1);
    }

    if (compare(a, b) == 1) {
        return a;
    }

    struct BigInteger new_a, q, remain, rev_a;
    rev_a = makeBigInt();
    struct node *itr = a.head;
    while (itr != NULL) {
        AddAtBegan(itr->data, &rev_a);
        itr = itr->next;
    }

    remain = makeBigInt();
    q = makeBigInt();
    new_a = makeBigInt();
    itr = rev_a.head;
    int i;
    for (i = 0; i < b.size; i++) {
        AddAtBegan(itr->data, &new_a);
        if (i == b.size) {
            break;
        }
        itr = itr->next;
    }

    b.length = 1;
    new_a.length = 1;

    while (itr != NULL) {
        long_division(new_a, b, &q, &remain);
        AddAtBegan(itr->data, &remain);
        free(new_a.head);
        new_a.head = NULL;
        new_a = remain;
        itr = itr->next;
    }

    long_division(new_a, b, &q, &remain);
    remain = truncate(remain);
    return remain;
}
